#include "event_changePV.h"

event_changePV_t * event_creerChangePV(int PV) {
	event_changePV_verificationsArgs(PV);
	event_changePV_t * e_cPV = malloc(sizeof(event_changePV_t));
	verifAlloc(e_cPV,"Erreur d'allocation de l'event_changePV");
	e_cPV->PV = PV;
	return e_cPV;
}

void event_changePV_verificationsArgs(int PV) {
	if(PV == 0) { Exception("Merci d'enlever l'event_changePV pour modifier les PV de 0 car cela ne sert à rien"); }
}

void event_changePV_detruire(event_changePV_t * e_cPV) { free(e_cPV); }