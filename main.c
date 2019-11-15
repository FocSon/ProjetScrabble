#include <stdio.h>
#include <string.h>
#include "lib/libgraphique.h"

#define RESH 1000														//resolution de la fenetre
#define RESV 1000

#define BORDURE 5	
#define ESPACEMENT 100													//Bordures entre cases
#define TAILLECASE 48													//Taille des cases

#define FILEDICO "./dictionnaire_fr_ss_accents" 						// chemin du fichier dico
#define SIZEDICO 319000                         						// taille large du dico
#define MAXLENMOT 26                            						// taille du mot max

#define TAILLE_PLATEAU 15

#define DEBUG 1

typedef struct{															//structure qui contiens :
	char lettre[27];													//lettres
	int valeur[27];														//leur valeur
	int nbJetons[27];													//le nombre restant dans le jeu
	}Lettres;

int menu();
void survol(Point);
int traitement_choix(Point);

void afficher_plateau();

void initPoints(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2]);

void initIndexTirage(int indexTirage[27], Lettres);
Lettres initialiserLettres();

char tirerLettre(Lettres *indexLettre, int indexTirage[27]);
void actualiser_pioche(int indexTirage[27], int compteur);

int chargeDico(char *filedico, char tabdico[SIZEDICO][MAXLENMOT]);
int motValable(char motUtilise[TAILLE_PLATEAU] ,char dicoTab[SIZEDICO][MAXLENMOT], int);
int lireMots(char mot[TAILLE_PLATEAU], char contenu_plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2], char dicoTab[SIZEDICO][MAXLENMOT], int);

Point detecter_case(Point);													/************************/									
int estDansPlateau(Point);													/* Placer les lettres	*/
int CaseEstLibre(Point, char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2]);							/************************/	
int peutPlacer(char contenu_plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2], Point);

Point attendreSelectionLettre(int, Point lettres_placees[7], int, int);
char selectionLettre(int, char mains[2][7], Point);
void placerLettre(char contenu_plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2], char, Point, Point);
Point attendrePlacerLettre(char contenu_plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2], int, Point emplacement_lettre_old[TAILLE_PLATEAU], int);
void entourerCase(Point, Couleur);
int estDansMainJoueur(Point, int);
void updateMainJoueur(char mains[2][7], int, Point lettres_placees[7], Lettres*, int indexTirage[27]);
int lettreDejaPosee(Point lettres_placees[7], Point lettre_selectionnee);

void initContenuPlateau(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2]);						//fonctions en relation avec la plateau
void initMainJoueur(char mains[2][7], int indextirage[27], Lettres*);
void updateContenuPlateau(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2], Point, char);					//

void afficherMainJoueur(Point, char mains[2][7], int);
void cacherMainJoueur(Point pos_case, int joueur);

Point convertirEnCaseTableau(Point clic);

int clicBouton(Point clic);
void razAnciennesLettres(Point emplacement_lettre_old[TAILLE_PLATEAU], Point lettres_placees[7]);
char compDirection(Point, Point, Point);

void reinitTour(Point emplacement_lettre_old[7], Point lettre_placees[7], char contenu_plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2], char mains[2][7], int, Point);
void actualiser_plateau(Point emplacement_lettre_old, char contenu_plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2]);

void chargerSauvegarde(char contenu_plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2], char mains[2][7], int scores[2], int* joueur);
void sauvegarder(char contenu_plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2], char mains[2][7], int scores[2], int joueur);
void updatePlateauSave(char contenu_plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2]);

int score(int, Point emplacement_lettre_old[7], char contenu_plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2], Lettres);
int multiplicateurLettre(char);
int multiplicateurMot(char);

void switchValiderPasser(int numCoup, int comptLettre);

void piocher(char mains[2][7], Lettres *, int indexTirage[27], int);

