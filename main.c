#include <stdio.h>
#include <string.h>
#include "lib/libgraphique.h"

#define RESH 1000														//resolution de la fenetre
#define RESV 1000

#define BORDURE 5	
#define ESPACEMENT 100													//Bordures entre cases
#define TAILLECASE 48															//Taille des cases

#define FILEDICO "./dictionnaire_fr_ss_accents"							// chemin du fichier dico
#define SIZEDICO 319000                         						// taille large du dico
#define MAXLENMOT 26                            						// taille du mot max

#define DEBUG 1

typedef struct{															//structure qui contiens :
	char lettre[27];													//lettres
	int valeur[27];														//leur valeur
	int nbJetons[27];													//le nombre restant dans le jeu
	}Lettres;

void menu();
void survol(Point pos_souris);
int traitement_choix(Point clic);

void initIndexTirage(int indexTirage[2][27], Lettres indexLettre);
Lettres initialiserLettres();

char tirerLettre(Lettres indexLettre, int indexTirage[2][27]);
void chargeDico(char *filedico, char tabdico[SIZEDICO][MAXLENMOT]);
int motValable(char motUtilise[] ,char dicoTab[SIZEDICO][MAXLENMOT], int tailleDico);

Point detecter_case(Point);												/************************/									
int estDedans(Point);													/* Placer les lettres	*/
int estLibre(Point, char plateau[15][15][2]);							/************************/	
void selectionLettre(Point, int);
void entourerCase(Point);

void initContenuPlateau(char plateau[15][15][2]);						//fonctions en relation avec la plateau
void updateContenuPlateau(char plateau[15][15][2], Point);					//
void updateMainJoueur(Point, char mains[2][7], int);


