/**
 * @author Alain Barbier alias "Metroidzeta"
 * Copyright © 2025 Alain Barbier (Metroidzeta) - All rights reserved.
 *
 * This file is part of the project covered by the
 * "Educational and Personal Use License / Licence d’Utilisation Personnelle et Éducative".
 *
 * Permission is granted to fork and use this code for educational and personal purposes only.
 *
 * Commercial use, redistribution, or public republishing of modified versions
 * is strictly prohibited without the express written consent of the author.
 *
 * Coded with SDL2 (Simple DirectMedia Layer 2).
 *
 * Created by Metroidzeta.
 */

#include "headers/monstreData.h"

static inline monstreData_result_t monstreData_verifierArguments(SDL_Renderer *renderer, const char *nomFichier, const char *nom, int PVMax, int xp, int piecesOr) {
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
	if (!monstreData->texture) { LOG_ERROR("Echec creerTexture: %s (src: %s)", IMG_GetError(), nomFichier); return MONSTREDATA_ERR_LOAD_TEXTURE; }
	for (int i = 0; i < MD_ROWS; ++i) {
		const int y = 15 + i * 32;
		const int ligneIndex = i * MD_COLS;
		for (int j = 0; j < MD_COLS; ++j) { //                        ____.x____ .y  .w  .h
			monstreData->textureRegions[ligneIndex + j] = (SDL_Rect){ 3 + j * 24, y, 18, 18 };
		}
	}
	return MONSTREDATA_OK;
}

monstreData_t * monstreData_creer(SDL_Renderer *renderer, const char *nomFichier, const char *nom, int PVMax, int xp, int piecesOr, monstreData_result_t *res) {
	if (!res) { LOG_ERROR("Enum monstreData_result NULL"); return NULL; }
	*res = MONSTREDATA_OK;
	if ((*res = monstreData_verifierArguments(renderer, nomFichier, nom, PVMax ,xp, piecesOr))) return NULL;

	monstreData_t *monstreData = calloc(1, sizeof(monstreData_t));
	if (!monstreData) { *res = MONSTREDATA_ERR_MEMORY_BASE; return NULL; }

	monstreData->nom = my_strdup(nom); // important : ne pas faire "monstreData->nom = nom", car cela ne copie que le pointeur, pas le contenu
	if (!monstreData->nom) { monstreData_detruire(monstreData); *res = MONSTREDATA_ERR_MEMORY_NAME; return NULL; }

	if ((*res = monstreData_chargerTexture(monstreData, renderer, nomFichier))) { monstreData_detruire(monstreData); return NULL; }

	monstreData->PVMax = PVMax;
	monstreData->xp = xp;
	monstreData->piecesOr = piecesOr;

	return monstreData;
}

void monstreData_detruire(monstreData_t *monstreData) {
	if (!monstreData) return;
	SDL_DestroyTexture(monstreData->texture); monstreData->texture = NULL;
	free(monstreData->nom); monstreData->nom = NULL;
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