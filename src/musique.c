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

#include "headers/musique.h"

static inline musique_result_t musique_validerArguments(const char *nomFichier) {
	if (!nomFichier || !*nomFichier) return MUSIQUE_ERR_NULL_OR_EMPTY_FILENAME;
	if (strlen(nomFichier) >= MAX_TAILLE_STRING) return MUSIQUE_ERR_SIZE_MAX_FILENAME;
	return MUSIQUE_OK;
}

static musique_result_t musique_chargerPiste(musique_t *musique, const char *nomFichier) {
	musique->piste = creerPiste(nomFichier);
	if (!musique->piste) { LOG_ERROR("Echec creerPiste: %s (src: %s)", Mix_GetError(), nomFichier); return MUSIQUE_ERR_LOAD_SOUND; }
	musique->enLecture = false;
	musique->enPause = false;
	return MUSIQUE_OK;
}

musique_t * musique_creer(const char *nomFichier, musique_result_t *res) {
	if (!res) { LOG_ERROR("Enum musique_result NULL"); return NULL; }
	*res = MUSIQUE_OK;
	if ((*res = musique_validerArguments(nomFichier))) return NULL;

	musique_t *musique = calloc(1, sizeof(musique_t));
	if (!musique) { *res = MUSIQUE_ERR_MEMORY_BASE; return NULL; }

	musique->nom = my_strdup(nomFichier); // important : ne pas faire "musique->nom = nomFichier", car cela ne copie que le pointeur, pas le contenu
	if (!musique->nom) { musique_detruire(musique); *res = MUSIQUE_ERR_MEMORY_NAME; return NULL; }

	if ((*res = musique_chargerPiste(musique, nomFichier))) { musique_detruire(musique); return NULL; }
	return musique;
}

void musique_play(musique_t *musique) {
	if (!musique || !musique->piste) return;
	Mix_PlayMusic(musique->piste, -1); // -1 = loop infini
	musique->enLecture = true;
	musique->enPause = false;
}

void musique_stop(musique_t *musique) {
	if (!musique || !musique->piste) return;
	Mix_HaltMusic(); // stoppe toute la musique SDL_mixer
	musique->enLecture = false;
	musique->enPause = false;
}

void musique_pause(musique_t *musique) {
	if (!musique || !musique->piste) return;
	if (musique->enLecture && !musique->enPause) {
		Mix_PauseMusic();
		musique->enPause = true;
	}
}

void musique_resume(musique_t *musique) {
	if (!musique || !musique->piste) return;
	if (musique->enPause) {
		Mix_ResumeMusic();
		musique->enPause = false;
	}
}

void musique_detruire(musique_t *musique) {
	if (!musique) return;
	Mix_FreeMusic(musique->piste); musique->piste = NULL;
	free(musique->nom); musique->nom = NULL;
	free(musique);
}

const char * musique_strerror(musique_result_t res) {
	switch (res) {
		case MUSIQUE_OK: return "Succes";
		case MUSIQUE_ERR_NULL_POINTER: return "Musique NULL passe en parametre";
		case MUSIQUE_ERR_NULL_OR_EMPTY_FILENAME: return "Nom fichier NULL ou vide";
		case MUSIQUE_ERR_SIZE_MAX_FILENAME: return "Nom fichier trop long";
		case MUSIQUE_ERR_MEMORY_BASE: return "Echec allocation memoire base";
		case MUSIQUE_ERR_MEMORY_NAME: return "Echec allocation memoire nom";
		case MUSIQUE_ERR_LOAD_SOUND: return "Echec chargement du son";
		default: return "Erreur inconnue";
	}
}