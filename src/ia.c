#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "../include/ia.h"
#include "../include/plateau.h"

#define PRIORITE_MAX 3

Mouvement *IANiveau1(Case **plateau, int taille, char couleur)
{
	Mouvement *mouvs;
	Mouvement *m;
	int nb_mouv = 0;
	time_t t;

	// La tour peut être déplacer sur 1 de ces 8 pions adjacents.
	mouvs = (Mouvement *)malloc(sizeof(Mouvement) * taille * taille * 8);
	m = (Mouvement *)malloc(sizeof(Mouvement));
	if (mouvs == NULL || m == NULL)
		return NULL;

	nb_mouv = GenererMouvements(plateau, taille, mouvs, couleur);
	
	// Initialise le générateur de nombre aléatoire.
	srand(time(&t));
	*m = mouvs[rand() % nb_mouv];
	
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
