#include <stdio.h>
#include <string.h>
#include "lib/libgraphique.h"

#define RESH 1000														//resolution de la fenetre
#define RESV 1000

#define BORDURE 5	
#define ESPACEMENT 100													//Bordures entre cases
#define TAILLECASE 48															//Taille des cases

#define FILEDICO "./dictionnaire_fr_ss_accents" // chemin du fichier dico
#define SIZEDICO 319000                         // taille large du dico
#define MAXLENMOT 26                            // taille du mot max

#define DEBUG 1

typedef struct{															//structure qui contiens :
	char lettre[27];													//lettres
	int valeur[27];														//leur valeur
	int nbJetons[27];													//le nombre restant dans le jeu
	}Lettres;

void menu();
void survol(Point);
int traitement_choix(Point);

void initPoints(char plateau[15][15][2]);

void initIndexTirage(int indexTirage[27], Lettres);
Lettres initialiserLettres();

char tirerLettre(Lettres, int indexTirage[27]);
int chargeDico(char *filedico, char tabdico[SIZEDICO][MAXLENMOT]);
int motValable(char motUtilise[] ,char dicoTab[SIZEDICO][MAXLENMOT], int);

Point detecter_case(Point);												/************************/									
int estDansPlateau(Point);													/* Placer les lettres	*/
int CaseEstLibre(Point, char plateau[15][15][2]);							/************************/	
int peutPlacer(char contenu_plateau[15][15][2], Point, int autours[4]);

Point attendreSelectionLettre(int);
char selectionLettre(int, char mains[2][7], Point);
void placerLettre(char contenu_plateau[15][15][2], char, Point);
Point attendrePlacerLettre(char contenu_plateau[15][15][2], int autours[4]);
void entourerCase(Point, Couleur);
int estDansMainJoueur(Point, int);

void initContenuPlateau(char plateau[15][15][2]);						//fonctions en relation avec la plateau
void initMainJoueur(char mains[2][7], int indextirage[27], Lettres);
void updateContenuPlateau(char plateau[15][15][2], Point, char);					//

void afficherMainJoueur(Point, char mains[2][7], int);
void cacherMainJoueur(Point pos_case, int joueur);

Point convertirEnCaseTableau(Point clic);

int clicBoutonValide(Point);
int recupMot(Point, int autours[4], char plateau[15][15][2]);


