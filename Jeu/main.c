#include <stdio.h>
#include <string.h>
#include "../lib/libgraphique.h"

#define RESH 1000								
#define RESV 1000

#define BORDURE 5														//Bordures entre cases	
#define ESPACEMENT 100													//Bordure entre le tableau et le bord de la fenetre graphique
#define TAILLECASE 48													//Taille des cases du plateau

#define FILEDICO "./dictionnaire_fr_ss_accents" 						// chemin du fichier dico
#define SIZEDICO 319000                         						// taille large du dico
#define MAXLENMOT 26                            						// taille du mot max

#define TAILLEPLATEAU 15												//Nombre de ligne et de colone dans le plateau
#define TAILLEMAIN 7													//Nombre de lettres dans la main

typedef struct{															//structure qui contiens :
	char lettre[27];													//lettres
	int valeur[27];														//leur valeur
	int nbJetons[27];													//le nombre de jetons présent initiallement dans le jeu
	}Lettres;

int menu();																//S'occupe de l'affichage de menu
void survol(Point);														//Interpretation partie temps réel du menu 
int traitement_choix(Point);											//Permet de savoir le choix du joueur

void afficher_plateau();

void initPoints(char plateau[TAILLEPLATEAU][TAILLEPLATEAU][2]);			//Initialise les cases speciales du plateau (Mot compte double triple etc..)

void initIndexTirage(int indexTirage[27], Lettres);						//Initialisation de la variable utilisée pour le tirage aléatoire
Lettres initialiserLettres();											//Donne a chaque lettre les valeurs qui lui sont associées (cf. struct Lettres)

char tirerLettre(Lettres *indexLettre, int indexTirage[27]);			//Tirage aléatoire
void actualiser_pioche(int indexTirage[27], int compteur);				//Apres chaque tirage la pioche s'adapte pour un tirage plus réaliste

int chargeDico(char *filedico, char tabdico[SIZEDICO][MAXLENMOT]);		
int motValable(char motUtilise[TAILLEPLATEAU] ,char dicoTab[SIZEDICO][MAXLENMOT], int);				//Verifie que le mot est présent dans le dico
int lireMots(char mot[TAILLEPLATEAU], char contenu_plateau[TAILLEPLATEAU][TAILLEPLATEAU][2], char dicoTab[SIZEDICO][MAXLENMOT], int);//Verifie si un mot non compris dans le dico est posé sur la plateau

Point detecter_case(Point);												//repositionne un clic sur la grille du plateau						
int estDansPlateau(Point);												//Verifie si le clic est sur le plateau		
int CaseEstLibre(Point, char plateau[TAILLEPLATEAU][TAILLEPLATEAU][2]);	//Verifie si la case est libre			
int peutPlacer(char contenu_plateau[TAILLEPLATEAU][TAILLEPLATEAU][2], Point);//Verifie si une lettre est autours de là ou on veut poser notre lettre
int lettreDejaPosee(Point lettres_placees[TAILLEMAIN], Point lettre_selectionnee);//Verifie si la lettre est deja posée

Point attendreSelectionLettre(int, Point lettres_placees[TAILLEMAIN], int, int, int, Point);//attend le selection d'une lettre de la main et renvoi le point correspondant: fonction avec 2 modes. Le 1er mode demande un clic et le second prends un point deja existant
char selectionLettre(int, char mains[2][TAILLEMAIN], Point);								//converti le point obtenu avec attendreSelectionLettre en une lettre de la main
void entourerCase(Point, Couleur);															//entoure la lettre dans la main avec une couleur choisie
int estDansMainJoueur(Point, int);															//verifie que le clic est dans la main du joueur correspondant

void placerLettre(char contenu_plateau[TAILLEPLATEAU][TAILLEPLATEAU][2], char, Point, Point);//Place la lettre selectionnée dans le plateau et l'enlève de la main (graphiquement)
Point attendrePlacerLettre(char contenu_plateau[TAILLEPLATEAU][TAILLEPLATEAU][2], int, Point emplacement_lettre_old[TAILLEPLATEAU], int, int, Point lettres_placees[TAILLEMAIN]);//attend qu'une lettre soir placée ou qu'on selectionne une autre lettre de la main

void initContenuPlateau(char plateau[TAILLEPLATEAU][TAILLEPLATEAU][2]);						//initialiser le plateau avec ' '
void updateContenuPlateau(char plateau[TAILLEPLATEAU][TAILLEPLATEAU][2], Point, char);		//Update de la variable du plateau avec le lettre placee
Point convertirEnCaseTableau(Point clic);													//converti un point et le transforme en point dans la variable tableau							

void initMainJoueur(char mains[2][TAILLEMAIN], int indextirage[27], Lettres*);				//initialise la main du joueur en tirant 7 lettres pour chaque joueur
void afficherMainJoueur(Point, char mains[2][TAILLEMAIN], int);								//Affiche la main du joueur selectionné
void cacherMainJoueur(Point pos_case, int joueur);											//Cache la main du joueur selectionné
void updateMainJoueur(char mains[2][TAILLEMAIN], int, Point lettres_placees[TAILLEMAIN], Lettres*, int indexTirage[27]);//met a jour la main du joueur a la fin de son tour

int clicBouton(Point clic);																	//Verifie si on clique sur un bouton et si oui lequel ?
void razAnciennesLettres(Point emplacement_lettre_old[TAILLEPLATEAU], Point lettres_placees[TAILLEMAIN]);//Remet a zero l'historique de l'emplacement des lettres du tour ainsi que celui des lettres choies dans la main
char compDirection(Point, Point, Point);													//Verifie si la lettre placee est sur la meme ligne / colonne que les precedentes

void reinitTour(Point emplacement_lettre_old[TAILLEMAIN], Point lettre_placees[TAILLEMAIN], char contenu_plateau[TAILLEPLATEAU][TAILLEPLATEAU][2], char mains[2][TAILLEMAIN], int, Point);//Reinitialise le tour si jamais le mot posé n'est pas bon
void actualiser_plateau(Point emplacement_lettre_old, char contenu_plateau[TAILLEPLATEAU][TAILLEPLATEAU][2]);			//Veille a remettre les bonnes cases des bonnes couleurs lorsque l'on enlève les lettres lors de la fonction reinitTour

