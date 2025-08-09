// @author Alain Barbier alias "Metroidzeta"

#ifndef SKIN_H
#define SKIN_H

#include "base.h"

#define REGION_WIDTH 48
#define REGION_HEIGHT 48
#define ROWS 4
#define COLS 3
#define TOTAL_REGIONS ROWS * COLS

typedef enum {
	SKIN_OK = 0,
	SKIN_ERR_NULL_POINTER,
	SKIN_ERR_NULL_RENDERER,
	SKIN_ERR_NULL_OR_EMPTY_FILENAME,
	SKIN_ERR_SIZE_MAX_FILENAME,
	SKIN_ERR_MEMORY_BASE,
	SKIN_ERR_MEMORY_NAME,
	SKIN_ERR_PATH_TOO_LONG_OR_EMPTY,
	SKIN_ERR_LOAD_TEXTURE
} skin_result_t;

typedef struct {
	char *nom;
	SDL_Texture *texture;  // Image de la skin
	SDL_Rect textureRegions[TOTAL_REGIONS];
} skin_t;

skin_result_t skin_creer(skin_t **out_skin, SDL_Renderer *renderer, const char *nomFichier);
void skin_afficher(SDL_Renderer *renderer, skin_t *skin, int numRegion, SDL_Rect *dstRect);
void skin_detruire(skin_t *skin);
const char * skin_strerror(skin_result_t res);

#endif