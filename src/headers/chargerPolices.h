// @author Alain Barbier alias "Metroidzeta"

#ifndef CHARGERPOLICES_H
#define CHARGERPOLICES_H

#include "base.h"
#include "arraylist.h"

typedef enum {
	CHARGERPOLICES_OK = 0,
	CHARGERPOLICES_ERR_NULL_POINTER,
	CHARGERPOLICES_ERR_CREATE_ARRAYLIST,
	CHARGERPOLICES_ERR_CREATE_POLICE
} chargerPolices_result_t;

typedef struct {
	const char *nomFichier;
	int taille;
} police_info_t;

chargerPolices_result_t chargerPolices_get(arraylist_t **polices);
const char * chargerPolices_strerror(chargerPolices_result_t res);

#endif