// @author Alain Barbier alias "Metroidzeta"

#ifndef CHARGERCARTES_H
#define CHARGERCARTES_H

#include "base.h"
#include "arraylist.h"
#include "carte.h"

typedef enum {
	CHARGERCARTES_OK = 0,
	CHARGERCARTES_ERR_NULL_POINTER_CARTES,
	CHARGERCARTES_ERR_NULL_POINTER_CHIPSETS,
	CHARGERCARTES_ERR_NULL_POINTER_MUSIQUES,
	CHARGERCARTES_ERR_CREATE_ARRAYLIST,
	CHARGERCARTES_ERR_GET_CHIPSET,
	CHARGERCARTES_ERR_GET_MUSIQUE,
	CHARGERCARTES_ERR_CREATE_CARTE
} chargerCartes_result_t;

typedef struct {
	const char *nom;
	int largeur;
	int hauteur;
	const char *nomChipset;
	const char *nomMusique;
	bool depuisFichiers;
} carte_info_t;

chargerCartes_result_t chargerCartes_get(arraylist_t **cartes, arraylist_t *chipsets, arraylist_t *musiques);
const char * chargerCartes_strerror(chargerCartes_result_t res);

#endif