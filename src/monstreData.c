// @author Alain Barbier alias "Metroidzeta"

#include "monstreData.h"

static void monstreData_verifierArguments(SDL_Renderer *renderer, const char *nomFichier, const char *nom, int PVMax, int xp, int piecesOr) {
	if (!renderer) Exception("Renderer NULL");
	if (!nomFichier || !*nomFichier) Exception("NomFichier monstreData NULL ou vide");
	if (!nom || !*nom) Exception("Nom monstreData NULL ou vide");
	if (PVMax < 0) Exception("PVMax monstreData < 0");
	if (xp < 0) Exception("Xp monstreData < 0");
	if (piecesOr < 0) Exception("PiecesOr monstreData < 0");
}

monstreData_t * monstreData_creer(SDL_Renderer *renderer, const char * nomFichier, const char * nom, int PVMax, int xp, int piecesOr) {
	monstreData_verifierArguments(renderer, nomFichier, nom, PVMax ,xp, piecesOr);

	monstreData_t *monstreData = malloc(sizeof(monstreData_t));
	if (!monstreData) Exception("Echec creation monstreData");
	memset(monstreData, 0, sizeof(monstreData_t)); // initialise tout à 0 / NULL pour éviter comportements indifinis en cas d'exception

	monstreData->nom = strdup(nom); // ne pas faire: "monstreData->nom = nom" car on ne copie alors que des adresses
	if (!monstreData->nom) { monstreData_detruire(monstreData); Exception("Echec creation nom copie monstreData"); }
	char chemin[100]; // chemin vers l'image du monstre
	snprintf(chemin, sizeof(chemin), "img/%s", nomFichier);
	monstreData->texture = creerTextureDepuisImage(renderer, chemin);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 9; ++j) { //                         ____.x____  ____.y_____  .w  .h
			monstreData->textureRegions[i * 9 + j] = (SDL_Rect){ 3 + j * 24, 15 + i * 32, 18, 18 };
		}
	}
	monstreData->PVMax = PVMax;
	monstreData->xp = xp;
	monstreData->piecesOr = piecesOr;
	return monstreData;
}

void monstreData_detruire(monstreData_t *monstreData) {
	if (!monstreData) return;
	SDL_DestroyTexture(monstreData->texture);
	free(monstreData->nom);
	free(monstreData);
}