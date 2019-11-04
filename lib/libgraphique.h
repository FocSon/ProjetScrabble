/*******************************************************************************
 * libraphique.c                                                               *
 * Quelques fonctions C basées sur la SDl pour réaliser des opérations         *
 * graphiques simples                                                          *
 *                                                                             *
 * Pour compiler en ligne de commande :                                        *
 * gcc ../lib/libgraphique.c prog.c -o resultat                                *
 *           `sdl-config --libs --cflags` -lm -lSDL                            *
 *                                                                             *
 * où                                                                          *
 *             prog.c : votre code source                                      *
 *             resultat    : nom de l'exécutable                               *
 *******************************************************************************
*/
#ifndef LIBGRAPHIQUE_H
#define LIBGRAPHIQUE_H
    

///////////////////////////////////////////////////////////////////////////////
//0. Directive préprocesseur
#include <SDL/SDL.h>
#include<SDL/SDL_ttf.h>


///////////////////////////////////////////////////////////////////////////////
//1. Déclaration des types "maison"
typedef Uint32 Couleur;
typedef struct {int x,y;} Point;
typedef struct {
    int touches[SDLK_LAST] ;
    int sourisx ;
    int sourisy ;
    int boutong ;
    int boutond ;
} Trace_evts ;

////////////////////////////////////////////////////////////////////////////////
// Déclaration des fonctions
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// 1. Ouvrir et fermer une fenêtre

// ouvrir une fenêtre de taille largeur (x), hauteur (y)
void ouvrir_fenetre(int largeur, int hauteur);

// terminer le programme
void fermer_fenetre();

// teste si la fin du programme a été demandée et le termine si nécessaire
// ne pas utiliser directement
void _test_arret() ;

////////////////////////////////////////////////////////////////////////////////
// 2. Fonctions de dessin

// actualise l'affichage des modifications graphiques
// sans appel à cet fonction les modifications sont non apparentes
void actualiser();

// fonction de dessin principale
// changer la couleur du Point pix
void changer_pixel(Point pix, Couleur couleur) ;

// dessine un rectangle de couleur de largeur et hauteur données
// coin est le coin haut, gauche
void dessiner_rectangle(Point coin, int largeur, int hauteur, Couleur couleur) ;

// trace une ligne du Point p1 au point p2 dela couleur donnée
// utilise l'algorithme de Bresenham
void dessiner_ligne(Point p1, Point p2, Couleur);

//dessine un disque (cercle plein) de couleur voulue en donnant rayon et centre
void dessiner_disque(Point centre, int rayon, Couleur couleur);

//dessine un cercle  de couleur voulue en donnant rayon et centre
void dessiner_cercle(Point centre, int rayon, Couleur couleur);

// affiche l'image sous forme .bmp (bitmap), contenue dans le même dossier
// nom est une chaine de caracteres qui est le nom (complet) du fichier image
// coin est le coin haut, gauche voulu pour l'image à afficher dans l'ecran
void afficher_image(char *nom, Point coin);

////////////////////////////////////////////////////////////////////////////////
// 3. Gestion des événements

// renvoie le code SDLK de la prochaine touche pressée
// fonction bloquante
int attendre_touche(void);

// renvoie le code SDLK de la prochaine touche pressée avant une durée donnée
// en milisecondes, fonction bloquante durant un certain temps.
// Renvoie 0 si aucune touche n'a été pressée.
int attendre_touche_duree(int);

// renvoie les coordonnees du prochain clic (gauche ou droite) de souris
// fonction bloquante
Point attendre_clic() ;

//comme la fonction attendre clic, mais on ajoute un signe
//négatif devant les coordonnées du point si c'est un clic droit
Point attendre_clic_gauche_droite() ;


////////////////////////////////////////////////////////////////////////////////
// 3 bis : fonctions optionnelles pour les événements, non bloquantes

//reinitialise la mémoire des événements à 0
void reinitialiser_evenements(void);

