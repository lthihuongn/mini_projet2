#include "mandelbrot.h"
#include "file.h"


void creer_color(){
}
void saisir_color(){
}
void afficher_color(){
}
void effacer_color(){
}
void creer_mandel_pic(){
}
void saisir_mandel_pic(){
}
void afficher_mandel_pic(){
}
void effacer_mandel_pic(){
}

int convergence(double x, double y){
    double u_reel = x;
    double u_imag = y;
    
    for(int n = 0; n < MAX_ITER; n++){
        double module_carre = u_reel * u_reel + u_imag * u_imag; //module carre de U(n)
        
        //si module >= 2, suite diverge
        if(module_carre >= 4.0){  // 2**2 = 4
            return n;
        }
        
        //Calculer U(n+1) = U(n)**2 + Z
        //(a+ib)**2 = a**2 - b**2 + 2iab
        double nouveau_reel = u_reel * u_reel - u_imag * u_imag + x;
        double nouveau_imag = 2 * u_reel * u_imag + y;
        
        u_reel = nouveau_reel;
        u_imag = nouveau_imag;
    }
    
    //ici, suite converge
    return 0;
}

color palette(int c) { //6 cycles de montees / descentes
    color couleur;
    int phase = c % 1536; //1536 = 6 * 256 (255 + 0) palette se repete tous les 1536 pas
    if (phase < 256) { //rouge monte
        couleur.red = phase;
        couleur.green = 0;
        couleur.blue = 0;
    } else if (phase < 512) { //rouge haut vert monte
        couleur.red = 255;
        couleur.green = phase - 256;
        couleur.blue = 0;
    } else if (phase < 768) { //rouge descend vert haut
        couleur.red = 767 - phase;
        couleur.green = 255;
        couleur.blue = 0;
    } else if (phase < 1024) { //vert haut bleu monte
        couleur.red = 0;
        couleur.green = 255;
        couleur.blue = phase - 768;
    } else if (phase < 1280) { //vert descend bleu haut
        couleur.red = 0;
        couleur.green = 1279 - phase;
        couleur.blue = 255;
    } else { //bleu haut rouge monte
        couleur.red = phase - 1280;
        couleur.green = 0;
        couleur.blue = 255;
    }
    return couleur;
}

mandel_pic new_mandel(int width, int height, double Xmin, double Ymin, double scale) {
    mandel_pic m;
    m.width = width;
    m.height = height;
    m.Xmin = Xmin;
    m.Ymin = Ymin;
    m.scale = scale;
    //m.pixwidth = 1.0 / scale;
    m.convrg = (int*)malloc(width * height * sizeof(int));

    m.Xmax = Xmin + (scale * 3.0);
    m.Ymax = Ymin + (scale * 3.0 * height / width);
    m.pixwidth = scale * 3.0/width;

    // convergence pour chaque pixel
    for (int py = 0; py < m.height; py++) {
        for (int px = 0; px < m.width; px++) {
            double x = m.Xmin + px * m.pixwidth;
            double y = m.Ymax - py * m.pixwidth;
            int index = py * m.width + px;
            m.convrg[index] = convergence(x, y);
        }
    }

    return m;
}

void save_mandel(mandel_pic m, const char* filename) {
    Pixmap p;
    p.largeur = m.width;
    p.hauteur = m.height;
    p.pixel = (Pixel*)malloc(m.width * m.height * sizeof(Pixel));

    for (int py = 0; py < m.height; py++) {
        for (int px = 0; px < m.width; px++) {
            //double x = m.Xmin + px * m.pixwidth;
            //double y = m.Ymax - py * m.pixwidth;

            //int c = convergence(x, y);
            int index = py * m.width + px;
            int c = m.convrg[index]; // Utiliser la valeur deja calculee

            creer_couleur_mandelbrot(&p.pixel[index], c);
        }
    }

    ecrire_fichier(filename, &p);
    free(p.pixel);
}

