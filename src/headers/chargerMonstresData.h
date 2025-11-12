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

#ifndef CHARGERMONSTRESDATA_H
#define CHARGERMONSTRESDATA_H

#include "arraylist.h"

typedef enum {
	CHARGERMONSTRESDATA_OK = 0,
	CHARGERMONSTRESDATA_ERR_CREATE_ARRAYLIST,
	CHARGERMONSTRESDATA_ERR_CREATE_MONSTREDATA
} chargerMonstresData_result_t;

arraylist_t * chargerMonstresData_get(SDL_Renderer *renderer, chargerMonstresData_result_t *res);
const char * chargerMonstresData_strerror(chargerMonstresData_result_t res);

#endif