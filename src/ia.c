#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "../include/ia.h"
#include "../include/plateau.h"

Mouvement *IANiveau1(Case **plateau, int taille)
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

	nb_mouv = GenererMouvements(plateau, taille, mouvs);
	
	// Initialise le générateur de nombre aléatoire.
	srand(time(&t));
	*m = mouvs[rand() % nb_mouv];
	
	// Execute le coup.
	DeplacerPion(plateau, taille, m->x_src, m->y_src, m->x_dst, m->y_dst);
	
	free(mouvs);

	return m;
}
