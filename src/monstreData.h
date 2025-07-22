// @author Alain Barbier alias "Metroidzeta"

#ifndef MONSTRE_DATA_H
#define MONSTRE_DATA_H

#include "base.h"

typedef struct monstreData_s {
	char *nom;
	SDL_Texture *texture;  // Image monstre
	SDL_Rect textureRegions[36]; // 4 * 9
	int PVMax;
	int xp;
	int piecesOr;
} monstreData_t;

monstreData_t * monstreData_creer(SDL_Renderer *renderer, const char *nomFichier, const char *nom, int PVMax, int xp, int piecesOr);
void monstreData_detruire(monstreData_t *monstreData);

#endif