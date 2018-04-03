#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../include/plateau.h"
#include "../include/avalam.h"
#include "../include/ia.h"
#include "../lib/graphics.h"

#define GEOMETRIE " 620x620"

// Volontairement plus petits que les dimensions de la fenêtre pour ne pas
// prendre en compte la marge lors du calcul des coordonnées au clic de la
// souris.
#define LARGEUR 600
#define HAUTEUR 600

#define RAYON 30
#define DIAMETRE 60

// TODO : ajouter une vérification des valeurs renvoyé par certaines fonctions
// (celles qui renvoient un pointeur).

// TODO : créer une structure 'jeu' pour contenir toutes les informations
// (plateau, taille, mouvements, couleur IA, etc.)
// 	=> juste un pointeur à passer (et bcp moins de params)

int partie_finie = 0;
Case **plateau = NULL;
int taille;

/*
 * Fonction exécutée lorsque le programme quitte normalement.
 */
void cleanup()
{
    char reponse = '\0';

    if (!partie_finie)
    {
		printf("\n\nLa partie n'est pas finie, voulez-vous l'enregistrer ? (o/n) : ");
		scanf(" %c", &reponse);

		if (reponse == 'o' || reponse == 'O')
        {
			if ( EXIT_SUCCESS == SauvegarderPartie(plateau, taille) )
                puts("Partie sauvegardée!");
            else
                puts("Erreur lors de la sauvegarde..");
        }
    }
}

int main(int argc, char *argv[])
{
	Mouvement *coup_ia = NULL;
	char coords_src[3];
	char coords_dst[3];
	
    int src_x, src_y;
    int dst_x, dst_y;
    
    int nb_tour = 0;
	int rouge = 0;
	int noir = 0;
	int ret;
	FILE* sauvegarde;
	char reponse;

	taille = TAILLE_STD;

	// TODO : plus de test sur la valeur entrée et le résultat de strtol().
	if (argc == 2)
		taille = strtol(argv[1], NULL, 10);
	
	// Charge une partie sauvegardée.
	if ( (sauvegarde = fopen("sauvegarde.txt", "r") ) != NULL)
	{
		printf("Il existe une partie sauvegardée. "
			"Voulez-vous la charger ? (o/n) : "
		);
		scanf(" %c", &reponse);

		if (reponse == 'o' || reponse == 'O')
			plateau = ChargerPartie();
		else
			plateau = InitPlateau(taille);
	}
	else
	{
		plateau = InitPlateau(taille);
	}

    // TODO : choisir entre console et GUI
    //
	// "Vide" le terminal et affiche le plateau.
	//printf("\033[2J\033[1;1H");
	//AfficherPlateau(plateau, taille);

    atexit(cleanup);

    // Initialise l'interfae graphique.
	gr_open_graph(GEOMETRIE);
	AfficherPlateauGUI(plateau, taille, LARGEUR, HAUTEUR, RAYON);

	// Boucle du jeu.
	while ( PartieNonFinit(plateau, taille, &rouge, &noir) )
	{
		if (nb_tour % 2 == 0)
		{
            // Demande un déplacement jusqu'à ce qu'un correct soit fournit.
			do {
				SaisieDeplacementGUI(taille, DIAMETRE, &src_x, &src_y, &dst_x, &dst_y);

				// Manipulation du code ASCII des caractères saisis pour avoir un
				// nombre entre 0 et 9.
				//ret = DeplacerPion(plateau, taille, 
				//	coords_src[0] - 0x41, coords_src[1] - 0x30,
				//	coords_dst[0] - 0x41, coords_dst[1] - 0x30
				//);

                ret = DeplacerPion(
                    plateau,
                    taille,
                    src_x, src_y,
                    dst_x, dst_y
                );
			
				if (ret)
					AfficherErreur(ret);
			} while (ret != 0);
		}
		else
		{
			 // TODO : configurer le niveau de l'IA dans l'interface graphique.
			 //coup_ia = IA(plateau, taille, 'R');
			 coup_ia = IA(plateau, taille, 'N', 2);
		}

		//printf("\033[2J\033[1;1H");
        //AfficherPlateau(plateau, taille);
	    AfficherPlateauGUI(plateau, taille, LARGEUR, HAUTEUR, RAYON);

		// Affiche le coup joué par l'IA.
		if (nb_tour % 2 != 0)
		{
			printf("\nCoup IA : %c%d -> %c%d\n",
				coup_ia->src_x + 0x41, coup_ia->src_y,
				coup_ia->dst_x + 0x41, coup_ia->dst_y
			);
		}
		
		nb_tour++;
	}
    partie_finie = 1;

	// La partie est finie : affiche les scores.
	printf("Rouge : %d pts\n", rouge);
	printf("Noir : %d pts\n", noir);

    if (plateau)
	    FreePlateau(plateau, taille);

    gr_close_graph();

	return 0;
}
