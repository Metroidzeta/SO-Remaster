// @author Alain Barbier alias "Metroidzeta"

#ifndef TOUCHES_H
#define TOUCHES_H

#include "base.h"
#include "jeu.h"

typedef struct touches_s { // Les touches du jeu (true = pressée, false = non pressée)
	bool HAUT;
	bool BAS;
	bool GAUCHE;
	bool DROITE;
	bool A;
	bool B;
	bool Q;
	bool S;
	bool ESPACE;
	bool ENTREE;
	bool ECHAP;
	bool RETOUR_ARRIERE;
	bool F1;
	bool F3;
	bool F5;
} touches_t;

touches_t touches_initialiser();
void touches_detection(SDL_Event * event, touches_t * touches, jeu_t * jeu);

#endif