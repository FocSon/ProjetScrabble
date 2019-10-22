#include <stdio.h>
#include <string.h>
#include "lib/libgraphique.h"

#define RESH 750														//resolution de la fenetre
#define RESV 750

#define MARGEG 79														//Marge a gauche du tableau(px)
#define MARGEH 35														//Marge haute du plateau(px)
#define BORDURE 3														//Bordures entre cases
#define CASEH 35														//Taille x de la case
#define CASEV 37														//Taille y de la case

#define FILEDICO "./dictionnaire_fr_ss_accents"							// chemin du fichier dico
#define SIZEDICO 319000                         						// taille large du dico
#define MAXLENMOT 26                            						// taille du mot max

char tirerLettre();
int chargeDico(char *filedico, char tabdico[SIZEDICO][MAXLENMOT]);
int motValable(char motUtilise[] ,char dicoTab[SIZEDICO][MAXLENMOT], int tailleDico);


/******************************************************************************/
/* MAIN		                                                                  */
/******************************************************************************/
int main()
	{
	int joueur=0;
	char dicoTab[SIZEDICO][MAXLENMOT];									//Le dictionnaire
	int tailleDico=chargeDico(FILEDICO, dicoTab);						//initialisation de dicoTab + renvoi la taille du dico dans tailleDico
	
	ouvrir_fenetre(RESH, RESV);											//ouvre la session graphique

	Point init={0,0};													/**********************/
	afficher_image("Images/plateau.bmp", init);							/*affichage du plateau*/
	actualiser();														/**********************/
	
	char lettre[2][7]={{')'}};											//Tableau qui vas stocker les cartes
	for(joueur=1; joueur<=2; joueur++)									/*******************************************/
		{																/*Varier les joueurs pour l'attribution    */
		for(int compteurLettre=0; compteurLettre<=7; compteurLettre++)	/*Avancer dans le tableau pour chaquejoueur*/
			lettre[joueur][compteurLettre]=tirerLettre();				/*Attribuer les lettres					   */
		}																/*******************************************/
				
	attendre_clic();													//Fin de session graphique
	fermer_fenetre();
	
	return 0;															//Fin du main sans erreur
	}
	

/******************************************************************************/
/* TIRER LETTRE                                                               */
/******************************************************************************/
char tirerLettre()														//fonction qui attribue les cartes
	{
	int idLettre = entier_aleatoire(26);
	int static nbJokair=0;
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
	
	
/******************************************************************************/
/* CHARGEDICO                                                                 */
/******************************************************************************/
int chargeDico(char *filedico, char tabdico[SIZEDICO][MAXLENMOT])
	{
    FILE *f = fopen(filedico,"r"); 										// ouverture du fichier

    if (!f) 															// si ouverture ratée
        {
        fprintf(stderr,"fopen: problème d'ouverture du fichier '%s'\n'",filedico);
        return 0;
        }

    char line[MAXLENMOT]; 												// la ligne servant à la lecture du fichier

    int mots=0; 														// indice dans tabdico 
    while(fgets(line,MAXLENMOT,f)) 										// tant que fgets ne renvoie pas nul (eof)
        {
        sscanf(line,"%s\n",tabdico[mots]); 								// je scanne line et écris dans tabdico
        mots++;
        }
    fclose(f);															// fermeture du fichier
    
    return mots;
	}

/******************************************************************************/
/* MOT VALABLE                                                                */
/******************************************************************************/
int motValable(char motUtilise[], char dicoTab[SIZEDICO][MAXLENMOT], int tailleDico)
	{
	char motTrouve[MAXLENMOT];											//Mot qui vas servir a comparer les resultats du dico
	
	for(int parcourirDico=0; parcourirDico<tailleDico; parcourirDico ++)//on parcour les mots du dico
		{
		for(int parcourirMot=0; dicoTab[parcourirDico][parcourirMot]!='\0'; parcourirMot++)//on assigne les lettres du mot a motTrouve
			motTrouve[parcourirMot]=dicoTab[parcourirDico][parcourirMot];
			
		if (strcmp(motTrouve,motUtilise)==0)							//on compare les deux chaines de caractères (si égales, la fonction strcmp renvoi 0)
			{															//return 1 si vrai
			return 1;
			}
		else 															//si les chaines sont différentes, on vide motTrouve pour le reutiliser
			for(int parcourirMot=0; parcourirMot<MAXLENMOT; parcourirMot++)
				motTrouve[parcourirMot]='\0';
		}
	
	return 0;															//si aucune correspondance, return 0
	}
