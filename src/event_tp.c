// @author Alain Barbier alias "Metroidzeta"

#include "event_tp.h"

event_tp_t * event_creerTP(int xCaseDst, int yCaseDst, carte_t * carteDst) {
	event_tp_verificationsArgs(xCaseDst,yCaseDst,carteDst);
	event_tp_t * e_tel = malloc(sizeof(event_tp_t)); verifAlloc(e_tel,"Erreur d'allocation de l'event_tp");
	*e_tel = (event_tp_t) {xCaseDst * TAILLE_CASES, yCaseDst * TAILLE_CASES, carteDst}; // Pour avoir le vraie valeur de x et y, il faut multiplier par TAILLE_CASES
	return e_tel;
}

void event_tp_verificationsArgs(int xCaseDst, int yCaseDst, carte_t * carteDst) {
	if(carteDst == NULL) { Exception("La carteDst de l'event_tp est NULL"); }
	if(xCaseDst < 0 || xCaseDst > carteDst->largeur - 1) { Exception("La xCaseDst de l'event_tp est < 0 ou > largeur - 1 de la carteDst"); }
	if(yCaseDst < 0 || yCaseDst > carteDst->hauteur - 1) { Exception("La yCaseDst de l'event_tp est < 0 ou > hauteur - 1 de la carteDst"); }
}

void event_tp_detruire(event_tp_t * e_tel) { free(e_tel); } // Pas besoin de free la carteDst utilisée car elle est détruite dans l'arraylist lesCartes