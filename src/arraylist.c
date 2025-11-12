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

#include "headers/arraylist.h"
#include "headers/carte.h"
#include "headers/heros.h"
#include "headers/bruitage.h"
#include "headers/event.h"

arraylist_t * arraylist_creer(al_type type, arraylist_result_t *res) {
	if (!res) { LOG_ERROR("Enum arraylist_result NULL"); return NULL; }
	*res = ARRAYLIST_OK;
	if (ARRAYLIST_INITIAL_CAPACITY < 1) { *res = ARRAYLIST_ERR_INVALID_INITIAL_CAPACITY; return NULL; }

	arraylist_t *a = calloc(1, sizeof(arraylist_t));
	if (!a) { *res = ARRAYLIST_ERR_MEMORY_BASE; return NULL; }

	a->tab = malloc(ARRAYLIST_INITIAL_CAPACITY * sizeof(void *));
	if (!a->tab) { free(a); *res = ARRAYLIST_ERR_MEMORY_TAB; return NULL; }

	a->altype = type;
	a->capacite = ARRAYLIST_INITIAL_CAPACITY;
	a->taille = 0;

	return a;
}

bool arraylist_isEmpty(const arraylist_t *a) { return !a || a->taille == 0; }

/** Double la capacite du tableau dynamique (realloc amortie) **/
static void arraylist_enlargeCapacity(arraylist_t *a) {
	if (a->capacite > SIZE_MAX / 2) Exception("Capacite arraylist depasse la limite size_t");
	const size_t newCapacite = a->capacite * 2;
	void **newTab = (void**) realloc(a->tab, newCapacite * sizeof(void *));
	if (!newTab) Exception("Echec reallocation tableau arraylist");
	a->tab = newTab;
	a->capacite = newCapacite;
}

void arraylist_add(arraylist_t *a, void *ptr) {
	if (!a || !ptr) return;
	if (a->taille == a->capacite) arraylist_enlargeCapacity(a);
	a->tab[a->taille++] = ptr;
}

void * arraylist_get(const arraylist_t *a, size_t pos) { // size_t pos est forcément positif (> 0)
	return (a && pos < a->taille) ? a->tab[pos] : NULL;
}

static void arraylist_detruireElement(void *elem, al_type type) {
	switch (type) { // un pointeur vers..
		case AL_SDL_RECT:      free(elem); break;                  // Rectangle (SDL_Rect)
		case AL_TEXTURE:       SDL_DestroyTexture(elem); break;    // Texture (SDL_Texture)
		case AL_SKIN:          skin_detruire(elem); break;         // Skin
		case AL_MONSTRE_DATA:  monstreData_detruire(elem); break;  // MonstreData
		case AL_MONSTRE:       monstre_detruire(elem); break;      // Monstre
		case AL_POLICE:        TTF_CloseFont(elem); break;         // Police (TTF_Font)
		case AL_MUSIQUE:       musique_detruire(elem); break;      // Musique
		case AL_BRUITAGE:      bruitage_detruire(elem); break;     // Bruitage
		case AL_CHIPSET:       chipset_detruire(elem); break;      // Chipset
		case AL_CARTE:         carte_detruire(elem); break;        // Carte
		case AL_HEROS:         heros_detruire(elem); break;        // Héros
		case AL_EVENT:         event_detruire(elem); break;        // Event
		default: break;
	}
}

static void arraylist_detruireElements(arraylist_t *a, bool libererMemoireElements) {
	for (size_t i = 0; i < a->taille; ++i) {
		if (libererMemoireElements) arraylist_detruireElement(a->tab[i], a->altype);
		a->tab[i] = NULL;
	}
}

void arraylist_clear(arraylist_t *a, bool libererMemoireElements) {
	if (!a) return;
	arraylist_detruireElements(a, libererMemoireElements);

	if (a->capacite != ARRAYLIST_INITIAL_CAPACITY) {
		void **newTab = (void **) realloc(a->tab, ARRAYLIST_INITIAL_CAPACITY * sizeof(void *));
		if (!newTab) Exception("Echec reallocation tableau arraylist");
		a->tab = newTab;
		a->capacite = ARRAYLIST_INITIAL_CAPACITY;
	}
	a->taille = 0;
}

void arraylist_detruire(arraylist_t *a, bool libererMemoireElements) {
	if (!a) return;
	arraylist_detruireElements(a, libererMemoireElements);
	free(a->tab); a->tab = NULL;
	free(a);
}

const char * arraylist_strerror(arraylist_result_t res) {
	switch (res) {
		case ARRAYLIST_OK: return "Succes";
		case ARRAYLIST_ERR_INVALID_INITIAL_CAPACITY: return "ARRAYLIST_INITIAL_CAPACITY < 1";
		case ARRAYLIST_ERR_MEMORY_BASE: return "Echec allocation memoire base";
		case ARRAYLIST_ERR_MEMORY_TAB: return "Echec allocation memoire tableau";
		default: return "Erreur inconnue";
	}
}