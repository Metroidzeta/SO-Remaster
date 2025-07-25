// @author Alain Barbier alias "Metroidzeta"

#ifndef MUSIQUE_H
#define MUSIQUE_H

#include "base.h"

typedef struct musique_s {
	char *nom;
	Mix_Music *piste; // piste audio
	bool enLecture;
} musique_t;

musique_t * musique_creer(const char *nomFichier);
void musique_play(musique_t *musique);
void musique_stop(musique_t *musique);
void musique_detruire(musique_t *musique);

#endif