/******************************************************************************/
/* MAIN		                                                                  */
/******************************************************************************/
int main()
	{
	ouvrir_fenetre(RESH, RESV);											//ouvre la session graphique

	char contenu_plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2];									//tableau qui contiens le contenu du plateau ainsi que les valeurs double ou triple des mots / lettres
	char mot[TAILLE_PLATEAU];

	char dicoTab[SIZEDICO][MAXLENMOT];									//Le dictionnaire
	int nbMotDico=chargeDico(FILEDICO, dicoTab);										//initialisation de dicoTab + renvoi la taille du dico dans tailleDico
	
	Lettres indexLettre=initialiserLettres();							//initalise la valeur des lettres ainsi que le n,ombre de jetons

	int charger=menu();																//ouvre le menu au joueur
	
	int indexTirage[27];												//initialisation du tableau nécessaire pour le tirage aléatoire
	initIndexTirage(indexTirage, indexLettre);							//variable qui vas stoquer l'emplacement de la lettre
	
	char mains[2][7];											//Tableau qui vas stocker les cartes

	initContenuPlateau(contenu_plateau);								//initialise le charactère par défaut dans le tableau
	initPoints(contenu_plateau);
	
	int scores[2];

	afficher_plateau();

	int numCoup=0;
	int joueur = 1;

	if(!charger)
		initMainJoueur(mains, indexTirage, &indexLettre);

	else
		{
		chargerSauvegarde(contenu_plateau, mains, scores, &joueur);
		numCoup++;
		}
		
	Point case_main_joueur[2] = {{26, 272},{926, 272}};
	
	Point emplacement_lettre_selectionnee;
	char lettre_selectionnee;
	Point emplacement_lettre;
	Point emplacement_lettre_old[TAILLE_PLATEAU]={0};
	Point lettres_placees[7]={0};

	int comptLettre=0;
	int tourBoucle=0;
	
	afficherMainJoueur(case_main_joueur[joueur-1], mains, 1);
	cacherMainJoueur(case_main_joueur[joueur%2],2);

	while(1)
		{
		do
			{
			while(1)
				{
				switchValiderPasser(numCoup, comptLettre);
				if(tourBoucle)			//si la validation n'est pas ok
					{
					reinitTour(emplacement_lettre_old, lettres_placees, contenu_plateau, mains, joueur, case_main_joueur[joueur-1]);
					tourBoucle=0;
					comptLettre=0;
					}
				emplacement_lettre_selectionnee = attendreSelectionLettre(joueur, lettres_placees, numCoup, comptLettre);
				if(clicBouton(emplacement_lettre_selectionnee)==1 || clicBouton(emplacement_lettre_selectionnee)==3)
					break;
				else if(clicBouton(emplacement_lettre_selectionnee)==2)
					{
					piocher(mains, &indexLettre, indexTirage, joueur);
					break;
					}
				lettres_placees[comptLettre]=emplacement_lettre_selectionnee;
				lettre_selectionnee = selectionLettre(joueur, mains, emplacement_lettre_selectionnee);
				emplacement_lettre = attendrePlacerLettre(contenu_plateau, comptLettre, emplacement_lettre_old, numCoup);
				updateContenuPlateau(contenu_plateau, emplacement_lettre, lettre_selectionnee);
				placerLettre(contenu_plateau, lettre_selectionnee, emplacement_lettre, emplacement_lettre_selectionnee);
				
				comptLettre++;
				}
			tourBoucle++;
			} while (!(lireMots(mot, contenu_plateau, dicoTab, nbMotDico)));
		scores[joueur-1]=score(scores[joueur-1], emplacement_lettre_old, contenu_plateau, indexLettre);
		cacherMainJoueur(case_main_joueur[joueur-1], joueur);
		updateMainJoueur(mains, joueur, lettres_placees, &indexLettre, indexTirage);
		joueur=(joueur%2)+1;
		if(clicBouton(emplacement_lettre_selectionnee)==3)
			{
			sauvegarder(contenu_plateau, mains, scores, joueur);
			fermer_fenetre();
			return 0;
			}
		razAnciennesLettres(emplacement_lettre_old, lettres_placees);
		comptLettre=0;
		tourBoucle=0;
		numCoup++;

		afficherMainJoueur(case_main_joueur[joueur-1], mains, joueur);
		}
	attendre_clic();									
	fermer_fenetre();
	return 0;														
	}

/******************************************************************************/
/* MENU									                                      */
/******************************************************************************/
int menu ()															//fonction qui vas s'occuper de traiter les infos recues durant l'affichage du menu
	{
	Point clic = {0,0};
	Point pos_souris ={0,0};
	int choix=0;
	
	afficher_image("./Images/menu.bmp", clic);
	
	do 	{
		do 	{
			reinitialiser_evenements();
			attente(50);
			traiter_evenements();

			pos_souris=deplacement_souris_a_eu_lieu();						//on regarde où est la souris
			survol(pos_souris);												//animation en fonction de là où est la souris
			
			clic = clic_a_eu_lieu();										//on prends le clic de l'utilisateur si clic il y a eu
			}while( ( (clic.y < 700 || clic.y > 800) || ( (clic.x < 108 || clic.x > 400) && (clic.x < 600 || clic.x > 890) ) ) && (clic.y > 921 || clic.y < 821 || clic.x < 354 || clic.x > 643 ) );

		choix=traitement_choix(clic);										//on regarde sur quel bouton l'utilisateur a cliqué

		if(choix==2)
			printf("\n\n\nValeurs et nombres de lettres du Scrabble :\n\nLa lettre A	Valeur : 1 point	Nombre : 9 pièces\nLa lettre B	Valeur : 3 points	Nombre : 2 pièces\nLa lettre C	Valeur : 3 points	Nombre : 2 pièces\nLa lettre D	Valeur : 2 points	Nombre : 3 pièces\nLa lettre E	Valeur : 1 point	Nombre : 15 pièces\nLa lettre F	Valeur : 4 points	Nombre : 2 pièces\nLa lettre G	Valeur : 2 points	Nombre : 2 pièces\nLa lettre H	Valeur : 4 points	Nombre : 2 pièces\nLa lettre I	Valeur : 1 point	Nombre : 8 pièces\nLa lettre J	Valeur : 8 points	Nombre : 1 pièce\nLa lettre K	Valeur : 10 points	Nombre : 1 pièce\nLa lettre L	Valeur : 1 point	Nombre : 5 pièces\nLa lettre M	Valeur : 2 points	Nombre : 3 pièces\nLa lettre N	Valeur : 1 point	Nombre : 6 pièces\nLa lettre O	Valeur : 1 point	Nombre : 6 pièces\nLa lettre P	Valeur : 3 points	Nombre : 2 pièces\nLa lettre Q	Valeur : 8 points	Nombre : 1 pièce\nLa lettre R	Valeur : 1 point	Nombre : 6 pièces\nLa lettre S	Valeur : 1 point	Nombre : 6 pièces\nLa lettre T	Valeur : 1 point	Nombre : 6 pièces\nLa lettre U	Valeur : 1 point	Nombre : 6 pièces\nLa lettre V	Valeur : 4 points	Nombre : 2 pièces\nLa lettre W	Valeur : 10 points	Nombre : 1 pièce\nLa lettre X	Valeur : 10 points	Nombre : 1 pièce\nLa lettre Y	Valeur : 10 points	Nombre : 1 pièce\nLa lettre Z	Valeur : 10 points	Nombre : 1 pièce\nJokers	Valeur : 0 point	Nombre : 2 pièces\n\n\n\nLes différentes valeurs des cases :\n\n    Case bleu ciel : Lettre compte double\n    Case bleu foncé : Lettre compte triple\n    Case rose : Mot compte double\n    Case rouge : Mot compte triple\n\n\n\nCommencer une partie de Scrabble :\n\nPour commencer, chaque joueur va tirer au hasard 7 lettres dans le sac. Avec celles-ci, chacun va alors essayer de créer un ou plusieurs mots. Le premier joueur doit obligatoirement poser le premier mot au centre du plateau, sur l’étoile. Ce mot doit être au minimum composé de 2 lettres. Le deuxième joueur doit s’appuyer sur ce mot pour placer le sien et ainsi de suite…\n\nUn joueur peut échanger ses lettres par d’autres en piochant dans le sac mais cela lui fera obligatoirement passer son tour.\n\n\n\nCalcul des scores du Scrabble :\n\nLe score d’un coup est calculé en additionnant la valeur de toutes les lettres des nouveaux mots formés (y compris celles déjà posées sur la grille). Si l’un des lettres du mot est sur une case bleu ciel, bleu foncé, la valeur doit être calculée. Idem pour les cases rose et rouge correspondant au mot compte double et mot compte triple.\n\nA savoir que si un mot est posé sur 2 cases « compte double » ou 2 cases « compte triple », la valeur du mot est alors multipliée par 4 et par 9.\nAttention ! Chaque case multiplicatrice ne sert qu’une fois!\n\nSi l’un des joueurs arrivent à placer ses 7 lettres d’un seul coup, on dit qu’il a fait un « Scrabble« . Ce coup lui rapporte une prime de 50 points.\n\n\n\nComment gagner au Scrabble :\n\nQuand le sac est vide et qu’un des joueurs pose toutes ses lettres, la partie est terminée. Celui-ci prend alors en bonus la valeur des lettres des autres joueurs. Alors, on fait les totaux de points de chacun des joueurs et celui possédant le plus de points gagne la partie de Scrabble.\n\nBon à savoir : L’idéal lors d’une partie de Scrabble est d’avoir un dictionnaire à portée de main afin de vérifier si le mot posé par l’un des joueurs est valable. Cela permettant d’éviter tout litige. Après vérification, si le mot posé n’existe pas, le joueur reprend toutes ses lettres et marque 0 point pour ce coup.\n");
			
		}while(choix==2);
		
	return choix;
	}

