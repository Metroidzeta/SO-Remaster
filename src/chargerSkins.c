// @author Alain Barbier alias "Metroidzeta"

#include "headers/chargerSkins.h"

static const skin_info_t tabSkins[] = { // sera remplacé plus tard par la lecture réelle de fichiers
	// Création des skins { nomFichier }
	{ "Evil.png" } // 0
};

static chargerSkins_result_t ajouterSkin(SDL_Renderer *renderer, const char *nomFichier, arraylist_t **skins) {
	skin_t *skin = NULL;
	skin_result_t res = skin_creer(&skin, renderer, nomFichier);
	if (res != SKIN_OK) { LOG_ERROR("Skin : %s (fichier : %s)", skin_strerror(res), nomFichier); return CHARGERSKINS_ERR_CREATE_SKIN; }
	arraylist_add(*skins, skin);
	return CHARGERSKINS_OK;
}

chargerSkins_result_t chargerSkins_get(SDL_Renderer *renderer, arraylist_t **skins) {
	arraylist_result_t resAL = arraylist_creer(skins, AL_SKIN);
	if (resAL != ARRAYLIST_OK) { LOG_ERROR("Arraylist skins : %s", arraylist_strerror(resAL)); return CHARGERSKINS_ERR_CREATE_ARRAYLIST; }

	const size_t nbFichiers = sizeof(tabSkins) / sizeof(tabSkins[0]);
	for (size_t i = 0; i < nbFichiers; ++i) {
		const skin_info_t *elem = &tabSkins[i];
		chargerSkins_result_t resSK = ajouterSkin(renderer, elem->nomFichier, skins);
		if (resSK != CHARGERSKINS_OK) return resSK;
	}

	return CHARGERSKINS_OK;
}

const char * chargerSkins_strerror(chargerSkins_result_t res) {
	switch (res) {
		case CHARGERSKINS_OK: return "Succes";
		case CHARGERSKINS_ERR_CREATE_ARRAYLIST: return "Echec creation arraylist skins";
		case CHARGERSKINS_ERR_CREATE_SKIN: return "Echec creation skin";
		default: return "Erreur";
	}
}