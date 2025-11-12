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

#ifndef CHARGERSKINS_H
#define CHARGERSKINS_H

#include "arraylist.h"

typedef enum {
	CHARGERSKINS_OK = 0,
	CHARGERSKINS_ERR_CREATE_ARRAYLIST,
	CHARGERSKINS_ERR_CREATE_SKIN
} chargerSkins_result_t;

arraylist_t * chargerSkins_get(SDL_Renderer *renderer, chargerSkins_result_t *res);
const char * chargerSkins_strerror(chargerSkins_result_t res);

#endif