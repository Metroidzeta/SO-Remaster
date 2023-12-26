// @author Metroidzeta

#ifndef CHIPSET_H
#define CHIPSET_H

#include "base.h"

typedef struct chipset_s {
	char * nom;
	SDL_Texture * textureChipset; // Image du chipset
	int tailleTuile;              // Taille d'une tuile (case) n*n en pixels du chipset
	int nbTuilesEnHauteur;
	int nbTuilesEnLargeur;
} chipset_t;

chipset_t * chipset_creer(SDL_Renderer * renderer, char * nom, int tailleTuile, const char * chemin);
void chipset_verificationsArgs(char * nom, int tailleTuile);
void chipset_detruire(chipset_t * chipset);

#endif