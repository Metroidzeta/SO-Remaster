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

#include "headers/chargerMusiques.h"
#include "headers/musique.h"

static const char * MUSIQUES_LIST[] = { // sera remplacé par une détection des fichiers présents dans le dossier "musiques" à l'avenir
	// Création des musiques { nomFichier }
	"Castle_1.ogg",                        // 0
	"Sarosa.ogg",                          // 1
	"bahamut_lagoon.ogg",                  // 2
	"Castle_3.ogg",                        // 3
	"2000_ordeal.ogg",                     // 4
	"cc_viper_manor.ogg",                  // 5
	"suikoden-ii-two-rivers.ogg",          // 6
	"mystery3.ogg",                        // 7
	"hunter.ogg",                          // 8
	"illusionary_world.ogg",               // 9
	"chapt1medfill.ogg"                    // 10
};

static chargerMusiques_result_t ajouterMusique(const char *nomFichier, arraylist_t *musiques) {
	musique_result_t mus_res;
	musique_t *musique = musique_creer(nomFichier, &mus_res);
	if (!musique) { LOG_ERROR("%s (src: %s)", musique_strerror(mus_res), nomFichier); return CHARGERMUSIQUES_ERR_CREATE_MUSIQUE; }

	arraylist_add(musiques, musique);
	return CHARGERMUSIQUES_OK;
}

arraylist_t * chargerMusiques_get(chargerMusiques_result_t *res) {
	if (!res) { LOG_ERROR("Enum chargerMusiques_result NULL"); return NULL; }
	*res = CHARGERMUSIQUES_OK;

	arraylist_result_t al_res;
	arraylist_t *musiques = arraylist_creer(AL_MUSIQUE, &al_res);
	if (!musiques) { LOG_ERROR("%s", arraylist_strerror(al_res)); *res = CHARGERMUSIQUES_ERR_CREATE_ARRAYLIST; return NULL; }

	const size_t nbMusiques = sizeof(MUSIQUES_LIST) / sizeof(MUSIQUES_LIST[0]);
	for (size_t i = 0; i < nbMusiques; ++i) {
		chargerMusiques_result_t chMus_res = ajouterMusique(MUSIQUES_LIST[i], musiques);
		if (chMus_res) { arraylist_detruire(musiques, true); *res = chMus_res; return NULL; }
	}
	return musiques;
}

const char * chargerMusiques_strerror(chargerMusiques_result_t res) {
	switch (res) {
		case CHARGERMUSIQUES_OK: return "Succes";
		case CHARGERMUSIQUES_ERR_CREATE_ARRAYLIST: return "Echec creation arraylist des musiques";
		case CHARGERMUSIQUES_ERR_CREATE_MUSIQUE: return "Echec creation de la musique";
		default: return "Erreur inconnue";
	}
}