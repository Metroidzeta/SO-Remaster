// @author Alain Barbier alias "Metroidzeta"

#ifndef MONSTRE_H
#define MONSTRE_H

#include "base.h"
#include "monstreData.h"

typedef struct monstre_s {
	monstreData_t *data;
	SDL_Point position;
	SDL_Rect hitBox;
	int PV[2]; // PV / PVMax (PV[0] / PV[1])
} monstre_t;

monstre_t * monstre_creer(monstreData_t *data, int xCase, int yCase);
int monstre_getXCase(monstre_t *monstre);
int monstre_getYCase(monstre_t *monstre);
void monstre_afficher(SDL_Renderer *renderer, monstre_t *monstre, int numRegion, SDL_Rect *dstRect);
void monstre_detruire(monstre_t *monstre);

#endif