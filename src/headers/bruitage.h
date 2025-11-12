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

#ifndef BRUITAGE_H
#define BRUITAGE_H

#include "base.h"

typedef struct {
	char *nom;
	Mix_Chunk *son;
} bruitage_t;

typedef enum {
	BRUITAGE_OK = 0,
	BRUITAGE_ERR_NULL_OR_EMPTY_FILENAME,
	BRUITAGE_ERR_SIZE_MAX_FILENAME,
	BRUITAGE_ERR_MEMORY_BASE,
	BRUITAGE_ERR_MEMORY_NAME,
	BRUITAGE_ERR_LOAD_SOUND
} bruitage_result_t;

bruitage_t * bruitage_creer(const char *nomFichier, bruitage_result_t *res);

void bruitage_play(bruitage_t *bruitage);

void bruitage_detruire(bruitage_t *bruitage);
const char * bruitage_strerror(bruitage_result_t res);

#endif