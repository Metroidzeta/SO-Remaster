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

#ifndef CHARGERPOLICES_H
#define CHARGERPOLICES_H

#include "arraylist.h"

typedef enum {
	CHARGERPOLICES_OK = 0,
	CHARGERPOLICES_ERR_CREATE_ARRAYLIST,
	CHARGERPOLICES_ERR_NULL_POINTER_ELEM,
	CHARGERPOLICES_ERR_CREATE_POLICE
} chargerPolices_result_t;

arraylist_t * chargerPolices_get(chargerPolices_result_t *res);
const char * chargerPolices_strerror(chargerPolices_result_t res);

#endif