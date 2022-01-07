# -*- Mode: Python3; coding: utf-8; indent-tabs-mpythoode: nil; tab-width: 4 -*-

'''
   Construir arquivo SVG com desenho de uma pequena árvore.
'''

from math import sin, cos, pi

# Constantes
EOL = '\n'
TAB = '\t'

VBX = 400  # viewBox largura
VBY = 400  # viewBox altura
COLOR_LINE = "#2B1100"
STROKE_LINE = "5"
COLOR_CIRCLE = "#00A000"

HEAD = '''<?xml version="1.0" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<svg width="#VBX#px" height="#VBY#px" viewBox="0 0 #VBX# #VBY#"
    xmlns="http://www.w3.org/2000/svg" version="1.1">
    <title>#TITLE#</title>
'''
FOOTER = "</svg>"
OUTPUT = "tree_1.svg"


def export(filename, svg):
    try:
        filename = open(filename, "w")
        filename.write(svg)
        filename.close()
    except Exception:
        print("Error!")
        exit(0)


def line(id, x0, y0, x1, y1):

    print("line {}: {},{} to {},{}".format(id, x0, y0, x1, y1))

    element = TAB + "<path" + EOL + TAB + TAB \
        + "style=\"fill:none;stroke:" + COLOR_LINE + ";" \
        + "stroke-width:" + STROKE_LINE + ";stroke-linecap:round;" \
        + "stroke-linejoin:round;stroke-miterlimit:4;" \
        + "stroke-dasharray:none;stroke-opacity:1\"" + EOL \
        + TAB + TAB + "d=\"M "

    # ajustar aos eixos X,Y
    y0 = VBY - y0
    y1 = VBY - y1

    element += "{},{} {},{}\"".format(x0, y0, x1, y1)
    element += EOL + TAB + TAB + "id=\"" + id + "\" />" + EOL

    return element


def ellipse(id, rx, ry, cx, cy):

    print("ellipse {}: cx= {}, cy= {} and rx= {}, ry= {}".format(id, cx,
          cy, rx, ry))

    element = TAB + "<ellipse" + EOL + TAB + TAB \
        + "style=\"opacity:0.6;fill:" + COLOR_CIRCLE + ";fill-opacity:1;" \
        + "stroke:none;stroke-width:0;stroke-linecap:round;" \
        + "stroke-linejoin:round;stroke-miterlimit:4;" \
        + "stroke-dasharray:none;stroke-opacity:1\"" + EOL

    # ajustar aos eixos X,Y
    cy = VBY - cy

    element += TAB + TAB + "ry=\"" + str(ry) + "\"" + EOL
    element += TAB + TAB + "rx=\"" + str(rx) + "\"" + EOL
    element += TAB + TAB + "cy=\"" + str(cy) + "\"" + EOL
    element += TAB + TAB + "cx=\"" + str(cx) + "\"" + EOL
    element += TAB + TAB + "id=\"ellipse_" + id + "\" />" + EOL

    return element


def move(x, y, radius, angle):

    # print("(x,y): {},{} => ".format(x,y), end="")

    # alterar X,Y
    x = int(x + radius * cos(angle * pi / 180))
    y = int(y + radius * sin(angle * pi / 180))

    # print("{},{} [radius = {}, angle = {}]".format(x, y, radius, angle))

    return (x, y)


def ramify(id, x0, y0, radius, angle, branches, svg=""):

    if radius < 0:
        return svg

    x1, y1 = move(x0, y0, radius, angle)
    svg = line(id + str(branches), x0, y0, x1, y1)
    svg += ellipse(id + str(branches), branches * 20, branches * 10, x1, y1)

    sX = (x1 - x0)//branches
    sY = (y1 - y0)//branches
    radius = radius//branches
    for i in range(1, branches):
        x = int(x0 + i * sX)
        y = int(y0 + i * sY)
        temp = id + '_' + str(i)
        svg += ramify(temp, x, y, radius, angle + 45, branches - 1, svg)
        svg += ramify(temp, x, y, radius, angle - 45, branches - 1, svg)

    return svg


def create(branches):

    x, y = VBX//2, 0    # coordenadas
    radius = 250        # raio, ângulo inicial da ramificação
    angle = 90
    stem = 20          # altura dd tronco em px

    # create SVG
    head = HEAD.replace("#VBX#", str(VBX))
    head = head.replace("#VBY#", str(VBY))
    head = head.replace("#TITLE#", "Tree SVG - Py")
    svg = head
    svg += line("base", 0, y, VBX, y)
    svg += line("trunk", x, y, x, y + stem)
    svg += ramify("branch", x, stem, radius, angle, branches)
    svg += FOOTER

    export(OUTPUT, svg)
    print("Check ", OUTPUT)
    print("Finished.")


if __name__ == '__main__':

    # Teste
    create(3)