/******************************************************************************/
/* MAIN		                                                                  */
/******************************************************************************/
int main()
	{
	ouvrir_fenetre(RESH, RESV);											//ouvre la session graphique
		
	int joueur=0;
	char dicoTab[SIZEDICO][MAXLENMOT];									//Le dictionnaire
	chargeDico(FILEDICO, dicoTab);										//initialisation de dicoTab + renvoi la taille du dico dans tailleDico
	
	Lettres indexLettre=initialiserLettres();							//initalise la valeur des lettres ainsi que le n,ombre de jetons
	
	int indexTirage[2][27];												//initialisation du tableau nécessaire pour le tirage aléatoire
	initIndexTirage(indexTirage, indexLettre);
	
	Point emplacement_lettre;											//variable qui vas stoquer l'emplacement de la lettre

	char contenu_plateau[15][15][2];									//tableau qui contiens le contenu du plateau ainsi que les valeurs double ou triple des mots / lettres
	initContenuPlateau(contenu_plateau);								//initialise le charactère par défaut dans le tableau
	
#if DEBUG
	for(int compteur=0; compteur < 27; compteur++)
		printf("lettre:%c	valeur:%d	nbJetons:%d\n", indexLettre.lettre[compteur], indexLettre.valeur[compteur], indexLettre.nbJetons[compteur]);
#endif
	menu();																//ouvre le menu au joueur

	Point init={0,0};													/**********************/
	dessiner_rectangle(init, RESH, RESV, noir);							/*					  */
	afficher_image("./Images/plateau.bmp", init);						/*affichage du plateau*/
	actualiser();														/**********************/
	
	char mains[2][7]={{' '}};											//Tableau qui vas stocker les cartes
	for(joueur=1; joueur<=2; joueur++)									/*******************************************/
		{																/*Varier les joueurs pour l'attribution    */
		for(int compteurLettre=0; compteurLettre<7; compteurLettre++)	/*Avancer dans le tableau pour chaquejoueur*/
			mains[joueur][compteurLettre]=tirerLettre(indexLettre, indexTirage);		/*Attribuer les lettres					   */
		}

	Point case_main_joueur1 = {26, 272};
	Point case_main_joueur2 = {926, 272};
	updateMainJoueur(case_main_joueur1, mains, 1);
	updateMainJoueur(case_main_joueur2, mains, 2);
	actualiser();

	selectionLettre(attendre_clic(),1);
	actualiser();

	while(emplacement_lettre.x != 476 || emplacement_lettre.y != 476)
		{
		emplacement_lettre=attendre_clic();								//on attend un clic
		emplacement_lettre= detecter_case(emplacement_lettre);
		}
	afficher_image("./Images/a.bmp", emplacement_lettre);				//on affiche la lettre du joueur(créer fonction qui determine la lettre

	updateContenuPlateau(contenu_plateau, emplacement_lettre);

	while(1)
		{																//boucle infinie paramètre a modifier pour condition de sortie
		
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
/* MENU									                                      */
/******************************************************************************/
void menu ()															//fonction qui vas s'occuper de traiter les infos recues durant l'affichage du menu
	{
	Point clic = {0,0};
	Point pos_souris ={0,0};
	int choix=0;
	
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
int estDedans(Point p)
	{
	if(p.x >= (BORDURE + ESPACEMENT) && p.x <= (RESH - (BORDURE + ESPACEMENT + TAILLECASE)) && p.y >= (BORDURE + ESPACEMENT) && p.y <= (RESV - (BORDURE + ESPACEMENT + TAILLECASE)))
		return 1;	
		
	return 0;
	}

/******************************************************************************/
/* CASE EST LIBRE ?                                                           */
/******************************************************************************/
int estLibre(Point p, char plateau[15][15][2])
	{
	if(plateau[((p.y-(BORDURE + ESPACEMENT))/(TAILLECASE + BORDURE))][((p.x-(BORDURE + ESPACEMENT))/(TAILLECASE + BORDURE))][0] == ' ')
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

/******************************************************************************/
/* UPDATE CONTENU PLATEAU                                                     */
/******************************************************************************/
void updateContenuPlateau(char plateau[15][15][2], Point p)
	{
	plateau[((p.y-(BORDURE + ESPACEMENT))/(TAILLECASE + BORDURE))][((p.x-(BORDURE + ESPACEMENT))/(TAILLECASE + BORDURE))][0] = 'a';
	}

void updateMainJoueur(Point pos_case, char mains[2][7], int joueur)
	{
	int i;
	char nom_lettre;
	char lettre[14];
	for(i=0; i<7; i++)
		{
		nom_lettre = mains[joueur][i];
		sprintf(lettre,"./Images/%c.bmp", nom_lettre);
		afficher_image(lettre, pos_case);
		pos_case.y+=(TAILLECASE+20);
		}
	}

void selectionLettre(Point p, int joueur)
	{
	if(p.x>=26 && p.x<=74 && p.y>=272 && p.y<=680 && joueur==1)
		{
		p.x = ((p.x - 26) - (p.x - 26) % 53) + 26;
		p.y = ((p.y - 272) - (p.y - 272) % 68) + 272;
		if(p.y==272 || p.y==340 || p.y==408 || p.y==479 || p.y==544 || p.y==612 || p.y==680)
			{
			entourerCase(p);
			}
		}
	printf("--------------------------------\n");
	printf("%d %d\n", p.x, p.y);
	printf("--------------------------------\n");

	/*if(joueur==1 && p.x>=26 && p.x<=74 && p.y>=272 && p.y<=680)
		{
		p.x = ((p.x - 26) - (p.x - 26) % 53) + 26;
		p.y = ((p.y - 272) - (p.y - 272) % 63) + 272;
		entourerCase(p);
		}
	else if(joueur==2 && p.x==926 && p.y>=272 && p.y<=680)
		{
		p.x = ((p.x - 926) - (p.x - 926) % 53) + 926;
		p.y = ((p.y - 272) - (p.y - 272) % 63) + 272;
		entourerCase(p);
		}
	return 'c';*/
	}

void entourerCase(Point p)
	{
	Point r1 = {(p.x-5),(p.y-5)};
	Point r2 = {(p.x-5),(p.y-5)};
	Point r3 = {(p.x+48),(p.y-5)};
	Point r4 = {(p.x-5),(p.y+48)};

	dessiner_rectangle(r1, 53, 5, rouge);
	dessiner_rectangle(r2, 5, 53, rouge);
	dessiner_rectangle(r3, 6, 58, rouge);
	dessiner_rectangle(r4, 53, 5, rouge);
	actualiser();
	}