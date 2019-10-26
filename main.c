#include <stdio.h>
#include <string.h>
#include "lib/libgraphique.h"

#define RESH 1000														//resolution de la fenetre
#define RESV 1000

#define BORDURE 5														//Bordures entre cases
#define TAILLECASE 53															//Taille des cases

#define FILEDICO "./dictionnaire_fr_ss_accents"							// chemin du fichier dico
#define SIZEDICO 319000                         						// taille large du dico
#define MAXLENMOT 26                            						// taille du mot max

#define DEBUG 0

typedef struct{															//structure qui contiens :
	char lettre[27];													//lettres
	int valeur[27];														//leur valeur
	int nbJetons[27];													//le nombre restant dans le jeu
	}Lettres;

void initIndexTirage(int indexTirage[2][27], Lettres indexLettre);
Lettres initialiserLettres();

char tirerLettre(Lettres indexLettre, int indexTirage[2][27]);
void chargeDico(char *filedico, char tabdico[SIZEDICO][MAXLENMOT]);
int motValable(char motUtilise[] ,char dicoTab[SIZEDICO][MAXLENMOT], int tailleDico);

Point detecter_case(Point);												/************************/									
int estDedans(Point);													/* Placer les lettres	*/
int estLibre(Point, char plateau[15][15][2]);							/************************/	

void initContenuPlateau(char plateau[15][15][2]);						//fonctions en relation avec la plateau
void updateContenuPlateau(char plateau[15][15][2], Point);					//


/******************************************************************************/
/* MAIN		                                                                  */
/******************************************************************************/
int main()
	{
	int joueur=0;
	char dicoTab[SIZEDICO][MAXLENMOT];									//Le dictionnaire
	chargeDico(FILEDICO, dicoTab);										//initialisation de dicoTab + renvoi la taille du dico dans tailleDico
	
	Lettres indexLettre=initialiserLettres();
	
	int indexTirage[2][27];
	initIndexTirage(indexTirage, indexLettre);
	
#if DEBUG
	for(int compteur=0; compteur < 27; compteur++)
		printf("lettre:%c	valeur:%d	nbJetons:%d\n", indexLettre.lettre[compteur], indexLettre.valeur[compteur], indexLettre.nbJetons[compteur]);
#endif

	ouvrir_fenetre(RESH, RESV);											//ouvre la session graphique

	Point init={0,0};													/**********************/
	afficher_image("./Images/plateau.bmp", init);						/*affichage du plateau*/
	actualiser();														/**********************/
	
	char contenu_plateau[15][15][2];									//tableau qui contiens le contenu du plateau
	initContenuPlateau(contenu_plateau);								//initialise le charactère par défaut dans le tableau

	Point emplacement_lettre;											//variable qui vas stoquer l'emplacement de la lettre
	
	char mains[2][7]={{')'}};											//Tableau qui vas stocker les cartes
	for(joueur=1; joueur<=2; joueur++)									/*******************************************/
		{																/*Varier les joueurs pour l'attribution    */
		for(int compteurLettre=0; compteurLettre<=7; compteurLettre++)	/*Avancer dans le tableau pour chaquejoueur*/
			mains[joueur][compteurLettre]=tirerLettre(indexLettre, indexTirage);		/*Attribuer les lettres					   */
		}																/*******************************************/

	while(1)
		{															//boucle infinie paramètre a modifier pour condition de sortie
		
		do
			{															//tant que le clic n'est pas dans le tableau ou que la case choisie n'est pas libre,
			emplacement_lettre=attendre_clic();								//on attend un clic
			emplacement_lettre= detecter_case(emplacement_lettre);			//on detecte la case sur laquelle est le clic
			} while (estDedans(detecter_case(emplacement_lettre)) == 0 || estLibre(detecter_case(emplacement_lettre), contenu_plateau) == 0);

		afficher_image("./Images/a.bmp", emplacement_lettre);				//on affiche la lettre du joueur(créer fonction qui determine la lettre
		actualiser();

		updateContenuPlateau(contenu_plateau, emplacement_lettre);			//on affecte la lettre choisis au tableau

#if DEBUG
	for(int b1=0; b1<15; b1++){
		for(int b2=0; b2<15; b2++){
			printf("%c", contenu_plateau[b1][b2][0]);
		}
	printf("\n");														//affichage tableau pour test
	}
	printf("--------------------------------\n");
	printf("%d %d\n", emplacement_lettre.x, emplacement_lettre.y);
	printf("--------------------------------\n");
#endif

		}

#if DEBUG
	//int test=motValable("a", dicoTab, tailleDico);					//test du dico
	//printf("%d", test);
#endif

	attendre_clic();													//Fin de session graphique
	fermer_fenetre();
	
	return 0;															//Fin du main sans erreur
	}

