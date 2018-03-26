#ifndef AI_H
#define AI_H

#include "plateau.h"
#include "avalam.h"

Mouvement *IA(Case **plateau, int taille, char couleur, int niveau_ia);

/* Evalue la valeur d'un coup pour déterminer sa prioritée. */
int EvaluerValeurCoup(
	Case **plateau,
	Mouvement *mouvs,
	int taille,
	int src_x, int src_y,
	int dst_x, int dst_y,
	char couleur
);

#endif // AI_H
