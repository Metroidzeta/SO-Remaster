// @author Alain Barbier alias "Metroidzeta"

#include "headers/event_changePM.h"

static void event_changePM_validerArguments(int PM) {
	if (PM == 0) Exception("Event_changePM inutile car modifie les PM de 0");
}

event_changePM_t * event_creerChangePM(int PM) {
	event_changePM_validerArguments(PM);

	event_changePM_t *e_cPM = calloc(1, sizeof(event_changePM_t));
	if (!e_cPM) Exception("Echec creation event_changePM");

	e_cPM->PM = PM;
	return e_cPM;
}

void event_changePM_detruire(event_changePM_t *e_cPM) {
	if (!e_cPM) return;
	free(e_cPM);
}