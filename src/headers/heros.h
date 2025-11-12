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

#ifndef HEROS_H
#define HEROS_H

#include "skin.h"
#include "carte.h"

#define FORCE_INITIALE 12
#define DEXTERITE_INITIALE 9
#define CONSTITUTION_INITIALE 10
#define ALIGNEMENT_INITIAL 50
#define PV_INITIAL 600
#define PM_INITIAL 250

#define OFFSET_EPEE_PX (TAILLE_CASES / 4)

typedef enum { BAS = 0, GAUCHE, DROITE, HAUT } Directions;
typedef enum { BARBARE = 0, GUERRIER, VOLEUR } Classes;

typedef struct {
	char *nom;
	SDL_Texture *textureNom;
	skin_t *skin;
	Classes classe; // BARBARE, GUERRIER ou VOLEUR (enum Classes)
	int niveau;
	int piecesOr;
	SDL_Point position, positionEcran;
	int xOffSet, yOffSet; // Le décalage entre la position (x,y) REELE / SUR L'ECRAN
	SDL_Rect hitBox, hitBoxEcran;
	SDL_Rect hitBoxEpee[4], hitBoxEpeeEcran[4]; // hitbox REELE / SUR L'ECRAN de l'épée du héros
	int force, dexterite, constitution;
	int PV[2]; // PV / PVMax (PV[0] / PV[1])
	int PM[2]; // PM / PMMax (PM[0] / PM[1])
	Directions direction;
	int alignement;
	bool peutAttaquer, attaqueEpee, estBloque, messageTete, estEnTrainDEcrire, estDansUnEvent;
	carte_t *carteActuelle;
	float tauxCrit;
	int frameDeplacement;
} heros_t;

typedef enum {
	HEROS_OK = 0,
	HEROS_ERR_NULL_RENDERER,
	HEROS_ERR_NULL_OR_EMPTY_NAME,
	HEROS_ERR_SIZE_MAX_NAME,
	HEROS_ERR_NULL_SKIN,
	HEROS_ERR_INVALID_LEVEL,
	HEROS_ERR_INVALID_PIECES_OR,
	HEROS_ERR_NULL_CARTE,
	HEROS_ERR_INVALID_XCASE,
	HEROS_ERR_INVALID_YCASE,
	HEROS_ERR_INVALID_TAUXCRIT,
	HEROS_ERR_MEMORY_BASE,
	HEROS_ERR_MEMORY_NAME,
	HEROS_ERR_LOAD_TEXTURE_NAME
} heros_result_t;

heros_t * heros_creer(SDL_Renderer *renderer, const char *nom, skin_t *skin, Classes classe, int niveau, int piecesOr, int xCase, int yCase, TTF_Font *police, carte_t *carte, float tauxCrit, heros_result_t *res);

void heros_updateOffSet(heros_t *heros);
void heros_afficherNom(SDL_Renderer *renderer, heros_t *heros, SDL_Rect rectPseudo);
void heros_afficherSkin(SDL_Renderer *renderer, heros_t *heros);
void heros_modifierAlignement(heros_t *heros, int n);
void heros_modifierPV(heros_t *heros, int n);
void heros_modifierPM(heros_t *heros, int n);
void heros_lvlup(heros_t *heros);
void heros_modifierPosition(heros_t *heros, int newX, int newY);
void heros_updateHitBoxEpee(heros_t *heros);
int heros_getXCase(heros_t *heros);
int heros_getYCase(heros_t *heros);
double heros_getRatioPV(heros_t *heros);
double heros_getRatioPM(heros_t *heros);
bool heros_deplacer(heros_t *heros, Directions direction);
const char * heros_getDirectionToString(heros_t *heros);
const char * heros_getClasseToString(heros_t *heros);

void heros_detruire(heros_t *heros);
const char * heros_strerror(heros_result_t res);

#endif