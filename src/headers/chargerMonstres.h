// @author Alain Barbier alias "Metroidzeta"

#ifndef CHARGERMONSTRES_H
#define CHARGERMONSTRES_H

#include "base.h"
#include "arraylist.h"
#include "carte.h"

typedef enum {
	CHARGERMONSTRES_OK = 0,
	CHARGERMONSTRES_ERR_NULL_POINTER_CARTES,
	CHARGERMONSTRES_ERR_NULL_POINTER_MONSTRESDATA,
	CHARGERMONSTRES_ERR_GET_CARTE,
	CHARGERMONSTRES_ERR_GET_MONSTREDATA,
	CHARGERMONSTRES_ERR_CREATE_MONSTRE
} chargerMonstres_result_t;

typedef struct {
	const char *nomCarte;
	const char *nomMonstreData;
	int xCase;
	int yCase;
} monstre_info_t;

chargerMonstres_result_t chargerMonstres_get(arraylist_t *cartes, arraylist_t *monstresData);
const char * chargerMonstres_strerror(chargerMonstres_result_t res);

#endif