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

#ifndef CARTE_H
#define CARTE_H

#define NB_COUCHES 3 // par défaut : 3
#define TAILLE_CARTE_MAX 100 // par défaut : 100
#define TUILE_VIDE 0 // par défaut : 0
#define NB_PAGES_EVENT 5 // par défaut : 5

#include "arraylist.h"
#include "chipset.h"
#include "musique.h"
#include "monstre.h"

// Forward declaration pour casser la circularité
typedef struct event_s event_t;

typedef struct {
	//int xCase;
	//int yCase;
	arraylist_t *events[NB_PAGES_EVENT];
} ensemble_events_t;

typedef struct carte_s {
	char *nom;
	int largeur;                                 // en cases
	int hauteur;                                 // en cases
	chipset_t *chipset;                          // chipset utilisé
	musique_t *musique;                          // musique utilisée
	int **couches[NB_COUCHES];                   // 3 couches (matrices) de numTuileChipset (C0 < C1 < Héros < C2)
	bool **murs;                                 // matrice booléenne représentant les murs sur chaque case (F = pas de mur, T = mur)
	SDL_Rect (*matriceRect)[TAILLE_CARTE_MAX];   // matrice globale de rectangles (partagée entre toutes les cartes) représentant les cases de chaque carte
	ensemble_events_t **ensembleEvents;          // matrice d'ensembles d'événements
	arraylist_t *monstres;                       // monstres présents sur la carte
} carte_t;

typedef enum {
	CARTE_OK = 0,
	CARTE_ERR_NULL_OR_EMPTY_NAME,
	CARTE_ERR_SIZE_MAX_NAME,
	CARTE_ERR_INVALID_LARGEUR,
	CARTE_ERR_INVALID_HAUTEUR,
	CARTE_ERR_NULL_CHIPSET,
	CARTE_ERR_MEMORY_BASE,
	CARTE_ERR_MEMORY_NAME,
	CARTE_ERR_MEMORY_LAYER,
	CARTE_ERR_MEMORY_WALLS,
	CARTE_ERR_MEMORY_SET_EVENTS,
	CARTE_ERR_CREATE_ARRAYLIST_MONSTERS,
	CARTE_ERR_MEMORY_SET_EVENTS_ROWS,
	CARTE_ERR_CREATE_ARRAYLIST_EVENTS
} carte_result_t;

carte_t * carte_creer(const char *nom, int largeur, int hauteur, chipset_t *chipset, musique_t *musique, int **c0, int **c1, int **c2, bool **murs, carte_result_t *res);
carte_t * carte_creerVide(const char *nom, int largeur, int hauteur, chipset_t *chipset, musique_t *musique, carte_result_t *res);

void carte_ecrireFichier(carte_t *carte);
bool carte_verifierCollisionsMurs(carte_t *carte, SDL_Rect *hitBox);
arraylist_t * carte_verifierCollisionsEvents(carte_t *carte, SDL_Rect *hitBox);
void carte_ajouterEvent(carte_t *carte, int numPage, int xCase, int yCase, event_t *ev);
void carte_ajouterMonstre(carte_t *carte, monstre_t *monstre);

void carte_detruire(carte_t *carte);
const char * carte_strerror(carte_result_t res);

#endif