/*
 * Generate an SVG drawing of a simple fractal tree (front view).
 *
 * Build: gcc -std=c23 -O2 -Wall -Wextra tree.c -o tree -lm
 *
 * Run:   ./tree [branches] [output.svg]
 *        ./tree 4 tree.svg
 */
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

constexpr double PI = 3.14159265358979323846;

typedef struct {
    int viewbox_w, viewbox_h;
    double stem_height;
    double initial_radius;
    double initial_angle;
    const char* line_color;
    int line_width;
    const char* leaf_color;
} Config;

typedef struct {
    double x, y;
} Point;

static Point point_move(Point p, double radius, double angle_deg);

typedef struct {
    char* data;
    size_t length;
    size_t capacity;
} StringBuilder;

static void sb_init(StringBuilder* sb);
static void sb_append(StringBuilder* sb, const char* text);
static void sb_appendf(StringBuilder* sb, const char* fmt, ...);
static void sb_free(StringBuilder* sb);

static double flip_y(double y);
static void svg_line(StringBuilder* out, const char* id, Point p0, Point p1);
static void svg_ellipse(StringBuilder* out, const char* id, Point c, double rx, double ry);
static void ramify(const char* id, Point p0, double radius, double angle, int branches, StringBuilder* out);
static void build_svg(StringBuilder* out, int branches);

static const Config CFG = {
    .viewbox_w = 400,
    .viewbox_h = 400,
    .stem_height = 20.0,
    .initial_radius = 250.0,
    .initial_angle = 90.0,
    .line_color = "#2B1100",
    .line_width = 5,
    .leaf_color = "#00A000",
};

int main(int argc, char** argv)
{
    int branches = (argc > 1) ? atoi(argv[1]) : 4;
    const char* output = (argc > 2) ? argv[2] : "tree.svg";

    if (branches <= 0) {
        fprintf(stderr, "branches must be a positive integer\n");
        return EXIT_FAILURE;
    }

    StringBuilder svg;
    build_svg(&svg, branches);

    FILE* file = fopen(output, "w");
    if (!file) {
        perror("fopen");
        sb_free(&svg);
        return EXIT_FAILURE;
    }
    fputs(svg.data, file);
    fclose(file);
    sb_free(&svg);

    printf("Check %s\n", output);
    printf("Finished.\n");
    return EXIT_SUCCESS;
}

static Point point_move(Point p, double radius, double angle_deg)
{
    double rad = angle_deg * PI / 180.0;
    return (Point) { p.x + radius * cos(rad), p.y + radius * sin(rad) };
}

static void sb_init(StringBuilder* sb)
{
    sb->capacity = 4096;
    sb->length = 0;
    sb->data = malloc(sb->capacity);
    if (!sb->data) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    sb->data[0] = '\0';
}

static void sb_append(StringBuilder* sb, const char* text)
{
    size_t add = strlen(text);
    if (sb->length + add + 1 > sb->capacity) {
        while (sb->length + add + 1 > sb->capacity)
            sb->capacity *= 2;
        char* grown = realloc(sb->data, sb->capacity);
        if (!grown) {
            fprintf(stderr, "Out of memory\n");
            exit(EXIT_FAILURE);
        }
        sb->data = grown;
    }
    memcpy(sb->data + sb->length, text, add + 1);
    sb->length += add;
}

static void sb_appendf(StringBuilder* sb, const char* fmt, ...)
{
    char buffer[512];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof buffer, fmt, args);
    va_end(args);
    sb_append(sb, buffer);
}

static void sb_free(StringBuilder* sb)
{
    free(sb->data);
    sb->data = NULL;
}

static double flip_y(double y) { return CFG.viewbox_h - y; }

static void svg_line(StringBuilder* out, const char* id, Point p0, Point p1)
{
    sb_appendf(out,
        "\t<path\n"
        "\t\tstyle=\"fill:none;stroke:%s;stroke-width:%d;"
        "stroke-linecap:round;stroke-linejoin:round\"\n"
        "\t\td=\"M %.2f,%.2f %.2f,%.2f\"\n"
        "\t\tid=\"%s\" />\n",
        CFG.line_color, CFG.line_width,
        p0.x, flip_y(p0.y), p1.x, flip_y(p1.y), id);
}

static void svg_ellipse(StringBuilder* out, const char* id, Point c,
    double rx, double ry)
{
    sb_appendf(out,
        "\t<ellipse\n"
        "\t\tstyle=\"opacity:0.6;fill:%s;stroke:none\"\n"
        "\t\trx=\"%.2f\" ry=\"%.2f\" cx=\"%.2f\" cy=\"%.2f\"\n"
        "\t\tid=\"leaf_%s\" />\n",
        CFG.leaf_color, rx, ry, c.x, flip_y(c.y), id);
}

static void ramify(const char* id, Point p0, double radius, double angle,
    int branches, StringBuilder* out)
{
    Point p1 = point_move(p0, radius, angle);

    svg_line(out, id, p0, p1);
    svg_ellipse(out, id, p1, branches * 20.0, branches * 10.0);

    if (branches <= 1)
        return;

    double step_x = (p1.x - p0.x) / branches;
    double step_y = (p1.y - p0.y) / branches;
    double child_radius = radius / branches;

    for (int i = 1; i < branches; ++i) {
        Point p = { p0.x + i * step_x, p0.y + i * step_y };
        char child_id[64];
        snprintf(child_id, sizeof child_id, "%s_%d", id, i);
        ramify(child_id, p, child_radius, angle + 45, branches - 1, out);
        ramify(child_id, p, child_radius, angle - 45, branches - 1, out);
    }
}

static void build_svg(StringBuilder* out, int branches)
{
    sb_init(out);
    sb_appendf(out,
        "<?xml version=\"1.0\" standalone=\"no\"?>\n"
        "<svg width=\"%d\" height=\"%d\" viewBox=\"0 0 %d %d\"\n"
        "\txmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n"
        "\t<title>Tree SVG - C</title>\n",
        CFG.viewbox_w, CFG.viewbox_h, CFG.viewbox_w, CFG.viewbox_h);

    double mid_x = CFG.viewbox_w / 2.0;

    svg_line(out, "ground", (Point) { 0, 0 }, (Point) { CFG.viewbox_w, 0 });
    svg_line(out, "trunk", (Point) { mid_x, 0 }, (Point) { mid_x, CFG.stem_height });

    ramify("branch", (Point) { mid_x, CFG.stem_height },
        CFG.initial_radius, CFG.initial_angle, branches, out);

    sb_append(out, "</svg>\n");
}
