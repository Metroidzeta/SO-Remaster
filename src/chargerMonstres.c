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

#include "headers/chargerMonstres.h"

typedef struct {
	const char *nomCarte;
	const char *nomMonstreData;
	int xCase;
	int yCase;
} monstre_info_t;

static const monstre_info_t MONSTRES_LIST[] = { // sera remplacé plus tard par la lecture réelle de fichiers (JSON dans le futur)
	// Création des monstres { nomCarte, nomMonstreData, xCase, yCase }
	{ "Sarosa_Foret_Est", "Blob Bleu", 9, 23 },
	{ "Sarosa_Foret_Est", "Blob Bleu", 34, 13 },
	{ "Sarosa_Foret_Est", "Lapin", 33, 24 },

	{ "Arene_Hunter", "Blob Bleu", 9, 9 },
	{ "Arene_Hunter", "Lapin", 10, 9 },
	{ "Arene_Hunter", "Blob Bleu", 9, 10 },
	{ "Arene_Hunter", "Lapin", 10, 10 }
};

static chargerMonstres_result_t ajouterMonstre(const monstre_info_t *elem, jeu_t *jeu) {
	carte_t *carte = getCarte2(jeu, elem->nomCarte);
	if (!carte) { LOG_ERROR("Nom carte: %s", elem->nomCarte); return CHARGERMONSTRES_ERR_GET_CARTE; }

	monstreData_t *data = getMonstreData2(jeu, elem->nomMonstreData);
	if (!data) { LOG_ERROR("Nom monstreData: %s", elem->nomMonstreData); return CHARGERMONSTRES_ERR_GET_MONSTREDATA; }

	monstre_result_t ms_res;
	monstre_t *monstre =  monstre_creer(data, elem->xCase, elem->yCase, &ms_res);
	if (!monstre) { LOG_ERROR("%s (nom: %s)", monstre_strerror(ms_res), elem->nomMonstreData); return CHARGERMONSTRES_ERR_CREATE_MONSTRE; }

	carte_ajouterMonstre(carte, monstre);
	return CHARGERMONSTRES_OK;
}

chargerMonstres_result_t chargerMonstres_inject(jeu_t *jeu) {
	if (!jeu) return CHARGERMONSTRES_ERR_NULL_POINTER_JEU;

	const size_t nbMonstres = sizeof(MONSTRES_LIST) / sizeof(MONSTRES_LIST[0]);
	for (size_t i = 0; i < nbMonstres; ++i) {
		const monstre_info_t *elem = &MONSTRES_LIST[i];
		chargerMonstres_result_t chMs_res = ajouterMonstre(elem, jeu);
		if (chMs_res) return chMs_res;
	}
	return CHARGERMONSTRES_OK;
}

const char * chargerMonstres_strerror(chargerMonstres_result_t res) {
	switch (res) {
		case CHARGERMONSTRES_OK: return "Succes";
		case CHARGERMONSTRES_ERR_NULL_POINTER_JEU: return "Jeu NULL passe en parametre";
		case CHARGERMONSTRES_ERR_GET_CARTE: return "Echec obtention carte (introuvable dans le jeu)";
		case CHARGERMONSTRES_ERR_GET_MONSTREDATA: return "Echec obtention du monstreData (introuvable dans le jeu)";
		case CHARGERMONSTRES_ERR_CREATE_MONSTRE: return "Echec creation du monstre";
		default: return "Erreur inconnue";
	}
}