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

#ifndef MUSIQUE_H
#define MUSIQUE_H

#include "base.h"

typedef struct {
	char *nom;
	Mix_Music *piste; // piste audio
	bool enLecture, enPause;
} musique_t;

typedef enum {
	MUSIQUE_OK = 0,
	MUSIQUE_ERR_NULL_POINTER,
	MUSIQUE_ERR_NULL_OR_EMPTY_FILENAME,
	MUSIQUE_ERR_SIZE_MAX_FILENAME,
	MUSIQUE_ERR_MEMORY_BASE,
	MUSIQUE_ERR_MEMORY_NAME,
	MUSIQUE_ERR_LOAD_SOUND
} musique_result_t;

musique_t * musique_creer(const char *nomFichier, musique_result_t *res);

void musique_play(musique_t *musique);
void musique_stop(musique_t *musique);
void musique_pause(musique_t *musique);
void musique_resume(musique_t *musique);

void musique_detruire(musique_t *musique);
const char * musique_strerror(musique_result_t res);

#endif