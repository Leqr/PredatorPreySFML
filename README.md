# PredatorPreySFML
Ce programme modélise un environnement composé de moutons, de loups et d’herbes diverses.

1. Compilation :

Quatre manières de compiler partie6 avec toutes les fonctionnalitées : 
(dans le répertoire du fichier source)

	scons application-run (grand monde, application finale)
	scons application-run —cfg=appTest.json (monde plus petit)
	scons InfectionTestManyAnimals-run --cfg=infectManyAnimals.json
	scons InfectionTest-run --cfg=infect.json

2. Utilisation : 

Les commandes sur le panneau de droite indique les fonctionnalitées des différentes touches.

2.1 Entités :

Deux types d’animaux :

	Loups (prédateur)
	Moutons (nourriture pour les loups)

Quatre types de plantes (nourriture pour les moutons) :

Toutes les plantes fournissent de l’énergie aux moutons.
Les plantes ont différents effets spéciaux sur les moutons s’en nourrissant.

	Virtuous Plant (augmente la health)
	Toxic Plant (pe transmettre un virus)
	Cacti (baisse l’énergie du mouton)
	Plantes standards (aucun effet spécial)

2.1.1 Troupeaux :

Les moutons sont regroupés sous forme de deux troupeaux. L’affection d’un mouton à un troupeau est faîte lors de l’initialisation de ce dernier en pressant la touche 1 pour le premier troupeau ou la touche 2 pour le second troupeau.

2.2 Virus :

Il est possible d’infecter des animaux.
En fonction de la virulence du virus l’animal survivra ou non.
Les virus peuvent être transmis au sein d’une population d’un même type (ici pas de transmission entre les moutons et les loups)

2.3 Statistiques :

Il est possible d’avoir différents types de statistiques.

Générales : Les statistiques générales donnent une vision globale de l’environnement et de ces occupants.

Individuelles : Les statistiques des attributs et différents états d’une entité spécifique. (entité la plus proche du curseur lors de la pression sur F).

2.4 Mode Debug : 

Le mode Debug donne des indications plus précises sur les animaux (état, sexe, énergie, troupeau si mouton)



/****** Attention *******/
Une erreur liée au Genome survient lorsqu’il y a beaucoup d’entités