/******************************************************************************/
/* SURVOL									                                  */
/******************************************************************************/
void survol(Point pos_souris)
	{
	Point temp;
	int rafraichir=1;
	
	if(pos_souris.y > 700 && pos_souris.y < 800 && pos_souris.x > 140 && pos_souris.x < 400)
		{
		temp.x=108;
		temp.y=700;
		afficher_image("./Images/jouer_select.bmp", temp);
		}
	else if(pos_souris.y > 700 && pos_souris.y < 800 && pos_souris.x > 600 && pos_souris.x < 890)
		{
		temp.x=600;
		temp.y=700;
		afficher_image("./Images/charger_select.bmp", temp);
		}
	else if(pos_souris.y > 821 && pos_souris.y < 921 && pos_souris.x > 354 && pos_souris.x < 643)
		{
		temp.x=354;
		temp.y=821;
		afficher_image("./Images/regles_select.bmp", temp);
		}
	else if(rafraichir)
		{
		temp.x=0;
		temp.y=0;
		afficher_image("./Images/menu.bmp", temp);
		rafraichir=0;
		}
	rafraichir=1;
	actualiser();
	}

/******************************************************************************/
/* TRAITEMENT CHOIX									                          */
/******************************************************************************/
int traitement_choix(Point clic)
	{
	int choix;
	
	if(clic.y >821)														//bouton règles
		choix=2;
	
	else if(clic.x<400)													//bouton jouer
		choix=0;
		
	else 																//bouton charger
		choix=1;
		
	return choix;
	}

void afficher_plateau()
	{
	Point init={0,0};									
	dessiner_rectangle(init, RESH, RESV, noir);					
	afficher_image("./Images/plateau.bmp", init);
	actualiser();													
	}
/******************************************************************************/
/* INITIALISATION INDEX TIRAGE ALEATOIRE                                      */
/******************************************************************************/
void initIndexTirage(int indexTirage[27], Lettres indexLettre)		//tableau qui vas contenir les données necessaire pour un tirage plus réaliste
	{
	int nbJetonsTot=0;														

	for(int idLettre=0; idLettre<27; idLettre++)
		{
		nbJetonsTot+=indexLettre.nbJetons[idLettre];					//calcule le nombre total de jeton en cumulant le nombre de jetons de chaque lettre
		indexTirage[idLettre]=nbJetonsTot;							//le nombre de jetons cumulé
		}		
	}
	