/******************************************************************************/
/* INITIALISATION INDEX TIRAGE ALEATOIRE                                      */
/******************************************************************************/
void initIndexTirage(int indexTirage[2][27], Lettres indexLettre)		//tableau qui vas contenir les données necessaire pour un tirage plus réaliste
	{
	int idLettre=0;														

	for(int nbJetonsTot=0; idLettre<27; idLettre++)
		{
		nbJetonsTot+=indexLettre.nbJetons[idLettre];					//calcule le nombre total de jeton en cumulant le nombre de jetons de chaque lettre
		indexTirage[0][idLettre]=idLettre;								//l'id de la lettre qui viens d'etre utilisé précédement pour récupérer son nombre de jetons
		indexTirage[1][idLettre]=nbJetonsTot;							//le nombre de jetons cumulé
		}		
	}

/******************************************************************************/
/* INITIALISER LETTRES                                                        */
/******************************************************************************/
Lettres initialiserLettres()
	{
	Lettres indexLettre;
	int valeur=0;
	int nbJetons=0;
	for(char lettre='a'; lettre<='z'; lettre++)							//boucle qui vas affecter chaque lettre à sa place dans le tableau
		{
		indexLettre.lettre[lettre-'a']=lettre;							//affecte la lettre
		
		if(lettre >= 'w' || lettre=='k')								//affecte une valeur celon la lettre
			valeur=10;
		else if(lettre == 'q' || lettre == 'j')
			valeur=8;
		else if(lettre == 'v' || lettre == 'h' || lettre == 'f')
			valeur=4;
		else if(lettre == 'b' || lettre == 'c' || lettre == 'p')
			valeur=3;
		else if(lettre == 'd' || lettre == 'g' || lettre == 'm')
			valeur=2;
		else
			valeur=1;
			
		if(valeur >= 8)													//affecter une quantité à la lettre
			nbJetons=1;
		else if(valeur == 4 || valeur== 3 || lettre == 'g')
			nbJetons=2;
		else if(lettre == 'd' || lettre == 'm')
			nbJetons=3;
		else if(lettre == 'l')
			nbJetons=5;
		else if((lettre >= 'r' && lettre <= 'u') || lettre == 'n' || lettre == 'o')
			nbJetons=6;
		else if(lettre == 'i')
			nbJetons=8;
		else if(lettre == 'a')
			nbJetons=9;
		else if(lettre == 'e')
			nbJetons=15;
		
		indexLettre.valeur[lettre-'a']=valeur;							//affecte les valeurs aux lettres
		indexLettre.nbJetons[lettre-'a']=nbJetons;
		}
	
	indexLettre.lettre[26]='%';											//cas du joker
	indexLettre.valeur[26]=0;
	indexLettre.nbJetons[26]=2;
	
	return indexLettre;
	}
	
/******************************************************************************/
/* TIRER LETTRE                                                               */
/******************************************************************************/
char tirerLettre(Lettres indexLettre, int tirage[2][27])				//fonction qui attribue les cartes
	{
	int idLettre=0;

	do
		{
		idLettre = entier_aleatoire(102);								//102 lettres dans le jeu, pour tirage plus realiste, on pioche parmis les 102 jetons

		for(int compteur=0; compteur<27; compteur++)					//parcourir les valeurs du tableau
			{
			if(idLettre < tirage[1][compteur])							//si le chiffre tiré est inferieur au nombre de jetons cumulés de a jusqu'a la lettre tiré, lui associe l'id de la lettre correspondante
				{
				idLettre=tirage[0][compteur];
				break;
				}
			}
			
		}while(indexLettre.nbJetons[idLettre] == 0);

	indexLettre.nbJetons[idLettre]-=1;
	
	return indexLettre.lettre[idLettre];
	}
	
	
