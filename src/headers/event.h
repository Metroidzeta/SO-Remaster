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

#ifndef EVENT_H
#define EVENT_H

#include "musique.h"

// Forward declaration pour casser la circularité
typedef struct carte_s carte_t;

typedef enum {
	EVENT_MSG,                // message du jeu
	EVENT_TP,                 // téléportation
	EVENT_JOUER_MUSIQUE,      // jouer musique
	EVENT_ARRET_MUSIQUE,      // arreter musique
	EVENT_MODIF_PV,           // modifier PV joueur
	EVENT_MODIF_PM,           // modifier PM joueur
	EVENT_LVLUP
} EventType;

typedef struct {
	int xDst;
	int yDst;
	carte_t *carteDst;
} TpData;

typedef union {
	char *msg;            // message du jeu
	TpData tp;            // tp (x, y et carte de destination)
	musique_t *musique;   // la nouvelle musique à utiliser
	int val_PV;           // changer les PV de cette valeur
	int val_PM;           // changer les PM de cette valeur
} EventData_t;

typedef struct event_s {
	EventType type;     // type de l'event
	EventData_t data;   // données de l'event
} event_t;

typedef enum {
	EVENT_OK = 0,
	EVENT_ERR_NULL_POINTER_DATA,
	EVENT_ERR_MEMORY_BASE,
	EVENT_ERR_NULL_OR_EMPTY_MSG,
	EVENT_ERR_SIZE_MAX_MSG,
	EVENT_ERR_MEMORY_MSG,
	EVENT_ERR_NULL_POINTER_CARTEDST,
	EVENT_ERR_INVALID_COORD,
	EVENT_ERR_NULL_POINTER_MUSIQUE,
	EVENT_ERR_ARGS_PV,
	EVENT_ERR_ARGS_PM
} event_result_t;

event_t * event_creerMSG(const char *msg, event_result_t *res);
event_t * event_creerTP(int xCaseDst, int yCaseDst, carte_t *carteDst, event_result_t *res);
event_t * event_creerJM(musique_t *musique, event_result_t *res);
event_t * event_creerAM(event_result_t *res);
event_t * event_creerModifPV(int PV, event_result_t *res);
event_t * event_creerModifPM(int PM, event_result_t *res);
event_t * event_creerLVLUP(event_result_t *res);

void event_detruire(event_t *ev);
const char * event_strerror(event_result_t res);

#endif