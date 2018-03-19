#include <stdio.h>
#include <stdlib.h>

#include "../include/plateau.h"

#define P_TRES_HAUTE	4
#define P_HAUTE 		3
#define P_NORMAL 		2
#define P_BASSE 		1
#define P_NULLE 		0

// TODO : Faire une fonction qui alloue de la mémoire et vérifie le résultat.
// Remplacer malloc() par cette fonction.

Case** InitPlateau(int taille)
{
	Case **plateau = NULL;
	int offset = 2;
	int milieu = 0;
	int nb_cases_col = 2;
	int nb_cases_init = 0;
	short moitie = 0;

	// Alloue de l'espace pour la 1ere ligne de la matrice.
	plateau = (Case **) malloc(sizeof(Case *) * taille);
	if (plateau == NULL)
		return NULL;
	
	for (int colonne = 0; colonne < taille; colonne++)
	{
		nb_cases_init = 0;

		// Alloue de l'espace pour chaque colonne de la matrice.
		plateau[colonne] = (Case *) malloc(sizeof(Case) * taille); 
		if (plateau[colonne] == NULL)
			return NULL;

		for (int ligne = 0; ligne < taille; ligne++)
		{
			// Initialise les cases de la colonne.
			if ( (ligne >= offset && nb_cases_init < nb_cases_col) )
			{
				milieu = taille / 2;

				// Case du milieu.
				if (colonne == milieu && ligne == milieu)
				{
					plateau[colonne][ligne].taille = 0;
					plateau[colonne][ligne].couleur = '.';
				}
				else
				{			
					plateau[colonne][ligne].taille = 1;

					if ( (colonne + ligne) % 2 == 0)
						plateau[colonne][ligne].couleur = 'N';
					else
						plateau[colonne][ligne].couleur = 'R';
				}

				nb_cases_init++;
			}
			else
			{
				plateau[colonne][ligne].taille = 0;
				plateau[colonne][ligne].couleur = ' ';
			}
		}

		if (!moitie)
		{
			// Première colonne.
			if (nb_cases_col == 2)
				offset -= 1;

			// On augmente le nombre de cases dans la colonne jusqu'à atteindre la
			// limite (taille).
			if (nb_cases_col + 2 < taille)
			{
				nb_cases_col += 2;
			}
			else if (nb_cases_col + 1 == taille)
			{
				nb_cases_col += 1;
				offset = 0;
				moitie = 1;
			}
		}
		else
		{
			// On diminue le nombre de case dans la colonne.
			if (nb_cases_col == taille)
			{
				nb_cases_col -= 1;
			}
			else
			{
				nb_cases_col -= 2;
				offset += 2;
			}

			// Dernière colonne.
			if (nb_cases_col == 2)
				offset -= 1;
		}
	}

	return plateau;
}

void FreePlateau(Case **p, int taille)
{
	for (int i = 0; i < taille; i++)
		free(p[i]);
	free(p);
}

void AfficherPlateau(Case **plateau, int taille)
{
	Case c;
	char lettre = 'A';

	// Affiche les coordonnées horizontales.
	printf("    ");
	for (int i=0; i < (taille* 5); ++i)
	{
		if (i % 5 == 0)
			putchar(lettre++);
		else
			putchar(' ');
	}
	putchar('\n');

	for (int y = 0; y < taille; y++)
	{
		AfficherDelimiteur(taille);

		for (int x = 0; x < taille; x++)
		{
			c = plateau[x][y];

			// Affiche les coordonnées verticales.
			if (x == 0)
				printf("%d ", y);

			if (c.taille > 0)
				printf("| %d%c ", c.taille, c.couleur);
			else
				printf("|    ");

			// Bord droit du tableau.
			if (x == taille-1)
				putchar('|');
		}
		putchar('\n');
	}
	
	AfficherDelimiteur(taille);

}

void AfficherDelimiteur(int taille)
{
	printf("  ");
	for (int i=0; i < (taille* 5); ++i)
	{
		if (i % 5 != 0)
			putchar('-');
		else
			putchar('+');
	}
	puts("+");
}

