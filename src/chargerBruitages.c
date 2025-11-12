/**
 * @author Alain Barbier alias "Metroidzeta"
 * Copyright © 2025 Alain Barbier (Metroidzeta) - All rights reserved.
 *
 * This file is part of the project covered by the
 * "Educational and Personal Use License / Licence d’Utilisation Personnelle et Éducative".
 *
 * Permission is granted to fork and use this code for educational and personal purposes only.
 *
 * Commercial use, redistribution, or public republishing of modified versions
 * is strictly prohibited without the express written consent of the author.
 *
 * Coded with SDL2 (Simple DirectMedia Layer 2).
 *
 * Created by Metroidzeta.
 */

#include "headers/chargerBruitages.h"
#include "headers/bruitage.h"

static const char * BRUITAGES_LIST[] = { // sera remplacé par une détection des fichiers présents dans le dossier "bruitages" à l'avenir
	// Création des bruitages { nomFichier }
	"Blow1.wav",                            // 0
	"Kill1.wav",                            // 1
	"Damage3.ogg"                           // 2
};

static chargerBruitages_result_t ajouterBruitage(const char *nomFichier, arraylist_t *bruitages) {
	bruitage_result_t res_bruit;
	bruitage_t *bruitage = bruitage_creer(nomFichier, &res_bruit);
	if (!bruitage) { LOG_ERROR("%s (src: %s)", bruitage_strerror(res_bruit), nomFichier); return CHARGERBRUITAGES_ERR_CREATE_BRUITAGE; }

	arraylist_add(bruitages, bruitage);
	return CHARGERBRUITAGES_OK;
}

arraylist_t * chargerBruitages_get(chargerBruitages_result_t *res) {
	if (!res) { LOG_ERROR("Enum chargerBruitages_result NULL"); return NULL; }
	*res = CHARGERBRUITAGES_OK;

	arraylist_result_t al_res;
	arraylist_t *bruitages = arraylist_creer(AL_BRUITAGE, &al_res);
	if (!bruitages) { LOG_ERROR("%s", arraylist_strerror(al_res)); *res = CHARGERBRUITAGES_ERR_CREATE_ARRAYLIST; return NULL; }

	const size_t nbBruitages = sizeof(BRUITAGES_LIST) / sizeof(BRUITAGES_LIST[0]);
	for (size_t i = 0; i < nbBruitages; ++i) {
		chargerBruitages_result_t chBruit_res = ajouterBruitage(BRUITAGES_LIST[i], bruitages);
		if (chBruit_res) { arraylist_detruire(bruitages, true); *res = chBruit_res; return NULL; }
	}
	return bruitages;
}

const char * chargerBruitages_strerror(chargerBruitages_result_t res) {
	switch (res) {
		case CHARGERBRUITAGES_OK: return "Succes";
		case CHARGERBRUITAGES_ERR_CREATE_ARRAYLIST: return "Echec creation arraylist des bruitages";
		case CHARGERBRUITAGES_ERR_CREATE_BRUITAGE: return "Echec creation du bruitage";
		default: return "Erreur inconnue";
	}
}