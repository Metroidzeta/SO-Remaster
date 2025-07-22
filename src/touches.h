// @author Alain Barbier alias "Metroidzeta"

#ifndef TOUCHES_H
#define TOUCHES_H

#include "base.h"
#include "jeu.h"

typedef struct touches_s { // Les touches du jeu (true = pressée, false = non pressée)
	bool HAUT, BAS, GAUCHE, DROITE;
	bool A, B, Q, S;
	bool ESPACE, ENTREE, ECHAP, RETOUR_ARRIERE;
	bool F1, F3, F5;
} touches_t;

touches_t touches_initialiser();
void touches_detection(SDL_Event *event, touches_t *touches, jeu_t *jeu);

#endif