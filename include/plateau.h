#ifndef PLATEAU_H
#define PLATEAU_H

#define TAILLE_STD 9

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
	int hauteur_plateau
);

/* Affiche une ligne pour quadriller le plateau. */
void AfficherDelimiteur(int taille);

int DeplacerPion(Case **plateau, int taille, 
	int colonne, int ligne,			// Coordonnées de la tour à bouger. 
	int colonne_dst, int ligne_dst	// Coordonnées ou bouger la tour.
);

int MouvementAutorise(Case *src, Case *dst);

/* Message d'erreur selon le retour de DeplacerPion(). */
void AfficherErreur(int code);

void SaisieDeplacement(char *coords_src, char *coords_dst);

/* Génére les mouvements possible pour une case. Retourne le nombre de mouvement
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

/* Enregistre la partie dans un fichier. */
int SauvegarderPartie(Case **plateau, int taille);

/* Charge une partie depuis un fichier. */
Case** ChargerPartie();

/* Compte le nombre de pion autour d'un pion donné. */
int NbCaseAutour(Case **plateau, int taille, int x, int y);

/* Evalue la valeur d'un coup pour déterminer sa prioritée. */
int EvaluerValeurCoup(
	Case **plateau,
	Mouvement *mouvs,
	int taille,
	int src_x, int src_y,
	int dst_x, int dst_y,
	char couleur
);

#endif // PLATEAU_H
