// @author Alain Barbier alias "Metroidzeta"

#ifndef CHARGERBRUITAGES_H
#define CHARGERBRUITAGES_H

#include "base.h"
#include "arraylist.h"
#include "bruitage.h"

typedef enum {
	CHARGERBRUITAGES_OK = 0,
	CHARGERBRUITAGES_ERR_NULL_POINTER,
	CHARGERBRUITAGES_ERR_CREATE_ARRAYLIST,
	CHARGERBRUITAGES_ERR_CREATE_BRUITAGE
} chargerBruitages_result_t;

chargerBruitages_result_t chargerBruitages_get(arraylist_t **bruitages);
const char * chargerBruitages_strerror(chargerBruitages_result_t res);

#endif