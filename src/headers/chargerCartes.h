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

#ifndef CHARGERCARTES_H
#define CHARGERCARTES_H

#include "jeu.h"

typedef enum {
	CHARGERCARTES_OK = 0,
	CHARGERCARTES_ERR_NULL_POINTER_JEU,
	CHARGERCARTES_ERR_NULL_POINTER_DIRECTORY,
	CHARGERCARTES_ERR_CREATE_ARRAYLIST,
	CHARGERCARTES_ERR_READ_BASE_COUCHES,
	CHARGERCARTES_ERR_PARSE_LARGEUR,
	CHARGERCARTES_ERR_PARSE_HAUTEUR,
	CHARGERCARTES_ERR_PARSE_CHIPSET,
	CHARGERCARTES_ERR_PARSE_MUSIQUE,
	CHARGERCARTES_ERR_PARSE_COUCHE0,
	CHARGERCARTES_ERR_PARSE_COUCHE1,
	CHARGERCARTES_ERR_PARSE_COUCHE2,
	CHARGERCARTES_ERR_READ_MURS_EVENTS,
	CHARGERCARTES_ERR_PARSE_MURS,
	CHARGERCARTES_ERR_CREATE_CARTE
} chargerCartes_result_t;

arraylist_t * chargerCartes_get(jeu_t *jeu, chargerCartes_result_t *res);
const char * chargerCartes_strerror(chargerCartes_result_t res);

#endif