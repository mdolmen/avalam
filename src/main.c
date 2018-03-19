#include <stdio.h>
#include <stdlib.h>

#include "../include/plateau.h"
#include "../include/ia.h"

// TODO : ajouter une vérification des valeurs renvoyé par certaines fonctions
// (celles qui renvoient un pointeur).

// TODO : quitter proprement en cours de partie (GUI ?).

// TODO : créer une structure 'jeu' pour contenir toutes les informations
// (plateau, taille, mouvements, couleur IA, etc.)
// 	=> juste un pointeur à passer (et bcp moins de params)

int main(int argc, char *argv[])
{
	Case **plateau = NULL;
	Mouvement *coup_ia = NULL;
	char coords_src[3];
	char coords_dst[3];
	int nb_tour = 0;
	int rouge = 0;
	int noir = 0;
	int taille;
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

	// "Vide" le terminal et affiche le plateau.
	printf("\033[2J\033[1;1H");
	AfficherPlateau(plateau, taille);

	// Boucle du jeu.
	while ( PartieNonFinit(plateau, taille, &rouge, &noir) )
	{
		if (nb_tour % 2 == 0)
		{
			// Demande un déplacement jusqu'à ce qu'un correct soit fournit.
			do {
				SaisieDeplacement(coords_src, coords_dst);

				// Manipulation du code ASCII des caractères saisis pour avoir un
				// nombre entre 0 et 9.
				ret = DeplacerPion(plateau, taille, 
					coords_src[0] - 0x41, coords_src[1] - 0x30,
					coords_dst[0] - 0x41, coords_dst[1] - 0x30
				);

				if (ret)
					AfficherErreur(ret);
			} while (ret != 0);
		}
		else
		{
			 coup_ia = IANiveau1(plateau, taille, 'R');
		}

		printf("\033[2J\033[1;1H");
		AfficherPlateau(plateau, taille);

		// Affiche le coup joué par l'IA.
		if (nb_tour % 2 != 0)
		{
			printf("\nCoup IA : %c%d -> %c%d\n",
				coup_ia->src_x + 0x41, coup_ia->src_y,
				coup_ia->dst_x + 0x41, coup_ia->dst_y
			);
			free(coup_ia);
		}
		
		nb_tour++;
	}

	// La partie est finie : affiche les scores.
	printf("\nRouge : %d pts\n", rouge);
	printf("Noir : %d pts\n", noir);

	FreePlateau(plateau, taille);

	return 0;
}
