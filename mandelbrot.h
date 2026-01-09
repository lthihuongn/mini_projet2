#ifndef MANDELBROT_H
#define MANDELBROT_H

#include "pix.h"

#define MANDELBROT_LARGEUR 900
#define MANDELBROT_HAUTEUR 600
#define MANDELBROT_X1 -2.0
#define MANDELBROT_Y1 1.0
#define MANDELBROT_X2 1.0
#define MANDELBROT_Y2 -1.0
#define MAX_ITER 85

typedef struct {
    char red;
    char green;
    char blue;
} color;

typedef struct {
    int width;
    int height;
    double Xmin;
    double Xmax;
    double Ymin;
    double Ymax;
    double scale;
    double pixwidth;
    int* convrg;
} mandel_pic;

void creer_color();
void saisir_color();
void afficher_color();
void effacer_color();


int convergence(double x, double y);
color palette(int c);
mandel_pic new_mandel(int width, int height, double Xmin, double Ymin, double scale);
void save_mandel(mandel_pic m, const char* filename);
int interpolate(mandel_pic m, double x, double y);
void creer_pixmap_mandelbrot(Pixmap *p);
void creer_pixmap_mandelbrot_zoom(Pixmap *p, double x1, double y1, double x2, double y2);
void creer_serie_zoom_mandelbrot(int nb_images, double x_target, double y_target);

double pixel_vers_x(int px, double x1, double x2, int largeur);
double pixel_vers_y(int py, double y1, double y2, int hauteur);
int calculer_index(int px, int py, int largeur);
void creer_couleur_mandelbrot(Pixel *pixel, int c);

#endif
