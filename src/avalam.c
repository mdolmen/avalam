#include <stdio.h>
#include <stdlib.h>

#include "../include/plateau.h"
#include "../include/avalam.h"

int MouvementAutorise(Case *src, Case *dst)
{
	int autorise = 1;
	int taille_tour = 0;

	// Une des tour est vide.
	if (src->taille == 0 || dst->taille == 0)
		autorise = 0;
	
	taille_tour = src->taille + dst->taille;
	
	// Taille de la nouvelle tour trop grande.
	if (taille_tour > 5)
		autorise = 0;
	
	return autorise;
}

void AfficherErreur(int code)
{
	switch (code)
	{
		case 0:
			break;
		case 1:
			puts("\n[-] Mauvaises coordonnées.");
			break;
		case 2:
			puts("\n[-] Tours non adjacentes.");
			break;
		case 3:
			puts("\n[-] Mouvement interdit.");
			break;
		default:
			puts("\n[-] Erreur inconnue.");
	}
}

int GenererMouvements(
	Case **plateau,
	int taille,
	Mouvement *mouvs,
	char couleur,
	int niveau_ia,
	int *priorite_max
)
{
	Case c;
	int nb_mouv = 0;

	// Réinitialise la priorité.
    if (priorite_max)
	    *priorite_max = P_BASSE;

	// Parcours l'ensemble du plateau.
	for (int x = 0; x < taille; x++)
	{
		for (int y = 0; y < taille; y++)
		{
			c = plateau[x][y];
			if (c.taille < 0 || c.taille > 5)
				continue;

			TestMouvementPossibleCase(
				plateau, 
				mouvs, 
				&c, 
				x, y, 
				taille, 
				&nb_mouv,
				couleur,
				niveau_ia,
				priorite_max
			);
		}
	}

	return nb_mouv;
}

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
)
{
	Case tmp;
	int i = 0, j = 0;
	int valeur = 0;

	// Initialise 'i' et 'j'.
	i = (x > 0) ? (x - 1) : x;
	j = (y > 0) ? (y - 1) : y;
	// Est-ce qu'une colonne existe à gauche ?
	if (i != x)
	{
		// Test les cases de la colonne de gauche.
		for (; (j <= y+1) && (j < taille); j++)
		{
			// TODO : factoriser tout ça dans une fonction.
			tmp = plateau[i][j];
			if (MouvementAutorise(c, &tmp)) 
			{ 
				// Coords de la source.
				mouvs[*nb_mouv].src_x = x;
				mouvs[*nb_mouv].src_y = y;
				// Coords de la destination.
				mouvs[*nb_mouv].dst_x = i;
				mouvs[*nb_mouv].dst_y = j;

				if (niveau_ia == 2)
				{
					valeur = EvaluerValeurCoup(
						plateau,
						mouvs,
						taille,
						x, y, // coordonnées source
						i, j, // coordonnées destination
						couleur
					);

					// Si la valeur retournée est 3 ou 1, le mouvement prioritaire
					// est de la destination vers la source (et non l'inverse, on
					// cherche à isoler le pion pour lequel on test les coups
					// possible. Un coup est forcément bidirectionnel).
					if (valeur == 3 || valeur == 1)
					{
						// Coords de la source.
						mouvs[*nb_mouv].src_x = i;
						mouvs[*nb_mouv].src_y = j;
						// Coords de la destination.
						mouvs[*nb_mouv].dst_x = x;
						mouvs[*nb_mouv].dst_y = y;
					}

					if (priorite_max)
						if (valeur > *priorite_max)
							*priorite_max = valeur;
				}
				
				mouvs[*nb_mouv].valeur = valeur;

				(*nb_mouv)++;
			}
		}
	}

	// Réinitialise 'i' et 'j'.
	i = x;
	j = (y > 0) ? (y - 1) : y;
	// Test les cases de la colonne du milieu.
	for (; (j <= y+1) && (j < taille); j++)
	{
		// Case pour laquelle on test les mouvement.
		if (j == y) { continue; }

		// TODO : factoriser tout ça dans une fonction.
		tmp = plateau[i][j];
		if (MouvementAutorise(c, &tmp)) 
		{ 
			// Coords de la source.
			mouvs[*nb_mouv].src_x = x;
			mouvs[*nb_mouv].src_y = y;
			// Coords de la destination.
			mouvs[*nb_mouv].dst_x = i;
			mouvs[*nb_mouv].dst_y = j;

			if (niveau_ia == 2)
			{
				valeur = EvaluerValeurCoup(
					plateau,
					mouvs,
					taille,
					x, y, // coordonnées source
					i, j, // coordonnées destination
					couleur
				);

				// Si la valeur retournée est 3 ou 1, le mouvement prioritaire
				// est de la destination vers la source (et non l'inverse, on
				// cherche à isoler le pion pour lequel on test les coups
				// possible. Un coup est forcément bidirectionnel).
				if (valeur == 3 || valeur == 1)
				{
					// Coords de la source.
					mouvs[*nb_mouv].src_x = i;
					mouvs[*nb_mouv].src_y = j;
					// Coords de la destination.
					mouvs[*nb_mouv].dst_x = x;
					mouvs[*nb_mouv].dst_y = y;
				}

				if (priorite_max)
					if (valeur > *priorite_max)
						*priorite_max = valeur;
			}
			
			mouvs[*nb_mouv].valeur = valeur;

			(*nb_mouv)++;
		}
	}
	
	// Réinitialise 'i' et 'j'.
	i = (x < taille - 1) ? (x + 1) : x;
	j = (y > 0) ? (y - 1) : y;
	// Test les cases de la colonne de droite.
	if (i != x)
	{
		for (; (j <= y+1) && (j < taille); j++)
		{
			// TODO : factoriser tout ça dans une fonction.
			tmp = plateau[i][j];
			if (MouvementAutorise(c, &tmp)) 
			{ 
				// Coords de la source.
				mouvs[*nb_mouv].src_x = x;
				mouvs[*nb_mouv].src_y = y;
				// Coords de la destination.
				mouvs[*nb_mouv].dst_x = i;
				mouvs[*nb_mouv].dst_y = j;

				if (niveau_ia == 2)
				{
					valeur = EvaluerValeurCoup(
						plateau,
						mouvs,
						taille,
						x, y, // coordonnées source
						i, j, // coordonnées destination
						couleur
					);

					// Si la valeur retournée est 3 ou 1, le mouvement prioritaire
					// est de la destination vers la source (et non l'inverse, on
					// cherche à isoler le pion pour lequel on test les coups
					// possible. Un coup est forcément bidirectionnel).
					if (valeur == 3 || valeur == 1)
					{
						// Coords de la source.
						mouvs[*nb_mouv].src_x = i;
						mouvs[*nb_mouv].src_y = j;
						// Coords de la destination.
						mouvs[*nb_mouv].dst_x = x;
						mouvs[*nb_mouv].dst_y = y;
					}

					if (priorite_max)
						if (valeur > *priorite_max)
							*priorite_max = valeur;
				}
				
				mouvs[*nb_mouv].valeur = valeur;

				(*nb_mouv)++;
			}
		}
	}
}

int PartieNonFinit(Case **plateau, int taille, int *rouge, int *noir)
{
	Mouvement *mouvs;
	int nb_mouv = 0;

	mouvs = (Mouvement *)malloc(sizeof(Mouvement) * taille * taille * 8);
	if (mouvs == NULL)
		return 0;

	nb_mouv = GenererMouvements(plateau, taille, mouvs, 0, 1, NULL);

	if (nb_mouv == 0)
	{
		CompterPoints(plateau, taille, rouge, noir);
	}

	return nb_mouv;
}

void CompterPoints(Case **plateau, int taille, int *rouge, int *noir)
{
	Case c;

	for (int y = 0; y < taille; y++)
	{
		for (int x = 0; x < taille; x++)
		{
			c = plateau[x][y];

			if (c.couleur == 'N')
				(*noir)++;
			else if (c.couleur == 'R')
				(*rouge)++;
		}
	}
}