/******************************************************************************/
/* INITIALISER POINTS	                                                      */
/******************************************************************************/
void initPoints(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2])
	{
	for(int lon=0; lon<TAILLE_PLATEAU; lon++)
		{
		for(int lar=0; lar<TAILLE_PLATEAU; lar++)
			{
			if( ((lon==lar || (14-lon)==lar) && ((lar>0 && lar<5) || (lar>9 && lar<14) || lar==7)) )
				plateau[lar][lon][1]='m';								//mot double
				
			else if( ((lon+7)%7==0 && (lar+7)%7==0) )
				plateau[lar][lon][1]='M';								//mot triple
			 
			else if( (lon==1 || lon==5 || lon==9 || lon==13) && (lar==1 || lar==5 || lar==9 || lar==13) )
				plateau[lar][lon][1]='L';								//lettre triple
				
			else if( ((lon+5)%8==0 && (lar%7==0)) || ((lar+5)%8==0 && (lon%7==0)) || ((lar==6 || lar==8 || lar==2 || lar==12) && (lon==6 || lon==8 || lon==2 || lon==12)))
				plateau[lar][lon][1]='l';								//lettre double

#if DEBUG
			printf("%c",plateau[lar][lon][1]);
#endif
			}
#if DEBUG
		printf("\n");
#endif
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
char tirerLettre(Lettres * indexLettre,int indexTirage[27])				//fonction qui attribue les cartes
	{
	int idLettre=entier_aleatoire(indexTirage[26]);								//102 lettres dans le jeu, pour tirage plus realiste, on pioche parmis les 102 jetons
	int compteur;

	for(compteur=0; compteur<27; compteur++)					//parcourir les valeurs du tableau
		{
		if(idLettre < indexTirage[compteur])							//si le chiffre tiré est inferieur au nombre de jetons cumulés de a jusqu'a la lettre tiré, lui associe l'id de la lettre correspondante
			{
			printf("avant %d\n", idLettre);
			printf("tirage %d", compteur);
			idLettre=compteur;
			printf("apres %d\n", idLettre);
			break;
			}
		}
		
	printf("nb jeton jokair avant retrait : %d\n", (*indexLettre).nbJetons[26]);
	(*indexLettre).nbJetons[idLettre]-=1;
	printf("nb jeton jokair apres retrait : %d\n", (*indexLettre).nbJetons[26]);
	
	actualiser_pioche(indexTirage, compteur);
	
	return (*indexLettre).lettre[idLettre];
	}
	
void actualiser_pioche(int indexTirage[27], int compteur)
	{
	for(;compteur<27;compteur++)
		{
		indexTirage[compteur]-=1;
		printf("index : %d		compteur : %d\n", indexTirage[compteur], compteur);
		}
	}
	
/******************************************************************************/
/* CHARGEDICO                                                                 */
/******************************************************************************/
int chargeDico(char *filedico, char tabdico[SIZEDICO][MAXLENMOT])
	{
    FILE *f = fopen(filedico,"r"); // ouverture du fichier

    if (!f) // si ouverture ratée
        {
        fprintf(stderr,"fopen: problème d'ouverture du fichier '%s'\n'",filedico);
        return 0;
        }

    char line[MAXLENMOT]; // la ligne servant à la lecture du fichier

    int mots=0; // indice dans tabdico 
    while(fgets(line,MAXLENMOT,f)) // tant que fgets ne renvoie pas nul (eof)
        {
        sscanf(line,"%s\n",tabdico[mots]); // je scanne line et écris dans tabdico
        mots++;
        }
    fclose(f); // fermeture du fichier
    
    return mots;
	}


/******************************************************************************/
/* MOT VALABLE                                                                */
/******************************************************************************/
int motValable(char motUtilise[TAILLE_PLATEAU], char dicoTab[SIZEDICO][MAXLENMOT], int nbMotDico)
	{	
	int debut=0, fin=nbMotDico, milieu=(debut+fin)/2;				//initialisation des variables repères pour la recherche dichotomique
	
	while(debut<=fin && strcmp(motUtilise, dicoTab[milieu])!=0)			//tant que le début et la fin ne se croisent pas ET que le mot n'est pas celui cherché
		{
		if(strcmp(motUtilise, dicoTab[milieu])>0)						//tester si le mot est avant ou après
			debut=milieu+1;												//et adapter les variables en conséquence
		else
			fin=milieu-1;
		milieu=(debut+fin)/2;

#if DEBUG
		printf("milieu :%d	debut : %d	fin : %d\n", milieu, debut, fin);
		printf("mot dico : .%s.\nmot à trouver : .%s.",dicoTab[milieu], motUtilise);
		printf("\n");
#endif
		}
		
	if(!strcmp(motUtilise, dicoTab[milieu]))							//on vérifie la raison de sortie de la boucle
		return 1;														//si sorti parceque le mot est trouvé, return 1
	else	
		return 0;														//si aucune correspondance, return 0
	}

/******************************************************************************/
/* DETECTER CASE                                                              */
/******************************************************************************/
Point detecter_case(Point p)
	{
	if(p.x >=(BORDURE + ESPACEMENT) && p.x <= (RESH - (BORDURE + ESPACEMENT)) && p.y >= (BORDURE + ESPACEMENT) && p.y <= (RESV - (BORDURE + ESPACEMENT)))
		{
		p.x = ((p.x - (BORDURE + ESPACEMENT)) - (p.x - (BORDURE + ESPACEMENT)) % (TAILLECASE + BORDURE)) + (BORDURE + ESPACEMENT);
		p.y = ((p.y - (BORDURE + ESPACEMENT)) - (p.y - (BORDURE + ESPACEMENT)) % (TAILLECASE + BORDURE)) + (BORDURE + ESPACEMENT);	
		}
	return p;
	}

/******************************************************************************/
/* POINT DANS TABLEAU ?                                                       */
/******************************************************************************/
int estDansPlateau(Point p)
	{
	if(p.x >= (BORDURE + ESPACEMENT) && p.x <= (RESH - (BORDURE + ESPACEMENT + TAILLECASE)) && p.y >= (BORDURE + ESPACEMENT) && p.y <= (RESV - (BORDURE + ESPACEMENT + TAILLECASE)))
		return 1;	
		
	return 0;
	}

/******************************************************************************/
/* CASE EST LIBRE ?                                                           */
/******************************************************************************/
int CaseEstLibre(Point p, char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2])
	{
	p=convertirEnCaseTableau(p);
	if(plateau[p.y][p.x][0] == ' ')
		return 1;
		
	return 0;
	}

/******************************************************************************/
/* INITIALISATION CONTENU DU TABLEAU                                          */
/******************************************************************************/
void initContenuPlateau(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2])
	{
	int i,j,k;
	
	for(i=0; i<TAILLE_PLATEAU; i++)
		{
		for(j=0; j<TAILLE_PLATEAU; j++)
			{
			for(k=0; k<2; k++)
				plateau[i][j][k] = ' ';
			}
		}
	}

void initMainJoueur(char mains[2][7], int indexTirage[27], Lettres * indexLettre)
	{		
	for(int joueur=1; joueur<=2; joueur++)												
		for(int compteurLettre=0; compteurLettre<7; compteurLettre++)
			mains[joueur-1][compteurLettre]=tirerLettre(indexLettre, indexTirage);
	}
	
