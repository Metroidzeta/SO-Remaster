// @author Alain Barbier alias "Metroidzeta"

#ifndef CHIPSET_H
#define CHIPSET_H

#include "base.h"

typedef enum {
	CHIPSET_OK = 0,
	CHIPSET_ERR_NULL_POINTER,
	CHIPSET_ERR_NULL_RENDERER,
	CHIPSET_ERR_NULL_OR_EMPTY_FILENAME,
	CHIPSET_ERR_TUILES_SIZE,
	CHIPSET_ERR_MEMORY_BASE,
	CHIPSET_ERR_MEMORY_NAME,
	CHIPSET_ERR_PATH_TOO_LONG_OR_EMPTY,
	CHIPSET_ERR_LOAD_TEXTURE,
	CHIPSET_ERR_QUERY_TEXTURE,
	CHIPSET_ERR_INVALID_DIMENSIONS,
	CHIPSET_ERR_TOO_MANY_TILES,
	CHIPSET_ERR_MEMORY_TUILES
} chipset_result_t;

typedef struct chipset_s {
	char *nom;
	SDL_Texture *texture; // Image du chipset
	int tailleTuile; // Taille d'une tuile (case) n*n en pixels du chipset
	int nbTuilesHauteur;
	int nbTuilesLargeur;
	SDL_Rect *tuiles;
} chipset_t;

chipset_result_t chipset_creer(chipset_t **out_chipset, SDL_Renderer *renderer, const char *nomFichier, int tailleTuile);
void chipset_detruire(chipset_t *chipset);
const char * chipset_strerror(chipset_result_t res);

#endif