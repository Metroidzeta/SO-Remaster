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

#ifndef CHARGERAFFICHAGES_H
#define CHARGERAFFICHAGES_H

#include "arraylist.h"

typedef enum {
	CHARGERAFFICHAGES_OK = 0,
	CHARGERAFFICHAGES_ERR_CREATE_ARRAYLIST,
	CHARGERAFFICHAGES_ERR_CREATE_AFFICHAGE
} chargerAffichages_result_t;

arraylist_t * chargerAffichages_get(SDL_Renderer *renderer, chargerAffichages_result_t *res);
const char * chargerAffichages_strerror(chargerAffichages_result_t res);

#endif