void updateMainJoueur(char mains[2][7], int joueur, Point lettres_placees[7], Lettres * indexLettre, int indexTirage[27])
	{		
	for(int compteur=0; compteur<7; compteur++)
		if(lettres_placees[compteur].x && lettres_placees[compteur].y)
			{
			lettres_placees[compteur].y = ((lettres_placees[compteur].y - 272) - (lettres_placees[compteur].y - 272) % 68)/68;
			mains[joueur-1][lettres_placees[compteur].y] = tirerLettre(indexLettre, indexTirage);
			}
	}
	
int lettreDejaPosee(Point lettres_placees[7], Point lettre_selectionnee)
	{
	printf("rentre dans lettre deja placee\n");																//pour éviter de poser 2 fois même lettre
	lettre_selectionnee.y = ((lettre_selectionnee.y - 272) - (lettre_selectionnee.y - 272) % 68)/68;
	int temp;
	for(int compteur=0; compteur<7; compteur++)
		if(lettres_placees[compteur].x && lettres_placees[compteur].y)
			{
			temp = ((lettres_placees[compteur].y - 272) - (lettres_placees[compteur].y - 272) % 68)/68;
			if(temp==lettre_selectionnee.y)
				return 1;
			}
	return 0;
	}

/******************************************************************************/
/* UPDATE CONTENU PLATEAU                                                     */
/******************************************************************************/
void updateContenuPlateau(char plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2], Point p, char lettre_selectionnee)
	{
	p=convertirEnCaseTableau(p);
	
	plateau[p.y][p.x][0] = lettre_selectionnee;
	
#if DEBUG
	for(int b1=0; b1<TAILLE_PLATEAU; b1++){
		for(int b2=0; b2<TAILLE_PLATEAU; b2++){
			printf("%c", plateau[b1][b2][0]);
		}
		printf("\n");
	}
#endif
	}

/******************************************************************************/
/* AFFICHER MAIN JOUEUR                                                         */
/******************************************************************************/
void afficherMainJoueur(Point pos_case, char mains[2][7], int joueur)
	{
	int i;
	char nom_lettre;
	char lettre[16];
	for(i=0; i<7; i++)
		{
		nom_lettre = mains[joueur-1][i];
		sprintf(lettre,"./Images/%c.bmp", nom_lettre);
		afficher_image(lettre, pos_case);
		actualiser();
		pos_case.y+=(TAILLECASE+20);
		}
	}

/******************************************************************************/
/* CACHER MAIN JOUEUR                                                         */
/******************************************************************************/
void cacherMainJoueur(Point pos_case, int joueur)
	{
	int i;
	for(i=0; i<7; i++)
		{
		afficher_image("./Images/cacher_jeu.bmp", pos_case);
		entourerCase(pos_case, noir);
		pos_case.y+=(TAILLECASE+20);
		}
	actualiser();
	}
	
/******************************************************************************/
/* SELECTION LETTRE                                                           */
/******************************************************************************/
char selectionLettre(int joueur, char mains[2][7], Point p)
	{
	char lettre_selectionnee;

	if(joueur == 1)
		{
		p.x = ((p.x - 26) - (p.x - 26) % 53) + 26;
		p.y = ((p.y - 272) - (p.y - 272) % 68) + 272;
		lettre_selectionnee = mains[joueur-1][(p.y-272)/68];
		entourerCase(p, rouge);
		}
	else if (joueur ==2)
		{
		p.x = ((p.x - 926) - (p.x - 926) % 53) + 926;
		p.y = ((p.y - 272) - (p.y - 272) % 68) + 272;
		lettre_selectionnee = mains[joueur-1][(p.y-272)/68];
		entourerCase(p, rouge);
		}

#if DEBUG
	printf("--------------------------------\n");
	printf("%d %d\n", p.x, p.y);
	printf("--------------------------------\n");
#endif
	

	return lettre_selectionnee;
	}

/******************************************************************************/
/* ENTOURE LA CASE EN NOIR OU ROUGE                                           */
/******************************************************************************/
void entourerCase(Point p, Couleur couleur)
	{
	Point r1 = {(p.x-5),(p.y-5)};
	Point r2 = {(p.x-5),(p.y-5)};
	Point r3 = {(p.x+48),(p.y-5)};
	Point r4 = {(p.x-5),(p.y+48)};

	dessiner_rectangle(r1, 53, 5, couleur);
	dessiner_rectangle(r2, 5, 53, couleur);
	dessiner_rectangle(r3, 5, 58, couleur);
	dessiner_rectangle(r4, 53, 5, couleur);
	actualiser();
	}

/******************************************************************************/
/* CLIC DANS LA MAIN DU JOUEUR ?                                              */
/******************************************************************************/
int estDansMainJoueur(Point p, int joueur)
	{
	if(joueur == 1)
		{
		if(p.x>=26 && p.x<=74 && ( (p.y>=272 && p.y<=320) || (p.y>=340 && p.y<=388) || (p.y>=408 && p.y<=456) || (p.y>=476 && p.y<=524) || (p.y>=544 && p.y<=592) || (p.y>=612 && p.y<=660) || (p.y>=680 && p.y<=728) ))
			return 1;
		}
	if(joueur == 2)
		{
		if(p.x>=926 && p.x<=974 && ( (p.y>=272 && p.y<=320) || (p.y>=340 && p.y<=388) || (p.y>=408 && p.y<=456) || (p.y>=476 && p.y<=524) || (p.y>=544 && p.y<=592) || (p.y>=612 && p.y<=660) || (p.y>=680 && p.y<=728) ))
			return 1;
		}
	return 0;
	}
	
/******************************************************************************/
/* AUTORISE A PLACER ?                                                        */
/******************************************************************************/
int peutPlacer(char contenu_plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2], Point clic)
	{	
	clic=convertirEnCaseTableau(clic);

	int compteur=0;
	
	for(int verif=(-1); verif<2;verif+=2)
		{
		if( (clic.y+verif<=14 && clic.y+verif>=0) && contenu_plateau[clic.y+verif][clic.x][0]!=' ')
			compteur++;
		if( (clic.x+verif<=14 && clic.x+verif>=0) && (contenu_plateau[clic.y][clic.x+verif][0]!=' ') )
			compteur++;
		}
	
	if(!compteur)
		return 0;
	
	return 1;
	}

