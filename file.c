#include "file.h"
void ecrire_fichier(const char* nom_fichier, Pixmap* p) {

    FILE *f = fopen(nom_fichier, "wb"); //ouverture en mode binaire
        if (!f) {
            perror("Erreur ouverture fichier");
            return;
        }

        //en-tete PPM
        fprintf(f, "P6\n%d %d\n255\n", p->largeur, p->hauteur);

        //pixels en binaire
        fwrite(p->pixel, sizeof(Pixel), p->largeur * p->hauteur, f);

        fclose(f);
}

void supprimer_fichier(const char* nom_fichier){
    if (remove(nom_fichier) != 0) {
        perror("Erreur suppression fichier");
    }
}
