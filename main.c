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


    // Ancienne approche
    /*creer_pixmap_mandelbrot(&p);
    ecrire_fichier("mandelbrot.ppm", &p);
    printf("rfvrv");
    supprimer_pixmap(&p);*/

    // Utilisation de la nouvelle approche avec mandel_pic
    mandel_pic m = new_mandel(900, 600, -2.0, -1.0, 1.0);
    save_mandel(m, "mandelbrot.ppm");
    free(m.convrg);
    
    
    
    
    
    // Test de interpolate() comme demandé dans la consigne
    printf("\n=== Test interpolate() ===\n");
    
    // 1) Calcul normal sur les coordonnées (Xmin=-0.755232, Ymin=0.121387, scale=0.01)
    printf("Calcul image 1 (normale)...\n");
    mandel_pic m1 = new_mandel(900, 600, -0.755232, 0.121387, 0.01);
    save_mandel(m1, "zoom1.ppm");
    printf("Image 1 terminée (zoom1.ppm)\n");
    
    // 2) Utiliser interpolate pour calculer l'image aux coordonnées 
    //    (Xmin=-0.752914, Ymin=0.123475, scale=0.00738)
    printf("\nCalcul image 2 avec interpolate()...\n");
    mandel_pic m2;
    m2.width = 900;
    m2.height = 600;
    m2.Xmin = -0.752914;
    m2.Ymin = 0.123475;
    m2.scale = 0.00738;
    m2.convrg = (int*)malloc(900 * 600 * sizeof(int));
    m2.Xmax = m2.Xmin + (m2.scale * 3.0);
    m2.Ymax = m2.Ymin + (m2.scale * 3.0 * m2.height / m2.width);
    m2.pixwidth = m2.scale * 3.0 / m2.width;
    
    // Calculer chaque pixel avec interpolate() et convergence() si nécessaire
    int nb_interpolated = 0;
    int nb_calculated = 0;
    
    for (int py = 0; py < m2.height; py++) {
        for (int px = 0; px < m2.width; px++) {
            double x = m2.Xmin + px * m2.pixwidth;
            double y = m2.Ymax - py * m2.pixwidth;
            int index = py * m2.width + px;
            
            // Essayer d'abord interpolate()
            int c = interpolate(m1, x, y);
            
            // Si interpolate() retourne -1, utiliser convergence()
            if (c == -1) {
                c = convergence(x, y);
                nb_calculated++;
            } else {
                nb_interpolated++;
            }
            
            m2.convrg[index] = c;
        }
    }
    
    printf("Résultat: %d pixels interpolés, %d pixels calculés\n", 
           nb_interpolated, nb_calculated);
    save_mandel(m2, "zoom2.ppm");
    printf("Image 2 terminée (zoom2.ppm)\n");
    
    free(m1.convrg);
    free(m2.convrg);
    
    printf("\n=== Série de zoom ===\n");
    creer_serie_zoom_mandelbrot(10, -0.99, 0.3);
    
    return 0;
}