// @author Alain Barbier alias "Metroidzeta"

#ifndef SKIN_H
#define SKIN_H

#include "base.h"

typedef struct skin_s {
	char * nom;
	SDL_Texture * texture;  // Image de la skin
	SDL_Rect textureRegions[12]; // 4 * 3
} skin_t;

skin_t * skin_creer(SDL_Renderer * renderer, char * nomFichier);
void skin_verificationsArgs(char * nomFichier);
void skin_afficher(SDL_Renderer * renderer, skin_t * skin, int numRegion, SDL_Rect * dstRect);
void skin_detruire(skin_t * skin);

#endif