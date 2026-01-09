#include<stdio.h>
#include<stdlib.h>
#include "pix.h"

#define FILENAME "image.ppm"

void ecrire_fichier(const char* nom_fichier, Pixmap* p);
void supprimer_fichier(const char* nom_fichier);