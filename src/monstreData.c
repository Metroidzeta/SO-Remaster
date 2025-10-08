// @author Alain Barbier alias "Metroidzeta"

#include "headers/monstreData.h"

static monstreData_result_t monstreData_verifierArguments(SDL_Renderer *renderer, const char *nomFichier, const char *nom, int PVMax, int xp, int piecesOr) {
	if (!renderer) return MONSTREDATA_ERR_NULL_RENDERER;
	if (!nomFichier || !*nomFichier) return MONSTREDATA_ERR_NULL_OR_EMPTY_FILENAME;
	if (strlen(nomFichier) >= MAX_TAILLE_STRING) return MONSTREDATA_ERR_SIZE_MAX_FILENAME;
	if (!nom || !*nom) return MONSTREDATA_ERR_NULL_OR_EMPTY_NAME;
	if (strlen(nom) >= MAX_TAILLE_STRING) return MONSTREDATA_ERR_SIZE_MAX_NAME;
	if (PVMax < 0) return MONSTREDATA_ERR_INVALID_PVMAX;
	if (xp < 0) return MONSTREDATA_ERR_INVALID_XP;
	if (piecesOr < 0) return MONSTREDATA_ERR_INVALID_PIECESOR;
	return MONSTREDATA_OK;
}

static monstreData_result_t monstreData_chargerTexture(monstreData_t *monstreData, SDL_Renderer *renderer, const char *nomFichier) {
	monstreData->texture = creerTexture(renderer, nomFichier);
	if (!monstreData->texture) { LOG_ERROR("Erreur creerTexture : %s", IMG_GetError()); return MONSTREDATA_ERR_LOAD_TEXTURE; }
	for (int i = 0; i < MONSTREDATA_ROWS; ++i) {
		const int y = 15 + i * 32;
		const int ligneIndex = i * MONSTREDATA_COLS;
		for (int j = 0; j < MONSTREDATA_COLS; ++j) { //               ____.x____ .y  .w  .h
			monstreData->textureRegions[ligneIndex + j] = (SDL_Rect){ 3 + j * 24, y, 18, 18 };
		}
	}
	return MONSTREDATA_OK;
}

monstreData_t * monstreData_creer(SDL_Renderer *renderer, const char *nomFichier, const char *nom, int PVMax, int xp, int piecesOr, monstreData_result_t *res) {
	monstreData_result_t code = monstreData_verifierArguments(renderer, nomFichier, nom, PVMax ,xp, piecesOr);
	if (code != MONSTREDATA_OK) { if (res) *res = code; return NULL; }

	monstreData_t *monstreData = calloc(1, sizeof(monstreData_t));
	if (!monstreData) { if (res) *res = MONSTREDATA_ERR_MEMORY_BASE; return NULL; }

	monstreData->nom = my_strdup(nom); // important : ne pas faire "monstreData->nom = nom", car cela ne copie que le pointeur, pas le contenu
	if (!monstreData->nom) { monstreData_detruire(monstreData); if (res) *res = MONSTREDATA_ERR_MEMORY_NAME; return NULL; }

	if ((code = monstreData_chargerTexture(monstreData, renderer, nomFichier)) != MONSTREDATA_OK) { monstreData_detruire(monstreData); if (res) *res = code; return NULL; }

	monstreData->PVMax = PVMax;
	monstreData->xp = xp;
	monstreData->piecesOr = piecesOr;

	if (res) *res = MONSTREDATA_OK;
	return monstreData;
}

void monstreData_detruire(monstreData_t *monstreData) {
	if (!monstreData) return;
	SDL_DestroyTexture(monstreData->texture);
	free(monstreData->nom);
	free(monstreData);
}

const char * monstreData_strerror(monstreData_result_t res) {
	switch (res) {
		case MONSTREDATA_OK: return "Succes";
		case MONSTREDATA_ERR_NULL_RENDERER: return "Renderer NULL passe en parametre";
		case MONSTREDATA_ERR_NULL_OR_EMPTY_FILENAME: return "Nom fichier NULL ou vide";
		case MONSTREDATA_ERR_SIZE_MAX_FILENAME: return "Nom fichier trop long";
		case MONSTREDATA_ERR_NULL_OR_EMPTY_NAME: return "Nom NULL ou vide";
		case MONSTREDATA_ERR_SIZE_MAX_NAME: return "Nom trop long";
		case MONSTREDATA_ERR_INVALID_PVMAX: return "PVMax < 0";
		case MONSTREDATA_ERR_INVALID_XP: return "XP < 0";
		case MONSTREDATA_ERR_INVALID_PIECESOR: return "PiecesOr < 0";
		case MONSTREDATA_ERR_MEMORY_BASE: return "Echec allocation memoire base";
		case MONSTREDATA_ERR_MEMORY_NAME: return "Echec allocation memoire nom";
		case MONSTREDATA_ERR_LOAD_TEXTURE: return "Echec chargement texture";
		default: return "Erreur";
	}
}