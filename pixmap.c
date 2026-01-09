#include "pix.h"


void creer_pixmap_carre(Pixmap *p, int largeur, int hauteur){
    p->largeur = largeur;
    p->hauteur = hauteur;
    p->pixel = (Pixel*)malloc(largeur * hauteur * sizeof(Pixel));
    for(int i = 0; i < largeur * hauteur; i++){
        creer_pixel(&p->pixel[i], 255, 255, 0); // remplit memoire de pixels jaunes
    }
}

void creer_pixmap_disque(Pixmap *p, int largeur, int hauteur, int diametre){
    p->largeur = largeur;
    p->hauteur = hauteur;
    p->pixel = (Pixel*)malloc(largeur * hauteur * sizeof(Pixel));
    
    //centre de l'image
    int centre_x = largeur / 2;
    int centre_y = hauteur / 2;
    int rayon = diametre / 2;
    
    //parcourt chaque pixel
    for(int y = 0; y < hauteur; y++){
        for(int x = 0; x < largeur; x++){
            int index = y * largeur + x;
            
            //calculer la distance au centre
            int dx = x - centre_x;
            int dy = y - centre_y;
            int distance_carree = dx*dx + dy*dy;
            int rayon_carre = rayon * rayon;
            
            //si dans disque: rouge sinon noir
            if(distance_carree <= rayon_carre){
                creer_pixel(&p->pixel[index], 255, 0, 0);
            } else {
                creer_pixel(&p->pixel[index], 0, 0, 0);
            }
        }
    }
}

void supprimer_pixmap(Pixmap *p){
    free(p->pixel);
}
void saisir_pixmap(Pixmap *p){
    
}
void afficher_pixmap(Pixmap *p){
    for(int i = 0; i < p->largeur * p->hauteur; i++){
        afficher_pixel(&p->pixel[i]);
        printf("%d", i);
    }
}