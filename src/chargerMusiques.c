// @author Alain Barbier alias "Metroidzeta"

#include "headers/chargerMusiques.h"

static const musique_info_t tabMusiques[] = { // sera remplacé plus tard par la lecture réelle de fichiers (JSON dans le futur)
	// Création des musiques { nomFichier }
	{ "Castle_1.mp3" },                    // 0
	{ "Sarosa.mp3" },                      // 1
	{ "bahamut_lagoon.mp3" },              // 2
	{ "Castle_3.mp3" },                    // 3
	{ "2000_ordeal.mp3" },                 // 4
	{ "cc_viper_manor.mp3" },              // 5
	{ "suikoden-ii-two-rivers.mp3" },      // 6
	{ "mystery3.ogg" },                    // 7
	{ "hunter.ogg" },                      // 8
	{ "illusionary_world.mp3" },           // 9
	{ "chapt1medfill.mp3" }                // 10
};

static chargerMusiques_result_t ajouterMusique(const musique_info_t *elem, arraylist_t **musiques) {
	musique_t *musique = NULL;
	musique_result_t res = musique_creer(&musique, elem->nomFichier);
	if (res != MUSIQUE_OK) { LOG_ERROR("Musique : %s (fichier : %s)", musique_strerror(res), elem->nomFichier); return CHARGERMUSIQUES_ERR_CREATE_MUSIQUE; }
	arraylist_add(*musiques, musique);
	return CHARGERMUSIQUES_OK;
}

chargerMusiques_result_t chargerMusiques_get(arraylist_t **musiques) {
	if (!musiques) return CHARGERMUSIQUES_ERR_NULL_POINTER;

	arraylist_result_t resAL = arraylist_creer(musiques, AL_MUSIQUE);
	if (resAL != ARRAYLIST_OK) { LOG_ERROR("Arraylist musiques : %s", arraylist_strerror(resAL)); return CHARGERMUSIQUES_ERR_CREATE_ARRAYLIST; }

	const size_t nbMusiques = sizeof(tabMusiques) / sizeof(tabMusiques[0]);
	for (size_t i = 0; i < nbMusiques; ++i) {
		const musique_info_t *elem = &tabMusiques[i];
		chargerMusiques_result_t resCMU = ajouterMusique(elem, musiques);
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
		default: return "Erreur";
	}
}