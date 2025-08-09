// @author Alain Barbier alias "Metroidzeta"

#include "headers/chargerBruitages.h"

static const bruitage_info_t tabBruitages[] = { // sera remplacé plus tard par la lecture réelle de fichiers (JSON dans le futur)
	// Création des bruitages { nomFichier }
	{ "Blow1.wav" },                        // 0
	{ "Kill1.wav" },                        // 1
	{ "Damage3.ogg" }                       // 2
};

static chargerBruitages_result_t ajouterBruitage(const bruitage_info_t *elem, arraylist_t **bruitages) {
	bruitage_t *bruitage = NULL;
	bruitage_result_t res = bruitage_creer(&bruitage, elem->nomFichier);
	if (res != BRUITAGE_OK) { LOG_ERROR("Bruitage : %s (fichier : %s)", bruitage_strerror(res), elem->nomFichier); return CHARGERBRUITAGES_ERR_CREATE_BRUITAGE; }
	arraylist_add(*bruitages, bruitage);
	return CHARGERBRUITAGES_OK;
}

chargerBruitages_result_t chargerBruitages_get(arraylist_t **bruitages) {
	if (!bruitages) return CHARGERBRUITAGES_ERR_NULL_POINTER;

	arraylist_result_t resAL = arraylist_creer(bruitages, AL_BRUITAGE);
	if (resAL != ARRAYLIST_OK) { LOG_ERROR("Arraylist bruitages : %s", arraylist_strerror(resAL)); return CHARGERBRUITAGES_ERR_CREATE_ARRAYLIST; }

	const size_t nbBruitages = sizeof(tabBruitages) / sizeof(tabBruitages[0]);
	for (size_t i = 0; i < nbBruitages; ++i) {
		const bruitage_info_t *elem = &tabBruitages[i];
		chargerBruitages_result_t resBR = ajouterBruitage(elem, bruitages);
		if (resBR != CHARGERBRUITAGES_OK) return resBR;
	}
	return CHARGERBRUITAGES_OK;
}

const char * chargerBruitages_strerror(chargerBruitages_result_t res) {
	switch (res) {
		case CHARGERBRUITAGES_OK: return "Succes";
		case CHARGERBRUITAGES_ERR_NULL_POINTER: return "Pointeur sur arraylist bruitages NULL passe en parametre";
		case CHARGERBRUITAGES_ERR_CREATE_ARRAYLIST: return "Echec creation arraylist bruitages";
		case CHARGERBRUITAGES_ERR_CREATE_BRUITAGE: return "Echec creation bruitage";
		default: return "Erreur";
	}
}