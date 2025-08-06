// @author Alain Barbier alias "Metroidzeta"

#include "headers/event_jm.h"

static void event_jm_validerArguments(musique_t *musique) {
	if (!musique) Exception("Musique event_jm NULL");
}

event_jm_t * event_creerJM(musique_t *musique) {
	event_jm_validerArguments(musique);

	event_jm_t *e_jm = calloc(1, sizeof(event_jm_t));
	if (!e_jm) Exception("Echec allocation event_jm");

	e_jm->musique = musique;
	return e_jm;
}

void event_jm_detruire(event_jm_t *e_jm) { // Ne pas libérer e_jm->musique : partagée, allouée ailleurs
	if (!e_jm) return;
	free(e_jm);
}