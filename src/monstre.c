// @author Alain Barbier alias "Metroidzeta"

#include "headers/monstre.h"

static monstre_result_t monstre_validerArguments(monstreData_t *data, int xCase, int yCase) {
	if (!data) return MONSTRE_ERR_NULL_DATA;
	if (xCase < 0) return MONSTRE_ERR_INVALID_XCASE;
	if (yCase < 0) return MONSTRE_ERR_INVALID_YCASE;
	return MONSTRE_OK;
}

monstre_t * monstre_creer(monstreData_t *data, int xCase, int yCase, monstre_result_t *res) {
	monstre_result_t code = monstre_validerArguments(data, xCase, yCase);
	if (code != MONSTRE_OK) { if (res) *res = code; return NULL; }

	monstre_t *monstre = calloc(1, sizeof(monstre_t));
	if (!monstre) { if (res) *res = MONSTRE_ERR_MEMORY_BASE; return NULL; }

	monstre->data = data;
	monstre->position = (SDL_Point){ xCase * TAILLE_CASES, yCase * TAILLE_CASES };
	monstre->hitBox = (SDL_Rect){ monstre->position.x, monstre->position.y, TAILLE_CASES, TAILLE_CASES };
	monstre->PV[0] = monstre->PV[1] = data->PVMax; // PV / PVMax

	if (res) *res = MONSTRE_OK;
	return monstre;
}

int monstre_getXCase(monstre_t *monstre) { return monstre->position.x / TAILLE_CASES; }
int monstre_getYCase(monstre_t *monstre) { return monstre->position.y / TAILLE_CASES; }

void monstre_afficher(SDL_Renderer *renderer, monstre_t *monstre, int numRegion, SDL_Rect *dstRect) {
	dessinerTexture(renderer, monstre->data->texture, &monstre->data->textureRegions[numRegion], dstRect, "Echec dessin monstre avec SDL_RenderCopy");
}

void monstre_detruire(monstre_t *monstre) { // // Ne pas libérer monstre->data : partagée, allouée ailleurs
	if (!monstre) return;
	free(monstre);
}

const char * monstre_strerror(monstre_result_t res) {
	switch (res) {
		case MONSTRE_OK: return "Succes";
		case MONSTRE_ERR_NULL_DATA: return "Data NULL passe en parametre";
		case MONSTRE_ERR_INVALID_XCASE: return "xCase < 0";
		case MONSTRE_ERR_INVALID_YCASE: return "yCase < 0";
		case MONSTRE_ERR_MEMORY_BASE: return "Echec allocation memoire base";
		default: return "Erreur";
	}
}