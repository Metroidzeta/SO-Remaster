// @author Alain Barbier alias "Metroidzeta"

#include "monstre.h"

static void monstre_validerArguments(monstreData_t *data, int xCase, int yCase) {
	if (!data) Exception("Data monstre NULL");
	if (xCase < 0) Exception("xCase monstre < 0");
	if (yCase < 0) Exception("yCase monstre < 0");
}

monstre_t * monstre_creer(monstreData_t *data, int xCase, int yCase) {
	monstre_validerArguments(data, xCase, yCase);

	monstre_t *monstre = malloc(sizeof(monstre_t));
	if (!monstre) Exception("Echec creation monstre");
	memset(monstre, 0, sizeof(monstre_t)); // initialise tout à 0 / NULL pour éviter comportements indifinis en cas d'exception

	monstre->data = data;
	monstre->position = (SDL_Point){ xCase * TAILLE_CASES, yCase * TAILLE_CASES };
	monstre->hitBox = (SDL_Rect){ monstre->position.x, monstre->position.y, TAILLE_CASES, TAILLE_CASES };
	monstre->PV[0] = monstre->PV[1] = data->PVMax; // PV / PVMax
	return monstre;
}

int monstre_getXCase(monstre_t *monstre) { return monstre->position.x / TAILLE_CASES; }
int monstre_getYCase(monstre_t *monstre) { return monstre->position.y / TAILLE_CASES; }

void monstre_afficher(SDL_Renderer *renderer, monstre_t *monstre, int numRegion, SDL_Rect *dstRect) {
	dessinerTexture(renderer, monstre->data->texture, &monstre->data->textureRegions[numRegion], dstRect, "Impossible de dessiner le monstre avec SDL_RenderCopy");
}

void monstre_detruire(monstre_t *monstre) { // Pas besoin de free le monstreData car il est détruit dans l'arraylist lesMonstresData
	free(monstre);
}