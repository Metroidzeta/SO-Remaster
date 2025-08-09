// @author Alain Barbier alias "Metroidzeta"

#ifndef CHARGERMONSTRESDATA_H
#define CHARGERMONSTRESDATA_H

#include "base.h"
#include "arraylist.h"
#include "monstreData.h"

typedef enum {
	CHARGERMONSTRESDATA_OK = 0,
	CHARGERMONSTRESDATA_ERR_NULL_POINTER,
	CHARGERMONSTRESDATA_ERR_CREATE_ARRAYLIST,
	CHARGERMONSTRESDATA_ERR_CREATE_MONSTREDATA
} chargerMonstresData_result_t;

typedef struct {
	const char *nomFichier;
	const char *nom;
	int PVMax;
	int xp;
	int piecesOr;
} monstreData_info_t;

chargerMonstresData_result_t chargerMonstresData_get(SDL_Renderer *renderer, arraylist_t **monstresData);
const char * chargerMonstresData_strerror(chargerMonstresData_result_t res);

#endif