/******************************************************************************/
/* MAIN		                                                                  */
/******************************************************************************/
int main()
	{
	ouvrir_fenetre(RESH, RESV);											//ouvre la session graphique
	
	char contenu_plateau[15][15][2];									//tableau qui contiens le contenu du plateau ainsi que les valeurs double ou triple des mots / lettres
	initContenuPlateau(contenu_plateau);								//initialise le charactère par défaut dans le tableau
	
	Lettres indexLettre=initialiserLettres();							//initalise la valeur des lettres ainsi que le n,ombre de jetons
	
	char dicoTab[SIZEDICO][MAXLENMOT];									//Le dictionnaire
	int nbMotDico=chargeDico(FILEDICO, dicoTab);										//initialisation de dicoTab + renvoi la taille du dico dans tailleDico
		
	int indexTirage[27];												//initialisation du tableau nécessaire pour le tirage aléatoire
	initIndexTirage(indexTirage, indexLettre);							//variable qui vas stoquer l'emplacement de la lettre

	char mains[2][7]={{' '}};											//Tableau qui vas stocker les cartes
	initMainJoueur(mains, indexTirage, indexLettre);
	
	initPoints(contenu_plateau);

	int autours[4] = {0};
	
#if DEBUG
	for(int compteur=0; compteur < 27; compteur++)
		printf("lettre:%c	valeur:%d	nbJetons:%d\n", indexLettre.lettre[compteur], indexLettre.valeur[compteur], indexLettre.nbJetons[compteur]);
#endif
#if DEBUG
	int test=motValable("zythums", dicoTab, nbMotDico);									//test du dico
	printf("%d", test);
#endif
	menu();																//ouvre le menu au joueur

	Point init={0,0};													/**********************/
	dessiner_rectangle(init, RESH, RESV, noir);							/*					  */
	afficher_image("./Images/plateau.bmp", init);						/*affichage du plateau*/
	actualiser();														/**********************/

	Point case_main_joueur[2] = {{26, 272},{926, 272}};
	
	afficherMainJoueur(case_main_joueur[0], mains, 1);
	cacherMainJoueur(case_main_joueur[1],2);
	actualiser();
	
	Point emplacement_lettre_selectionnee;
	char lettre_selectionnee;
	Point emplacement_lettre;
	Point emplacement_lettre_old;
	Point emplacement_lettre_tab;

	int joueur = 1;
	int comptLettre=1;

	do
		{
		emplacement_lettre_selectionnee = attendreSelectionLettre(joueur);							//premier tour
		lettre_selectionnee = selectionLettre(joueur, mains, emplacement_lettre_selectionnee);
		emplacement_lettre = attendre_clic();
		emplacement_lettre_tab = convertirEnCaseTableau(emplacement_lettre);
		} while(emplacement_lettre_tab.x != 7 && emplacement_lettre_tab.y != 7);
	emplacement_lettre=detecter_case(emplacement_lettre);
	updateContenuPlateau(contenu_plateau, emplacement_lettre, lettre_selectionnee);
	placerLettre(contenu_plateau, lettre_selectionnee, emplacement_lettre);


	while(1)
		{
		while(1)
			{
			afficherMainJoueur(case_main_joueur[joueur-1], mains, joueur);
			actualiser();
			
			emplacement_lettre_selectionnee = attendreSelectionLettre(joueur);
			if(clicBoutonValide(emplacement_lettre_selectionnee)==1 && comptLettre>=1 /*&& recupMot(emplacement_lettre_old, autours, contenu_plateau)==1*/)
				break;
				
			lettre_selectionnee = selectionLettre(joueur, mains, emplacement_lettre_selectionnee);
			emplacement_lettre = attendrePlacerLettre(contenu_plateau, autours);
			updateContenuPlateau(contenu_plateau, emplacement_lettre, lettre_selectionnee);
			placerLettre(contenu_plateau, lettre_selectionnee, emplacement_lettre);
			
			comptLettre++;
			emplacement_lettre_old=emplacement_lettre;
			}
		cacherMainJoueur(case_main_joueur[joueur-1], joueur);
			
		if(joueur == 1)
			joueur ++;
		else
			joueur=1;

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

	attendre_clic();									
	fermer_fenetre();
	return 0;														
	}

/******************************************************************************/
/* MENU									                                      */
/******************************************************************************/
void menu ()															//fonction qui vas s'occuper de traiter les infos recues durant l'affichage du menu
	{
	Point clic = {0,0};
	Point pos_souris ={0,0};
	int choix=0;
	
	afficher_image("./Images/menu.bmp", clic);
	
	do 	{
		do 	{
			reinitialiser_evenements();
			traiter_evenements();

			pos_souris=deplacement_souris_a_eu_lieu();						//on regarde où est la souris
			survol(pos_souris);												//animation en fonction de là où est la souris
			
			clic = clic_a_eu_lieu();										//on prends le clic de l'utilisateur si clic il y a eu
			}while( ( (clic.y < 700 || clic.y > 800) || ( (clic.x < 108 || clic.x > 400) && (clic.x < 600 || clic.x > 890) ) ) && (clic.y > 921 || clic.y < 821 || clic.x < 354 || clic.x > 643 ) );

		choix=traitement_choix(clic);										//on regarde sur quel bouton l'utilisateur a cliqué

		if(choix==3)
			printf("\n\n\nValeurs et nombres de lettres du Scrabble :\n\nLa lettre A	Valeur : 1 point	Nombre : 9 pièces\nLa lettre B	Valeur : 3 points	Nombre : 2 pièces\nLa lettre C	Valeur : 3 points	Nombre : 2 pièces\nLa lettre D	Valeur : 2 points	Nombre : 3 pièces\nLa lettre E	Valeur : 1 point	Nombre : 15 pièces\nLa lettre F	Valeur : 4 points	Nombre : 2 pièces\nLa lettre G	Valeur : 2 points	Nombre : 2 pièces\nLa lettre H	Valeur : 4 points	Nombre : 2 pièces\nLa lettre I	Valeur : 1 point	Nombre : 8 pièces\nLa lettre J	Valeur : 8 points	Nombre : 1 pièce\nLa lettre K	Valeur : 10 points	Nombre : 1 pièce\nLa lettre L	Valeur : 1 point	Nombre : 5 pièces\nLa lettre M	Valeur : 2 points	Nombre : 3 pièces\nLa lettre N	Valeur : 1 point	Nombre : 6 pièces\nLa lettre O	Valeur : 1 point	Nombre : 6 pièces\nLa lettre P	Valeur : 3 points	Nombre : 2 pièces\nLa lettre Q	Valeur : 8 points	Nombre : 1 pièce\nLa lettre R	Valeur : 1 point	Nombre : 6 pièces\nLa lettre S	Valeur : 1 point	Nombre : 6 pièces\nLa lettre T	Valeur : 1 point	Nombre : 6 pièces\nLa lettre U	Valeur : 1 point	Nombre : 6 pièces\nLa lettre V	Valeur : 4 points	Nombre : 2 pièces\nLa lettre W	Valeur : 10 points	Nombre : 1 pièce\nLa lettre X	Valeur : 10 points	Nombre : 1 pièce\nLa lettre Y	Valeur : 10 points	Nombre : 1 pièce\nLa lettre Z	Valeur : 10 points	Nombre : 1 pièce\nJokers	Valeur : 0 point	Nombre : 2 pièces\n\n\n\nLes différentes valeurs des cases :\n\n    Case bleu ciel : Lettre compte double\n    Case bleu foncé : Lettre compte triple\n    Case rose : Mot compte double\n    Case rouge : Mot compte triple\n\n\n\nCommencer une partie de Scrabble :\n\nPour commencer, chaque joueur va tirer au hasard 7 lettres dans le sac. Avec celles-ci, chacun va alors essayer de créer un ou plusieurs mots. Le premier joueur doit obligatoirement poser le premier mot au centre du plateau, sur l’étoile. Ce mot doit être au minimum composé de 2 lettres. Le deuxième joueur doit s’appuyer sur ce mot pour placer le sien et ainsi de suite…\n\nUn joueur peut échanger ses lettres par d’autres en piochant dans le sac mais cela lui fera obligatoirement passer son tour.\n\n\n\nCalcul des scores du Scrabble :\n\nLe score d’un coup est calculé en additionnant la valeur de toutes les lettres des nouveaux mots formés (y compris celles déjà posées sur la grille). Si l’un des lettres du mot est sur une case bleu ciel, bleu foncé, la valeur doit être calculée. Idem pour les cases rose et rouge correspondant au mot compte double et mot compte triple.\n\nA savoir que si un mot est posé sur 2 cases « compte double » ou 2 cases « compte triple », la valeur du mot est alors multipliée par 4 et par 9.\nAttention ! Chaque case multiplicatrice ne sert qu’une fois!\n\nSi l’un des joueurs arrivent à placer ses 7 lettres d’un seul coup, on dit qu’il a fait un « Scrabble« . Ce coup lui rapporte une prime de 50 points.\n\n\n\nComment gagner au Scrabble :\n\nQuand le sac est vide et qu’un des joueurs pose toutes ses lettres, la partie est terminée. Celui-ci prend alors en bonus la valeur des lettres des autres joueurs. Alors, on fait les totaux de points de chacun des joueurs et celui possédant le plus de points gagne la partie de Scrabble.\n\nBon à savoir : L’idéal lors d’une partie de Scrabble est d’avoir un dictionnaire à portée de main afin de vérifier si le mot posé par l’un des joueurs est valable. Cela permettant d’éviter tout litige. Après vérification, si le mot posé n’existe pas, le joueur reprend toutes ses lettres et marque 0 point pour ce coup.\n");
			
		}while(choix==3);
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
		actualiser();
		rafraichir=1;
		}
	else if(pos_souris.y > 700 && pos_souris.y < 800 && pos_souris.x > 600 && pos_souris.x < 890)
		{
		temp.x=600;
		temp.y=700;
		afficher_image("./Images/charger_select.bmp", temp);
		actualiser();
		rafraichir=1;
		}
	else if(pos_souris.y > 821 && pos_souris.y < 921 && pos_souris.x > 354 && pos_souris.x < 643)
		{
		temp.x=354;
		temp.y=821;
		afficher_image("./Images/regles_select.bmp", temp);
		actualiser();
		rafraichir=1;
		}
	else if(rafraichir==1)
		{
		temp.x=0;
		temp.y=0;
		afficher_image("./Images/menu.bmp", temp);
		actualiser();
		rafraichir=0;
		}
	}