int DeplacerPion(Case **plateau, int taille, 
	int col, int ligne, 
	int col_dst, int ligne_dst
)
{
	Case *src, *dst;
	int erreur = 0;
	int ecart_col = 0; 
	int ecart_ligne = 0;
	short coord_existent = 1;
	short adjacentes = 1;
	
	// TODO : Isoler chaque test dans une fonction ?

	// Vérifie que les coordonnées appartiennent à la matrice.
	if ( !(col >= 0 && col < taille) )
		coord_existent = 0;
	if ( !(ligne >= 0 && ligne < taille) )
		coord_existent = 0;
	
	if ( !(col_dst >= 0 && col_dst < taille) )
		coord_existent = 0;
	if ( !(ligne_dst >= 0 && ligne_dst < taille) )
		coord_existent = 0;

	if (coord_existent)
	{
		// Test l'adjacence des tours.
		ecart_col = (col > col_dst) ? (col - col_dst) : (col_dst - col);
		if (ecart_col > 1)
			adjacentes = 0;
		
		ecart_ligne = (ligne > ligne_dst) ? (ligne - ligne_dst) : (ligne_dst - ligne);
		if (ecart_ligne > 1)
			adjacentes = 0;
		
		if (adjacentes)
		{
			src = &plateau[col][ligne];
			dst = &plateau[col_dst][ligne_dst];
			
			if ( MouvementAutorise(src, dst) )
			{
				// Déplace la tour.
				dst->taille += src->taille;
				dst->couleur = src->couleur;

				src->taille = 0;
				src->couleur = '.';

				//deplace = 1;
			}
			else
			{
				erreur = 3;
			}
		}
		else
		{
			erreur = 2;
		}
	}
	else
	{
		erreur = 1;
	}

	return erreur;
}

int MouvementAutorise(Case *src, Case *dst)
{
	int autorise = 1;
	int taille_tour = 0;

	// Les cases n'appartiennent pas au plateau (cases jouables).
	//if (src->taille == -1 || dst->taille == -1)
	//	autorise = 0;
	
	// Une des tour est vide.
	if (src->taille == 0 || dst->taille == 0)
		autorise = 0;
	
	taille_tour = src->taille + dst->taille;
	
	// Taille de la nouvelle tour trop grande.
	if (taille_tour > 5)
		autorise = 0;
	
	return autorise;
}

