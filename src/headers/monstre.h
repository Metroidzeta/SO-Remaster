// @author Alain Barbier alias "Metroidzeta"

#ifndef MONSTRE_H
#define MONSTRE_H

#include "base.h"
#include "monstreData.h"

typedef enum {
	MONSTRE_OK = 0,
	MONSTRE_ERR_NULL_POINTER,
	MONSTRE_ERR_NULL_DATA,
	MONSTRE_ERR_INVALID_XCASE,
	MONSTRE_ERR_INVALID_YCASE,
	MONSTRE_ERR_MEMORY_BASE
} monstre_result_t;

typedef struct monstre_s {
	monstreData_t *data;
	SDL_Point position;
	SDL_Rect hitBox;
	int PV[2]; // PV / PVMax (PV[0] / PV[1])
} monstre_t;

monstre_result_t monstre_creer(monstre_t **out_monstre, monstreData_t *data, int xCase, int yCase);
int monstre_getXCase(monstre_t *monstre);
int monstre_getYCase(monstre_t *monstre);
void monstre_afficher(SDL_Renderer *renderer, monstre_t *monstre, int numRegion, SDL_Rect *dstRect);
void monstre_detruire(monstre_t *monstre);
const char * monstre_strerror(monstre_result_t res);

#endif