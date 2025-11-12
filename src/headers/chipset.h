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

#ifndef CHIPSET_H
#define CHIPSET_H

#include "base.h"

/**
 * Représente un chipset, c’est-à-dire un ensemble de tuiles graphiques
 * découpées à partir d’une image source unique servant à composer les cartes du jeu.
 */
typedef struct {
	char *nom;
	SDL_Texture *texture; // Image du chipset
	int tailleTuile; // Taille d'une tuile (case) n*n en pixels du chipset
	int nbTuilesHauteur;
	int nbTuilesLargeur;
	SDL_Rect *tuiles;
} chipset_t;

typedef enum {
	CHIPSET_OK = 0,
	CHIPSET_ERR_NULL_RENDERER,
	CHIPSET_ERR_NULL_OR_EMPTY_FILENAME,
	CHIPSET_ERR_SIZE_MAX_FILENAME,
	CHIPSET_ERR_TUILES_SIZE,
	CHIPSET_ERR_MEMORY_BASE,
	CHIPSET_ERR_MEMORY_NAME,
	CHIPSET_ERR_LOAD_TEXTURE,
	CHIPSET_ERR_QUERY_TEXTURE,
	CHIPSET_ERR_INVALID_DIMENSIONS,
	CHIPSET_ERR_TOO_MANY_TILES,
	CHIPSET_ERR_MEMORY_TUILES
} chipset_result_t;

chipset_t * chipset_creer(SDL_Renderer *renderer, const char *nomFichier, int tailleTuile, chipset_result_t *res);

void chipset_detruire(chipset_t *chipset);
const char * chipset_strerror(chipset_result_t res);

#endif