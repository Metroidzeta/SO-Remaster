// @author Alain Barbier alias "Metroidzeta"

#include "monstre.h"

monstre_t * monstre_creer(monstre_data_t * data, int xCase, int yCase) {
	monstre_verificationsArgs(data,xCase,yCase);
	monstre_t * monstre = malloc(sizeof(monstre_t)); verifAlloc(monstre,"Erreur d'allocation du monstre");
	monstre->data = data;
	monstre->position = (SDL_Point) {xCase * TAILLE_CASES, yCase * TAILLE_CASES};
	monstre->hitBox = (SDL_Rect) {monstre->position.x, monstre->position.y, TAILLE_CASES, TAILLE_CASES};
	monstre->PV[0] = monstre->PV[1] = data->PVMax; // PV / PVMax
	return monstre;
}

void monstre_verificationsArgs(monstre_data_t * data, int xCase, int yCase) {
	if(data == NULL) { Exception("La data du monstre est NULL"); }
	if(xCase < 0) { Exception("La xCase du monstre < 0"); }
	if(yCase < 0) { Exception("La yCase du monstre < 0"); }
}

int monstre_getXCase(monstre_t * monstre) { return monstre->position.x / TAILLE_CASES; }
int monstre_getYCase(monstre_t * monstre) { return monstre->position.y / TAILLE_CASES; }

void monstre_afficher(SDL_Renderer * renderer, monstre_t * monstre, int numRegion, SDL_Rect * dstRect) {
	dessinerTexture(renderer,monstre->data->texture,&monstre->data->textureRegions[numRegion],dstRect,"Impossible de dessiner le monstre avec SDL_RenderCopy");
}

void monstre_detruire(monstre_t * monstre) { // Pas besoin de free le monstreData car il est détruit dans l'arraylist lesMonstresData
	free(monstre);
}