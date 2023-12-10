// @author Metroidzeta

#ifndef EVENT_JM_H
#define EVENT_JM_H

#include "base.h"
#include "musique.h"

typedef struct event_jm_s { // Event de type E_JOUER_MUSIQUE, pour jouer une (ou changer de) musique
	musique_t * musique;
} event_jm_t;

event_jm_t * event_creerJM(musique_t * musique);
void event_jm_verificationsArgs(musique_t * musique);
void event_jm_detruire(event_jm_t * e_jm);

#endif