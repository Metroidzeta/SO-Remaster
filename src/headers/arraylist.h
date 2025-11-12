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

#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include "base.h"

#define ARRAYLIST_INITIAL_CAPACITY 4

// exemple: carte_t *carte; arraylist_foreach(carte_t*, carte, jeu->cartes) { printf("nom: %s\n", carte->nom); }
#define arraylist_foreach(type, var, list) \
	for (size_t _i_##var = 0; \
	     (list) && _i_##var < (list)->taille && ((var = (type)(list)->tab[_i_##var]) || 1); \
	     ++_i_##var)

typedef enum {
	AL_SDL_RECT = 0,
	AL_TEXTURE,
	AL_SKIN,
	AL_MONSTRE_DATA,
	AL_MONSTRE,
	AL_POLICE,
	AL_MUSIQUE,
	AL_BRUITAGE,
	AL_CHIPSET,
	AL_CARTE,
	AL_HEROS,
	AL_EVENT
} al_type;

typedef struct {
	al_type altype;     // Le type des élements du tableau
	void **tab;         // Le tableau qui contient les éléments
	size_t capacite;    // Taille réelle (capacité de stockage) du tableau
	size_t taille;      // Nombre d'éléments actuellement stockés dans le tableau
} arraylist_t;

typedef enum {
	ARRAYLIST_OK = 0,
	ARRAYLIST_ERR_INVALID_INITIAL_CAPACITY,
	ARRAYLIST_ERR_MEMORY_BASE,
	ARRAYLIST_ERR_MEMORY_TAB
} arraylist_result_t;

arraylist_t * arraylist_creer(al_type type, arraylist_result_t *res);
bool arraylist_isEmpty(const arraylist_t * a);
void arraylist_add(arraylist_t *a, void *ptr);
void * arraylist_get(const arraylist_t *a, size_t pos);
void arraylist_clear(arraylist_t *a, bool libererMemoireElements);
void arraylist_detruire(arraylist_t *a, bool libererMemoireElements);
const char * arraylist_strerror(arraylist_result_t res);

#endif