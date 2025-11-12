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

#include "headers/chargerSkins.h"
#include "headers/skin.h"

static const char * SKINS_LIST[] = { // sera remplacé par une détection des fichiers présents dans le dossier "img/skins" à l'avenir
	// Création des skins { nomFichier }
	"Evil.png"                          // 0
};

static chargerSkins_result_t ajouterSkin(SDL_Renderer *renderer, const char *nomFichier, arraylist_t *skins) {
	skin_result_t skin_res;
	skin_t *skin = skin_creer(renderer, nomFichier, &skin_res);
	if (!skin) { LOG_ERROR("%s (src: %s)", skin_strerror(skin_res), nomFichier); return CHARGERSKINS_ERR_CREATE_SKIN; }

	arraylist_add(skins, skin);
	return CHARGERSKINS_OK;
}

arraylist_t * chargerSkins_get(SDL_Renderer *renderer, chargerSkins_result_t *res) {
	if (!res) { LOG_ERROR("Enum chargerSkins_result NULL"); return NULL; }
	*res = CHARGERSKINS_OK;

	arraylist_result_t al_res;
	arraylist_t *skins = arraylist_creer(AL_SKIN, &al_res);
	if (!skins) { LOG_ERROR("%s", arraylist_strerror(al_res)); *res = CHARGERSKINS_ERR_CREATE_ARRAYLIST; return NULL; }

	const size_t nbSkins = sizeof(SKINS_LIST) / sizeof(SKINS_LIST[0]);
	for (size_t i = 0; i < nbSkins; ++i) {
		chargerSkins_result_t chSkin_res = ajouterSkin(renderer, SKINS_LIST[i], skins);
		if (chSkin_res) { arraylist_detruire(skins, true); *res = chSkin_res; return NULL; }
	}
	return skins;
}

const char * chargerSkins_strerror(chargerSkins_result_t res) {
	switch (res) {
		case CHARGERSKINS_OK: return "Succes";
		case CHARGERSKINS_ERR_CREATE_ARRAYLIST: return "Echec creation arraylist des skins";
		case CHARGERSKINS_ERR_CREATE_SKIN: return "Echec creation du skin";
		default: return "Erreur inconnue";
	}
}