// memorise les evenements ayant eu lieu depuis la derniere
// reinitialisation
void traiter_evenements(void);

// indique si la touche de code SDL en question a été pressée 
// entre la derniere reinitialisation et le dernier traitement
int touche_a_ete_pressee(int);

//renvoie les coordonnees du dernier clic obtenu
// entre la derniere reinitialisation et le dernier traitement
// Point (-1,-1) si pas de clic
Point clic_a_eu_lieu();

// renvoie un point de coordonnées relatives souris obtenu
// entre la derniere reinitialisation et le dernier traitement
// (0,0) au lancement, dernière position reçue si sortie de fenêtre
Point deplacement_souris_a_eu_lieu();

////////////////////////////////////////////////////////////////////////////////
// 4. Affichage de texte
// pour fonctionner, la police doit se trouver dans le dossier lib
// (la police peut être changée en changeant la variable globale NOM_POLICE )

//affiche du texte de taille de police donnée ; coin est le coin haut gauche du texte
void afficher_texte(char *texte, int taille, Point coin, Couleur couleur);

//renvoie un point qui contient la taille en pixels que prendrait ce texte si on l'affichait
//(largeur,hauteur) à la taille de police donnée
Point taille_texte(char *texte, int taille);


////////////////////////////////////////////////////////////////////////////////
// 5. Autres

// renvoie une Couleur (UInt32) RGB avec les valeurs entre 0 et 255 données
// en rouge r, vert g et bleu b
Couleur fabrique_couleur(int r, int g, int b);

// pause le programme pour une duree en millisecondes
void attente(int duree_ms);
    
//renvoie la couleur du point donné ; renvoie noir si le point est hors de l'écran
Couleur couleur_point(Point p);

//renvoie un entier au hasard entre 0 et n-1
int entier_aleatoire(int n) ;



//CouleurS ///////////////////////
// 16 couleurs de base en francais

#define argent     0xC0C0C0
#define blanc      0xFFFFFF
#define bleu       0x0000FF
#define bleumarine 0x000080
#define citronvert 0x00FF00
#define cyan       0x00FFFF
#define magenta    0xFF00FF
#define gris       0x808080
#define jaune      0xFFFF00
#define marron     0x800000
#define noir       0x000000
#define rouge      0xFF0000
#define sarcelle   0x008080
#define vert       0x00FF00
#define vertclair  0x008000
#define vertolive  0x808000
#define violet     0x800080


