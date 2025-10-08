// @author Alain Barbier alias "Metroidzeta"

#include "headers/chargerMonstres.h"

static const monstre_info_t tabMonstres[] = { // sera remplacé plus tard par la lecture réelle de fichiers (JSON dans le futur)
	// Création des monstres { nomCarte, nomMonstreData, xCase, yCase }
	{ "Sarosa_Foret_Est", "Blob Bleu", 9, 23 },
	{ "Sarosa_Foret_Est", "Blob Bleu", 34, 13 },
	{ "Sarosa_Foret_Est", "Lapin", 33, 24 },

	{ "Arene_Hunter", "Blob Bleu", 9, 9 },
	{ "Arene_Hunter", "Lapin", 10, 9 },
	{ "Arene_Hunter", "Blob Bleu", 9, 10 },
	{ "Arene_Hunter", "Lapin", 10, 10 }
};

static carte_t * getCarte(arraylist_t *cartes, const char *nom) {
	for (int i = 0; i < cartes->taille; ++i) {
		carte_t *carte = arraylist_get(cartes, i);
		if (strcmp(carte->nom, nom) == 0) return carte;
	}
	return NULL;
}

static monstreData_t * getMonstreData(arraylist_t *monstresData, const char *nom) {
	for (int i = 0; i < monstresData->taille; ++i) {
		monstreData_t *monstreData = arraylist_get(monstresData, i);
		if (strcmp(monstreData->nom, nom) == 0) return monstreData;
	}
	return NULL;
}

static chargerMonstres_result_t ajouterMonstre(const monstre_info_t *elem, arraylist_t *cartes, arraylist_t *monstresData) {
	carte_t *carte = NULL;
	if (elem->nomCarte != NULL) { // une carte NULL = refusé -> à l'ajout du monstre dans la carte
		carte = getCarte(cartes, elem->nomCarte);
		if (!carte) { LOG_ERROR("Monstre, carte inexistante : %s", elem->nomCarte); return CHARGERMONSTRES_ERR_GET_CARTE; }
	}

	monstreData_t *data = NULL;
	if (elem->nomMonstreData != NULL) { // une data NULL = refusé -> à la création du monstre
		data = getMonstreData(monstresData, elem->nomMonstreData);
		if (!data) { LOG_ERROR("Monstre, data monstre inexistante : %s", elem->nomMonstreData); return CHARGERMONSTRES_ERR_GET_MONSTREDATA; }
	}

	monstre_result_t res;
	monstre_t * monstre =  monstre_creer(data, elem->xCase, elem->yCase, &res);
	if (!monstre) { LOG_ERROR("Echec creation monstre (nomData : %s) : %s ", elem->nomMonstreData, monstre_strerror(res)); return CHARGERMONSTRES_ERR_CREATE_MONSTRE; }

	carte_ajouterMonstre(carte, monstre);
	return CHARGERMONSTRES_OK;
}

chargerMonstres_result_t chargerMonstres_get(arraylist_t *cartes, arraylist_t *monstresData) {
	if (!cartes) return CHARGERMONSTRES_ERR_NULL_POINTER_CARTES;
	if (!monstresData) return CHARGERMONSTRES_ERR_NULL_POINTER_MONSTRESDATA;

	const size_t nbMonstres = sizeof(tabMonstres) / sizeof(tabMonstres[0]);
	for (size_t i = 0; i < nbMonstres; ++i) {
		const monstre_info_t *elem = &tabMonstres[i];
		chargerMonstres_result_t resCMO = ajouterMonstre(elem, cartes, monstresData);
		if (resCMO != CHARGERMONSTRES_OK) return resCMO;
	}
	return CHARGERMONSTRES_OK;
}

const char * chargerMonstres_strerror(chargerMonstres_result_t res) {
	switch (res) {
		case CHARGERMONSTRES_OK: return "Succes";
		case CHARGERMONSTRES_ERR_NULL_POINTER_CARTES: return "Arraylist cartes NULL passe en paramatre";
		case CHARGERMONSTRES_ERR_NULL_POINTER_MONSTRESDATA: return "Arraylist monstresData NULL passe en parametre";
		case CHARGERMONSTRES_ERR_GET_CARTE: return "Echec obtention carte";
		case CHARGERMONSTRES_ERR_GET_MONSTREDATA: return "Echec obtention monstreData";
		case CHARGERMONSTRES_ERR_CREATE_MONSTRE: return "Echec creation monstre";
		default: return "Erreur inconnue";
	}
}