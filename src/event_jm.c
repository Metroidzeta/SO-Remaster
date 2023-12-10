#include "event_jm.h"

event_jm_t * event_creerJM(musique_t * musique) {
	event_jm_verificationsArgs(musique);
	event_jm_t * e_jm = malloc(sizeof(event_jm_t));
	verifAlloc(e_jm,"Erreur d'allocation de l'event_jm");
	e_jm->musique = musique;
	return e_jm;
}

void event_jm_verificationsArgs(musique_t * musique) {
	if(musique == NULL) { Exception("La musique de l'event_jm est NULL"); }
}

void event_jm_detruire(event_jm_t * e_jm) { free(e_jm); }