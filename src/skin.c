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

#include "headers/skin.h"

static inline skin_result_t skin_validerArguments(SDL_Renderer *renderer, const char *nomFichier) {
	if (!renderer) return SKIN_ERR_NULL_RENDERER;
	if (!nomFichier || !*nomFichier) return SKIN_ERR_NULL_OR_EMPTY_FILENAME;
	if (strlen(nomFichier) >= MAX_TAILLE_STRING) return SKIN_ERR_SIZE_MAX_FILENAME;
	return SKIN_OK;
}

static skin_result_t skin_chargerTexture(SDL_Renderer *renderer, skin_t *skin, const char *nomFichier) {
	skin->texture = creerTexture(renderer, nomFichier);
	if (!skin->texture) { LOG_ERROR("Echec creerTexture: %s (src: %s)", IMG_GetError(), nomFichier); return SKIN_ERR_LOAD_TEXTURE; }
	for (int i = 0; i < SK_ROWS; ++i) {
		const int y = i * SK_REGION_HEIGHT;
		const int ligneIndex = i * SK_COLS;
		for (int j = 0; j < SK_COLS; ++j) { //                 _________.x________ .y  ______.w_______  _______.h_______
			skin->textureRegions[ligneIndex + j] = (SDL_Rect){ j * SK_REGION_WIDTH, y, SK_REGION_WIDTH, SK_REGION_HEIGHT };
		}
	}
	return SKIN_OK;
}

skin_t * skin_creer(SDL_Renderer *renderer, const char *nomFichier, skin_result_t *res) {
	if (!res) { LOG_ERROR("Enum skin_result NULL"); return NULL; }
	*res = SKIN_OK;
	if ((*res = skin_validerArguments(renderer, nomFichier))) return NULL;

	skin_t *skin = calloc(1, sizeof(skin_t));
	if (!skin) { *res = SKIN_ERR_MEMORY_BASE; return NULL; }

	skin->nom = my_strdup(nomFichier); // important : ne pas faire "skin->nom = nomFichier", car cela ne copie que le pointeur, pas le contenu
	if (!skin->nom) { skin_detruire(skin); *res = SKIN_ERR_MEMORY_NAME; return NULL; }

	if ((*res = skin_chargerTexture(renderer, skin, nomFichier))) { skin_detruire(skin); return NULL; }
	return skin;
}

skin_result_t skin_afficher(SDL_Renderer *renderer, skin_t *skin, int numRegion, SDL_Rect *dstRect) {
	if (!renderer) return SKIN_ERR_NULL_RENDERER;
	if (!skin) return SKIN_ERR_NULL_POINTER;
	if (numRegion < 0 || numRegion >= SK_TOTAL_REGIONS) { LOG_ERROR("Indice de région invalide : %d", numRegion); return SKIN_ERR_INVALID_NUMREGION; }
	dessinerTexture(renderer, skin->texture, &skin->textureRegions[numRegion], dstRect, "Echec dessin de la skin avec SDL_RenderCopy");
	return SKIN_OK;
}

void skin_detruire(skin_t *skin) {
	if (!skin) return;
	SDL_DestroyTexture(skin->texture); skin->texture = NULL;
	free(skin->nom); skin->nom = NULL;
	free(skin);
}

const char * skin_strerror(skin_result_t res) {
	switch (res) {
		case SKIN_OK: return "Succes";
		case SKIN_ERR_NULL_POINTER: return "Skin NULL passe en parametre";
		case SKIN_ERR_NULL_RENDERER: return "Renderer NULL passe en parametre";
		case SKIN_ERR_NULL_OR_EMPTY_FILENAME: return "Nom fichier NULL ou vide";
		case SKIN_ERR_SIZE_MAX_FILENAME : return "Nom fichier trop long";
		case SKIN_ERR_MEMORY_BASE: return "Echec allocation memoire base";
		case SKIN_ERR_MEMORY_NAME: return "Echec allocation memoire nom";
		case SKIN_ERR_PATH_TOO_LONG_OR_EMPTY: return "Chemin fichier trop long ou vide";
		case SKIN_ERR_LOAD_TEXTURE: return "Echec chargement texture";
		case SKIN_ERR_INVALID_NUMREGION: return "Indice de region invalide";
		default: return "Erreur";
	}
}