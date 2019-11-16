READ ME - Projet Scrabble

Un projet réalisé par Antoine Lumerutti et Pablo Rican

Notre projet est un jeu de Scrabble jouable en 1 vs 1 sur ordinateur en local.

Fichiers du jeu :
	- main.c : Contient tout le code créé pour le Scrabble
	- dictionnaire_fr_ss_accents : Dictionnaire qui permet de contrôler l'existence du mot
	- Images : Contient toutes les images nécessaires du point de vu graphique du jeu
	- Save : Contient les fichiers de sauvegarde d'une partie en cours

Pour jouer :
	- Placer vous dans le dossier "Jeu"
	- Lancez l'executable en tapant dans la console "./resultat". Si l'executable n'existe pas,
	tapez "make" dans la console, un executable sera créé.

Fonctionnalités :
	- Pour commencer une partie, il vous suffit de cliquer sur le bouton "Jouer" du menu.

	- Le premier joueur pourra choisir sa première lettre et la placer au milieux du plateau.
	Une fois le mot placé, il faut appuyer sur le bouton "Valider", si votre mot est correct,
	le tour passe au deuxième joueur, en revanche, si le mot n'est pas bon,
	les lettres reviennent dans votre main et vous devez à nouveau placer votre mot.
	Pour valider le premier mot, il faut au moins placer deux lettres.

	- Si vos lettres ne vous inspirent pas, pas de panique, vous pouvez piocher de nouvelles lettres.
	Pour ce faire, appuyez sur le bouton "Piocher", sélectionnez les lettres à changer,
	puis appuyez sur le bouton "Valider". Le tour passera au joueur suivant, et votre jeu sera changé.

	- Dans le cas où vous souhaitez juste passer votre tour, avant de sélectionner une lettre,
	le bouton à l'emplacement "Valider", se nomme "Passer". Cliquer dessus et le tour passera au joueur suivant.

	- Vous êtes en plein milieu d'une partie endiablée, mais il faut que vous arrêtiez ? Pas de panique !
	Un bouton "Sauvegarder" et la pour vous sauver ! Appuyez sur ce bouton après avoir validé votre tour,
	le jeu se fermera en ayant sauvegardé le contenu du plateau, vos mains, vos scores etc...
	Pour pouvoir réaccéder à cette folle partie, relancer le jeu et une fois dans le menu,
	cliquez sur le bouton "Charger". Le plateau se réouvrira avec toutes vos pièces

