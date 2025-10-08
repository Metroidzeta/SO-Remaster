// @author Alain Barbier alias "Metroidzeta"

#include "headers/chargerMusiques.h"

static const char * tabMusiques[] = { // sera remplacé par une détection des fichiers présents dans le dossier "musiques" à l'avenir
	// Création des musiques { nomFichier }
	"Castle_1.mp3",                        // 0
	"Sarosa.mp3",                          // 1
	"bahamut_lagoon.mp3",                  // 2
	"Castle_3.mp3",                        // 3
	"2000_ordeal.mp3",                     // 4
	"cc_viper_manor.mp3",                  // 5
	"suikoden-ii-two-rivers.mp3",          // 6
	"mystery3.ogg",                        // 7
	"hunter.ogg",                          // 8
	"illusionary_world.mp3",               // 9
	"chapt1medfill.mp3"                    // 10
};

static chargerMusiques_result_t ajouterMusique(const char *nomFichier, arraylist_t **musiques) {
	musique_result_t res;
	musique_t *musique = musique_creer(nomFichier, &res);
	if (!musique) { LOG_ERROR("Musique : %s (fichier : %s)", musique_strerror(res), nomFichier); return CHARGERMUSIQUES_ERR_CREATE_MUSIQUE; }
	arraylist_add(*musiques, musique);
	return CHARGERMUSIQUES_OK;
}

chargerMusiques_result_t chargerMusiques_get(arraylist_t **musiques) {
	if (!musiques) return CHARGERMUSIQUES_ERR_NULL_POINTER;
	arraylist_result_t resAL;
	*musiques = arraylist_creer(AL_MUSIQUE, &resAL);
	if (!*musiques) { LOG_ERROR("Arraylist musiques : %s", arraylist_strerror(resAL)); return CHARGERMUSIQUES_ERR_CREATE_ARRAYLIST; }

	const size_t nbMusiques = sizeof(tabMusiques) / sizeof(tabMusiques[0]);
	for (size_t i = 0; i < nbMusiques; ++i) {
		chargerMusiques_result_t resCMU = ajouterMusique(tabMusiques[i], musiques);
		if (resCMU != CHARGERMUSIQUES_OK) return resCMU;
	}
	return CHARGERMUSIQUES_OK;
}

const char * chargerMusiques_strerror(chargerMusiques_result_t res) {
	switch (res) {
		case CHARGERMUSIQUES_OK: return "Succes";
		case CHARGERMUSIQUES_ERR_NULL_POINTER: return "Pointeur sur arraylist musiques NULL passe en parametre";
		case CHARGERMUSIQUES_ERR_CREATE_ARRAYLIST: return "Echec creation arraylist musiques";
		case CHARGERMUSIQUES_ERR_CREATE_MUSIQUE: return "Echec creation musique";
		default: return "Erreur inconnue";
	}
}