void chargerSauvegarde(char contenu_plateau[TAILLEPLATEAU][TAILLEPLATEAU][2], char mains[2][TAILLEMAIN], int scores[2], int* joueur, int indexTirage[27]);	//Charge la sauvegard
void sauvegarder(char contenu_plateau[TAILLEPLATEAU][TAILLEPLATEAU][2], char mains[2][TAILLEMAIN], int scores[2], int joueur, int indexTirage[27]);			//Sauvegarde avec le bouton sauver (cloture avant le tour comme un appui sur valider ou passer)
void updatePlateauSave(char contenu_plateau[TAILLEPLATEAU][TAILLEPLATEAU][2]);																				//met a jour le plateau avec avoir chargé la sauvegarde

int score(int, Point emplacement_lettre_old[TAILLEMAIN], char contenu_plateau[TAILLEPLATEAU][TAILLEPLATEAU][2], Lettres);//Met a jour les scores après chaque tour
void afficherScore(int scores[2]);																						//affiche les scores sur le plateau
int multiplicateurLettre(char);																							//Précise a la fonction score si la case a des effets sur les points de la lettre
int multiplicateurMot(char);																							//Précise a la fonction score si la case a des effets sur les points du mot

void switchValiderPasser(int numCoup, int comptLettre);																	//Change entre le bouton passer et valider sur le plateau

void piocher(char mains[2][TAILLEMAIN], Lettres *, int indexTirage[27], int);											//Permet de piocher

