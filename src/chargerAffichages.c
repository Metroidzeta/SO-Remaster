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

#include "headers/chargerAffichages.h"

static const char * AFFICHAGES_LIST[] = { // sera remplacé par une détection des fichiers présents dans le dossier "img/affichages" à l'avenir
	// Création des affichages { nomFichier }
	"fioles.png",                            // 0 (fioles)
	"xp.png"                                 // 1 (barre XP)
};

static chargerAffichages_result_t ajouterAffichage(SDL_Renderer *renderer, const char *nomFichier, arraylist_t *affichages) {
	SDL_Texture *texture = creerTexture(renderer, nomFichier);
	if (!texture) { LOG_ERROR("Echec creerTexture: %s (src: %s)", IMG_GetError(), nomFichier); return CHARGERAFFICHAGES_ERR_CREATE_AFFICHAGE; }

	arraylist_add(affichages, texture);
	return CHARGERAFFICHAGES_OK;
}

arraylist_t * chargerAffichages_get(SDL_Renderer *renderer, chargerAffichages_result_t *res) {
	if (!res) { LOG_ERROR("Enum chargerAffichages_result NULL"); return NULL; }
	*res = CHARGERAFFICHAGES_OK;

	arraylist_result_t al_res;
	arraylist_t *affichages = arraylist_creer(AL_TEXTURE, &al_res);
	if (!affichages) { LOG_ERROR("%s", arraylist_strerror(al_res)); *res = CHARGERAFFICHAGES_ERR_CREATE_ARRAYLIST; return NULL; }

	const size_t nbAffichages = sizeof(AFFICHAGES_LIST) / sizeof(AFFICHAGES_LIST[0]);
	for (size_t i = 0; i < nbAffichages; ++i) {
		chargerAffichages_result_t chAffich_res = ajouterAffichage(renderer, AFFICHAGES_LIST[i], affichages);
		if (chAffich_res) { arraylist_detruire(affichages, true); *res = chAffich_res; return NULL; }
	}
	return affichages;
}

const char * chargerAffichages_strerror(chargerAffichages_result_t res) {
	switch (res) {
		case CHARGERAFFICHAGES_OK: return "Succes";
		case CHARGERAFFICHAGES_ERR_CREATE_ARRAYLIST: return "Echec creation arraylist des affichages";
		case CHARGERAFFICHAGES_ERR_CREATE_AFFICHAGE: return "Echec creation de l'affichage";
		default: return "Erreur inconnue";
	}
}