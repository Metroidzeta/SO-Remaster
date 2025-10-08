// @author Alain Barbier alias "Metroidzeta"

#include "headers/chargerBruitages.h"

static const char * tabBruitages[] = { // sera remplacé par une détection des fichiers présents dans le dossier "bruitages" à l'avenir
	// Création des bruitages { nomFichier }
	"Blow1.wav",                            // 0
	"Kill1.wav",                            // 1
	"Damage3.ogg"                           // 2
};

static chargerBruitages_result_t ajouterBruitage(const char *nomFichier, arraylist_t **bruitages) {
	bruitage_result_t res;
	bruitage_t *bruitage = bruitage_creer(nomFichier, &res);
	if (!bruitage) { LOG_ERROR("Bruitage : %s (fichier : %s)", bruitage_strerror(res), nomFichier); return CHARGERBRUITAGES_ERR_CREATE_BRUITAGE; }
	arraylist_add(*bruitages, bruitage);
	return CHARGERBRUITAGES_OK;
}

chargerBruitages_result_t chargerBruitages_get(arraylist_t **bruitages) {
	if (!bruitages) return CHARGERBRUITAGES_ERR_NULL_POINTER;
	arraylist_result_t resAL;
	*bruitages = arraylist_creer(AL_BRUITAGE, &resAL);
	if (!*bruitages) { LOG_ERROR("Arraylist bruitages : %s", arraylist_strerror(resAL)); return CHARGERBRUITAGES_ERR_CREATE_ARRAYLIST; }

	const size_t nbBruitages = sizeof(tabBruitages) / sizeof(tabBruitages[0]);
	for (size_t i = 0; i < nbBruitages; ++i) {
		chargerBruitages_result_t resBR = ajouterBruitage(tabBruitages[i], bruitages);
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
		default: return "Erreur inconnue";
	}
}