/******************************************************************************/
/* MAIN		                                                                  */
/******************************************************************************/
int main()
	{
	ouvrir_fenetre(RESH, RESV);							

	char contenu_plateau[TAILLEPLATEAU][TAILLEPLATEAU][2];				//tableau qui contiens le contenu du plateau ainsi que les valeurs double ou triple des mots / lettres
	char mot[TAILLEPLATEAU];

	char dicoTab[SIZEDICO][MAXLENMOT];									//Le dictionnaire
	int nbMotDico=chargeDico(FILEDICO, dicoTab);
	
	Lettres indexLettre=initialiserLettres();						

	int charger=menu();													
	
	int indexTirage[27];											
	initIndexTirage(indexTirage, indexLettre);		
	
	char mains[2][TAILLEMAIN];										

	initContenuPlateau(contenu_plateau);	
	initPoints(contenu_plateau);
	
	int scores[2];

	afficher_plateau();

	int numCoup=0;
	int joueur = 1;

	if(!charger)
		initMainJoueur(mains, indexTirage, &indexLettre);

	else
		{
		chargerSauvegarde(contenu_plateau, mains, scores, &joueur, indexTirage);
		numCoup++;																		//Si on charge on ne veut pas rejouer le premier coup (coup spécial car besoin de placer au milieu ect..) donc on passe drectement au tour suivant
		}
		
	Point  const case_main_joueur[2] = {{26, 272},{926, 272}};							//Haut gauche des mains des joueurs
	
	Point emplacement_lettre_selectionnee;												//lettre_selectionnee concerne la lettre directement depuis la main et emplacement_lettre_selectionné est son point
	char lettre_selectionnee;															//alors que emplacement_lettre correspond a son emplacement dans le plateau
	Point emplacement_lettre;
	Point emplacement_lettre_old[TAILLEPLATEAU]={0};									//historiques
	Point lettres_placees[TAILLEMAIN]={0};

	int comptLettre=0;
	int tourBoucle=0;
	int nb_changement_lettre=0;
	
	afficherMainJoueur(case_main_joueur[joueur-1], mains, joueur);
	cacherMainJoueur(case_main_joueur[joueur%2], joueur);

	while(indexTirage[26]>0)												//Le jeu s'arrête si plus de lettres
		{
		afficherScore(scores);															//affiche les scores a chaque tour
		do
			{
			while(1)										
				{
				switchValiderPasser(numCoup, comptLettre);	
				if(tourBoucle)															//si la validation du mot n'est pas ok
					{
					reinitTour(emplacement_lettre_old, lettres_placees, contenu_plateau, mains, joueur, case_main_joueur[joueur-1]);
					tourBoucle=0;
					comptLettre=0;
					}
				emplacement_lettre_selectionnee = attendreSelectionLettre(joueur, lettres_placees, numCoup, comptLettre, nb_changement_lettre, emplacement_lettre);//On demande la lettre a prendre
				if(clicBouton(emplacement_lettre_selectionnee)==1 || clicBouton(emplacement_lettre_selectionnee)==3)												//Si on veut sauvegarder ou valider, on sort de la boucle pour terminer le tour
					break;
				else if(clicBouton(emplacement_lettre_selectionnee)==2 && indexTirage[26]>7)															//Si on veut piocher et qu'il reste plus de 7 lettres (regles officielles de la FFcs et oui il en existe une)
					{
					piocher(mains, &indexLettre, indexTirage, joueur);																								
					break;																																			//fin du tour
					}
				else if(clicBouton(emplacement_lettre_selectionnee)==2 && indexTirage[26]<=7)															//Si pas assez de lettre on continue
					continue;
				lettres_placees[comptLettre]=emplacement_lettre_selectionnee;
				lettre_selectionnee = selectionLettre(joueur, mains, emplacement_lettre_selectionnee);																//on deduis la lettre du point obtenu precedement
				emplacement_lettre = attendrePlacerLettre(contenu_plateau, comptLettre, emplacement_lettre_old, numCoup, joueur, lettres_placees);					//on attend un clic dans le plateau ou un autre choix de lettre
				if(estDansMainJoueur(emplacement_lettre, joueur))																									//Si autre choix de lettre alors on redemande une position pour la placer dans le tableau
					{
					nb_changement_lettre++;
					entourerCase(emplacement_lettre_selectionnee, noir);
					continue;
					}
				updateContenuPlateau(contenu_plateau, emplacement_lettre, lettre_selectionnee);
				placerLettre(contenu_plateau, lettre_selectionnee, emplacement_lettre, emplacement_lettre_selectionnee);
				
				comptLettre++;
				nb_changement_lettre=0;
				}
			tourBoucle++;
			} while (!(lireMots(mot, contenu_plateau, dicoTab, nbMotDico)));//Verifie que le mot est valable sinon finis le tour
		scores[joueur-1]=score(scores[joueur-1], emplacement_lettre_old, contenu_plateau, indexLettre);
		cacherMainJoueur(case_main_joueur[joueur-1], joueur);
		updateMainJoueur(mains, joueur, lettres_placees, &indexLettre, indexTirage);
		joueur=(joueur%2)+1;
		if(clicBouton(emplacement_lettre_selectionnee)==3)
			{
			sauvegarder(contenu_plateau, mains, scores, joueur, indexTirage);
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
			}while( ( (clic.y < 723 || clic.y > 801) || ( (clic.x < 168 || clic.x > 388) && (clic.x < 608 || clic.x > 828) ) ) && (clic.y > 912 || clic.y < 835 || clic.x < 388 || clic.x > 608 ) );

		choix=traitement_choix(clic);										//on regarde sur quel bouton l'utilisateur a cliqué

		if(choix==2)//Les regles
			printf("\n\n\nValeurs et nombres de lettres du Scrabble :\n\nLa lettre A	Valeur : 1 point	Nombre : 9 pièces\nLa lettre B	Valeur : 3 points	Nombre : 2 pièces\nLa lettre C	Valeur : 3 points	Nombre : 2 pièces\nLa lettre D	Valeur : 2 points	Nombre : 3 pièces\nLa lettre E	Valeur : 1 point	Nombre : 15 pièces\nLa lettre F	Valeur : 4 points	Nombre : 2 pièces\nLa lettre G	Valeur : 2 points	Nombre : 2 pièces\nLa lettre H	Valeur : 4 points	Nombre : 2 pièces\nLa lettre I	Valeur : 1 point	Nombre : 8 pièces\nLa lettre J	Valeur : 8 points	Nombre : 1 pièce\nLa lettre K	Valeur : 10 points	Nombre : 1 pièce\nLa lettre L	Valeur : 1 point	Nombre : 5 pièces\nLa lettre M	Valeur : 2 points	Nombre : 3 pièces\nLa lettre N	Valeur : 1 point	Nombre : 6 pièces\nLa lettre O	Valeur : 1 point	Nombre : 6 pièces\nLa lettre P	Valeur : 3 points	Nombre : 2 pièces\nLa lettre Q	Valeur : 8 points	Nombre : 1 pièce\nLa lettre R	Valeur : 1 point	Nombre : 6 pièces\nLa lettre S	Valeur : 1 point	Nombre : 6 pièces\nLa lettre T	Valeur : 1 point	Nombre : 6 pièces\nLa lettre U	Valeur : 1 point	Nombre : 6 pièces\nLa lettre V	Valeur : 4 points	Nombre : 2 pièces\nLa lettre W	Valeur : 10 points	Nombre : 1 pièce\nLa lettre X	Valeur : 10 points	Nombre : 1 pièce\nLa lettre Y	Valeur : 10 points	Nombre : 1 pièce\nLa lettre Z	Valeur : 10 points	Nombre : 1 pièce\nJokers	Valeur : 0 point	Nombre : 2 pièces\n\n\n\nLes différentes valeurs des cases :\n\n    Case bleu ciel : Lettre compte double\n    Case bleu foncé : Lettre compte triple\n    Case rose : Mot compte double\n    Case rouge : Mot compte triple\n\n\n\nCommencer une partie de Scrabble :\n\nPour commencer, chaque joueur va tirer au hasard 7 lettres dans le sac. Avec celles-ci, chacun va alors essayer de créer un ou plusieurs mots. Le premier joueur doit obligatoirement poser le premier mot au centre du plateau, sur l’étoile. Ce mot doit être au minimum composé de 2 lettres. Le deuxième joueur doit s’appuyer sur ce mot pour placer le sien et ainsi de suite…\n\nUn joueur peut échanger ses lettres par d’autres en piochant dans le sac mais cela lui fera obligatoirement passer son tour.\n\n\n\nCalcul des scores du Scrabble :\n\nLe score d’un coup est calculé en additionnant la valeur de toutes les lettres des nouveaux mots formés (y compris celles déjà posées sur la grille). Si l’un des lettres du mot est sur une case bleu ciel, bleu foncé, la valeur doit être calculée. Idem pour les cases rose et rouge correspondant au mot compte double et mot compte triple.\n\nA savoir que si un mot est posé sur 2 cases « compte double » ou 2 cases « compte triple », la valeur du mot est alors multipliée par 4 et par 9.\nAttention ! Chaque case multiplicatrice ne sert qu’une fois!\n\nSi l’un des joueurs arrivent à placer ses 7 lettres d’un seul coup, on dit qu’il a fait un « Scrabble« . Ce coup lui rapporte une prime de 50 points.\n\n\n\nComment gagner au Scrabble :\n\nQuand le sac est vide et qu’un des joueurs pose toutes ses lettres, la partie est terminée. Celui-ci prend alors en bonus la valeur des lettres des autres joueurs. Alors, on fait les totaux de points de chacun des joueurs et celui possédant le plus de points gagne la partie de Scrabble.\n\nBon à savoir : L’idéal lors d’une partie de Scrabble est d’avoir un dictionnaire à portée de main afin de vérifier si le mot posé par l’un des joueurs est valable. Cela permettant d’éviter tout litige. Après vérification, si le mot posé n’existe pas, le joueur reprend toutes ses lettres et marque 0 point pour ce coup.\n");
			
		}while(choix==2);//tant que l'on choisis d'afficher les regles
		
	return choix;
	}

