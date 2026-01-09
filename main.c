#include <stdio.h>
#include <stdlib.h>
#include "pix.h"
#include "file.h"
#include "mandelbrot.h"

int main(int argc, char const *argv[]){
    Pixmap p;

    creer_pixmap_carre(&p, 600, 400);
    ecrire_fichier("carre.ppm", &p);
    supprimer_pixmap(&p);
    
    creer_pixmap_disque(&p, 600, 400, 200);
    ecrire_fichier("disque.ppm", &p);
    supprimer_pixmap(&p);
    
    creer_pixmap_mandelbrot(&p);
    ecrire_fichier("mandelbrot.ppm", &p);
    printf("rfvrv");
    supprimer_pixmap(&p);
    
    creer_serie_zoom_mandelbrot(10, -0.99, 0.3);
    
    return 0;
}