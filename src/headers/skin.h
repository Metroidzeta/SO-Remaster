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

#ifndef SKIN_H
#define SKIN_H

#include "base.h"

#define SK_REGION_WIDTH 48
#define SK_REGION_HEIGHT 48
#define SK_ROWS 4
#define SK_COLS 3
#define SK_TOTAL_REGIONS SK_ROWS * SK_COLS

typedef struct {
	char *nom;
	SDL_Texture *texture; // Image de la skin
	SDL_Rect textureRegions[SK_TOTAL_REGIONS];
} skin_t;

typedef enum {
	SKIN_OK = 0,
	SKIN_ERR_NULL_POINTER,
	SKIN_ERR_NULL_RENDERER,
	SKIN_ERR_NULL_OR_EMPTY_FILENAME,
	SKIN_ERR_SIZE_MAX_FILENAME,
	SKIN_ERR_MEMORY_BASE,
	SKIN_ERR_MEMORY_NAME,
	SKIN_ERR_PATH_TOO_LONG_OR_EMPTY,
	SKIN_ERR_LOAD_TEXTURE,
	SKIN_ERR_INVALID_NUMREGION
} skin_result_t;

skin_t * skin_creer(SDL_Renderer *renderer, const char *nomFichier, skin_result_t *res);

skin_result_t skin_afficher(SDL_Renderer *renderer, skin_t *skin, int numRegion, SDL_Rect *dstRect);

void skin_detruire(skin_t *skin);
const char * skin_strerror(skin_result_t res);

#endif