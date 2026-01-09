#include "pix.h"

void creer_pixel(Pixel *px, unsigned char r, unsigned char g, unsigned char b) {
    px->r = r;
    px->g = g;
    px->b = b;
}

void afficher_pixel(Pixel *px) {
    printf("Pixel RGB(%u, %u, %u)\n", px->r, px->g, px->b);
}

void saisir_pixel(Pixel *px) {
}

void suppimer_pixel(Pixel *px) {
}