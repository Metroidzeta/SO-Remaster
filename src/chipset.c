// @author Metroidzeta

#include "chipset.h"

chipset_t * chipset_creer(SDL_Renderer * renderer, char * nom, int tailleBloc, const char * chemin) {
	chipset_verificationsArgs(nom,tailleBloc);
	chipset_t * chipset = malloc(sizeof(chipset_t));
	verifAlloc(chipset,"Erreur d'allocation du chipset");

	chipset->nom = strdup(nom); // il ne faut pas écrire : "chipset->nom = nom;" car on ne copie alors que des adresses
	verifAllocStrCopy(chipset->nom,nom);

	chipset->tailleBloc = tailleBloc;
	chipset->imageChipset = creerTextureImage(renderer,chemin);
	int hauteurChipset, largeurChipset;
	SDL_QueryTexture(chipset->imageChipset,NULL,NULL,&largeurChipset,&hauteurChipset);
	chipset->nbBlocsEnHauteur = hauteurChipset / tailleBloc;
	chipset->nbBlocsEnLargeur = largeurChipset / tailleBloc;

	return chipset;
}

void chipset_verificationsArgs(char * nom, int tailleBloc) {
	if(nom == NULL) { Exception("Le nom du chipset est NULL"); }
	if(nom[0] == '\0') { Exception("Le nom du chipset est vide"); }
	if(tailleBloc < 1) { Exception("La tailleBloc du chipset < 0"); }
}

void chipset_detruire(chipset_t * chipset) {
	SDL_DestroyTexture(chipset->imageChipset);
	free(chipset->nom);
	free(chipset);
}