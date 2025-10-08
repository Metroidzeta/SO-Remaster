// @author Alain Barbier alias "Metroidzeta"

#ifndef CHARGEREVENTS_H
#define CHARGEREVENTS_H

#include "base.h"
#include "event.h"
#include "arraylist.h"

typedef enum {
	CHARGEREVENTS_OK = 0,
	CHARGEREVENTS_ERR_NULL_POINTER_CARTE,
	CHARGEREVENTS_ERR_NULL_POINTER_CARTES,
	CHARGEREVENTS_ERR_NULL_POINTER_MUSIQUES,
	CHARGEREVENTS_ERR_READ_MURS_EVENTS,
	CHARGEREVENTS_ERR_GET_CARTEDST,
	CHARGEREVENTS_ERR_GET_MUSIQUE
} chargerEvents_result_t;

typedef struct {
	const char *nomCarte;         // nom de la carte où est l'event
	int numPage;                  // numPage de l'event (par défaut : 0)
	int xCase;                    // xCase de l'event
	int yCase;                    // yCase de l'event
} EventInfo_t;

chargerEvents_result_t chargerEvents_get(arraylist_t *cartes, arraylist_t *musiques);
const char * chargerEvents_strerror(chargerEvents_result_t res);

#endif