/******************************************************************************/
/* CHARGEDICO                                                                 */
/******************************************************************************/
void chargeDico(char *filedico, char tabdico[SIZEDICO][MAXLENMOT])
	{
    FILE *f = fopen(filedico,"r"); 										// ouverture du fichier

    if (!f) 															// si ouverture ratée
        fprintf(stderr,"fopen: problème d'ouverture du fichier '%s'\n'",filedico);

    char line[MAXLENMOT]; 												// la ligne servant à la lecture du fichier

    int mots=0; 														// indice dans tabdico 
    while(fgets(line,MAXLENMOT,f)) 										// tant que fgets ne renvoie pas nul (eof)
        {
        sscanf(line,"%s\n",tabdico[mots]); 								// je scanne line et écris dans tabdico
        mots++;
        }
    fclose(f);															// fermeture du fichier
	}


/******************************************************************************/
/* MOT VALABLE                                                                */
/******************************************************************************/
int motValable(char motUtilise[], char dicoTab[SIZEDICO][MAXLENMOT], int tailleDico)
	{	
	int debut=0, fin=tailleDico-1, milieu=(debut+fin)/2;				//initialisation des variables repères pour la recherche dichotomique
	
	while(debut<=fin && strcmp(motUtilise, dicoTab[milieu])!=0)			//tant que le début et la fin ne se croisent pas ET que le mot n'est pas celui cherché
		{
		if(strcmp(motUtilise, dicoTab[milieu])>0)						//tester si le mot est avant ou après
			debut=milieu+1;												//et adapter les variables en conséquence
		else
			fin=milieu-1;
		milieu=(debut+fin)/2;
		}
		
	if(strcmp(motUtilise, dicoTab[milieu])==0)							//on vérifie la raison de sortie de la boucle
		return 1;														//si sorti parceque le mot est trouvé, return 1
	else	
		return 0;														//si aucune correspondance, return 0
	}

/******************************************************************************/
/* DETECTER CASE                                                              */
/******************************************************************************/
Point detecter_case(Point p)
	{
	p.x = ((p.x - BORDURE) - (p.x - BORDURE) % TAILLECASE) + BORDURE;
	p.y = ((p.y - BORDURE) - (p.y - BORDURE) % TAILLECASE) + BORDURE;
	return p;
	}

/******************************************************************************/
/* POINT DANS TABLEAU ?                                                       */
/******************************************************************************/
int estDedans(Point p)
	{
	if(p.x >= BORDURE && p.x <= 755 && p.y >= BORDURE && p.y <= 755)
		return 1;	
		
	return 0;
	}

/******************************************************************************/
/* CASE EST LIBRE ?                                                           */
/******************************************************************************/
int estLibre(Point p, char plateau[15][15][2])
	{
	if(plateau[((p.y-5)/53)][((p.x-5)/53)][0] == ' ')
		return 1;
		
	return 0;
	}

/******************************************************************************/
/* INITIALISATION CONTENU DU TABLEAU                                          */
/******************************************************************************/
void initContenuPlateau(char plateau[15][15][2]){
	int i,j,k;
	
	for(i=0; i<15; i++)
		{
		for(j=0; j<15; j++)
			{
			for(k=0; k<2; k++)
				plateau[i][j][k] = ' ';
			}
		}
	}

/******************************************************************************/
/* UPDATE CONTENU PLATEAU                                                     */
/******************************************************************************/
void updateContenuPlateau(char plateau[15][15][2], Point p){
	plateau[((p.y-5)/53)][((p.x-5)/53)][0] = 'a';
}
