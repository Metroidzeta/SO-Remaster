// @author Metroidzeta

#ifndef CHIPSET_H
#define CHIPSET_H

#include "base.h"

typedef struct chipset_s {
	char * nom;
	SDL_Texture * imageChipset;
	int tailleBloc;               // Taille d'un bloc (case) x*x en pixels du chipset
	int nbBlocsEnHauteur;
	int nbBlocsEnLargeur;
} chipset_t;

chipset_t * chipset_creer(SDL_Renderer * renderer, char * nom, int tailleBloc, const char * chemin);
void chipset_verificationsArgs(char * nom, int tailleBloc);
void chipset_detruire(chipset_t * chipset);

#endif