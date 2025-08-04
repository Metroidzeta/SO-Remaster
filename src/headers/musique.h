// @author Alain Barbier alias "Metroidzeta"

#ifndef MUSIQUE_H
#define MUSIQUE_H

#include "base.h"

typedef enum {
	MUSIQUE_OK = 0,
	MUSIQUE_ERR_NULL_POINTER,
	MUSIQUE_ERR_NULL_OR_EMPTY_FILENAME,
	MUSIQUE_ERR_MEMORY_BASE,
	MUSIQUE_ERR_MEMORY_NAME,
	MUSIQUE_ERR_LOAD_SOUND
} musique_result_t;

typedef struct musique_s {
	char *nom;
	Mix_Music *piste; // piste audio
	bool enLecture;
} musique_t;

musique_result_t musique_creer(musique_t **out_musique, const char *nomFichier);
void musique_play(musique_t *musique);
void musique_stop(musique_t *musique);
void musique_detruire(musique_t *musique);
const char * musique_strerror(musique_result_t res);

#endif