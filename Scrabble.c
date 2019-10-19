#include <stdio.h>
#include "lib/libgraphique.h"

#define RESH 690
#define RESV 690

int main()
	{
	ouvrir_fenetre(RESH, RESV);
	Point init={0,0};
	
	afficher_image("Images/plateau.bmp", init);
	actualiser();
	
	attendre_clic();
	fermer_fenetre();
	
	return 0;
	}
