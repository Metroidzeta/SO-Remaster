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

#include "headers/chargerMonstresData.h"
#include "headers/monstreData.h"

typedef struct {
	const char *nomFichier;
	const char *nom;
	int PVMax;
	int xp;
	int piecesOr;
} monstreData_info_t;

static const monstreData_info_t MONSTRESDATA_LIST[] = { // sera remplacé plus tard par la lecture réelle de fichiers (JSON dans le futur)
	// Création des monstresData { nomFichier, nom, PVMax, XP, piecesOr }
	{ "blob_bleu.png", "Blob Bleu", 432, 33, 9 },                        // 0 (Blob bleu)
	{ "lapin.png", "Lapin", 609, 163, 14 }                               // 1 (Lapin)
};

static chargerMonstresData_result_t ajouterMonstreData(SDL_Renderer *renderer, const monstreData_info_t *elem, arraylist_t *monstresData) {
	monstreData_result_t monstDat_res;
	monstreData_t *monstreData = monstreData_creer(renderer, elem->nomFichier, elem->nom, elem->PVMax, elem->xp, elem->piecesOr, &monstDat_res);
	if (!monstreData) { LOG_ERROR("%s (src: %s)", monstreData_strerror(monstDat_res), elem->nomFichier); return CHARGERMONSTRESDATA_ERR_CREATE_MONSTREDATA; }

	arraylist_add(monstresData, monstreData);
	return CHARGERMONSTRESDATA_OK;
}

arraylist_t * chargerMonstresData_get(SDL_Renderer *renderer, chargerMonstresData_result_t *res) {
	if (!res) { LOG_ERROR("Enum chargerMonstresData_result NULL"); return NULL; }
	*res = CHARGERMONSTRESDATA_OK;

	arraylist_result_t al_res;
	arraylist_t *monstresData = arraylist_creer(AL_MONSTRE_DATA, &al_res);
	if (!monstresData) { LOG_ERROR("%s", arraylist_strerror(al_res)); *res = CHARGERMONSTRESDATA_ERR_CREATE_ARRAYLIST; return NULL; }

	const size_t nbMonstresData = sizeof(MONSTRESDATA_LIST) / sizeof(MONSTRESDATA_LIST[0]);
	for (size_t i = 0; i < nbMonstresData; ++i) {
		const monstreData_info_t *elem = &MONSTRESDATA_LIST[i];
		chargerMonstresData_result_t chMonstDat_res = ajouterMonstreData(renderer, elem, monstresData);
		if (chMonstDat_res) { arraylist_detruire(monstresData, true); *res = chMonstDat_res; return NULL; }
	}
	return monstresData;
}

const char * chargerMonstresData_strerror(chargerMonstresData_result_t res) {
	switch (res) {
		case CHARGERMONSTRESDATA_OK: return "Succes";
		case CHARGERMONSTRESDATA_ERR_CREATE_ARRAYLIST: return "Echec creation arraylist des monstresData";
		case CHARGERMONSTRESDATA_ERR_CREATE_MONSTREDATA: return "Echec creation du monstreData";
		default: return "Erreur inconnue";
	}
}