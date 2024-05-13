// @author Alain Barbier alias "Metroidzeta"

#include "event_changePV.h"

static void event_changePV_verificationsArgs(int PV) {
	if(PV == 0) { Exception("L'event_changePV ne sert à rien car il modifie les PV de 0"); }
}

event_changePV_t * event_creerChangePV(int PV) {
	event_changePV_verificationsArgs(PV);
	event_changePV_t * e_cPV = malloc(sizeof(event_changePV_t)); verifAlloc(e_cPV,"Erreur d'allocation de l'event_changePV");
	e_cPV->PV = PV;
	return e_cPV;
}

void event_changePV_detruire(event_changePV_t * e_cPV) { free(e_cPV); }