int interpolate(mandel_pic m, double x, double y) {
    // verifie si (x,y) est dans l'image
    if (x < m.Xmin || x > m.Xmax || y < m.Ymin || y > m.Ymax) {
        return -1; // hors limites
    }

    // conversion coordonnees math en coordonnees pixel
    double px_exact = (x - m.Xmin) / m.pixwidth;
    double py_exact = (m.Ymax - y) / m.pixwidth;

    //trouve 4 pixels qui entourent le point (x,y)
    int px0 = (int)px_exact;
    int py0 = (int)py_exact;
    int px1 = px0 + 1;
    int py1 = py0 + 1;

    // limites pour les 4 points
    if (px1 >= m.width) px1 = m.width - 1;
    if (py1 >= m.height) py1 = m.height - 1;
    if (px0 < 0) px0 = 0;
    if (py0 < 0) py0 = 0;

    // convergence des 4 points
    int c00 = m.convrg[py0 * m.width + px0];
    int c01 = m.convrg[py0 * m.width + px1];
    int c10 = m.convrg[py1 * m.width + px0];
    int c11 = m.convrg[py1 * m.width + px1];

    // Test aléatoire : forcer le recalcul pour 1% des pixels
    if (random() < RAND_MAX / 100) {
        return -1;
    }

    // si les 4 points sont égaux
    if (c00 == c01 && c01 == c10 && c10 == c11) {
        return c00;
    }

    return -1;
}

//pixel (0-900, 0-600) en coordonnees math (-2 a 1, 1 a -1)
double pixel_vers_x(int px, double x1, double x2, int largeur){
    double proportion = (double)px / (largeur - 1);
    double largeur_zone = x2 - x1;
    return x1 + proportion * largeur_zone;
}

double pixel_vers_y(int py, double y1, double y2, int hauteur){
    double proportion = (double)py / (hauteur - 1);
    double hauteur_zone = y2 - y1;
    return y1 + proportion * hauteur_zone;
}

//index dans le tableau de pixels
int calculer_index(int px, int py, int largeur){
    return py * largeur + px;
}

//couleur selon la convergence
void creer_couleur_mandelbrot(Pixel *pixel, int c){
    creer_pixel(pixel, palette(c).red, palette(c).green, palette(c).blue);
}

void creer_pixmap_mandelbrot(Pixmap *p){
    p->largeur = MANDELBROT_LARGEUR;
    p->hauteur = MANDELBROT_HAUTEUR;
    
    int nb_pixels = MANDELBROT_LARGEUR * MANDELBROT_HAUTEUR;
    p->pixel = (Pixel*)malloc(nb_pixels * sizeof(Pixel));
    
    for(int py = 0; py < MANDELBROT_HAUTEUR; py++){
        for(int px = 0; px < MANDELBROT_LARGEUR; px++){
            //position pixel en coordonnées mathématiques
            double x = pixel_vers_x(px, MANDELBROT_X1, MANDELBROT_X2, MANDELBROT_LARGEUR);
            double y = pixel_vers_y(py, MANDELBROT_Y1, MANDELBROT_Y2, MANDELBROT_HAUTEUR);
            
            //si ce point converge
            int c = convergence(x, y);
            
            //placer pixel dans le tableau
            int index = calculer_index(px, py, MANDELBROT_LARGEUR);
            
            //couleur du pixel
            creer_couleur_mandelbrot(&p->pixel[index], c*10);
        }
    }
}

