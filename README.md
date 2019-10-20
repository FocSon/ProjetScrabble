# ProjetScrabble
DUT INFO project (S1)

infos utiles sur le plateau
  taille : 690x690px
  bordures entre les cases : 3px
  taille cases : 35x37px
  taille lettres : 35x36px
  coté gauche : 79px de largeur (jusqu'à la bordure noire)
  coté haut : 35px de largeur (jusqu'à la bordure noire)
  

Pour verifier si point compte double ou triple des mots ou des points, verifier la couleur de la case avant de mettre lettre dessus
CECI EST IMPOSSIBLE : les couleurs sur les cases ne sont pas homogènes

====> tableau double dim ou on renseigne laveleur des cases et si le lettre ou le mot qui vaut + (M3=mot compte triple OU L2 pour lettre compte double) par exemple
Possibilité de trouver un algo qui définit la valeur des cases =====> il semble y avoir un patern

Mot compte triple: aux quatres cions +espacés de 6 cases entre elles
===> essayer
if(7%case+1==1)
  tableauPoint[posx][posy]=3;
  
Mot compte double :x=y et 1<x<5 + la case du milieu
                   donc if(x=y && ((x<5 && x>1) || x=largeur du plateau/2 )) alors mot compte double
(x et y représentent le nombres de cases sur le plateau en x et en y)

lettre compte double: espacé de 3 cases des bords et colés aux bordues + aux quatres coins du centres + milieu de chaque côtés espacés de 3 puis de 2 à sa gauche et à sa droite
if( ((x==0 || x=longeur) && (y+5)%8==0 ) || ((y==0 || y=largeur) && (x+5)%8==0 ) || (x==longeur/2 && (y+5)%8==0) || (y==longeur/2 && (x+5)%8==0) || ((y<largeur/2) && (x==(larg/2)+1 || x==(larg/2)+1) && (y+2)%4==0) || ((y>largeur/2) && (x==(larg/2)+1 || x==(larg/2)+1) && ((y-largeur/2)+3)%4==0) )
à verifier

lettre compte triple: en x=2, x=6, x=10 et x=14
                      ce qui reviens à (x+3)%4==0
                      
                      en y=2, y=6, y=10 et y=14
                      ce qui reviens à (y+3)%4==0
                      
                      donc if((x+3)%4==0 && (y+3)%3==0) alors la lettre compte triple
