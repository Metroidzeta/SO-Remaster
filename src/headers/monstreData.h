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

#ifndef MONSTREDATA_H
#define MONSTREDATA_H

#include "base.h"

#define MD_ROWS 4
#define MD_COLS 9
#define MD_TOTAL_REGIONS MD_ROWS * MD_COLS

typedef struct {
	char *nom;
	SDL_Texture *texture;  // Image monstre
	SDL_Rect textureRegions[MD_TOTAL_REGIONS];
	int PVMax;
	int xp;
	int piecesOr;
} monstreData_t;

typedef enum {
	MONSTREDATA_OK = 0,
	MONSTREDATA_ERR_NULL_RENDERER,
	MONSTREDATA_ERR_NULL_OR_EMPTY_FILENAME,
	MONSTREDATA_ERR_SIZE_MAX_FILENAME,
	MONSTREDATA_ERR_NULL_OR_EMPTY_NAME,
	MONSTREDATA_ERR_SIZE_MAX_NAME,
	MONSTREDATA_ERR_INVALID_PVMAX,
	MONSTREDATA_ERR_INVALID_XP,
	MONSTREDATA_ERR_INVALID_PIECESOR,
	MONSTREDATA_ERR_MEMORY_BASE,
	MONSTREDATA_ERR_MEMORY_NAME,
	MONSTREDATA_ERR_LOAD_TEXTURE
} monstreData_result_t;

monstreData_t * monstreData_creer(SDL_Renderer *renderer, const char *nomFichier, const char *nom, int PVMax, int xp, int piecesOr, monstreData_result_t *res);

void monstreData_detruire(monstreData_t *monstreData);
const char * monstreData_strerror(monstreData_result_t res);

#endif