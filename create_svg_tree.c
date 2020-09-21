/*
 *  Construir arquivo SVG com desenho de uma pequena árvore.
 *
 *  input: número de ramos (branches)
 *  output: arquivo SVG    (tree.svg)
 *
 *  Compilar:
 *              gcc create_svg_tree.c -o tree -lm
 *
 *  Executar:
 *              tree [número de ramos] 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Geral */
#define TRUE  1
#define FALSE 0
#define TAB   "\t"
#define EOL   "\n"
#define PI    3.14159265

/* SVG */
#define VBX   "400"         // viewBox largura
#define VBY   "400"         // viewBox altura
#define TITLE "Tree SVG - C"

/* Line */
#define COLOR_LINE "#2B1100"
#define STROKE_LINE "8"

/* Circle */
#define COLOR_CIRCLE "#00FF00"

/* Constantes */
const char *HEAD =                                  // cabeçalho SVG
    "<?xml version=\"1.0\" standalone=\"no\"?>" EOL
    "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"" EOL
    "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" EOL
    "<svg width=\"" VBX "px\" height=\"" VBY 
    "px\" viewBox=\"0 0 " VBX " " VBY "\"" EOL
    TAB "xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">" EOL
    TAB "<title>" TITLE "</title>" EOL;
const char *FOOTER = "</svg>";                      // final SVG
const char *OUTPUT = "tree.svg";                    // arquivo de saída
const int BUFFER = 1024;    // tamanho do armazenamento temporário de string

/* Declarações */
void create(int branches);
void move(int *x, int *y, int radius, int angle);
char* line(char *id, int x0, int y0, int x1, int y1);
char* ellipse(char *id, int rx, int ry, int cx, int cy);
void ramify(char *id, int x0, int y0, int radius, int angle, int branches,
            char** svg);

void alert(char *message /* string */, int stop /* boolean: 1 or 0 */);
void join(char **original /* string */, const char *extra /* string */);
void export(const char *filename /* string */, const char *text /* string */);


int main(int argc, char **argv)
{
    if (argc > 1) {
        printf("Branches: %s\n", argv[1]);
        create(atoi(argv[1]));
    } else {
        printf("Example!\n");
        create(4);
    }

    return EXIT_SUCCESS;
}

void create(int branches)
{
    char *svg;      // texto SVG
    char *temp;     // string temporária

    int height, width;      // viewBox
    int x0, y0, x1, y1;     // coordenadas 
    int radius, angle;      // raio, ângulo
    int stem;               // altura dd tronco em px

    svg = "";
    temp = "";
    height = atoi(VBY);
    width  = atoi(VBX);
    stem = 50;
    radius = 250;
    angle = 90;
  
    // cabeçalho SVG
    join(&svg, HEAD);

    // base
    join(&svg, line("base", 0, 0, width, 0));

    // tronco
    x0 = width/2;
    y0 = stem;
    join(&svg, line("trunk", x0, 0, x0, y0));

    // ramo
    ramify("branch", x0, y0, radius, angle, branches, &temp);
    join(&svg, temp);

    // encerrar SVG
    join(&svg, FOOTER);

    // salvar arquivo
    export(OUTPUT, svg);

    printf("Check %s\n", OUTPUT);
    printf("Finished.\n");
}

char* line(char *id, int x0, int y0, int x1, int y1)
{
    printf("line %s: %i,%i to %i,%i\n", id, x0, y0, x1, y1);

    char *element = 
        TAB "<path" EOL
        TAB TAB "style=\"fill:none;stroke:" COLOR_LINE ";"
                "stroke-width:" STROKE_LINE ";stroke-linecap:round;"
                "stroke-linejoin:round;stroke-miterlimit:4;"
                "stroke-dasharray:none;stroke-opacity:1\"" EOL
        TAB TAB "d=\"M ";

    // ajustar aos eixos X,Y
    y0 = atoi(VBY) - y0;
    y1 = atoi(VBY) - y1;

    char buffer[BUFFER];
    sprintf(buffer,
            "%d,%d %d,%d\"" EOL
            TAB TAB "id=\"%s\" />" EOL,
             x0, y0, x1, y1, id);
    join(&element, buffer);

    return element;
}

char* ellipse(char *id, int rx, int ry, int cx, int cy)
{
    printf("ellipse %s: cx = %i, cy = %i and rx = %i, ry = %i\n",
            id, rx, ry, cx, cy);

    char *element =
        TAB "<ellipse" EOL
        TAB TAB "style=\"opacity:0.6;fill:" COLOR_CIRCLE ";fill-opacity:1;"
        "stroke:none;stroke-width:0;stroke-linecap:round;"
        "stroke-linejoin:round;stroke-miterlimit:4;"
        "stroke-dasharray:none;stroke-opacity:1\"" EOL;

    // ajustar aos eixos X,Y
    cy = atoi(VBY) - cy;       

    char buffer[BUFFER];
    sprintf(buffer, 
            TAB TAB "ry=\"%d\"" EOL
            TAB TAB "rx=\"%d\"" EOL
            TAB TAB "cy=\"%d\"" EOL
            TAB TAB "cx=\"%d\"" EOL
            TAB TAB "id=\"%s\" />" EOL,
            ry, rx, cy, cx, id);
    join(&element, buffer);

    return element;
}

void move(int *x, int *y, int radius, int angle)
{
    printf("(x,y): %i, %i => ", *x, *y);

    // alterar X,Y
    *x = (int) *x + radius * cos(angle * PI / 180);
    *y = (int) *y + radius * sin(angle * PI / 180);

    printf("%i, %i [radius = %i, angle = %i]\n", *x, *y, radius, angle);
}

void ramify(char *id, int x0, int y0, int radius, int angle, int branches,
            char** svg)
{
    int x1 = x0;
    int y1 = y0;

    move(&x1, &y1, radius, angle);
    join(&*svg, line("b1", x0, y0, x1, y1));
    join(&*svg, ellipse("e1", branches * 20, branches * 10, x1, y1));    

    int sX = (x1 - x0)/branches;
    int sY = (y1 - y0)/branches;
    radius = (int) radius/branches;
    for (int i = 1; i < branches; ++i)
    {
        int x = x0 + i * sX;
        int y = y0 + i * sY;
        ramify("", x, y, radius, angle + 45, branches - 1, &*svg);
        ramify("", x, y, radius, angle - 45, branches - 1, &*svg);      
    }
}

/******************/
/* Funções Comuns */
/******************/
void alert(char *message, int stop)
{
    printf("%s\n", message);
    if (stop == TRUE) exit(0);
}

/* Concatenar duas strings */
void join(char **original, const char *extra)
{
    char *temp;
    temp = malloc(strlen(*original) + strlen(extra) + 1);
    if (!temp) alert("Insufficient memory!", TRUE);

    strcpy(temp, *original);
    strcat(temp, extra);

    *original = malloc(strlen(temp) + 1);
    if (!*original) alert("Insufficient memory!", TRUE);

    strcpy(*original, temp);
    free(temp);
}

/* Salvar arquivo */
void export(const char *filename, const char *text)
{
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        alert("Error opening file!\n", TRUE);
    }

    fprintf(file, "%s\n", text);
    fclose(file);
}
