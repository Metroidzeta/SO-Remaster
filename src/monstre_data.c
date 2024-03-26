// @author Metroidzeta

#include "monstre_data.h"

monstre_data_t * monstre_data_creer(SDL_Renderer * renderer, char * nomFichier, char * nom, int PVMax, int xp, int piecesOr) {
	monstre_data_verificationsArgs(nomFichier,nom,PVMax,xp,piecesOr);
	monstre_data_t * monstreData = malloc(sizeof(monstre_data_t)); verifAlloc(monstreData,"Erreur d'allocation du monstre_data");
	monstreData->nom = strdup(nom); verifAllocStrCopy(monstreData->nom,nom); // il ne faut pas écrire : "monstreData->nom = nom;" car on ne copie alors que des adresses
	char chemin[100] = "img/"; // chemin vers l'image du monstre
	strcat(chemin,nomFichier);
	monstreData->texture = creerTextureDepuisImage(renderer,chemin);
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 9; j++) { //                          ____.x____  ____.y_____  .w  .h
			monstreData->textureRegions[i * 9 + j] = (SDL_Rect) {3 + j * 24, 15 + i * 32, 18, 18};
		}
	}
	monstreData->PVMax = PVMax;
	monstreData->xp = xp;
	monstreData->piecesOr = piecesOr;
	return monstreData;
}

void monstre_data_verificationsArgs(char * nomFichier, char * nom, int PVMax, int xp, int piecesOr) {
	if(nomFichier == NULL) { Exception("Le nomFichier du monstreData est NULL"); }
	if(nomFichier[0] == '\0') { Exception("Le nomFichier du monstreData est vide"); }
	if(nom == NULL) { Exception("Le nom du monstreData est NULL"); }
	if(nom[0] == '\0') { Exception("Le nom du monstreData est vide"); }
	if(PVMax < 0) { Exception("Le PVMax du monstreData < 0"); }
	if(xp < 0) { Exception("L'xp du monstreData < 0"); }
	if(piecesOr < 0) { Exception("Les piecesOr du monstreData < 0"); }
}

void monstre_data_detruire(monstre_data_t * monstreData) {
	SDL_DestroyTexture(monstreData->texture);
	free(monstreData->nom);
	free(monstreData);
}