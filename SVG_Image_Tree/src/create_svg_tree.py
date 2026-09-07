#!/usr/bin/env python3
"""
Generate an SVG drawing of a simple fractal tree (front view).

Usage:
    python tree.py [branches] [output.svg]
    python tree.py 4 tree.svg
"""

from __future__ import annotations

import math
import sys

from dataclasses import dataclass
from pathlib import Path


@dataclass(frozen=True)
class Config:
    viewbox_w: int = 400
    viewbox_h: int = 400
    stem_height: float = 20.0
    initial_radius: float = 250.0
    initial_angle: float = 90.0
    line_color: str = "#2B1100"
    line_width: int = 5
    leaf_color: str = "#00A000"


CFG = Config()


@dataclass(frozen=True)
class Point:
    x: float
    y: float

    def moved(self, radius: float, angle_deg: float) -> Point:
        rad = math.radians(angle_deg)
        return Point(self.x + radius * math.cos(rad), self.y + radius * math.sin(rad))


@dataclass(frozen=True)
class Line:
    id: str
    p0: Point
    p1: Point


@dataclass(frozen=True)
class Leaf:
    id: str
    center: Point
    rx: float
    ry: float


type Element = Line | Leaf


def flip_y(y: float) -> float:
    return CFG.viewbox_h - y


def render(element: Element) -> str:
    match element:
        case Line(id=id_, p0=p0, p1=p1):
            return (
                f"\t<path\n"
                f'\t\tstyle="fill:none;stroke:{CFG.line_color};stroke-width:{CFG.line_width};'
                f'stroke-linecap:round;stroke-linejoin:round"\n'
                f'\t\td="M {p0.x:.2f},{flip_y(p0.y):.2f} {p1.x:.2f},{flip_y(p1.y):.2f}"\n'
                f'\t\tid="{id_}" />\n'
            )
        case Leaf(id=id_, center=c, rx=rx, ry=ry):
            return (
                f"\t<ellipse\n"
                f'\t\tstyle="opacity:0.6;fill:{CFG.leaf_color};stroke:none"\n'
                f'\t\trx="{rx:.2f}" ry="{ry:.2f}" cx="{c.x:.2f}" cy="{flip_y(c.y):.2f}"\n'
                f'\t\tid="leaf_{id_}" />\n'
            )


def ramify(
    id_: str, p0: Point, radius: float, angle: float, branches: int
) -> list[Element]:
    p1 = p0.moved(radius, angle)
    elements: list[Element] = [
        Line(id_, p0, p1),
        Leaf(id_, p1, branches * 20.0, branches * 10.0),
    ]

    if branches <= 1:
        return elements

    step_x = (p1.x - p0.x) / branches
    step_y = (p1.y - p0.y) / branches
    child_radius = radius / branches

    for i in range(1, branches):
        p = Point(p0.x + i * step_x, p0.y + i * step_y)
        child_id = f"{id_}_{i}"
        elements += ramify(child_id, p, child_radius, angle + 45, branches - 1)
        elements += ramify(child_id, p, child_radius, angle - 45, branches - 1)

    return elements


def build_svg(branches: int) -> str:
    mid_x = CFG.viewbox_w / 2

    elements: list[Element] = [
        Line("ground", Point(0, 0), Point(CFG.viewbox_w, 0)),
        Line("trunk", Point(mid_x, 0), Point(mid_x, CFG.stem_height)),
    ]
    elements += ramify(
        "branch",
        Point(mid_x, CFG.stem_height),
        CFG.initial_radius,
        CFG.initial_angle,
        branches,
    )

    header = (
        '<?xml version="1.0" standalone="no"?>\n'
        f'<svg width="{CFG.viewbox_w}" height="{CFG.viewbox_h}" '
        f'viewBox="0 0 {CFG.viewbox_w} {CFG.viewbox_h}"\n'
        '\txmlns="http://www.w3.org/2000/svg" version="1.1">\n'
        "\t<title>Tree SVG - Python</title>\n"
    )
    body = "".join(render(e) for e in elements)
    return header + body + "</svg>\n"


def main(argv: list[str]) -> int:
    branches = int(argv[1]) if len(argv) > 1 else 4
    output = Path(argv[2]) if len(argv) > 2 else Path("tree.svg")

    if branches <= 0:
        print("branches must be a positive integer", file=sys.stderr)
        return 1

    output.write_text(build_svg(branches), encoding="utf-8")
    print(f"Check {output}")
    print("Finished.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