/******************************************************************************/
/* PLACER LETTRE SUR LE PLATEAU                                               */
/******************************************************************************/
void placerLettre(char contenu_plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2], char lettre_selectionnee, Point p_plateau, Point p_main)
	{
	char chemin[16];

	sprintf(chemin, "./Images/%c.bmp", lettre_selectionnee);
	afficher_image(chemin, p_plateau);
	dessiner_rectangle(p_main, 48, 48, blanc);
	entourerCase(p_main, noir);
	actualiser();
	}

/******************************************************************************/
/* ATTENDRE SELECTION LETTRE DANS LA MAIN                                     */
/******************************************************************************/
Point attendreSelectionLettre(int joueur, Point lettres_placees[7], int numCoup, int comptLettres)
	{
	Point p;

	do
		{
		p = attendre_clic();
		} while( (estDansMainJoueur(p, joueur) == 0 || lettreDejaPosee(lettres_placees, p)==1) && !(clicBouton(p) && ((numCoup==0 && comptLettres>=2) || numCoup)) );

	if(!((p.x>385 && p.x<616) && (p.y>920 && p.y<990)) && !clicBouton(p))
		{
		p.y = (p.y-272) - ((p.y-272) % 68) + 272;

		if(joueur == 1)
			p.x = 26;
		else
			p.x = 926;
		}

	return p;
	}

/******************************************************************************/
/* ATTENDRE PLACER LETTRE                                                     */
/******************************************************************************/
Point attendrePlacerLettre(char contenu_plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2], int comptLettre, Point emplacement_lettre_old[TAILLE_PLATEAU], int num_coup)
	{
	Point emplacement_lettre;
	printf("comptlettre dans fonction attendre placer a : %d et au coup :%d\n", comptLettre, num_coup);
	if(num_coup==0 && comptLettre==0)
		{
		Point caseDansTableau;
		do
			{															//tant que le clic n'est pas dans le tableau ou que la case choisie n'est pas libre,
			emplacement_lettre=attendre_clic();								//on attend un clic
			emplacement_lettre= detecter_case(emplacement_lettre);			//on detecte la case sur laquelle est le clic
			emplacement_lettre_old[comptLettre]=emplacement_lettre;
			caseDansTableau=convertirEnCaseTableau(emplacement_lettre);
			printf("emplacement lettre : x:%d y:%d\n", caseDansTableau.x, caseDansTableau.y);
			} while (caseDansTableau.x!=7 || caseDansTableau.y!=7);
		return emplacement_lettre;
		}
	if(comptLettre<2)
		{
		printf("dans boucle <2\n");
		do
			{															//tant que le clic n'est pas dans le tableau ou que la case choisie n'est pas libre,
			emplacement_lettre=attendre_clic();								//on attend un clic
			emplacement_lettre= detecter_case(emplacement_lettre);			//on detecte la case sur laquelle est le clic
			emplacement_lettre_old[comptLettre]=emplacement_lettre;
			printf("emplacement lettre : %d\n", emplacement_lettre_old[comptLettre].x);
			} while (estDansPlateau(detecter_case(emplacement_lettre)) == 0 || CaseEstLibre(detecter_case(emplacement_lettre), contenu_plateau) == 0 || peutPlacer(contenu_plateau, emplacement_lettre)==0);
		return emplacement_lettre;
		}
		
	do
		{															//tant que le clic n'est pas dans le tableau ou que la case choisie n'est pas libre,
		emplacement_lettre=attendre_clic();								//on attend un clic
		emplacement_lettre= detecter_case(emplacement_lettre);			//on detecte la case sur laquelle est le clic
		emplacement_lettre_old[comptLettre]=emplacement_lettre;
		printf("emplacement lettre : %d\n", emplacement_lettre_old[comptLettre].x);
		} while (estDansPlateau(detecter_case(emplacement_lettre)) == 0 || CaseEstLibre(detecter_case(emplacement_lettre), contenu_plateau) == 0 || peutPlacer(contenu_plateau, emplacement_lettre)==0 || compDirection(emplacement_lettre_old[0], emplacement_lettre_old[1], emplacement_lettre_old[comptLettre])==0);
	return emplacement_lettre;
	}

Point convertirEnCaseTableau(Point clic)
	{
	clic.x=((clic.x-(BORDURE + ESPACEMENT))/(TAILLECASE + BORDURE));
	clic.y=((clic.y-(BORDURE + ESPACEMENT))/(TAILLECASE + BORDURE));
	
	return clic;
	}
	
void razAnciennesLettres(Point emplacement_lettre_old[TAILLE_PLATEAU], Point lettres_placees[7])
	{
	for(int raz=0;raz<TAILLE_PLATEAU;raz++)
		{
		emplacement_lettre_old[raz].x=0;
		emplacement_lettre_old[raz].y=0;
		}
	for(int raz=0;raz<7;raz++)
		{
		lettres_placees[raz].x=0;
		lettres_placees[raz].y=0;
		}
	}
	
char compDirection(Point lettre1, Point lettre2, Point lettre3)
	{
	printf("appel a la fonction compdirection\n coordonnées des lettres : lettre1.x : %d lettre2.x : %d\n", lettre1.x, lettre2.x);
	if( ((lettre1.x==lettre3.x) || (lettre1.y==lettre3.y)) && ((lettre2.x==lettre3.x) || (lettre2.y==lettre3.y)) )
		return 1;
	return 0;
	}

