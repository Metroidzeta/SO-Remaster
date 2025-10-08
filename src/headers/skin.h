// @author Alain Barbier alias "Metroidzeta"

#ifndef SKIN_H
#define SKIN_H

#include "base.h"

#define SKIN_REGION_WIDTH 48
#define SKIN_REGION_HEIGHT 48
#define SKIN_ROWS 4
#define SKIN_COLS 3
#define SKIN_TOTAL_REGIONS SKIN_ROWS * SKIN_COLS

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

typedef struct {
	char *nom;
	SDL_Texture *texture;  // Image de la skin
	SDL_Rect textureRegions[SKIN_TOTAL_REGIONS];
} skin_t;

skin_t * skin_creer(SDL_Renderer *renderer, const char *nomFichier, skin_result_t *res);
skin_result_t skin_afficher(SDL_Renderer *renderer, skin_t *skin, int numRegion, SDL_Rect *dstRect);
void skin_detruire(skin_t *skin);
const char * skin_strerror(skin_result_t res);

#endif