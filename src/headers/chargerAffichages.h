// @author Alain Barbier alias "Metroidzeta"

#ifndef CHARGERAFFICHAGES_H
#define CHARGERAFFICHAGES_H

#include "base.h"
#include "arraylist.h"

typedef enum {
	CHARGERAFFICHAGES_OK = 0,
	CHARGERAFFICAHGES_ERR_NULL_POINTER,
	CHARGERAFFICHAGES_ERR_CREATE_ARRAYLIST,
	CHARGERAFFICHAGES_ERR_CREATE_AFFICHAGE
} chargerAffichages_result_t;

chargerAffichages_result_t chargerAffichages_get(SDL_Renderer *renderer, arraylist_t **affichages);
const char * chargerAffichages_strerror(chargerAffichages_result_t res);

#endif