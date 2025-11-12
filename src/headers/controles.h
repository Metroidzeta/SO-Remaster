/**
 * @author Alain Barbier alias "Metroidzeta"
 * Copyright © 2025 Alain Barbier (Metroidzeta) - All rights reserved.
 *
 * This file is part of the project covered by the
 * "Educational and Personal Use License / Licence d’Utilisation Personnelle et Éducative".
 *
 * Permission is granted to fork and use this code for educational and personal purposes only.
 *
 * Commercial use, redistribution, or public republishing of modified versions
 * is strictly prohibited without the express written consent of the author.
 *
 * Coded with SDL2 (Simple DirectMedia Layer 2).
 *
 * Created by Metroidzeta.
 */

#ifndef CONTROLES_H
#define CONTROLES_H

#include "jeu.h"

typedef struct { // les touches du jeu (true = pressée, false = non pressée)
	bool HAUT, BAS, GAUCHE, DROITE;
	bool A, B, Q, S;
	bool ESPACE, ENTREE, ECHAP, RETOUR_ARRIERE;
	bool F1, F3, F5;
} controles_t;

controles_t controles_init(void);
void controles_detection(SDL_Event *event, controles_t *controles, jeu_t *jeu);

#endif