int lireMots(char mot[TAILLE_PLATEAU], char contenu_plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2], char dicoTab[SIZEDICO][MAXLENMOT], int nbMotDico)
	{
	int a, i, j, k, l, m=0;
	int mot_mauvais = 0;

	for(a=0; a<TAILLE_PLATEAU; a++)
		mot[a] = '\0';

	for(i=0; i<TAILLE_PLATEAU; i++)
		{
		for(j=0; j<TAILLE_PLATEAU; j++)
			{
			if(contenu_plateau[i][j][0] != ' ')
				{
				mot[m] = contenu_plateau[i][j][0];
				m++;

				if(contenu_plateau[i][j+1][0] == ' ')
					{
					printf("%s\n", mot);
					if(motValable(mot, dicoTab, nbMotDico) == 0)
						mot_mauvais++;

					for(a=0; a<TAILLE_PLATEAU; a++)
						mot[a] = '\0';
					m=0;
					}
				}
			}
		}
	for(a=0; a<TAILLE_PLATEAU; a++)
		mot[a] = '\0';
	m=0;
		
	for(k=0; k<TAILLE_PLATEAU; k++)
		{
		for(l=0; l<TAILLE_PLATEAU; l++)
			{
			if(contenu_plateau[l][k][0] != ' ')
				{
				printf(".%c.", contenu_plateau[l][k][0]);
				mot[m] = contenu_plateau[l][k][0];
				m++;

				if(contenu_plateau[l+1][k][0] == ' ')
					{
					printf("%s\n", mot);
					if(motValable(mot, dicoTab, nbMotDico) == 0)
						mot_mauvais++;

					for(a=0; a<TAILLE_PLATEAU; a++)
						mot[a] = '\0';
					m=0;
					}
				}
			}
		}
	printf("%d\n", mot_mauvais);

	if(mot_mauvais!=0)
		return 0;

	return 1;

	}

void reinitTour(Point emplacement_lettre_old[7], Point lettres_placees[7], char contenu_plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2], char mains[2][7], int joueur, Point case_main_joueur)
	{
	for(int compteur=0; compteur<7; compteur++)
		if(emplacement_lettre_old[compteur].x && emplacement_lettre_old[compteur].y)
			actualiser_plateau(emplacement_lettre_old[compteur], contenu_plateau);
	
	afficherMainJoueur(case_main_joueur, mains, joueur);
	razAnciennesLettres(emplacement_lettre_old, lettres_placees);
	}

void actualiser_plateau(Point emplacement_lettre_old, char contenu_plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2])
	{
	Point case_tableau_old;
	case_tableau_old=convertirEnCaseTableau(emplacement_lettre_old);
	
	if(case_tableau_old.y==7 && case_tableau_old.x==7)
		afficher_image("./Images/case_centre.bmp", emplacement_lettre_old);
	else if(contenu_plateau[case_tableau_old.y][case_tableau_old.x][1]=='m')
		afficher_image("./Images/case_mot_compte_double.bmp", emplacement_lettre_old);
	else if(contenu_plateau[case_tableau_old.y][case_tableau_old.x][1]=='M')
		afficher_image("./Images/case_mot_compte_triple.bmp", emplacement_lettre_old);
	else if(contenu_plateau[case_tableau_old.y][case_tableau_old.x][1]=='l')
		afficher_image("./Images/case_lettre_compte_double.bmp", emplacement_lettre_old);
	else if(contenu_plateau[case_tableau_old.y][case_tableau_old.x][1]=='L')
		afficher_image("./Images/case_lettre_compte_triple.bmp", emplacement_lettre_old);
	else
		afficher_image("./Images/cacher_jeu.bmp", emplacement_lettre_old);
		
	contenu_plateau[case_tableau_old.y][case_tableau_old.x][0]=' ';
	}

void sauvegarder(char contenu_plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2], char mains[2][7], int scores[2], int joueur)
	{
	int compteurDim1;
	int compteurDim2;
	
	FILE* save_scores=fopen("Save/save.scores_tourJoueur", "w");
	if(!save_scores)
        fprintf(stderr,"fopen: problème d'ouverture du fichier scores");
	else
		{
		for(compteurDim1=0; compteurDim1<2; compteurDim1++)
			fprintf(save_scores, "%d,", scores[compteurDim1]);
		fprintf(save_scores, "%d", joueur);
		fclose(save_scores);
		}
		
	FILE* save_mains=fopen("Save/save.mains", "w");
	if(!save_mains)
        fprintf(stderr,"fopen: problème d'ouverture du fichier mains");
	else
		{
		for(compteurDim1=0; compteurDim1<2; compteurDim1++)
			for(compteurDim2=0; compteurDim2<7; compteurDim2++)
				fprintf(save_mains, "%c", mains[compteurDim1][compteurDim2]);
		fclose(save_mains);
		}
		
	FILE* save_plateau=fopen("Save/save.plateau", "w");
	if(!save_plateau)
        fprintf(stderr,"fopen: problème d'ouverture du fichier contenu_plateau");
	else
		{
		for(compteurDim1=0; compteurDim1<15; compteurDim1++)
			for(compteurDim2=0; compteurDim2<15; compteurDim2++)
				fprintf(save_plateau, "%c", contenu_plateau[compteurDim2][compteurDim1][0]);
		fclose(save_plateau);
		}
	}

