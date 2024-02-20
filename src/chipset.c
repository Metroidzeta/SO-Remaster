// @author Metroidzeta

#include "chipset.h"

chipset_t * chipset_creer(SDL_Renderer * renderer, char * nomFichier, int tailleTuile) {
	chipset_verificationsArgs(nomFichier,tailleTuile);
	chipset_t * chipset = malloc(sizeof(chipset_t)); verifAlloc(chipset,"Erreur d'allocation du chipset");

	chipset->nom = strdup(nomFichier); verifAllocStrCopy(chipset->nom,nomFichier); // il ne faut pas écrire : "chipset->nom = nomFichier;" car on ne copie alors que des adresses
	char chemin[100] = "img/"; // chemin vers le chipset
	strcat(chemin,nomFichier);

	chipset->tailleTuile = tailleTuile;
	chipset->texture = creerTextureDepuisImage(renderer,chemin);
	int largeurChipset, hauteurChipset;
	SDL_QueryTexture(chipset->texture,NULL,NULL,&largeurChipset,&hauteurChipset);
	chipset->nbTuilesEnLargeur = largeurChipset / tailleTuile;
	chipset->nbTuilesEnHauteur = hauteurChipset / tailleTuile;
	int nbTuiles = chipset->nbTuilesEnLargeur * chipset->nbTuilesEnHauteur;
	chipset->tuilesRegions = malloc(sizeof(SDL_Rect) * nbTuiles); verifAlloc(chipset->tuilesRegions,"Erreur d'allocation du chipset");
	for(int i = 0; i < chipset->nbTuilesEnHauteur; i++) {
		for(int j = 0; j < chipset->nbTuilesEnLargeur; j++) {
			chipset->tuilesRegions[i * chipset->nbTuilesEnLargeur + j] = (SDL_Rect) {j * tailleTuile, i * tailleTuile, tailleTuile, tailleTuile};
		}
	}
	return chipset;
}

void chipset_verificationsArgs(char * nomFichier, int tailleTuile) {
	if(nomFichier == NULL) { Exception("Le nomFichier du chipset est NULL"); }
	if(nomFichier[0] == '\0') { Exception("Le nomFichier du chipset est vide"); }
	if(tailleTuile < 1) { Exception("La tailleTuile du chipset < 1"); }
}

void chipset_detruire(chipset_t * chipset) {
	free(chipset->tuilesRegions);
	SDL_DestroyTexture(chipset->texture);
	free(chipset->nom);
	free(chipset);
}