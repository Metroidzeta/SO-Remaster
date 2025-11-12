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

#ifndef CHARGEREVENTS_H
#define CHARGEREVENTS_H

#include "jeu.h"

typedef enum {
	CHARGEREVENTS_OK = 0,
	CHARGEREVENTS_ERR_NULL_POINTER_JEU,
	CHARGEREVENTS_ERR_NULL_POINTER_DIRECTORY,
	CHARGEREVENTS_ERR_NULL_POINTER_CARTE,
	CHARGEREVENTS_ERR_READ_FILE_MURS_EVENTS,
	CHARGEREVENTS_ERR_CREATE_EVENT
} chargerEvents_result_t;

chargerEvents_result_t chargerEvents_inject(jeu_t *jeu);
const char * chargerEvents_strerror(chargerEvents_result_t res);

#endif