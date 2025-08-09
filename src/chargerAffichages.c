// @author Alain Barbier alias "Metroidzeta"

#include "headers/chargerAffichages.h"

static const affichage_info_t tabAffichages[] = { // sera remplacé plus tard par la lecture réelle de fichiers (JSON dans le futur)
	// Création des affichages { nomFichier }
	{ "fioles.png" },                        // 0 (fioles)
	{ "xp.png" }                             // 1 (barre XP)
};

static chargerAffichages_result_t ajouterAffichage(SDL_Renderer *renderer, const affichage_info_t *elem, arraylist_t **affichages) {
	SDL_Texture *texture = creerImage(renderer, elem->nomFichier);
	if (!texture) { LOG_ERROR("Affichage : %s (fichier : %s)", IMG_GetError(), elem->nomFichier); return CHARGERAFFICHAGES_ERR_CREATE_AFFICHAGE; }
	arraylist_add(*affichages, texture);
	return CHARGERAFFICHAGES_OK;
}

chargerAffichages_result_t chargerAffichages_get(SDL_Renderer *renderer, arraylist_t **affichages) {
	if (!affichages) return CHARGERAFFICAHGES_ERR_NULL_POINTER;

	arraylist_result_t resAL = arraylist_creer(affichages, AL_TEXTURE);
	if (resAL != ARRAYLIST_OK) { LOG_ERROR("Arraylist affichages : %s", arraylist_strerror(resAL)); return CHARGERAFFICHAGES_ERR_CREATE_ARRAYLIST; }

	const size_t nbAffichages = sizeof(tabAffichages) / sizeof(tabAffichages[0]);
	for (size_t i = 0; i < nbAffichages; ++i) {
		const affichage_info_t *elem = &tabAffichages[i];
		chargerAffichages_result_t resAF = ajouterAffichage(renderer, elem, affichages);
		if (resAF != CHARGERAFFICHAGES_OK) return resAF;
	}
	return CHARGERAFFICHAGES_OK;
}

const char * chargerAffichages_strerror(chargerAffichages_result_t res) {
	switch (res) {
		case CHARGERAFFICHAGES_OK: return "Succes";
		case CHARGERAFFICAHGES_ERR_NULL_POINTER: return "Pointeur sur arraylist affichages NULL passe en parametre";
		case CHARGERAFFICHAGES_ERR_CREATE_ARRAYLIST: return "Echec creation arraylist affichages";
		case CHARGERAFFICHAGES_ERR_CREATE_AFFICHAGE: return "Echec creation affichage";
		default: return "Erreur";
	}
}