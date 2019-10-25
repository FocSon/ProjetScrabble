#include<stdio.h>
#include"./lib/libgraphique.h"

Point detecter_case(Point);
int estDedans(Point);
int estLibre(Point, char plateau[15][15][2]);
void initContenuPlateau(char plateau[15][15][2]);
void updateContenuPlateau(char plateau[15][15][2], Point);

typedef struct {char lettre; int valeur;} Lettre;

int main(void){

	Lettre a = {'a', 1};
	Lettre b = {'b', 3};
	Lettre c = {'c', 3};
	Lettre d = {'d', 2};
	Lettre e = {'e', 1};
	Lettre f = {'f', 4};
	Lettre g = {'g', 2};
	Lettre h = {'h', 4};
	Lettre i = {'i', 1};
	Lettre j = {'j', 8};
	Lettre k = {'k', 5};
	Lettre l = {'l', 1};
	Lettre m = {'m', 3};
	Lettre n = {'n', 1};
	Lettre o = {'o', 1};
	Lettre p = {'p', 3};
	Lettre q = {'q', 10};
	Lettre r = {'r', 1};
	Lettre s = {'s', 1};
	Lettre t = {'t', 1};
	Lettre u = {'u', 1};
	Lettre v = {'v', 4};
	Lettre w = {'w', 4};
	Lettre x = {'x', 8};
	Lettre y = {'y', 4};
	Lettre z = {'z', 10};

	char contenu_plateau[15][15][2];

	int b1,b2;

	initContenuPlateau(contenu_plateau);

	ouvrir_fenetre(1000, 1000);

	Point plateau = {0, 0};
	Point emplacement_lettre;

	afficher_image("./src/img/plateau.bmp", plateau);
	actualiser();

	while(1){
		
		do {
			emplacement_lettre=attendre_clic();
			emplacement_lettre= detecter_case(emplacement_lettre);
		} while (estDedans(detecter_case(emplacement_lettre)) == 0 || estLibre(detecter_case(emplacement_lettre), contenu_plateau) == 0);

	afficher_image("./src/img/a.bmp", emplacement_lettre);
	actualiser();

	updateContenuPlateau(contenu_plateau, emplacement_lettre);

	/*for(b1=0; b1<15; b1++){
		for(b2=0; b2<15; b2++){
			printf("%c", contenu_plateau[b1][b2][0]);
		}
	printf("\n");														//affichage tableau pour test
	}
	printf("--------------------------------\n");
	printf("%d %d\n", emplacement_lettre.x, emplacement_lettre.y);
	printf("--------------------------------\n");*/

	}
}

Point detecter_case(Point p){
	p.x = ((p.x - 5) - (p.x - 5) % 53) + 5;
	p.y = ((p.y - 5) - (p.y - 5) % 53) + 5;
	return p;
}

int estDedans(Point p){
	if(p.x >=5 && p.x <= 755 && p.y >= 5 && p.y <= 755){
		return 1;
	}
	return 0;
}

int estLibre(Point p, char plateau[15][15][2]){
	if(plateau[((p.y-5)/53)][((p.x-5)/53)][0] != ' '){
		return 0;
	}
	return 1;
}

void initContenuPlateau(char plateau[15][15][2]){
	int i,j,k;
	for(i=0; i<15; i++){
		for(j=0; j<15; j++){
			for(k=0; k<2; k++){
				plateau[i][j][k] = ' ';
			}
		}
	}
}

void updateContenuPlateau(char plateau[15][15][2], Point p){
	plateau[((p.y-5)/53)][((p.x-5)/53)][0] = 'a';
}