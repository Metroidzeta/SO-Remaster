// @author Alain Barbier alias "Metroidzeta"

#ifndef MONSTREDATA_H
#define MONSTREDATA_H

#include "base.h"

#define MONSTREDATA_ROWS 4
#define MONSTREDATA_COLS 9
#define MONSTREDATA_TOTAL_REGIONS MONSTREDATA_ROWS * MONSTREDATA_COLS

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

typedef struct {
	char *nom;
	SDL_Texture *texture;  // Image monstre
	SDL_Rect textureRegions[MONSTREDATA_TOTAL_REGIONS];
	int PVMax;
	int xp;
	int piecesOr;
} monstreData_t;

monstreData_t * monstreData_creer(SDL_Renderer *renderer, const char *nomFichier, const char *nom, int PVMax, int xp, int piecesOr, monstreData_result_t *res);
void monstreData_detruire(monstreData_t *monstreData);
const char * monstreData_strerror(monstreData_result_t res);

#endif