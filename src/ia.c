#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "../include/plateau.h"
#include "../include/avalam.h"
#include "../include/ia.h"

Mouvement *IA(Case **plateau, int taille, char couleur, int niveau_ia)
{
	Mouvement *mouvs;
	Mouvement *m;
	int priorite_max = P_BASSE;
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
		
        index = rand() % nb_mouv;
		*m = mouvs[index];
	}
	else
	{
		nb_mouv = GenererMouvements(plateau, taille, mouvs, couleur, 2, &priorite_max);

		index = rand() % nb_mouv;
        printf("index : %d\n", index);

		// Prend le premier mouvement de priorite la plus haute parmit la liste
		// de mouvements. Parcour la liste de manière circulaire en commencant à
		// un index aléatoire pour mieux réparir les coups sur le plateau de
		// jeu.
		for (int x = 0; mouvs[index].valeur != priorite_max && x < nb_mouv; index++, x++)
		{
			if (index == nb_mouv-1)
				index = 0;
		}

		*m = mouvs[index];
	}
	
	// Execute le coup.
	DeplacerPion(plateau, taille, m->src_x, m->src_y, m->dst_x, m->dst_y);
	
	free(mouvs);

	return m;
}

int EvaluerValeurCoup(
	Case **plateau,
	Mouvement *mouvs,
	int taille,
	int src_x, int src_y,
	int dst_x, int dst_y,
	char couleur
)
{
	Case src, dst;
	int cases_autour = 0;

	cases_autour = NbCaseAutour(plateau, taille, src_x, src_y);
	src = plateau[src_x][src_y];
	dst = plateau[dst_x][dst_y];
	
	// Taille tour finale == 5 (taille max), seconde priorité.
	if (src.taille + dst.taille == 5 && src.couleur == couleur)
		return P_HAUTE;

	// 1 seul pion autour:
	// 	- si le pion est de notre couleur, priorité max
	// 	- sinon, coup défensif
	if (cases_autour == 1)
		return (dst.couleur == couleur) ? P_TRES_HAUTE : P_MOYENNE;
	
	return P_BASSE;
}
