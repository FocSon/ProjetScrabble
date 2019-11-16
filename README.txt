READ ME - Projet Scrabble

Un projet réalisé par Antoine Lumerutti et Pablo Rican

Notre projet est un jeu de Scrabble jouable en 1 vs 1 sur ordinateur en local.

Fichiers du jeu :
	- main.c : Contient tout le code créé pour le Scrabble
	- dictionnaire_fr_ss_accents : Dictionnaire qui permet de contrôler l'existence d'un mot
	- Images : Contient toutes les images nécessaires du point de vue graphique du jeu
	- Save : Contient les fichiers de sauvegarde d'une partie en cours

Pour jouer :
	- Placez-vous dans le dossier "Jeu"
	- Lancez l'exécutable en tapant dans la console "./resultat". Si l'exécutable n'existe pas,
	tapez "make" dans la console, un exécutable sera créé.

Fonctionnalités :
	- Pour commencer une partie, il vous suffit de cliquer sur le bouton "Jouer" du menu.

	- Le premier joueur pourra choisir sa première lettre et la placer au milieu du plateau.
	Une fois le mot placé, il faut appuyer sur le bouton "Valider"; si votre mot est correct,
	le tour passe au deuxième joueur; en revanche, si le mot n'est pas bon,
	les lettres reviennent dans votre main et vous devez à nouveau placer un mot.
	Pour valider le premier mot, il faut au moins placer deux lettres.

	- Si vos lettres ne vous inspirent pas, pas de panique, vous pouvez piocher de nouvelles lettres.
	Pour ce faire, appuyez sur le bouton "Piocher", sélectionnez les lettres à changer,
	puis appuyez sur le bouton "Valider". Le tour passera au joueur suivant, et votre jeu sera changé.

	- Dans le cas où vous souhaitez juste passer votre tour, avant de sélectionner une lettre,
	le bouton à l'emplacement "Valider", se nomme "Passer". Cliquez dessus et le tour passera au joueur suivant.

	- Vous êtes en plein milieu d'une partie endiablée, mais vous devez l'interrompre ? Pas de panique !
	Un bouton "Sauvegarder" est là pour vous sauver ! Appuyez sur ce bouton après avoir validé votre tour,
	le jeu se fermera en ayant sauvegardé le contenu du plateau, vos mains, vos scores etc...
	Pour pouvoir réaccéder à cette partie, relancer le jeu et une fois dans le menu,
	cliquez sur le bouton "Charger". Le plateau se réouvrira avec toutes vos pièces.

Déroulement de la création du projet :
	Nous avons d'abord essayé de visualiser un peu toutes les fonctions, les types et variables
	dont allions avoir besoin. Nous avons ensuite commencé à créer la partie graphique du jeu (Pablo),
	et en parallèle tout ce qui n'avait pas besoin de cette partie graphique (recherche dans le dictionnaire,
	tirer les lettres, les initialiser, leur donner une valeur etc... (Antoine)). Une fois ce code mit en commun,
	nous avons testé tous les bugs que l'on pouvait rencontrer avec cette base de jeu. Puis nous nous sommes
	réparti les fonctions : interpretation des clics sur le plateau etc... (Pablo), création du menu,
	conditions pour poser les lettres etc... (Antoine).
	Enfin, nous avons peaufiné nos fonctions. Nous avons eu une situatation "critique" (une sortie de tableau,
	qui faisait échouer le programme) malgré cela, nous nous sommes réparti les tâches afin de ne pas prendre de retard,
	Antoine essayait de résoudre le problème tandis que Pablo continuait à avancer dans le programme.
	Une fois le problème résolu, il ne restait plus que quelques fonctionnalités à ajouter.

Bugs connus :
	- Quand il reste moins de 7 lettres dans la pioche, la condition d'arrêt ne fonctionne pas bien

Problèmes rencontrés :
	- Quelques erreurs avec des sorties de tableaux (peut prendre du temps pour trouver de quel tableau il s'agit)
	- Long travail sur la partie graphique du jeu.

Nos idées :
	- Ne pas contraindre un joueur à placer sa lettre sur le plateau quand il l'a sélectionnée mais pouvoir changer de sélection.
	- Travail sur les graphismes du jeu.
	- Boutons dynamiques dans le menu.
	- Encadrer les lettres sélectionnées en rouge pour que ce soit plus visuel.
	- Affichage du score directement sur le jeu et pas dans le terminal.

Conclusion :
	Ce projet nous a appris à travailler en équipe (horaires décalés,
	apprendre à bien communiquer avec son coéquipier, échanger des idées et les laisser murir etc...).
	On a pu découvrir comment créer un plus gros programme avec beaucoup plus de lignes de code, de fonctions etc...
	Ce projet a été très enrichissant, et nous sommes plutôt satisfaits du résultat.