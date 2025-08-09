// @author Alain Barbier alias "Metroidzeta"

#ifndef CHARGERSKINS_H
#define CHARGERSKINS_H

#include "base.h"
#include "arraylist.h"
#include "skin.h"

typedef enum {
	CHARGERSKINS_OK = 0,
	CHARGERSKINS_ERR_NULL_POINTER,
	CHARGERSKINS_ERR_CREATE_ARRAYLIST,
	CHARGERSKINS_ERR_CREATE_SKIN
} chargerSkins_result_t;

typedef struct {
	const char *nomFichier;
} skin_info_t;

chargerSkins_result_t chargerSkins_get(SDL_Renderer *renderer, arraylist_t **skins);
const char * chargerSkins_strerror(chargerSkins_result_t res);

#endif