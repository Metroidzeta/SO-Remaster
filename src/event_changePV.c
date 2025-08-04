// @author Alain Barbier alias "Metroidzeta"

#include "headers/event_changePV.h"

static void event_changePV_validerArguments(int PV) {
	if (PV == 0) Exception("Event_changePV inutile car modifie les PV de 0");
}

event_changePV_t * event_creerChangePV(int PV) {
	event_changePV_validerArguments(PV);

	event_changePV_t *e_cPV = malloc(sizeof(event_changePV_t));
	if (!e_cPV) Exception("Echec creation event_changePV");

	e_cPV->PV = PV;
	return e_cPV;
}

void event_changePV_detruire(event_changePV_t *e_cPV) { free(e_cPV); }