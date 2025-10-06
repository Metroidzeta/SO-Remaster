// @author Alain Barbier alias "Metroidzeta"

#ifndef CHARGERMUSIQUES_H
#define CHARGERMUSIQUES_H

#include "base.h"
#include "arraylist.h"
#include "musique.h"

typedef enum {
	CHARGERMUSIQUES_OK = 0,
	CHARGERMUSIQUES_ERR_NULL_POINTER,
	CHARGERMUSIQUES_ERR_CREATE_ARRAYLIST,
	CHARGERMUSIQUES_ERR_CREATE_MUSIQUE
} chargerMusiques_result_t;

chargerMusiques_result_t chargerMusiques_get(arraylist_t **musiques);
const char * chargerMusiques_strerror(chargerMusiques_result_t res);

#endif