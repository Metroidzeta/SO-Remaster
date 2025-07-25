// @author Alain Barbier alias "Metroidzeta"

#include "event_tp.h"

static void event_tp_verificationsArgs(int xCaseDst, int yCaseDst, carte_t *carteDst) {
	if (!carteDst) Exception("CarteDst event_tp NULL");
	if (xCaseDst < 0 || xCaseDst >= carteDst->largeur || yCaseDst < 0 || yCaseDst >= carteDst->hauteur) {
		Exception("Coordonnées d'event_tp hors limite carteDst (< 0 ou > limite)");
	}
}

event_tp_t * event_creerTP(int xCaseDst, int yCaseDst, carte_t *carteDst) {
	event_tp_verificationsArgs(xCaseDst, yCaseDst, carteDst);

	event_tp_t *e_tel = malloc(sizeof(event_tp_t));
	if (!e_tel) Exception("Échec creation event_tp");

	*e_tel = (event_tp_t){ xCaseDst * TAILLE_CASES, yCaseDst * TAILLE_CASES, carteDst }; // vraie valeur de x et y : il faut multiplier par TAILLE_CASES
	return e_tel;
}

void event_tp_detruire(event_tp_t * e_tel) { free(e_tel); } // Pas besoin de free la carteDst utilisée car elle est détruite dans l'arraylist lesCartes