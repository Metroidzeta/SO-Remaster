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

#include "headers/chargerChipsets.h"
#include "headers/chipset.h"

typedef struct {
	const char *nomFichier;
	int tailleTuile;
} chipsetData_t;

static const chipsetData_t CHIPSETS_LIST[] = { // sera remplacé plus tard par la lecture réelle de fichiers (JSON dans le futur)
	// Création des chipsets { nomFichier, tailleTuile [par défaut : 16] }
	{ "BZ.png", 16 },                       // 0
	{ "VillageTangaFinal.png", 16 },        // 1
	{ "grey_cas42.png", 16 },               // 2
	{ "PalaisRoland2.png", 16 },            // 3
	{ "PalaisRolandInt.png", 16 },          // 4
	{ "PalaisRolandNouveau.png", 48 },      // 5
	{ "MaraisTanga.png", 16 },              // 6
	{ "marais2.png", 16 },                  // 7
	{ "Coacville_exterieur.png", 16 },      // 8
	{ "chipset173.png", 16 },               // 9
	{ "chipset175.png", 16 },               // 10
	{ "HunterArene.png", 16 },              // 11
	{ "grass.png", 32 },                    // 12
	{ "chipset5c.png", 16 }                 // 13
};

static chargerChipsets_result_t ajouterChipset(SDL_Renderer *renderer, const chipsetData_t *elem, arraylist_t *chipsets) {
	chipset_result_t chipt_res;
	chipset_t *chipset = chipset_creer(renderer, elem->nomFichier, elem->tailleTuile, &chipt_res);
	if (!chipset) { LOG_ERROR("%s", chipset_strerror(chipt_res)); return CHARGERCHIPSETS_ERR_CREATE_CHIPSET; }

	arraylist_add(chipsets, chipset);
	return CHARGERCHIPSETS_OK;
}

arraylist_t * chargerChipsets_get(SDL_Renderer *renderer, chargerChipsets_result_t *res) {
	if (!res) { LOG_ERROR("Enum chargerChipsets_result NULL"); return NULL; }
	*res = CHARGERCHIPSETS_OK;

	arraylist_result_t al_res;
	arraylist_t *chipsets = arraylist_creer(AL_CHIPSET, &al_res);
	if (!chipsets) { LOG_ERROR("%s", arraylist_strerror(al_res)); *res = CHARGERCHIPSETS_ERR_CREATE_ARRAYLIST; return NULL; }

	const size_t nbChipsets = sizeof(CHIPSETS_LIST) / sizeof(CHIPSETS_LIST[0]);
	for (size_t i = 0; i < nbChipsets; ++i) {
		const chipsetData_t *elem = &CHIPSETS_LIST[i];
		chargerChipsets_result_t chChipt_res = ajouterChipset(renderer, elem, chipsets);
		if (chChipt_res) { arraylist_detruire(chipsets, true); *res = chChipt_res; return NULL; }
	}
	return chipsets;
}

const char * chargerChipsets_strerror(chargerChipsets_result_t res) {
	switch (res) {
		case CHARGERCHIPSETS_OK: return "Succes";
		case CHARGERCHIPSETS_ERR_CREATE_ARRAYLIST: return "Echec creation arraylist des chipsets";
		case CHARGERCHIPSETS_ERR_CREATE_CHIPSET: return "Echec creation du chipset";
		default: return "Erreur inconnue";
	}
}