//5)
//coordonnees personalisees : peut zoomer sur une zone specifique
void creer_pixmap_mandelbrot_zoom(Pixmap *p, double x1, double y1, double x2, double y2){
    p->largeur = MANDELBROT_LARGEUR;
    p->hauteur = MANDELBROT_HAUTEUR;
    
    int nb_pixels = MANDELBROT_LARGEUR * MANDELBROT_HAUTEUR;
    p->pixel = (Pixel*)malloc(nb_pixels * sizeof(Pixel));
    
    for(int py = 0; py < MANDELBROT_HAUTEUR; py++){
        for(int px = 0; px < MANDELBROT_LARGEUR; px++){
            double x = pixel_vers_x(px, x1, x2, MANDELBROT_LARGEUR);
            double y = pixel_vers_y(py, y1, y2, MANDELBROT_HAUTEUR);
            
            int c = 
            (x, y);
            
            int index = calculer_index(px, py, MANDELBROT_LARGEUR);
            
            creer_couleur_mandelbrot(&p->pixel[index], c);
        }
    }
}

void creer_serie_zoom_mandelbrot(int nb_images, double x_target, double y_target){
    Pixmap p;
    
    double x1_init = MANDELBROT_X1;
    double y1_init = MANDELBROT_Y1;
    double x2_init = MANDELBROT_X2;
    double y2_init = MANDELBROT_Y2;
    
    
    for(int i = 0; i < nb_images; i++){
        //facteur de zoom
        double t = (double)i / (nb_images - 1);  //de 0 a 1
        
        //taille de la fenetre
        double largeur_init = x2_init - x1_init;
        double hauteur_init = y1_init - y2_init;
        
        //fenetre reduite progressivement
        double facteur_zoom = 1.0 - t * 0.99;
        double largeur = largeur_init * facteur_zoom;
        double hauteur = hauteur_init * facteur_zoom;
        
        //fenetre centree sur le point cible
        double x1 = x_target - largeur / 2;
        double x2 = x_target + largeur / 2;
        double y1 = y_target + hauteur / 2;
        double y2 = y_target - hauteur / 2;
        
        //creer l'image
        creer_pixmap_mandelbrot_zoom(&p, x1, y1, x2, y2);
        
        char nom_fichier[30];
        sprintf(nom_fichier, "im/im%d.ppm", i);
        
        ecrire_fichier(nom_fichier, &p);
        supprimer_pixmap(&p);
    }
}

// nouvelle creer_serie_zoom_mandelbrot sans tout recalculer
void generer_serie_zoom(double xmin, double ymin, double scale, int nb_images, int num_depart) {
    
    mandel_pic prev = new_mandel(900, 600, xmin, ymin, scale);
    char nom_fichier[40];
    sprintf(nom_fichier, "im/im%d.ppm", num_depart);
    save_mandel(prev, nom_fichier);
    
    for (int i = 1; i < nb_images; i++) {
        //calcul nouveau scale (zoom progressif)
        double factor = 0.95; // Zoom de 5% a chaque image
        double new_scale = scale * pow(factor, i);
        
        //nouvelle image
        mandel_pic next;
        next.width = 900;
        next.height = 600;
        next.Xmin = xmin;
        next.Ymin = ymin;
        next.scale = new_scale;
        next.convrg = (int*)malloc(900 * 600 * sizeof(int));
        next.Xmax = next.Xmin + (next.scale * 3.0);
        next.Ymax = next.Ymin + (next.scale * 3.0 * next.height / next.width);
        next.pixwidth = next.scale * 3.0 / next.width;
        
        // utiliser interpolate() pour accelerer
        int interpolated = 0;
        int calculated = 0;
        for (int py = 0; py < next.height; py++) {
            for (int px = 0; px < next.width; px++) {
                double x = next.Xmin + px * next.pixwidth;
                double y = next.Ymax - py * next.pixwidth;
                int index = py * next.width + px;
                
                int c = interpolate(prev, x, y);
                if (c == -1) {
                    c = convergence(x, y);
                    calculated++;
                } else {
                    interpolated++;
                }
                next.convrg[index] = c;
            }
        }
        
        sprintf(nom_fichier, "im/im%d.ppm", num_depart + i);
        save_mandel(next, nom_fichier);
        
        free(prev.convrg);
        prev = next;
    }
    
    free(prev.convrg);
}
