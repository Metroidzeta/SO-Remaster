// @author Alain Barbier alias "Metroidzeta"

#ifndef CHARGERCHIPSETS_H
#define CHARGERCHIPSETS_H

#include "base.h"
#include "arraylist.h"
#include "chipset.h"

typedef enum {
	CHARGERCHIPSETS_OK = 0,
	CHARGERCHIPSETS_ERR_CREATE_ARRAYLIST,
	CHARGERCHIPSETS_ERR_CREATE_CHIPSET
} chargerChipsets_result_t;

typedef struct {
	const char *nomFichier;
	int tailleTuile;
} chipset_info_t;

chargerChipsets_result_t chargerChipsets_get(SDL_Renderer *renderer, arraylist_t **chipsets);
const char * chargerChipsets_strerror(chargerChipsets_result_t res);

#endif