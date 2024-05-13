// @author Alain Barbier alias "Metroidzeta"

#ifndef MONSTRE_DATA_H
#define MONSTRE_DATA_H

#include "base.h"

typedef struct monstre_data_s {
	char * nom;
	SDL_Texture * texture;  // Image du monstre
	SDL_Rect textureRegions[36]; // 4 * 9
	int PVMax;
	int xp;
	int piecesOr;
} monstre_data_t;

monstre_data_t * monstre_data_creer(SDL_Renderer * renderer, const char * nomFichier, const char * nom, int PVMax, int xp, int piecesOr);
void monstre_data_detruire(monstre_data_t * monstreData);

#endif