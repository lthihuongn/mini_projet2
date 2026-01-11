#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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


    // Ancienne approche
    /*creer_pixmap_mandelbrot(&p);
    ecrire_fichier("mandelbrot.ppm", &p);
    printf("rfvrv");
    supprimer_pixmap(&p);*/

    // Utilisation de la nouvelle approche avec mandel_pic
    mandel_pic m = new_mandel(900, 600, -2.0, -1.0, 1.0);
    save_mandel(m, "mandelbrot.ppm");
    free(m.convrg);


    
    // Mode sans argument : lancer le calcul complet
    // Lancer 4 processus en parallèle
    printf("Lancement de 4 processus en parallèle...\n");
    
    double xmin = -2.0;
    double ymin = -1.0;
    double scale = 1.0;
    
    char cmd[200];
    
    // Processus 1 : images 0-9
    sprintf(cmd, "./main %.10f %.10f %.10f 10 0 &", xmin, ymin, scale);
    system(cmd);
    
    // Processus 2 : images 10-19
    sprintf(cmd, "./main %.10f %.10f %.10f 10 10 &", xmin, ymin, scale);
    system(cmd);
    
    // Processus 3 : images 20-29
    sprintf(cmd, "./main %.10f %.10f %.10f 10 20 &", xmin, ymin, scale);
    system(cmd);
    
    // Processus 4 : images 30-39 (sans & pour attendre)
    sprintf(cmd, "./main %.10f %.10f %.10f 10 30", xmin, ymin, scale);
    system(cmd);
    
    return 0;
}