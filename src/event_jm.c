// @author Alain Barbier alias "Metroidzeta"

#include "event_jm.h"

static void event_jm_validerArguments(musique_t *musique) {
	if (!musique) Exception("Musique event_jm NULL");
}

event_jm_t * event_creerJM(musique_t *musique) {
	event_jm_validerArguments(musique);

	event_jm_t *e_jm = malloc(sizeof(event_jm_t));
	if (!e_jm) Exception("Echec allocation event_jm");
	memset(e_jm, 0, sizeof(event_jm_t)); // initialise tout à 0 / NULL pour éviter comportements indifinis en cas d'exception

	e_jm->musique = musique;
	return e_jm;
}

void event_jm_detruire(event_jm_t *e_jm) { free(e_jm); } // Pas besoin de free la musique utilisée car elle est détruite dans l'arraylist lesMusiques