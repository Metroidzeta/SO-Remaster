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

#include "headers/chipset.h"

static inline chipset_result_t chipset_validerArguments(SDL_Renderer *renderer, const char *nomFichier, int tailleTuile) {
	if (!renderer) return CHIPSET_ERR_NULL_RENDERER;
	if (!nomFichier || !*nomFichier) return CHIPSET_ERR_NULL_OR_EMPTY_FILENAME;
	if (strlen(nomFichier) >= MAX_TAILLE_STRING) return CHIPSET_ERR_SIZE_MAX_FILENAME;
	if (tailleTuile < 1) return CHIPSET_ERR_TUILES_SIZE;
	return CHIPSET_OK;
}

static chipset_result_t chipset_chargerTexture(chipset_t *chipset, SDL_Renderer *renderer, const char *nomFichier) {
	chipset->texture = creerTexture(renderer, nomFichier);
	if (!chipset->texture) { LOG_ERROR("Echec creerTexture: %s (src: %s)", IMG_GetError(), nomFichier); return CHIPSET_ERR_LOAD_TEXTURE; }
	return CHIPSET_OK;
}

static chipset_result_t chipset_extraireTuiles(chipset_t *chipset) {
	const int nbTuilesLargeur = chipset->nbTuilesLargeur, nbTuilesHauteur = chipset->nbTuilesHauteur;
	const int tailleTuile = chipset->tailleTuile;
	const int nbTuiles = nbTuilesLargeur * nbTuilesHauteur;
	chipset->tuiles = malloc(nbTuiles * sizeof(SDL_Rect));
	if (!chipset->tuiles) return CHIPSET_ERR_MEMORY_TUILES;

	for (int i = 0; i < nbTuilesHauteur; i++) {
		const int y = i * tailleTuile;
		const int ligneIndex = i * nbTuilesLargeur;
		for (int j = 0; j < nbTuilesLargeur; j++) {
			chipset->tuiles[ligneIndex + j] = (SDL_Rect){ j * tailleTuile, y, tailleTuile, tailleTuile };
		}
	}

	return CHIPSET_OK;
}

static chipset_result_t chipset_calculerDimensions(chipset_t *chipset) {
	int largeur, hauteur;
	const int tailleTuile = chipset->tailleTuile;
	if (SDL_QueryTexture(chipset->texture, NULL, NULL, &largeur, &hauteur) != 0) { LOG_ERROR("Echec SDL_QueryTexture : %s", SDL_GetError()); return CHIPSET_ERR_QUERY_TEXTURE; }
	if (largeur % tailleTuile != 0 || hauteur % tailleTuile != 0) return CHIPSET_ERR_INVALID_DIMENSIONS;

	chipset->nbTuilesLargeur = largeur / tailleTuile;
	chipset->nbTuilesHauteur = hauteur / tailleTuile;
	if (chipset->nbTuilesLargeur > INT_MAX / chipset->nbTuilesHauteur) return CHIPSET_ERR_TOO_MANY_TILES;

	return CHIPSET_OK;
}

chipset_t * chipset_creer(SDL_Renderer *renderer, const char *nomFichier, int tailleTuile, chipset_result_t *res) {
	if (!res) { LOG_ERROR("Enum chipset_result NULL"); return NULL; }
	*res = CHIPSET_OK;
	if ((*res = chipset_validerArguments(renderer, nomFichier, tailleTuile))) return NULL;

	chipset_t *chipset = calloc(1, sizeof(chipset_t));
	if (!chipset) { *res = CHIPSET_ERR_MEMORY_BASE; return NULL; }

	chipset->nom = my_strdup(nomFichier); // important : ne pas faire "chipset->nom = nomFichier", car cela ne copie que le pointeur, pas le contenu
	if (!chipset->nom) { chipset_detruire(chipset); *res = CHIPSET_ERR_MEMORY_NAME; return NULL; }

	chipset->tailleTuile = tailleTuile;
	if ((*res = chipset_chargerTexture(chipset, renderer, nomFichier))) { chipset_detruire(chipset); return NULL; }
	if ((*res = chipset_calculerDimensions(chipset))) { chipset_detruire(chipset); return NULL; }
	if ((*res = chipset_extraireTuiles(chipset))) { chipset_detruire(chipset); return NULL; }

	return chipset;
}

void chipset_detruire(chipset_t *chipset) {
	if (!chipset) return;
	free(chipset->tuiles); chipset->tuiles = NULL;
	SDL_DestroyTexture(chipset->texture); chipset->texture = NULL;
	free(chipset->nom); chipset->nom = NULL;
	free(chipset);
}

const char * chipset_strerror(chipset_result_t res) {
	switch (res) {
		case CHIPSET_OK: return "Succes";
		case CHIPSET_ERR_NULL_RENDERER: return "Renderer NULL passe en parametre";
		case CHIPSET_ERR_NULL_OR_EMPTY_FILENAME: return "Nom fichier NULL ou vide";
		case CHIPSET_ERR_SIZE_MAX_FILENAME: return "Nom fichier trop long";
		case CHIPSET_ERR_TUILES_SIZE: return "tailleTuile < 1";
		case CHIPSET_ERR_MEMORY_BASE: return "Echec allocation memoire base";
		case CHIPSET_ERR_MEMORY_NAME: return "Echec allocation memoire nom";
		case CHIPSET_ERR_LOAD_TEXTURE: return "Echec chargement de la texture";
		case CHIPSET_ERR_QUERY_TEXTURE: return "Echec demande texture";
		case CHIPSET_ERR_INVALID_DIMENSIONS: return "Dimensions image incompatibles avec taille tuiles (pas mod 0)";
		case CHIPSET_ERR_TOO_MANY_TILES: return "Dimensions image trop elevees";
		case CHIPSET_ERR_MEMORY_TUILES: return "Echec allocation memoire tuiles";
		default: return "Erreur inconnue";
	}
}