/******************************************************************************/
/* SURVOL									                                  */
/******************************************************************************/
void survol(Point pos_souris)//interprete le temps reel
	{
	Point temp;
	int rafraichir=1;
	
	if(pos_souris.y > 723 && pos_souris.y < 801 && pos_souris.x > 168 && pos_souris.x < 388)
		{
		temp.x=168;
		temp.y=723;
		afficher_image("./Images/jouer_select.bmp", temp);
		}
	else if(pos_souris.y > 723 && pos_souris.y < 801 && pos_souris.x > 608 && pos_souris.x < 828)
		{
		temp.x=608;
		temp.y=723;
		afficher_image("./Images/charger_select.bmp", temp);
		}
	else if(pos_souris.y > 835 && pos_souris.y < 912 && pos_souris.x > 388 && pos_souris.x < 608)
		{
		temp.x=388;
		temp.y=835;
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
	
	if(clic.y >802)														//bouton règles
		choix=2;
	
	else if(clic.x<389)													//bouton jouer
		choix=0;
		
	else 																//bouton charger
		choix=1;
		
	return choix;
	}

/******************************************************************************/
/* AFFICHER PLATEAU						                                      */
/******************************************************************************/
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
void initPoints(char plateau[TAILLEPLATEAU][TAILLEPLATEAU][2])
	{
	for(int lon=0; lon<TAILLEPLATEAU; lon++)
		{
		for(int lar=0; lar<TAILLEPLATEAU; lar++)
			{
			if( ((lon==lar || (14-lon)==lar) && ((lar>0 && lar<5) || (lar>9 && lar<14) || lar==7)) )
				plateau[lar][lon][1]='m';								//mot double
				
			else if( ((lon+7)%7==0 && (lar+7)%7==0) )
				plateau[lar][lon][1]='M';								//mot triple
			 
			else if( (lon==1 || lon==5 || lon==9 || lon==13) && (lar==1 || lar==5 || lar==9 || lar==13) )
				plateau[lar][lon][1]='L';								//lettre triple
				
			else if( ((lon+5)%8==0 && (lar%7==0)) || ((lar+5)%8==0 && (lon%7==0)) || ((lar==6 || lar==8 || lar==2 || lar==12) && (lon==6 || lon==8 || lon==2 || lon==12)))
				plateau[lar][lon][1]='l';								//lettre double
			}
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
	int idLettre=entier_aleatoire(indexTirage[26]);				//Tire un nombre aléatoire avec le nombre total de jetons restants
	int compteur;

	for(compteur=0; compteur<27; compteur++)					//parcourir les valeurs du tableau
		{
		if(idLettre < indexTirage[compteur])					//si le chiffre tiré est inferieur au nombre de jetons cumulés de a jusqu'a la lettre tiré, lui associe l'id de la lettre correspondante
			{
			idLettre=compteur;
			break;
			}
		}
		
	(*indexLettre).nbJetons[idLettre]-=1;
	
	if(indexTirage[26]>0)
		actualiser_pioche(indexTirage, compteur);
	
	return (*indexLettre).lettre[idLettre];
	}

/******************************************************************************/
/* ACTUALISER PIOCHE                                                          */
/******************************************************************************/	
void actualiser_pioche(int indexTirage[27], int compteur)
	{
	for(;compteur<27;compteur++)//Baisse le nombre de jeton de 1 depuis le jeton tiré jusqu'à la derniere lettre
		{
		indexTirage[compteur]-=1;
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
int motValable(char motUtilise[TAILLEPLATEAU], char dicoTab[SIZEDICO][MAXLENMOT], int nbMotDico)
	{	
	int debut=0, fin=nbMotDico, milieu=(debut+fin)/2;				//initialisation des variables repères pour la recherche dichotomique
	
	while(debut<=fin && strcmp(motUtilise, dicoTab[milieu])!=0)			//tant que le début et la fin ne se croisent pas ET que le mot n'est pas celui cherché
		{
		if(strcmp(motUtilise, dicoTab[milieu])>0)						//tester si le mot est avant ou après
			debut=milieu+1;												//et adapter les variables en conséquence
		else
			fin=milieu-1;
		milieu=(debut+fin)/2;
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
		p.x = ((p.x - (BORDURE + ESPACEMENT)) - (p.x - (BORDURE + ESPACEMENT)) % (TAILLECASE + BORDURE)) + (BORDURE + ESPACEMENT);//replace dans la plateau
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
int CaseEstLibre(Point p, char plateau[TAILLEPLATEAU][TAILLEPLATEAU][2])
	{
	p=convertirEnCaseTableau(p);
	if(plateau[p.y][p.x][0] == ' ')
		return 1;
	return 0;
	}

/******************************************************************************/
/* INITIALISATION CONTENU DU TABLEAU                                          */
/******************************************************************************/
void initContenuPlateau(char plateau[TAILLEPLATEAU][TAILLEPLATEAU][2])
	{
	int i,j,k;
	
	for(i=0; i<TAILLEPLATEAU; i++)
		{
		for(j=0; j<TAILLEPLATEAU; j++)
			{
			for(k=0; k<2; k++)
				plateau[i][j][k] = ' ';
			}
		}
	}

/******************************************************************************/
/* INIT MAIN JOUER                                                            */
/******************************************************************************/
void initMainJoueur(char mains[2][TAILLEMAIN], int indexTirage[27], Lettres * indexLettre)
	{		
	for(int joueur=1; joueur<=2; joueur++)												
		for(int compteurLettre=0; compteurLettre<TAILLEMAIN; compteurLettre++)
			mains[joueur-1][compteurLettre]=tirerLettre(indexLettre, indexTirage);
	}
	
/******************************************************************************/
/* UPDATE MAIN JOUEUR                                                         */
/******************************************************************************/
void updateMainJoueur(char mains[2][TAILLEMAIN], int joueur, Point lettres_placees[TAILLEMAIN], Lettres * indexLettre, int indexTirage[27])
	{		
	for(int compteur=0; compteur<TAILLEMAIN; compteur++)
		if(lettres_placees[compteur].x && lettres_placees[compteur].y)
			{
			lettres_placees[compteur].y = ((lettres_placees[compteur].y - 272) - (lettres_placees[compteur].y - 272) % 68)/68;
			mains[joueur-1][lettres_placees[compteur].y] = tirerLettre(indexLettre, indexTirage);
			}
	}
	
/******************************************************************************/
/* LETTRE DEJA POSEE                                                          */
/******************************************************************************/
int lettreDejaPosee(Point lettres_placees[TAILLEMAIN], Point lettre_selectionnee)
	{
	lettre_selectionnee.y = ((lettre_selectionnee.y - 272) - (lettre_selectionnee.y - 272) % 68)/68;	//pour éviter de poser 2 fois même lettre
	int temp;
	for(int compteur=0; compteur<TAILLEMAIN; compteur++)
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
void updateContenuPlateau(char plateau[TAILLEPLATEAU][TAILLEPLATEAU][2], Point p, char lettre_selectionnee)
	{
	p=convertirEnCaseTableau(p);
	plateau[p.y][p.x][0] = lettre_selectionnee;
	}

/******************************************************************************/
/* AFFICHER MAIN JOUEUR                                                         */
/******************************************************************************/
void afficherMainJoueur(Point pos_case, char mains[2][TAILLEMAIN], int joueur)
	{
	int i;
	char nom_lettre;
	char lettre[16];
	for(i=0; i<TAILLEMAIN; i++)
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
	for(i=0; i<TAILLEMAIN; i++)
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
char selectionLettre(int joueur, char mains[2][TAILLEMAIN], Point p)
	{
	char lettre_selectionnee;

	if(joueur == 1)
		{
		p.x = ((p.x - 26) - (p.x - 26) % 53) + 26;
		p.y = ((p.y - 272) - (p.y - 272) % 68) + 272;
		lettre_selectionnee = mains[joueur-1][(p.y-272)/68];//Formule qui permet de dire quelle lettre est choisie
		entourerCase(p, rouge);//entoure la case selectionnee
		}
	else if (joueur == 2)
		{
		p.x = ((p.x - 926) - (p.x - 926) % 53) + 926;
		p.y = ((p.y - 272) - (p.y - 272) % 68) + 272;
		lettre_selectionnee = mains[joueur-1][(p.y-272)/68];//
		entourerCase(p, rouge);//
		}
	return lettre_selectionnee;
	}

/******************************************************************************/
/* ENTOURE LA CASE EN NOIR OU ROUGE                                           */
/******************************************************************************/
void entourerCase(Point p, Couleur couleur)
	{
	Point r1 = {(p.x-5),(p.y-5)};
	Point r2 = {(p.x-5),(p.y-5)};//colore le contour de la lettre dans la main
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
int peutPlacer(char contenu_plateau[TAILLEPLATEAU][TAILLEPLATEAU][2], Point clic)
	{	
	clic=convertirEnCaseTableau(clic);

	int compteur=0;
	
	for(int verif=(-1); verif<2;verif+=2)
		{
		if( (clic.y+verif<=14 && clic.y+verif>=0) && contenu_plateau[clic.y+verif][clic.x][0]!=' ')//on verifie si lettre au dessus ou au dessous
			compteur++;
		if( (clic.x+verif<=14 && clic.x+verif>=0) && (contenu_plateau[clic.y][clic.x+verif][0]!=' ') )//on verifie si lettre a gauche ou a droite
			compteur++;
		}
	
	if(!compteur)
		return 0;
	
	return 1;
	}

/******************************************************************************/
/* PLACER LETTRE SUR LE PLATEAU                                               */
/******************************************************************************/
void placerLettre(char contenu_plateau[TAILLEPLATEAU][TAILLEPLATEAU][2], char lettre_selectionnee, Point p_plateau, Point p_main)
	{
	char chemin[16];

	sprintf(chemin, "./Images/%c.bmp", lettre_selectionnee);
	afficher_image(chemin, p_plateau);
	dessiner_rectangle(p_main, 48, 48, blanc);//vider la case dans la main
	entourerCase(p_main, noir);
	actualiser();
	}

/******************************************************************************/
/* ATTENDRE SELECTION LETTRE DANS LA MAIN                                     */
/******************************************************************************/
Point attendreSelectionLettre(int joueur, Point lettres_placees[TAILLEMAIN], int numCoup, int comptLettres, int demander_clic, Point selection_lettre)
	{
	Point p;
	if(!demander_clic)//mode demander le clic
		do
			{
			p = attendre_clic();
			} while( (estDansMainJoueur(p, joueur) == 0 || lettreDejaPosee(lettres_placees, p)==1) && !(clicBouton(p) && ((numCoup==0 && comptLettres>=2) || numCoup)) );
	else//mode on donne le clic
		p = selection_lettre;
	if(!((p.x>385 && p.x<616) && (p.y>920 && p.y<990)) && !clicBouton(p))//on replace le clic sauf si le clic est un clic sur un bouton
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
Point attendrePlacerLettre(char contenu_plateau[TAILLEPLATEAU][TAILLEPLATEAU][2], int comptLettre, Point emplacement_lettre_old[TAILLEPLATEAU], int num_coup, int joueur, 	Point lettres_placees[TAILLEMAIN])
	{
	Point emplacement_lettre;
	if(num_coup==0 && comptLettre==0)
		{
		Point caseDansTableau;
		do
			{															//tant que le clic n'est pas dans le tableau ou que la case choisie n'est pas libre,
			emplacement_lettre=attendre_clic();								//on attend un clic
			emplacement_lettre= detecter_case(emplacement_lettre);			//on detecte la case sur laquelle est le clic
			if((estDansMainJoueur(emplacement_lettre, joueur) && lettreDejaPosee(lettres_placees, emplacement_lettre)==0))//si on souhaite changer de lettre
				break;
			emplacement_lettre_old[comptLettre]=emplacement_lettre;
			caseDansTableau=convertirEnCaseTableau(emplacement_lettre);//on converti pour verifier que la premiere lettre est placee au milieu
			} while (caseDansTableau.x!=7 || caseDansTableau.y!=7);
		return emplacement_lettre;
		}
	if(comptLettre<2)// si moins de deux lettres on ne compare pas la direction
		{
		do
			{															//tant que le clic n'est pas dans le tableau ou que la case choisie n'est pas libre,
			emplacement_lettre=attendre_clic();								//on attend un clic
			emplacement_lettre= detecter_case(emplacement_lettre);			//on detecte la case sur laquelle est le clic
			if((estDansMainJoueur(emplacement_lettre, joueur) && lettreDejaPosee(lettres_placees, emplacement_lettre)==0))//si on souhaite changer de lettre
				break;
			emplacement_lettre_old[comptLettre]=emplacement_lettre;
			} while(estDansPlateau(detecter_case(emplacement_lettre)) == 0 || CaseEstLibre(detecter_case(emplacement_lettre), contenu_plateau) == 0 || peutPlacer(contenu_plateau, emplacement_lettre)==0);
		return emplacement_lettre;
		}
		
	do
		{															//tant que le clic n'est pas dans le tableau ou que la case choisie n'est pas libre,
		emplacement_lettre=attendre_clic();								//on attend un clic
		emplacement_lettre= detecter_case(emplacement_lettre);			//on detecte la case sur laquelle est le clic
		if((estDansMainJoueur(emplacement_lettre, joueur) && lettreDejaPosee(lettres_placees, emplacement_lettre)==0))//si on souhaite changer de lettre
			break;
		emplacement_lettre_old[comptLettre]=emplacement_lettre;
		} while(estDansPlateau(detecter_case(emplacement_lettre)) == 0 || CaseEstLibre(detecter_case(emplacement_lettre), contenu_plateau) == 0 || peutPlacer(contenu_plateau, emplacement_lettre)==0 || compDirection(emplacement_lettre_old[0], emplacement_lettre_old[1], emplacement_lettre_old[comptLettre])==0);
	return emplacement_lettre;
	}

/******************************************************************************/
/* CONVERTIR CASE TABLEAU                                                     */
/******************************************************************************/
Point convertirEnCaseTableau(Point clic)
	{
	clic.x=((clic.x-(BORDURE + ESPACEMENT))/(TAILLECASE + BORDURE));//formule qui converti le clic en case du plateau
	clic.y=((clic.y-(BORDURE + ESPACEMENT))/(TAILLECASE + BORDURE));
	
	return clic;
	}

/******************************************************************************/
/* RAZ HISTORIQUES                                                            */
/******************************************************************************/
void razAnciennesLettres(Point emplacement_lettre_old[TAILLEPLATEAU], Point lettres_placees[TAILLEMAIN])
	{
	for(int raz=0;raz<TAILLEPLATEAU;raz++)
		{
		emplacement_lettre_old[raz].x=0;
		emplacement_lettre_old[raz].y=0;
		}
	for(int raz=0;raz<TAILLEMAIN;raz++)
		{
		lettres_placees[raz].x=0;
		lettres_placees[raz].y=0;
		}
	}

/******************************************************************************/
/* COMPARER DIRECTION                                                         */
/******************************************************************************/
char compDirection(Point lettre1, Point lettre2, Point lettre3)
	{
	if( ((lettre1.x==lettre3.x) || (lettre1.y==lettre3.y)) && ((lettre2.x==lettre3.x) || (lettre2.y==lettre3.y)) )
		return 1;
	return 0;
	}

/******************************************************************************/
/* LIRE MOT				                                                      */
/******************************************************************************/
int lireMots(char mot[TAILLEPLATEAU], char contenu_plateau[TAILLEPLATEAU][TAILLEPLATEAU][2], char dicoTab[SIZEDICO][MAXLENMOT], int nbMotDico)
	{
	int a, i, j, k, l, m=0;
	int mot_mauvais = 0;
	int jokairDansMot[2]={42,42};//initialisation des jokair a tes valeurs impossibles a avoir normalement
	char lettre1;				//lettre prise par le premier jokair si premier jokair il y a
	char lettre2;				//lettre prise par le second jokair si second jokair il y a
	int comptNbJok=0;

	for(a=0; a<TAILLEPLATEAU; a++)// \0 pour eviter des depassements
		mot[a] = '\0';

	for(i=0; i<TAILLEPLATEAU; i++)
		{
		for(j=0; j<TAILLEPLATEAU; j++)
			{
			if(contenu_plateau[i][j][0] != ' ')//si on tombe sur une lettre
				{
				mot[m] = contenu_plateau[i][j][0];//on l'ajoute au mot en cour
					if(mot[m]=='%')//si c'est un jokair on mémorise la place
					{
					jokairDansMot[comptNbJok]=m;
					comptNbJok++;
					}
				m++;					
				if(contenu_plateau[i][j+1][0] == ' ')
					{
					if(comptNbJok>=1)//si un jokair a est dans le mot
						for(lettre1='a'; lettre1<='z' && motValable(mot, dicoTab, nbMotDico)==0; lettre1++)//sortons de la boucle si le mot jeste avec le jokair est trouvé
							{
							mot[jokairDansMot[0]]=lettre1;
							if(comptNbJok==2)//si deux jokair dans le mot alors boucle imbriquée pour tester toutes les valeurs
								for(lettre2='a'; lettre2<='z' && motValable(mot, dicoTab, nbMotDico)==0; lettre2++)
									mot[jokairDansMot[1]]=lettre2;
							}
							
					if(motValable(mot, dicoTab, nbMotDico) == 0)// si le mot mot est faux alors on ajoute 1 au compteur
						mot_mauvais++;

					for(a=0; a<TAILLEPLATEAU; a++)
						mot[a] = '\0';//reinitialisation
					m=0;
					comptNbJok=0;
					for(a=0; a<2; a++)
						jokairDansMot[a] = 42;
					}
				}
			}
		}

	for(a=0; a<TAILLEPLATEAU; a++)
		mot[a] = '\0';
	m=0;
	comptNbJok=0;
	for(a=0; a<2; a++)
		jokairDansMot[a] = 42;
	
	for(k=0; k<TAILLEPLATEAU; k++)
		{
		for(l=0; l<TAILLEPLATEAU; l++)
			{
			if(contenu_plateau[l][k][0] != ' ')
				{
				mot[m] = contenu_plateau[l][k][0];
				if(mot[m]=='%')
					{
					jokairDansMot[comptNbJok]=m;
					comptNbJok++;
					}
				m++;		
				if(contenu_plateau[l+1][k][0] == ' ')
					{
					if(comptNbJok>=1)//si un jokair a est dans le mot
						for(lettre1='a'; lettre1<='z' && motValable(mot, dicoTab, nbMotDico)==0; lettre1++)//sortons de la boucle si le mot jeste avec le jokair est trouvé
							{
							mot[jokairDansMot[0]]=lettre1;
							if(comptNbJok==2)//si deux jokair dans le mot alors boucle imbriquée pour tester toutes les valeurs
								for(lettre2='a'; lettre2<='z' && motValable(mot, dicoTab, nbMotDico)==0; lettre2++)
									mot[jokairDansMot[1]]=lettre2;
							}
							
					if(motValable(mot, dicoTab, nbMotDico) == 0)// si le mot mot est faux alors on ajoute 1 au compteur
						mot_mauvais++;

					for(a=0; a<TAILLEPLATEAU; a++)
						mot[a] = '\0';//reinitialisation
					m=0;
					comptNbJok=0;
					for(a=0; a<2; a++)
						jokairDansMot[a] = 42;
					}
				}
			}
		}
	if(mot_mauvais > 0)
		return 0;
	return 1;
	}

/******************************************************************************/
/* REINITIALISER LE TOUR                                                      */
/******************************************************************************/
void reinitTour(Point emplacement_lettre_old[TAILLEMAIN], Point lettres_placees[TAILLEMAIN], char contenu_plateau[TAILLEPLATEAU][TAILLEPLATEAU][2], char mains[2][TAILLEMAIN], int joueur, Point case_main_joueur)
	{
	for(int compteur=0; compteur<TAILLEMAIN; compteur++)
		if(emplacement_lettre_old[compteur].x && emplacement_lettre_old[compteur].y)
			actualiser_plateau(emplacement_lettre_old[compteur], contenu_plateau);
	
	afficherMainJoueur(case_main_joueur, mains, joueur);
	razAnciennesLettres(emplacement_lettre_old, lettres_placees);
	}

/******************************************************************************/
/* ACTUALISER LE PLATEAU                                                      */
/******************************************************************************/
void actualiser_plateau(Point emplacement_lettre_old, char contenu_plateau[TAILLEPLATEAU][TAILLEPLATEAU][2])
	{
	Point case_tableau_old;
	case_tableau_old=convertirEnCaseTableau(emplacement_lettre_old);
	
	if(case_tableau_old.y==7 && case_tableau_old.x==7)
		afficher_image("./Images/case_centre.bmp", emplacement_lettre_old);
	else if(contenu_plateau[case_tableau_old.y][case_tableau_old.x][1]=='m')//On regarde la valeur de la case et on affiche la valeur correspondante
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
	
/******************************************************************************/
/* SAUVEGARDER				                                                  */
/******************************************************************************/
void sauvegarder(char contenu_plateau[TAILLEPLATEAU][TAILLEPLATEAU][2], char mains[2][TAILLEMAIN], int scores[2], int joueur, int indexTirage[27])
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
			for(compteurDim2=0; compteurDim2<TAILLEMAIN; compteurDim2++)
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
		
	FILE* save_tirage=fopen("Save/save.tirage", "w");
	if(!save_mains)
        fprintf(stderr,"fopen: problème d'ouverture du fichier mains");
	else
		{
		for(compteurDim1=0; compteurDim1<27; compteurDim1++)
			fprintf(save_mains, "%d,", indexTirage[compteurDim1]);
		fclose(save_tirage);
		}
	}

/******************************************************************************/
/* CHARGER				                                                      */
/******************************************************************************/
void chargerSauvegarde(char contenu_plateau[TAILLEPLATEAU][TAILLEPLATEAU][2], char mains[2][TAILLEMAIN], int scores[2], int * joueur, int indexTirage[27])
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
		fgets(contenu, 15, save_mains);
		fclose(save_mains);
		}
	for(compteurDim1=0; compteurDim1<2; compteurDim1++)
		for(compteurDim2=0; compteurDim2<TAILLEMAIN; compteurDim2++)
			mains[compteurDim1][compteurDim2]=contenu[compteurDim2+(compteurDim1*TAILLEMAIN)];//le calcul permet d'assigner la valeur correctement (0,1,2,3,4,5,6 + 0*7 pour chaque valeur lors du premier coup puis la meme chose avec +1*7 le second coup)
		
	FILE* save_plateau=fopen("./Save/save.plateau", "r");
	if(!save_plateau)
        fprintf(stderr,"fopen: problème d'ouverture du fichier contenu_plateau");
	else
		{
		fgets(contenu, TAILLEPLATEAU*TAILLEPLATEAU, save_plateau);
		fclose(save_plateau);
		}	
	for(compteurDim1=0; compteurDim1<TAILLEPLATEAU; compteurDim1++)
		for(compteurDim2=0; compteurDim2<TAILLEPLATEAU; compteurDim2++)
			contenu_plateau[compteurDim2][compteurDim1][0]=contenu[compteurDim2+(compteurDim1*TAILLEPLATEAU)];//de même
			
	updatePlateauSave(contenu_plateau);
		
	FILE* save_tirage=fopen("./Save/save.tirage", "r");
	if(!save_tirage)
        fprintf(stderr,"fopen: problème d'ouverture du fichier scores");
	else
		{
		for(compteurDim1=0; compteurDim1<27; compteurDim1++)
			fscanf(save_tirage, "%d,", &indexTirage[compteurDim1]);
		fclose(save_tirage);
		}
	}

/******************************************************************************/
/* MISE A JOUR PLATEAU                                                        */
/******************************************************************************/
void updatePlateauSave(char contenu_plateau[TAILLEPLATEAU][TAILLEPLATEAU][2])
	{
	char chemin[16];
	Point pos={0,0};
	
	for(int compteurDim1=0; compteurDim1<TAILLEPLATEAU; compteurDim1++)
		{
		for(int compteurDim2=0; compteurDim2<TAILLEPLATEAU; compteurDim2++)
			{
			if(contenu_plateau[compteurDim1][compteurDim2][0]!=' ')
				{
				sprintf(chemin, "./Images/%c.bmp", contenu_plateau[compteurDim1][compteurDim2][0]);//affiche chaque lettre deja posée
				pos.y=(ESPACEMENT+BORDURE)+(compteurDim1*(TAILLECASE+BORDURE));
				pos.x=(ESPACEMENT+BORDURE)+(compteurDim2*(TAILLECASE+BORDURE));
				afficher_image(chemin, pos);
				}
			}
		}
	actualiser();
	}
	
/******************************************************************************/
/* CLIC BOUTON                                            			          */
/******************************************************************************/
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

/******************************************************************************/
/* SCORE				                                                      */
/******************************************************************************/
int score(int scoreJoueur, Point emplacement_lettre_old[TAILLEMAIN], char contenu_plateau[TAILLEPLATEAU][TAILLEPLATEAU][2], Lettres indexLettre)
	{
	char lettre;
	
	int compLettre;
	int compteur;
	
	int multLettre=0;
	int multMot=0;
	int scoreTour=0;
	
	for(compteur=0; compteur<TAILLEMAIN && emplacement_lettre_old[compteur].x; compteur++)								//si le clic est non nul dans l'historique (car initialisée a nul si non utilisée) alors on boucle
		{
		emplacement_lettre_old[compteur]=convertirEnCaseTableau(emplacement_lettre_old[compteur]);						//on obtiens l'emplacement dans le tableau contenu_plateau de chaque lettre posée
		lettre=contenu_plateau[emplacement_lettre_old[compteur].y][emplacement_lettre_old[compteur].x][0];				//on récupère la lettre grace a la valeur précédente
		for(compLettre=0; compLettre<27 && lettre!=indexLettre.lettre[compLettre]; compLettre++){}						//on cherche l'identificateur de la lettre dans la structure Lettres pour pouvoir récupérer sa valeur en points 
		multLettre=multiplicateurLettre(contenu_plateau[emplacement_lettre_old[compteur].y][emplacement_lettre_old[compteur].x][1]);//on récupère son multiplicateur si la case sur laquelle elle est posée est une case spéciale
		multMot+=multiplicateurMot(contenu_plateau[emplacement_lettre_old[compteur].y][emplacement_lettre_old[compteur].x][1]);//de même pour le mot
		scoreTour+=(indexLettre.valeur[compLettre]*multLettre);//on ajoute donc la valeur de la lettre et son multiplicateur si il y en a un
		}
	if(!multMot)//si aucun multiplicateur de mot alors on le met a 1
		multMot++;
	scoreJoueur+=scoreTour*multMot;//on multiplie le mot par son coef
	if(compteur==TAILLEMAIN)
		scoreJoueur+=50;//si on pose 7 lettres d'un coup + 50 (si on a 
	return scoreJoueur;
	}

/******************************************************************************/
/* MULTIPLICATEUR LETTRE                                                      */
/******************************************************************************/
int multiplicateurLettre(char codeMultLettre)
	{
	if(codeMultLettre=='L')
		return 3;
	else if(codeMultLettre=='l')
		return 2;
	return 1;
	}

/******************************************************************************/
/* MULTIPLICATEUR MOT                                                         */
/******************************************************************************/
int multiplicateurMot(char codeMultMot)
	{
	if(codeMultMot=='M')
		return 3;
	else if(codeMultMot=='m')
		return 2;
	return 0;//si on retourne 1 alors la valeur est faussée
	}

/******************************************************************************/
/* ECHANGER LES BOUTONS VALIDER ET PASSER SON TOUR                            */
/******************************************************************************/
void switchValiderPasser(int numCoup, int comptLettre)
	{
	Point afficherBouton={100,922};
	if(comptLettre==0 && numCoup)//si on a pas encore posé de lettre ou si on est pas au premier coup
		afficher_image("./Images/bouton_passer.bmp", afficherBouton);
	else if(comptLettre==1 || !numCoup)//si on est au premier coup ou si on n'as pas encore posé de lettre
		afficher_image("./Images/bouton_valider.bmp", afficherBouton);
	actualiser();
	}

/******************************************************************************/
/* PIOCHER				                                                      */
/******************************************************************************/
void piocher(char mains[2][TAILLEMAIN], Lettres * indexLettre, int indexTirage[27], int joueur)
	{
	Point afficher_bouton={276,922};
	Point lettre_a_changer[10];
	int compteur;
	
	afficher_image("./Images/bouton_piocher_select.bmp", afficher_bouton);
	switchValiderPasser(1, 1);
	
	for(compteur=0; compteur<10; compteur++)
		{
		lettre_a_changer[compteur].x=0;
		lettre_a_changer[compteur].y=0;
		}
	
	for(compteur=0; clicBouton(lettre_a_changer[compteur-1])!=1; compteur++)//tant que pas de validation,
		{
		do{
			lettre_a_changer[compteur]=attendreSelectionLettre(joueur, lettre_a_changer, 1, 0, 0, lettre_a_changer[compteur]);//on demande de selectionner des lettres
			}while(clicBouton(lettre_a_changer[compteur])==2 || clicBouton(lettre_a_changer[compteur])==3);// et les autres bouttons ne fonctionnent pzs (car attendreselectionlettre autorise a cliquer sur les boutons
		if(clicBouton(lettre_a_changer[compteur])!=1)//si le clic n'est pas sur valider
			selectionLettre(joueur, mains, lettre_a_changer[compteur]);//on entoure ne rouge avec cette fonction
		}
	updateMainJoueur(mains, joueur, lettre_a_changer, indexLettre, indexTirage);//on demande de tirer les lettres choisies
	afficher_image("./Images/bouton_piocher.bmp", afficher_bouton);
	}

/******************************************************************************/
/* AFFICHAGE DES SCORES                                                       */
/******************************************************************************/
void afficherScore(int scores[2])
	{
	char score_joueur_1[15];
	char score_joueur_2[15];

	Point emplacement_joueur_1 = {5, 210};
	Point emplacement_joueur_2 = {905, 210};
	Point emplacement_score_joueur_1 = {15, 760};
	Point emplacement_score_joueur_2 = {915, 760};

	Point taille_texte_joueur_1;
	Point taille_texte_joueur_2;
	Point taille_score_texte_joueur_1;
	Point taille_score_texte_joueur_2;

	for(int i=0; i<15; i++)
		{
		score_joueur_1[i]='\0';
		score_joueur_2[i]='\0';
		}

	sprintf(score_joueur_1, "%d pts", scores[0]);	//Rajoute pts derrière le score (joueur 1)
	sprintf(score_joueur_2, "%d pts", scores[1]);	//Rajoute pts derrière le score (joueur 1)

	taille_texte_joueur_1 = taille_texte("Joueur 1", 20);																	/********************************************************/
	taille_texte_joueur_2 = taille_texte("Joueur 2", 20);																	/*														*/
																															/*														*/
	taille_score_texte_joueur_1 = taille_texte(score_joueur_1, 20);															/*														*/
	taille_score_texte_joueur_2 = taille_texte(score_joueur_2, 20);
																															/*Recouvre la zone de texte en vert (couleur du plateau)*/
	dessiner_rectangle(emplacement_joueur_1, taille_texte_joueur_1.x, taille_texte_joueur_1.y, 1687154);
	dessiner_rectangle(emplacement_joueur_2, taille_texte_joueur_2.x, taille_texte_joueur_2.y, 1687154);					/*														*/
																															/*														*/
	dessiner_rectangle(emplacement_score_joueur_1, taille_score_texte_joueur_1.x, taille_score_texte_joueur_1.y, 1687154);	/*														*/
	dessiner_rectangle(emplacement_score_joueur_2, taille_score_texte_joueur_2.x, taille_score_texte_joueur_2.y, 1687154);	/********************************************************/

	afficher_texte("Joueur 1", 20, emplacement_joueur_1, blanc);			/************************************/
	afficher_texte("Joueur 2", 20, emplacement_joueur_2, blanc);
																			/* Affiche le score des joueurs		*/
	afficher_texte(score_joueur_1, 20, emplacement_score_joueur_1, blanc);
	afficher_texte(score_joueur_2, 20, emplacement_score_joueur_2, blanc);	/************************************/
	}
