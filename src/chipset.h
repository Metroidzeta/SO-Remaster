// @author Alain Barbier alias "Metroidzeta"

#ifndef CHIPSET_H
#define CHIPSET_H

#include "base.h"

typedef struct chipset_s {
	char * nom;
	SDL_Texture * texture; // Image du chipset
	int tailleTuile; // Taille d'une tuile (case) n*n en pixels du chipset
	int nbTuilesEnHauteur;
	int nbTuilesEnLargeur;
	SDL_Rect * tuilesRegions;
} chipset_t;

chipset_t * chipset_creer(SDL_Renderer * renderer, const char * nomFichier, int tailleTuile);
void chipset_detruire(chipset_t * chipset);

#endif