/******************************************************************************/
/* TRAITEMENT CHOIX									                          */
/******************************************************************************/
int traitement_choix(Point clic)
	{
	int choix;
	
	if(clic.y >821)														//bouton règles
		choix=3;
	
	else if(clic.x<400)													//bouton jouer
		choix=1;
		
	else 																//bouton charger
		choix=2;
		
	return choix;
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
void initPoints(char plateau[15][15][2])
	{
	for(int lon=0; lon<15; lon++)
		{
		for(int lar=0; lar<15; lar++)
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
char tirerLettre(Lettres indexLettre, int tirage[27])				//fonction qui attribue les cartes
	{
	int idLettre=0;
	int c=0;

	for(int c=0; c<27; c++)
		{
		printf("tirage indice %d : %d\n", c, tirage[c]);
		}

	do
		{
		idLettre = entier_aleatoire(102);								//102 lettres dans le jeu, pour tirage plus realiste, on pioche parmis les 102 jetons

		for(c=0; c<27; c++)					//parcourir les valeurs du tableau
			{
			if(idLettre < tirage[c])							//si le chiffre tiré est inferieur au nombre de jetons cumulés de a jusqu'a la lettre tiré, lui associe l'id de la lettre correspondante
				{
				printf("avant %d\n", idLettre);
				printf("tirage %d", c);
				idLettre=c;
				printf("apres %d\n", idLettre);
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
int motValable(char motUtilise[], char dicoTab[SIZEDICO][MAXLENMOT], int nbMotDico)
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
		printf("mot dico : %s\nmot à trouver : %s",dicoTab[milieu], motUtilise);
		printf("\n");
#endif
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
int CaseEstLibre(Point p, char plateau[15][15][2])
	{
	p=convertirEnCaseTableau(p);
	if(plateau[p.y][p.x][0] == ' ')
		return 1;
		
	return 0;
	}

/******************************************************************************/
/* INITIALISATION CONTENU DU TABLEAU                                          */
/******************************************************************************/
void initContenuPlateau(char plateau[15][15][2])
	{
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

void initMainJoueur(char mains[2][7], int indexTirage[27], Lettres indexLettre)
	{		
	for(int joueur=1; joueur<=2; joueur++)												
		for(int compteurLettre=0; compteurLettre<7; compteurLettre++)
			mains[joueur-1][compteurLettre]=tirerLettre(indexLettre, indexTirage);
	}

/******************************************************************************/
/* UPDATE CONTENU PLATEAU                                                     */
/******************************************************************************/
void updateContenuPlateau(char plateau[15][15][2], Point p, char lettre_selectionnee)
	{
	p=convertirEnCaseTableau(p);
	plateau[p.y][p.x][0] = lettre_selectionnee;
	}

/******************************************************************************/
/* UPDATE MAIN JOUEUR                                                         */
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
int peutPlacer(char contenu_plateau[15][15][2], Point clic, int autours[4])
	{	
	clic=convertirEnCaseTableau(clic);
	
	for(int c=0; c<4; c++)
		autours[c]=0;
	
	int compteur=0;
	
	for(int verif=(-1); verif<2;verif+=2)
		{
		if( (clic.y+verif<=14 && clic.y+verif>=0) && contenu_plateau[clic.y+verif][clic.x][0]!=' ')
			{
			autours[compteur]=(verif+2);							
			compteur++;													//retourne 1 si lettre en dessous et 3 si lettre au dessus
			}
		if( (clic.x+verif<=14 && clic.x+verif>=0) && (contenu_plateau[clic.y][clic.x+verif][0]!=' ') )
			{
			autours[compteur]=(verif+3);											//retourne 2 si lettre à droite et 4 si lettre à gauche
			compteur++;
			}
		}
	
	if(compteur==0)
		return 0;
	
	return 1;
	}

/******************************************************************************/
/* PLACER LETTRE SUR LE PLATEAU                                               */
/******************************************************************************/
void placerLettre(char contenu_plateau[15][15][2], char lettre_selectionnee, Point p)
	{
	char chemin[16];

	sprintf(chemin, "./Images/%c.bmp", lettre_selectionnee);
	afficher_image(chemin, p);
	actualiser();
	}

/******************************************************************************/
/* ATTENDRE SELECTION LETTRE DANS LA MAIN                                     */
/******************************************************************************/
Point attendreSelectionLettre(int joueur)
	{
	Point p;

	do
		{
		p = attendre_clic();
		} while(estDansMainJoueur(p, joueur) == 0 && clicBoutonValide(p)==0);

	return p;
	}

/******************************************************************************/
/* ATTENDRE PLACER LETTRE                                                     */
/******************************************************************************/
Point attendrePlacerLettre(char contenu_plateau[15][15][2], int autours[4])
	{
	Point emplacement_lettre;

	do
		{															//tant que le clic n'est pas dans le tableau ou que la case choisie n'est pas libre,
		emplacement_lettre=attendre_clic();								//on attend un clic
		emplacement_lettre= detecter_case(emplacement_lettre);			//on detecte la case sur laquelle est le clic
		} while (estDansPlateau(detecter_case(emplacement_lettre)) == 0 || CaseEstLibre(detecter_case(emplacement_lettre), contenu_plateau) == 0 || peutPlacer(contenu_plateau, emplacement_lettre, autours)==0);
	return emplacement_lettre;
	}

/******************************************************************************/
/* BOUTON VALIDER CLIQUÉ ?                                                     */
/******************************************************************************/
int clicBoutonValide(Point clic)
	{
	if((clic.x>385 && clic.x<616) && (clic.y>920 && clic.y<990))
		return 1;
	return 0;
	}

Point convertirEnCaseTableau(Point clic)
	{
	clic.x=((clic.x-(BORDURE + ESPACEMENT))/(TAILLECASE + BORDURE));
	clic.y=((clic.y-(BORDURE + ESPACEMENT))/(TAILLECASE + BORDURE));
	
	return clic;
	}

int recupMot(Point derniereLettre, int autours[4], char plateau[15][15][2])
	{
	derniereLettre=convertirEnCaseTableau(derniereLettre);
	
	for(char caract=plateau[derniereLettre.y][derniereLettre.x][0]; caract!=' ';)
		{
		peutPlacer(plateau, derniereLettre, autours);
		
		
		
		derniereLettre.y++;
		derniereLettre.x++;
		}
	
	return 1;
	}
