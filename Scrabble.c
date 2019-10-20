#include <stdio.h>
#include <string.h>
#include "lib/libgraphique.h"

#define RESH 690
#define RESV 690

#define margeGauche 79
#define margeHaut 35

char tirerLettre();

int nbJokair=0;

int main()
	{
	ouvrir_fenetre(RESH, RESV);
	Point init={0,0};
	char lettre[2][7]={{' '}};
	int joueur=0;
	
	afficher_image("Images/plateau.bmp", init);							//affichage du plateau
	actualiser();
																		//attribuer les lettres à chaque joueur
	for(joueur=1; joueur<=2; joueur++)										/////////////////////////////////////////////
		{																	/*Varier les joueurs pour l'attribution    */
		for(int compteurLettre=0; compteurLettre<=7; compteurLettre++)		/*Avancer dans le tableau pour chaquejoueur*/
			lettre[joueur][compteurLettre]=tirerLettre();					/*Attribuer les lettres					   */
		}																	/////////////////////////////////////////////
	
	attendre_clic();
	fermer_fenetre();
	
	return 0;
	}

char tirerLettre()														//fonction qui attribue les cartes
	{
	int idLettre = entier_aleatoire(26);
	char indexLettre[26]={' ','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','x','y','z'};
	
	char lettre=indexLettre[idLettre];
	
	if(lettre==' ')														//seulement 2 jokair dans le jeu
		nbJokair++;
		
	if(nbJokair==2)														//Si + de 2 jokair, changer la lettre
		{
		while(lettre==' ')
			lettre=indexLettre[idLettre];
		}
	
	return lettre;
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