// 140 couleurs en anglais
#define aliceblue            0xF0F8FF
#define antiquewhite         0xFAEBD7
#define aqua                 0x00FFFF
#define aquamarine           0x7FFFD4
#define azure                0xF0FFFF
#define beige                0xF5F5DC
#define bisque               0xFFE4C4
#define black                0x000000
#define blanchedalmond       0xFFEBCD
#define blue                 0x0000FF
#define blueviolet           0x8A2BE2
#define brown                0xA52A2A
#define burlywood            0xDEB887
#define cadetblue            0x5F9EA0
#define chartreuse           0x7FFF00
#define chocolate            0xD2691E
#define coral                0xFF7F50
#define cornflowerblue       0x6495ED
#define cornsilk             0xFFF8DC
#define crimson              0xDC143C
#define cyan                 0x00FFFF
#define darkblue             0x00008B
#define darkcyan             0x008B8B
#define darkgoldenrod        0xB8860B
#define darkgray             0xA9A9A9
#define darkgreen            0x006400
#define darkkhaki            0xBDB76B
#define darkmagenta          0x8B008B
#define darkolivegreen       0x556B2F
#define darkorange           0xFF8C00
#define darkorchid           0x9932CC
#define darkred              0x8B0000
#define darksalmon           0xE9967A
#define darkseagreen         0x8FBC8F
#define darkslateblue        0x483D8B
#define darkslategray        0x2F4F4F
#define darkturquoise        0x00CED1
#define darkviolet           0x9400D3
#define deeppink             0xFF1493
#define deepskyblue          0x00BFFF
#define dimgray              0x696969
#define dodgerblue           0x1E90FF
#define firebrick            0xB22222
#define floralwhite          0xFFFAF0
#define forestgreen          0x228B22
#define fuchsia              0xFF00FF
#define gainsboro            0xDCDCDC
#define ghostwhite           0xF8F8FF
#define gold                 0xFFD700
#define goldenrod            0xDAA520
#define gray                 0x808080
#define green                0x008000
#define greenyellow          0xADFF2F
#define honeydew             0xF0FFF0
#define hotpink              0xFF69B4
#define indianred            0xCD5C5C
#define indigo               0x4B0082
#define ivory                0xFFFFF0
#define khaki                0xF0E68C
#define lavender             0xE6E6FA
#define lavenderblush        0xFFF0F5
#define lawngreen            0x7CFC00
#define lemonchiffon         0xFFFACD
#define lightblue            0xADD8E6
#define lightcoral           0xF08080
#define lightcyan            0xE0FFFF
#define lightgoldenrodyellow 0xFAFAD2
#define lightgreen           0x90EE90
#define lightgrey            0xD3D3D3
#define lightpink            0xFFB6C1
#define lightsalmon          0xFFA07A
#define lightseagreen        0x20B2AA
#define lightskyblue         0x87CEFA
#define lightslategray       0x778899
#define lightsteelblue       0xB0C4DE
#define lightyellow          0xFFFFE0
#define lime                 0x00FF00
#define limegreen            0x32CD32
#define linen                0xFAF0E6
#define magenta              0xFF00FF
#define maroon               0x800000
#define mediumaquamarine     0x66CDAA
#define mediumblue           0x0000CD
#define mediumorchid         0xBA55D3
#define mediumpurple         0x9370DB
#define mediumseagreen       0x3CB371
#define mediumslateblue      0x7B68EE
#define mediumspringgreen    0x00FA9A
#define mediumturquoise      0x48D1CC
#define mediumvioletred      0xC71585
#define midnightblue         0x191970
#define mintcream            0xF5FFFA
#define mistyrose            0xFFE4E1
#define moccasin             0xFFE4B5
#define navajowhite          0xFFDEAD
#define navy                 0x000080
#define oldlace              0xFDF5E6
#define olive                0x808000
#define olivedrab            0x6B8E23
#define orange               0xFFA500
#define orangered            0xFF4500
#define orchid               0xDA70D6
#define palegoldenrod        0xEEE8AA
#define palegreen            0x98FB98
#define paleturquoise        0xAFEEEE
#define palevioletred        0xDB7093
#define papayawhip           0xFFEFD5
#define peachpuff            0xFFDAB9
#define peru                 0xCD853F
#define pink                 0xFFC0CB
#define plum                 0xDDA0DD
#define powderblue           0xB0E0E6
#define purple               0x800080
#define red                  0xFF0000
#define rosybrown            0xBC8F8F
#define royalblue            0x4169E1
#define saddlebrown          0x8B4513
#define salmon               0xFA8072
#define sandybrown           0xF4A460
#define seagreen             0x2E8B57
#define seashell             0xFFF5EE
#define sienna               0xA0522D
#define silver               0xC0C0C0
#define skyblue              0x87CEEB
#define slateblue            0x6A5ACD
#define slategray            0x708090
#define snow                 0xFFFAFA
#define springgreen          0x00FF7F
#define steelblue            0x4682B4
#define tann                 0xD2B48C
#define teal                 0x008080
#define thistle              0xD8BFD8
#define tomato               0xFF6347
#define turquoise            0x40E0D0
#define violetlight          0xEE82EE
#define wheat                0xF5DEB3
#define white                0xFFFFFF
#define whitesmoke           0xF5F5F5
#define yellow               0xFFFF00
#define yellowgreen          0x9ACD32


#endif