void chargerSauvegarde(char contenu_plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2], char mains[2][7], int scores[2], int * joueur)
	{
	int compteurDim1;
	int compteurDim2;
	char contenu[300];
	
	FILE* save_scores=fopen("./Save/save.scores_tourJoueur", "r");
	if(!save_scores)
        fprintf(stderr,"fopen: problème d'ouverture du fichier scores");
	else
		{
		fscanf(save_scores, "%d,%d,%d", &scores[0], &scores[1], joueur);
		fclose(save_scores);
		}
		
	FILE* save_mains=fopen("./Save/save.mains", "r");
	if(!save_mains)
        fprintf(stderr,"fopen: problème d'ouverture du fichier mains");
	else
		{
		fgets(contenu, 7*2, save_mains);
		fclose(save_mains);
		}
	for(compteurDim1=0; compteurDim1<2; compteurDim1++)
		for(compteurDim2=0; compteurDim2<7; compteurDim2++)
			mains[compteurDim1][compteurDim2]=contenu[compteurDim2+(compteurDim1*7)];
		
	FILE* save_plateau=fopen("./Save/save.plateau", "r");
	if(!save_plateau)
        fprintf(stderr,"fopen: problème d'ouverture du fichier contenu_plateau");
	else
		{
		fgets(contenu, TAILLE_PLATEAU*TAILLE_PLATEAU, save_plateau);
		fclose(save_plateau);
		}	
	for(compteurDim1=0; compteurDim1<TAILLE_PLATEAU; compteurDim1++)
		for(compteurDim2=0; compteurDim2<TAILLE_PLATEAU; compteurDim2++)
			contenu_plateau[compteurDim2][compteurDim1][0]=contenu[compteurDim2+(compteurDim1*TAILLE_PLATEAU)];
			
	updatePlateauSave(contenu_plateau);
	}

void updatePlateauSave(char contenu_plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2])
	{
	char chemin[16];
	Point pos={0,0};
	
	for(int compteurDim1=0; compteurDim1<TAILLE_PLATEAU; compteurDim1++)
		{
		for(int compteurDim2=0; compteurDim2<TAILLE_PLATEAU; compteurDim2++)
			{
			if(contenu_plateau[compteurDim1][compteurDim2][0]!=' ')
				{
				sprintf(chemin, "./Images/%c.bmp", contenu_plateau[compteurDim1][compteurDim2][0]);
				pos.y=(ESPACEMENT+BORDURE)+(compteurDim1*(TAILLECASE+BORDURE));
				pos.x=(ESPACEMENT+BORDURE)+(compteurDim2*(TAILLECASE+BORDURE));
				afficher_image(chemin, pos);
				}
			}
		}
	actualiser();
	}
	
int clicBouton(Point clic)
	{
	if(clic.y>919 && clic.y<989)
		{
		if(clic.x>100 && clic.x<265)			//valider ou passser
			return 1;
		else if(clic.x>275 && clic.x<440)		//piocher
			return 2;
		else if(clic.x>670 && clic.x<900)		//sauver
			return 3;
		}
	return 0;
	}

int score(int scoreJoueur, Point emplacement_lettre_old[7], char contenu_plateau[TAILLE_PLATEAU][TAILLE_PLATEAU][2], Lettres indexLettre)
	{
	char lettre;
	
	int compLettre;
	int compteur;
	
	int multLettre=0;
	int multMot=0;
	int scoreTour=0;
	
	for(compteur=0; compteur<7 && emplacement_lettre_old[compteur].x; compteur++)
		{
		emplacement_lettre_old[compteur]=convertirEnCaseTableau(emplacement_lettre_old[compteur]);
		lettre=contenu_plateau[emplacement_lettre_old[compteur].y][emplacement_lettre_old[compteur].x][0];
		for(compLettre=0; compLettre<27 && lettre!=indexLettre.lettre[compLettre]; compLettre++){}
		multLettre=multiplicateurLettre(contenu_plateau[emplacement_lettre_old[compteur].y][emplacement_lettre_old[compteur].x][1]);
		multMot+=multiplicateurMot(contenu_plateau[emplacement_lettre_old[compteur].y][emplacement_lettre_old[compteur].x][1]);
		scoreTour+=(indexLettre.valeur[compLettre]*multLettre);
		printf("DANS SCORE :	Lettre : %c 		valeur : %d		multLettre : %d		multMot : %d\n", lettre, indexLettre.valeur[compLettre], multLettre, multMot);
		}
	if(!multMot)
		multMot++;
	scoreJoueur+=scoreTour*multMot;
	if(compteur==7)
		scoreJoueur+=50;
	printf("score joueur : %d\n", scoreJoueur);
	return scoreJoueur;
	}

int multiplicateurLettre(char codeMultLettre)
	{
	if(codeMultLettre=='L')
		return 3;
	else if(codeMultLettre=='l')
		return 2;
	return 1;
	}

int multiplicateurMot(char codeMultMot)
	{
	if(codeMultMot=='M')
		return 3;
	else if(codeMultMot=='m')
		return 2;
	return 0;
	}



void switchValiderPasser(int numCoup, int comptLettre)
	{
	Point afficherBouton={100,919};
	if(comptLettre==0 && numCoup)
		afficher_image("./Images/bouton_passer.bmp", afficherBouton);
	else if(comptLettre==1 || !numCoup)
		afficher_image("./Images/bouton_valider.bmp", afficherBouton);
	actualiser();
	}
	
void piocher(char mains[2][7], Lettres * indexLettre, int indexTirage[27], int joueur)
	{
	Point lettre_a_changer[7];
	int compteur;
	
	switchValiderPasser(1, 1);
	
	printf("Selectionnez les lettres que vous souhaitez changer puis appuyez sur valider\n");
	
	for(compteur=0; compteur<7; compteur++)
		{
		lettre_a_changer[compteur].x=0;
		lettre_a_changer[compteur].y=0;
		}
	
	for(compteur=0; compteur<7 && clicBouton(lettre_a_changer[compteur-1])!=1; compteur++)
		{
		do{
			lettre_a_changer[compteur]=attendreSelectionLettre(joueur, lettre_a_changer, 1, 0);
			}while(clicBouton(lettre_a_changer[compteur])==2 || clicBouton(lettre_a_changer[compteur])==3);
		if(clicBouton(lettre_a_changer[compteur])!=1)
			selectionLettre(joueur, mains, lettre_a_changer[compteur]);
		}
	updateMainJoueur(mains, joueur, lettre_a_changer, indexLettre, indexTirage);
	}





//clic sur le bouton piocher
//clic sur les lettres à changer
//clic sur valide
//pioche les lettres
//fin de tour normale





