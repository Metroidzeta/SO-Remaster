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

#include "headers/chargerPolices.h"

typedef struct {
	const char *nomFichier;
	int taille;
} policeData_t;

static const policeData_t POLICES_LIST[] = { // sera remplacé plus tard par la lecture réelle de fichiers (JSON dans le futur)
	// Création des polices { nomFichier, taille }
	{ "arial.ttf", TAILLE_CASES * 0.34 },                    // 0 (FPS)
	{ "arial.ttf", TAILLE_CASES * 0.68 },                    // 1 (Texte de base)
	{ "Zelda Breath of the Wild.otf", TAILLE_CASES * 0.95 }  // 2 (Coups d'attaques)
};

static chargerPolices_result_t ajouterPolice(const policeData_t *elem, arraylist_t *polices) {
	if (!elem) return CHARGERPOLICES_ERR_NULL_POINTER_ELEM;
	TTF_Font *police = creerPolice(elem->nomFichier, elem->taille);
	if (!police) { LOG_ERROR("%s (src: %s)", TTF_GetError(), elem->nomFichier); return CHARGERPOLICES_ERR_CREATE_POLICE; }

	arraylist_add(polices, police);
	return CHARGERPOLICES_OK;
}

arraylist_t * chargerPolices_get(chargerPolices_result_t *res) {
	if (!res) { LOG_ERROR("Enum chargerPolices_result NULL"); return NULL; }
	*res = CHARGERPOLICES_OK;

	arraylist_result_t al_res;
	arraylist_t *polices = arraylist_creer(AL_POLICE, &al_res);
	if (!polices) { LOG_ERROR("%s", arraylist_strerror(al_res)); *res = CHARGERPOLICES_ERR_CREATE_ARRAYLIST; return NULL; }

	const size_t nbPolices = sizeof(POLICES_LIST) / sizeof(POLICES_LIST[0]);
	for (size_t i = 0; i < nbPolices; ++i) {
		const policeData_t *elem = &POLICES_LIST[i];
		chargerPolices_result_t chPoli_res = ajouterPolice(elem, polices);
		if (chPoli_res) { arraylist_detruire(polices, true); *res = chPoli_res; return NULL; }
	}
	return polices;
}

const char * chargerPolices_strerror(chargerPolices_result_t res) {
	switch (res) {
		case CHARGERPOLICES_OK: return "Succes";
		case CHARGERPOLICES_ERR_CREATE_ARRAYLIST: return "Echec creation arraylist des polices";
		case CHARGERPOLICES_ERR_NULL_POINTER_ELEM: return "Pointeur sur l'elem de la POLICES_LIST null";
		case CHARGERPOLICES_ERR_CREATE_POLICE: return "Echec creation de la police";
		default: return "Erreur inconnue";
	}
}