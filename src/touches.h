// @author Metroidzeta

#ifndef TOUCHES_H
#define TOUCHES_H

#include "base.h"
#include "jeu.h"

typedef struct touches_s { // Les touches du jeu (true = pressée, false = non pressée)
	bool bouton_HAUT;
	bool bouton_BAS;
	bool bouton_GAUCHE;
	bool bouton_DROITE;
	bool bouton_A;
	bool bouton_B;
	bool bouton_Q;
	bool bouton_S;
	bool bouton_ESPACE;
	bool bouton_ENTREE;
	bool bouton_ECHAP;
	bool bouton_RETOUR_ARRIERE;
	bool bouton_F1;
	bool bouton_F3;
	bool bouton_F5;
} touches_t;

touches_t touches_initialiser();
void touches_detection(SDL_Event * event, touches_t * touches, jeu_t * jeu);

#endif