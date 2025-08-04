// @author Alain Barbier alias "Metroidzeta"

#ifndef MONSTRE_DATA_H
#define MONSTRE_DATA_H

#include "base.h"

typedef enum {
	MONSTREDATA_OK = 0,
	MONSTREDATA_ERR_NULL_POINTER,
	MONSTREDATA_ERR_NULL_RENDERER,
	MONSTREDATA_ERR_NULL_OR_EMPTY_FILENAME,
	MONSTREDATA_ERR_NULL_OR_EMPTY_NAME,
	MONSTREDATA_ERR_INVALID_PVMAX,
	MONSTREDATA_ERR_INVALID_XP,
	MONSTREDATA_ERR_INVALID_PIECESOR,
	MONSTREDATA_ERR_MEMORY_BASE,
	MONSTREDATA_ERR_MEMORY_NAME,
	MONSTREDATA_ERR_PATH_TOO_LONG_OR_EMPTY,
	MONSTREDATA_ERR_LOAD_TEXTURE
} monstreData_result_t;

typedef struct monstreData_s {
	char *nom;
	SDL_Texture *texture;  // Image monstre
	SDL_Rect textureRegions[36]; // 4 * 9
	int PVMax;
	int xp;
	int piecesOr;
} monstreData_t;

monstreData_result_t monstreData_creer(monstreData_t **out_monstreData, SDL_Renderer *renderer, const char *nomFichier, const char *nom, int PVMax, int xp, int piecesOr);
void monstreData_detruire(monstreData_t *monstreData);
const char * monstreData_strerror(monstreData_result_t res);

#endif