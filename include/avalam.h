#ifndef AVALAM_H
#define AVALAM_H

#include "plateau.h"

#define COL_MAX 5

#define P_TRES_HAUTE	4
#define P_HAUTE 		3
#define P_MOYENNE 		2
#define P_BASSE 		1
#define P_NULLE 		0

typedef struct {
	int taille;
	char couleur;
} Case;

typedef struct {
	int src_x;
	int src_y;
	int dst_x;
	int dst_y;
	int valeur;
} Mouvement;

int MouvementAutorise(Case *src, Case *dst);

/* Message d'erreur selon le retour de DeplacerPion(). */
void AfficherErreur(int code);

/*
 * Génére les mouvements possible pour une case. Retourne le nombre de mouvement
 * générés. 
 */
int GenererMouvements(
	Case **plateau, 
	int taille, 
	Mouvement *mouvs, 
	char couleur,
	int niveau_ia,
	int *priorite_max
);

/*
 * Les mouvement possible pour 'case' sont enregistré dans 'mouvs'.
 * Le nombre de mouvement totale est actualisé dans 'nb_mouv'.
 */
void TestMouvementPossibleCase(
	Case **plateau, 
	Mouvement *mouvs, 
	Case *c, 
	int x, int y, 
	int taille, 
	int *nb_mouv,
	char couleur,
	int niveau_ia,
	int *priorite_max
);

/*
 * Affiche le nombre de points par couleur si la partie est finit (plus de
 * mouvement possible.
 */
int PartieNonFinit(Case **plateau, int taille, int *rouge, int *noir);

/* 
 * Compte les points pour chaque couleur.
 * Les paramètres 'rouge' et 'noir' contiendront les résultats.
 */
void CompterPoints(Case **plateau, int taille, int *rouge, int *noir);

/* Evalue la valeur d'un coup pour déterminer sa prioritée. */
int EvaluerValeurCoup(
	Case **plateau,
	Mouvement *mouvs,
	int taille,
	int src_x, int src_y,
	int dst_x, int dst_y,
	char couleur
);

#endif // AVALAM_H
