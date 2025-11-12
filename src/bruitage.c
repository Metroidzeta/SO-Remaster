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

#include "headers/bruitage.h"

static inline bruitage_result_t bruitage_validerArguments(const char *nomFichier) {
	if (!nomFichier || !*nomFichier) return BRUITAGE_ERR_NULL_OR_EMPTY_FILENAME;
	if (strlen(nomFichier) >= MAX_TAILLE_STRING) return BRUITAGE_ERR_SIZE_MAX_FILENAME;
	return BRUITAGE_OK;
}

static bruitage_result_t bruitage_chargerSon(bruitage_t *bruitage, const char *nomFichier) {
	bruitage->son = creerSon(nomFichier);
	if (!bruitage->son) { LOG_ERROR("Echec creerSon %s (src: %s)", Mix_GetError(), nomFichier); return BRUITAGE_ERR_LOAD_SOUND; }
	return BRUITAGE_OK;
}

bruitage_t * bruitage_creer(const char *nomFichier, bruitage_result_t *res) {
	if (!res) { LOG_ERROR("Enum bruitage_result NULL"); return NULL; }
	*res = BRUITAGE_OK;
	if ((*res = bruitage_validerArguments(nomFichier))) return NULL;

	bruitage_t *bruitage = calloc(1, sizeof(bruitage_t));
	if (!bruitage) { *res = BRUITAGE_ERR_MEMORY_BASE; return NULL; }

	bruitage->nom = my_strdup(nomFichier); // important : ne pas faire "bruitage->nom = nomFichier", car cela ne copie que le pointeur, pas le contenu
	if (!bruitage->nom) { bruitage_detruire(bruitage); *res = BRUITAGE_ERR_MEMORY_NAME; return NULL; }

	if ((*res = bruitage_chargerSon(bruitage, nomFichier))) { bruitage_detruire(bruitage); return NULL; }
	return bruitage;
}

void bruitage_play(bruitage_t *bruitage) {
	if (!bruitage || !bruitage->son) return;
	Mix_PlayChannel(-1, bruitage->son, 0); // -1 = jouer sur le premier channel disponible
}

void bruitage_detruire(bruitage_t *bruitage) {
	if (!bruitage) return;
	Mix_FreeChunk(bruitage->son); bruitage->son = NULL;
	free(bruitage->nom); bruitage->nom = NULL;
	free(bruitage);
}

const char * bruitage_strerror(bruitage_result_t res) {
	switch (res) {
		case BRUITAGE_OK: return "Succes";
		case BRUITAGE_ERR_NULL_OR_EMPTY_FILENAME: return "Nom fichier NULL ou vide";
		case BRUITAGE_ERR_SIZE_MAX_FILENAME: return "Nom fichier trop long";
		case BRUITAGE_ERR_MEMORY_BASE: return "Echec allocation memoire base";
		case BRUITAGE_ERR_MEMORY_NAME: return "Echec allocation memoire nom";
		case BRUITAGE_ERR_LOAD_SOUND: return "Echec chargement du son";
		default: return "Erreur inconnue";
	}
}