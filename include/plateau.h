#ifndef PLATEAU_H
#define PLATEAU_H

#include "avalam.h"

#define TAILLE_STD 9

Case** InitPlateau(int taille);

// TODO : Changer le style des commentaires pour générer de la doc.

/* Libère la mémoire allouer au plateau. */
void FreePlateau(Case **plateau, int taille);

/* Affiche le plateau. */
void AfficherPlateau(Case **plateau, int taille);

/* Affiche le plateau avec une interface graphique. */
void AfficherPlateauGUI(
	Case **plateau, 
	int taille,
	int largeur_plateau,
	int hauteur_plateau,
    int rayon
);

/* Affiche une ligne pour quadriller le plateau. */
void AfficherDelimiteur(int taille);

int DeplacerPion(Case **plateau, int taille, 
	int colonne, int ligne,			// Coordonnées de la tour à bouger. 
	int colonne_dst, int ligne_dst	// Coordonnées ou bouger la tour.
);

void SaisieDeplacement(char *coords_src, char *coords_dst);

void SaisieDeplacementGUI(
    int taille,
    int diametre, 
    int *src_x, int *src_y,
    int *dst_x, int *dst_y
);


/* Enregistre la partie dans un fichier. */
int SauvegarderPartie(Case **plateau, int taille);

/* Charge une partie depuis un fichier. */
Case** ChargerPartie();

/* Compte le nombre de pion autour d'un pion donné. */
int NbCaseAutour(Case **plateau, int taille, int x, int y);

#endif // PLATEAU_H
