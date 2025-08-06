// @author Alain Barbier alias "Metroidzeta"

#ifndef BRUITAGE_H
#define BRUITAGE_H

#include "base.h"

typedef enum {
	BRUITAGE_OK = 0,
	BRUITAGE_ERR_NULL_POINTER,
	BRUITAGE_ERR_NULL_OR_EMPTY_FILENAME,
	BRUITAGE_ERR_MEMORY_BASE,
	BRUITAGE_ERR_MEMORY_NAME,
	BRUITAGE_ERR_LOAD_SOUND
} bruitage_result_t;

typedef struct {
	char *nom;
	Mix_Chunk *son;
} bruitage_t;

bruitage_result_t bruitage_creer(bruitage_t **out_bruitage, const char *nomFichier);
void bruitage_play(bruitage_t *bruitage);
void bruitage_detruire(bruitage_t *bruitage);
const char * bruitage_strerror(bruitage_result_t res);

#endif