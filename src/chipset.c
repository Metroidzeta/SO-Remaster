// @author Metroidzeta

#include "chipset.h"

chipset_t * chipset_creer(SDL_Renderer * renderer, char * nom, int tailleTuile, const char * chemin) {
	chipset_verificationsArgs(nom,tailleTuile);
	chipset_t * chipset = malloc(sizeof(chipset_t));
	verifAlloc(chipset,"Erreur d'allocation du chipset");

	chipset->nom = strdup(nom); // il ne faut pas écrire : "chipset->nom = nom;" car on ne copie alors que des adresses
	verifAllocStrCopy(chipset->nom,nom);

	chipset->tailleTuile = tailleTuile;
	chipset->textureChipset = creerTextureImage(renderer,chemin);
	int hauteurChipset, largeurChipset;
	SDL_QueryTexture(chipset->textureChipset,NULL,NULL,&largeurChipset,&hauteurChipset);
	chipset->nbTuilesEnHauteur = hauteurChipset / tailleTuile;
	chipset->nbTuilesEnLargeur = largeurChipset / tailleTuile;

	return chipset;
}

void chipset_verificationsArgs(char * nom, int tailleTuile) {
	if(nom == NULL) { Exception("Le nom du chipset est NULL"); }
	if(nom[0] == '\0') { Exception("Le nom du chipset est vide"); }
	if(tailleTuile < 1) { Exception("La tailleTuile du chipset < 1"); }
}

void chipset_detruire(chipset_t * chipset) {
	SDL_DestroyTexture(chipset->textureChipset);
	free(chipset->nom);
	free(chipset);
}