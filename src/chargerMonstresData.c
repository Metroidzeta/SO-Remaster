// @author Alain Barbier alias "Metroidzeta"

#include "headers/chargerMonstresData.h"

static const monstreData_info_t tabMonstresData[] = { // sera remplacé plus tard par la lecture réelle de fichiers (JSON dans le futur)
	// Création des monstresData { nomFichier, nom, PVMax, XP, piecesOr }
	{ "blob_bleu.png", "Blob Bleu", 432, 33, 9 },                        // 0 (Blob bleu)
	{ "lapin.png", "Lapin", 609, 163, 14 }                               // 1 (Lapin)
};

static chargerMonstresData_result_t ajouterMonstreData(SDL_Renderer *renderer, const monstreData_info_t *elem, arraylist_t **monstresData) {
	monstreData_t *monstreData = NULL;
	monstreData_result_t res = monstreData_creer(&monstreData, renderer, elem->nomFichier, elem->nom, elem->PVMax, elem->xp, elem->piecesOr);
	if (res != MONSTREDATA_OK) { LOG_ERROR("MonstreData : %s (fichier : %s)", monstreData_strerror(res), elem->nomFichier); return CHARGERMONSTRESDATA_ERR_CREATE_MONSTREDATA; }
	arraylist_add(*monstresData, monstreData);
	return CHARGERMONSTRESDATA_OK;
}

chargerMonstresData_result_t chargerMonstresData_get(SDL_Renderer *renderer, arraylist_t **monstresData) {
	if (!monstresData) return CHARGERMONSTRESDATA_ERR_NULL_POINTER;

	arraylist_result_t resAL = arraylist_creer(monstresData, AL_MONSTRE_DATA);
	if (resAL != ARRAYLIST_OK) { LOG_ERROR("Arraylist monstresData : %s", arraylist_strerror(resAL)); return CHARGERMONSTRESDATA_ERR_CREATE_ARRAYLIST; }

	const size_t nbMonstresData = sizeof(tabMonstresData) / sizeof(tabMonstresData[0]);
	for (size_t i = 0; i < nbMonstresData; ++i) {
		const monstreData_info_t *elem = &tabMonstresData[i];
		chargerMonstresData_result_t resCMD = ajouterMonstreData(renderer, elem, monstresData);
		if (resCMD != CHARGERMONSTRESDATA_OK) return resCMD;
	}
	return CHARGERMONSTRESDATA_OK;
}

const char * chargerMonstresData_strerror(chargerMonstresData_result_t res) {
	switch (res) {
		case CHARGERMONSTRESDATA_OK: return "Succes";
		case CHARGERMONSTRESDATA_ERR_NULL_POINTER: return "Pointeur sur arraylist monstresData NULL passe en parametre";
		case CHARGERMONSTRESDATA_ERR_CREATE_ARRAYLIST: return "Echec creation arraylist monstresData";
		case CHARGERMONSTRESDATA_ERR_CREATE_MONSTREDATA: return "Echec creation monstreData";
		default: return "Erreur inconnue";
	}
}