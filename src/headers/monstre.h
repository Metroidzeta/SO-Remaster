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

#ifndef MONSTRE_H
#define MONSTRE_H

#include "monstreData.h"

typedef struct {
	monstreData_t *data;
	SDL_Point position;
	SDL_Rect hitBox;
	int PV[2]; // PV / PVMax (PV[0] / PV[1])
} monstre_t;

typedef enum {
	MONSTRE_OK = 0,
	MONSTRE_ERR_NULL_DATA,
	MONSTRE_ERR_INVALID_XCASE,
	MONSTRE_ERR_INVALID_YCASE,
	MONSTRE_ERR_MEMORY_BASE
} monstre_result_t;

monstre_t * monstre_creer(monstreData_t *data, int xCase, int yCase, monstre_result_t *res);

int monstre_getXCase(monstre_t *monstre);
int monstre_getYCase(monstre_t *monstre);
void monstre_afficher(SDL_Renderer *renderer, monstre_t *monstre, int numRegion, SDL_Rect *dstRect);

void monstre_detruire(monstre_t *monstre);
const char * monstre_strerror(monstre_result_t res);

#endif