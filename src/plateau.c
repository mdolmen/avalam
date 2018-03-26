#include <stdio.h>
#include <stdlib.h>

#include "../include/plateau.h"
#include "../lib/graphics.h"

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

void AfficherPlateauGUI(
	Case **plateau, 
	int taille,
	int largeur_plateau,
	int hauteur_plateau,
    int rayon
)
{
	Case c;
	char lettre = 'A';
	int x_pion, y_pion;
    int diametre = 0;

	y_pion = hauteur_plateau - rayon;
	x_pion = rayon;
    diametre = rayon << 1;
	
    // Vide le contenu de la fenêtre graphique.
    gr_clear_graph();

    // Décalage en haut à gauche.
    x_pion += diametre;
    // Affiche les coordonnées horizontales.
    for (int x = 0; x < taille; x++)
    {
        gr_moveto(x_pion, y_pion);
        gr_set_color(black);
        gr_draw_char(lettre++);
        x_pion += diametre;
    }
    x_pion = rayon;
    y_pion -= diametre;

	for (int y = 0; y < taille; y++)
	{
        // Affiche les coordonnées verticale.
        gr_set_color(black);
        gr_moveto(x_pion, y_pion);
        gr_draw_char('0' + y);

        x_pion += diametre;

		for (int x = 0; x < taille; x++)
		{
			c = plateau[x][y];


            // Affiche un pion.
			if (c.taille > 0)
            {
                if (c.couleur == 'R')
                    gr_set_color(red);
                else
                    gr_set_color(black);

				gr_fill_circle(x_pion, y_pion, rayon);

                // Indique la taille de la tour.
                gr_set_color(white);
                // Hack pour ajuster les coordonnées au centre.
                gr_moveto(x_pion-2, y_pion-4);
                gr_draw_char(c.taille + '0');
            }

			// On décale le x du prochain cercle de rayon * 2.
			x_pion += diametre;
		}

	    x_pion = rayon;
		y_pion -= diametre;
	}
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

void SaisieDeplacement(char *coords_src, char *coords_dst)
{
	printf("\nTour à déplacer (A1) : ");
	scanf("%s", coords_src);
	printf("Destination (A1) : ");
	scanf("%s", coords_dst);

	coords_src[2] = '\0';
	coords_dst[2] = '\0';
}

void SaisieDeplacementGUI(
    int taille,
    int diametre, 
    int *src_x, int *src_y, 
    int *dst_x, int *dst_y
)
{
    struct status *io_status = NULL;

    io_status = gr_wait_event(BUTTON_DOWN);
    *src_x = (io_status->mouse_x / diametre) - 1;
    // L'indice le plus petit en y dans la structure du plateau correspond au
    // pion du haut de la fenêtre graphique. Cependant, la valeur en y la plus
    // haute (pour la souris) correspond au haut de la fenêtre graphique.
    *src_y = (taille - 1) - (io_status->mouse_y / diametre);

    io_status = gr_wait_event(BUTTON_DOWN);
    *dst_x = (io_status->mouse_x / diametre) - 1;
    *dst_y = (taille - 1) - (io_status->mouse_y / diametre);
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
