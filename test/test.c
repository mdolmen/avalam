#include <stdlib.h>
#include <check.h>

#include "../include/plateau.h"
#include "../include/ia.h"

START_TEST(test_init_plateau)
{
	Case **plateau = NULL;

	plateau = InitPlateau(TAILLE_STD);
	ck_assert_msg(
		plateau != NULL,
		"InitPlateau à retourner NULL au lieu d'un pointeur."
	);

	FreePlateau(plateau, TAILLE_STD);
}
END_TEST

/* DeplacerPion() : Bonnes conditions. */
START_TEST(test_deplacer_pion_0)
{
	Case **plateau = NULL;
	Case c1, c2;
	int ret;

	plateau = InitPlateau(TAILLE_STD);

	ret = DeplacerPion(plateau, TAILLE_STD, 2, 2, 1, 1);
	c1 = plateau[2][2];
	c2 = plateau[1][1];
	ck_assert_msg(
		ret == 0 && c1.taille == 0 && c2.taille == 2,
		"Erreur lors d'un déplacement de pion dans les bonnes conditions."
	);

	FreePlateau(plateau, TAILLE_STD);
}
END_TEST

/* DeplacerPion() : Les coordonnées n'appartiennent pas au plateau. */
START_TEST(test_deplacer_pion_1)
{
	Case **plateau = NULL;
	int ret;

	plateau = InitPlateau(TAILLE_STD);

	ret = DeplacerPion(plateau, TAILLE_STD, 12, 12, 1, 1);
	ck_assert_msg(
		ret == 1,
		"Code de retour attendu pour des mauvaises coordonnées : 1.\n"
		"\tret = %d\n",
		ret
	);

	FreePlateau(plateau, TAILLE_STD);
}
END_TEST

/* DeplacerPion() :  Tours non adjacentes. */
START_TEST(test_deplacer_pion_2)
{
	Case **plateau = NULL;
	Case c1, c2;
	int ret;

	plateau = InitPlateau(TAILLE_STD);

	ret = DeplacerPion(plateau, TAILLE_STD, 1, 1, 3, 3);
	c1 = plateau[1][1];
	c2 = plateau[3][3];
	ck_assert_msg(
		ret == 2 && c1.taille == 1 && c2.taille == 1,
		"Code de retour attendu pour des tours non adjacentes : 2.\n"
		"\tret = %d\n\tc1.taille = %d\n\tc2.taille = %d\n",
		ret, c1.taille, c2.taille
	);

	FreePlateau(plateau, TAILLE_STD);
}
END_TEST

/* DeplacerPion() :  Mouvement interdit. */
START_TEST(test_deplacer_pion_3)
{
	Case **plateau = NULL;
	Case c1, c2;
	int ret;

	plateau = InitPlateau(TAILLE_STD);

	ret = DeplacerPion(plateau, TAILLE_STD, 3, 4, 4, 4);
	c1 = plateau[3][4];
	c2 = plateau[4][4];
	ck_assert_msg(
		ret == 3 && c1.taille == 1 && c2.taille == 0,
		"Code de retour attendu pour des mauvaises coordonnées : 1.\n"
		"\tret = %d\n\tc1.taille = %d\n\tc2.taille = %d\n",
		ret, c1.taille, c2.taille
	);

	FreePlateau(plateau, TAILLE_STD);
}
END_TEST

/* MouvementAutorise() : OK. */
START_TEST(test_mouvement_autorise_0)
{
	Case **plateau = NULL;
	Case c1, c2;
	int ret;

	plateau = InitPlateau(TAILLE_STD);

	c1 = plateau[2][2];
	c2 = plateau[1][1];
	c1.taille = 1;
	c2.taille = 3;

	ret = MouvementAutorise(&c1, &c2);
	ck_assert_msg(
		ret == 1,
		"Le mouvement devrait être autorisé.\n\tret = %d\n",
		ret
	);

	FreePlateau(plateau, TAILLE_STD);
}
END_TEST

/* MouvementAutorise() : Interdit. */
START_TEST(test_mouvement_autorise_1)
{
	Case **plateau = NULL;
	Case c1, c2;
	int ret;

	plateau = InitPlateau(TAILLE_STD);

	c1 = plateau[2][2];
	c2 = plateau[1][1];
	c1.taille = 4;
	c2.taille = 3;

	ret = MouvementAutorise(&c1, &c2);
	ck_assert_msg(
		ret == 0,
		"Le mouvement devrait être interdit.\n\tret = %d\n",
		ret
	);

	FreePlateau(plateau, TAILLE_STD);
}
END_TEST

