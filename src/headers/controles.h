// @author Alain Barbier alias "Metroidzeta"

#ifndef CONTROLES_H
#define CONTROLES_H

#include "jeu.h"

typedef struct { // les touches du jeu (true = pressée, false = non pressée)
	bool HAUT, BAS, GAUCHE, DROITE;
	bool A, B, Q, S;
	bool ESPACE, ENTREE, ECHAP, RETOUR_ARRIERE;
	bool F1, F3, F5;
} controles_t;

controles_t controles_init();
void controles_detection(SDL_Event *event, controles_t *controles, jeu_t *jeu);

#endif