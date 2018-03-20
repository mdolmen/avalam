#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "../include/ia.h"
#include "../include/plateau.h"

Mouvement *IA(Case **plateau, int taille, char couleur, int niveau_ia)
{
	Mouvement *mouvs;
	Mouvement *m;
	int priorite_max = 0;
	int nb_mouv = 0;
	int index = 0;
	time_t t;

	// La tour peut être déplacer sur 1 de ces 8 pions adjacents.
	mouvs = (Mouvement *)malloc(sizeof(Mouvement) * taille * taille * 8);
	
	m = (Mouvement *)malloc(sizeof(Mouvement));
	
	if (mouvs == NULL || m == NULL)
		return NULL;

	// Initialise le générateur de nombre aléatoire.
	srand(time(&t));

	if (niveau_ia == 1)
	{
		nb_mouv = GenererMouvements(plateau, taille, mouvs, couleur, 1, NULL);
		
		*m = mouvs[rand() % nb_mouv];
	}
	else
	{
		nb_mouv = GenererMouvements(plateau, taille, mouvs, couleur, 2, &priorite_max);

		index = rand() % nb_mouv;

		// Prend le premier mouvement de priorite la plus haute parmit la liste
		// de mouvements. Parcour la liste de manière circulaire en commencant à
		// un index aléatoire pour mieux réparir les coups sur le plateau de
		// jeu.
		for ( ; mouvs[index].valeur != priorite_max; index++)
		{
			if (index == nb_mouv)
				index = 0;
		}

		*m = mouvs[index];
	}
	
	// Execute le coup.
	DeplacerPion(plateau, taille, m->src_x, m->src_y, m->dst_x, m->dst_y);
	
	free(mouvs);

	return m;
}

/*
 * Le niveau de priorité le plus haut est prioritaire.
Mouvement *IANiveau2(Case **plateau, int taille)
{
	Mouvement **mouvs;
	Mouvement *m;
	int priorite_0 = 0;
	int priorite_1 = 0;
	int priorite_2 = 0;
	int priorite_3 = 0;

	// Tableau 2 dimensions dynamique.
	mouvs = (Mouvement **)malloc(sizeof(Mouvement *) * NIVEAUX_PRIORITE);
	
	// La tour peut être déplacer sur 1 de ces 8 pions adjacents.
	for (int i = 0; i < NIVEAUX_PRIORITE; i++)
		mouvs = (Mouvement *)malloc(sizeof(Mouvement) * taille * taille * 8);
	
	m = (Mouvement *)malloc(sizeof(Mouvement));

	for (int i = 0; i < nb_mouv; i++)
	{
		EvaluerValeurCoup();
	}

	return m
}
 */