START_TEST(test_generer_mouvement)
{
	Case **plateau = NULL;
	Mouvement *mouvs;
	int nb_mouv = 0;

	plateau = InitPlateau(TAILLE_STD);

	// La tour peut être déplacer sur 1 de ces 8 pions adjacents.
	mouvs = (Mouvement *)malloc(sizeof(Mouvement) * TAILLE_STD * TAILLE_STD * 8);

	nb_mouv = GenererMouvements(plateau, TAILLE_STD, mouvs);
	
	ck_assert_msg(
		nb_mouv == 292,
		"Le nombre de mouvement possible attendu pour une grille %dx%d = 292"
		"\n\tnb_mouv = %d\n",
		TAILLE_STD, TAILLE_STD, nb_mouv
	);

	free(mouvs);
	FreePlateau(plateau, TAILLE_STD);
}
END_TEST

START_TEST(test_compter_points)
{
	Case **plateau = NULL;
	int rouge = 0;
	int noir = 0;

	plateau = InitPlateau(TAILLE_STD);

	CompterPoints(plateau, TAILLE_STD, &rouge, &noir);
	
	ck_assert_msg(
		(rouge == 24) && (noir == 24),
		"Nombre pions attendus : 24 (de chaque couleur)\n"
		"\tNoir = %d\n"
		"\tRouge = %d\n",
		noir, rouge
	);

	FreePlateau(plateau, TAILLE_STD);
}
END_TEST

START_TEST(test_partie_finit)
{
	Case **plateau = NULL;
	Case *c;
	int rouge = 0;
	int noir = 0;
	int nb_mouv = 0;

	plateau = InitPlateau(TAILLE_STD);

	// Met la taille de toutes les cases à 0 pour simuler le fait qu'il n'y ait
	// plus de coups possibles.
	for (int i = 0; i < TAILLE_STD; i++)
	{
		for (int j = 0; j < TAILLE_STD; j++)
		{
			c = &plateau[i][j];

			c->couleur = ' ';
			c->taille = 0;
		}
	}

	nb_mouv = PartieNonFinit(plateau, TAILLE_STD, &rouge, &noir);
	
	ck_assert_msg(
		nb_mouv == 0,
		"Aucun mouvement possible attendu.\n",
		"\tnb_mouv = %d\n",
		nb_mouv
	);

	FreePlateau(plateau, TAILLE_STD);
}
END_TEST

START_TEST(test_sauvegarde)
{
	Case **plateau = NULL;
	int code;

	plateau = InitPlateau(TAILLE_STD);

	code = SauvegarderPartie(plateau, TAILLE_STD);
	
	ck_assert_int_eq(code, EXIT_SUCCESS);

	FreePlateau(plateau, TAILLE_STD);
}
END_TEST

START_TEST(test_charger_partie)
{
	Case **plateau = NULL;

	plateau = ChargerPartie();
	
	ck_assert_msg(
		plateau != NULL,
		"Le chargement de la partie a retourner un pointeur NULL."
	);

	FreePlateau(plateau, TAILLE_STD);
}
END_TEST

/* Crée un jeu de tests. */
Suite *avalam_suite(void)
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("Avalam");

	/* Core test case. */
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_init_plateau);
	tcase_add_test(tc_core, test_deplacer_pion_0);
	tcase_add_test(tc_core, test_deplacer_pion_1);
	tcase_add_test(tc_core, test_deplacer_pion_2);
	tcase_add_test(tc_core, test_deplacer_pion_3);
	tcase_add_test(tc_core, test_mouvement_autorise_0);
	tcase_add_test(tc_core, test_mouvement_autorise_1);
	tcase_add_test(tc_core, test_generer_mouvement);
	tcase_add_test(tc_core, test_compter_points);
	tcase_add_test(tc_core, test_partie_finit);
	tcase_add_test(tc_core, test_sauvegarde);
	tcase_add_test(tc_core, test_charger_partie);

	suite_add_tcase(s, tc_core);

	return s;
}

/* Crée un SRunner pour exécuter le/les jeux de tests. */
int main(void)
{
	int failed;
	Suite *suite;
	SRunner *sr;

	/* Crée une suite de tests à exécuter. */
	suite = avalam_suite();
	sr = srunner_create(suite);

	srunner_run_all(sr, CK_NORMAL);
	failed = srunner_ntests_failed(sr);
	srunner_free(sr);

	return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