void SaisieDeplacement(char *coords_src, char *coords_dst)
{
	printf("\nTour à déplacer (A1) : ");
	scanf("%s", coords_src);
	printf("Destination (A1) : ");
	scanf("%s", coords_dst);

	coords_src[2] = '\0';
	coords_dst[2] = '\0';
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

int GenererMouvements(Case **plateau, int taille, Mouvement *mouvs, char couleur)
{
	Case c;
	int nb_mouv = 0;

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
				couleur
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
	char couleur
)
{
	Case tmp;
	int i = 0, j = 0;
	int valeur = 0;
	int niveau_ia = 2;

	// Initialise 'i' et 'j'.
	i = (x > 0) ? (x - 1) : x;
	j = (y > 0) ? (y - 1) : y;
	// Est-ce qu'une colonne existe à gauche ?
	if (i != x)
	{
		// Test les cases de la colonne de gauche.
		for (; (j <= y+1) && (j < taille); j++)
		{
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

	nb_mouv = GenererMouvements(plateau, taille, mouvs, 0);

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

/*
 * Sauvegarde une partie. Le format est le suivant :
 * 		taille_plateau
 * 		1N3NR4N0N...
 * 		...
 * Les lettres des couleurs sont aux indices impairs et les tailles aux indices
 * pairs.
 */
int SauvegarderPartie(Case **plateau, int taille)
{
	FILE *sauvegarde;
	Case c;

	sauvegarde = fopen("sauvegarde.txt", "w");

	if (sauvegarde == NULL)
		return EXIT_FAILURE;

	fprintf(sauvegarde, "%d\n", taille);

	for (int x = 0; x < taille; x++)
	{
		for (int y = 0; y < taille; y++)
		{
			c = plateau[x][y];

			fprintf(sauvegarde, "%d%c", c.taille, c.couleur);
		}
		fprintf(sauvegarde, "\n");
	}

	fclose(sauvegarde);

	return EXIT_SUCCESS;
}

Case** ChargerPartie()
{
	Case **plateau = NULL;
	FILE *sauvegarde;
	int taille;
	int taille_ligne;
	char *ligne_fichier;

	sauvegarde = fopen("sauvegarde.txt", "r");

	if (sauvegarde == NULL)
		exit(EXIT_FAILURE);

	fscanf(sauvegarde, "%d", &taille);

	// Une ligne à une taille de 2 fois le nombre de case.
	taille_ligne = taille * 2 + 1;
	ligne_fichier = (char *)malloc(sizeof(char) * taille_ligne);
	if (ligne_fichier == NULL)
		return NULL;

	// Alloue de l'espace pour la 1ere ligne de la matrice.
	plateau = (Case **) malloc(sizeof(Case *) * taille);
	if (plateau == NULL)
		return NULL;
	
	// Nettoie le buffer ('\n' restant de la ligne précédente).
	fgets(ligne_fichier, taille_ligne + 1, sauvegarde);

	for (int colonne = 0; colonne < taille; colonne++)
	{
		// Alloue de l'espace pour chaque colonne de la matrice.
		plateau[colonne] = (Case *) malloc(sizeof(Case) * taille); 
		if (plateau == NULL)
			return NULL;
		
		fgets(ligne_fichier, taille_ligne + 1, sauvegarde);

		for (int ligne = 0; ligne < taille; ligne++)
		{
			// Taille aux indices pairs et couleur aux indices impairs.
			// Manipulation du code ascii pour la conversion du char en int.
			plateau[colonne][ligne].taille = ligne_fichier[ligne * 2] - 0x30;
			plateau[colonne][ligne].couleur = ligne_fichier[ligne * 2 + 1];
		}
	}

	fclose(sauvegarde);

	return plateau;
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
	Case c;
	int cases_autour = 0;

	//c = plateau[src_x][src_y];
	cases_autour = NbCaseAutour(plateau, taille, src_x, src_y);
	
	// Taille tour finale == 5 (taille max), seconde priorité.
	if (plateau[src_x][src_y].taille + plateau[dst_x][dst_y].taille == 5)
		return 2;

	// 1 seul pion autour:
	// 	- si le pion est de notre couleur, priorité max : return 3
	// 	- sinon, coup défensif : return 1
	if (cases_autour == 1)
		return (plateau[dst_x][dst_y].couleur == couleur) ? 3 : 1;
	
	// Nb pions autour impair.

	return 0;
}

int NbCaseAutour(Case **plateau, int taille, int x, int y)
{
	Case tmp;
	int i = 0, j = 0;
	int nb_case_autour = 0;

	// Initialise 'i' et 'j'.
	i = (x > 0) ? (x - 1) : x;
	j = (y > 0) ? (y - 1) : y;
	// Est-ce qu'une colonne existe à gauche ?
	if (i != x)
	{
		// Test les cases de la colonne de gauche.
		for (; (j <= y+1) && (j < taille); j++)
		{
			tmp = plateau[i][j];

			if (tmp.taille != 0) 
				nb_case_autour++;
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

		tmp = plateau[i][j];

		if (tmp.taille != 0) 
			nb_case_autour++;
	}
	
	// Réinitialise 'i' et 'j'.
	i = (x < taille - 1) ? (x + 1) : x;
	j = (y > 0) ? (y - 1) : y;
	// Test les cases de la colonne de droite.
	if (i != x)
	{
		for (; (j <= y+1) && (j < taille); j++)
		{
			tmp = plateau[i][j];

			if (tmp.taille != 0) 
				nb_case_autour++;
		}
	}

	return nb_case_autour;
}
