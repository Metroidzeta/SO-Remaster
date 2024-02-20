#include "event_changePM.h"

event_changePM_t * event_creerChangePM(int PM) {
	event_changePM_verificationsArgs(PM);
	event_changePM_t * e_cPM = malloc(sizeof(event_changePM_t)); verifAlloc(e_cPM,"Erreur d'allocation de l'event_changePM");
	e_cPM->PM = PM;
	return e_cPM;
}

void event_changePM_verificationsArgs(int PM) {
	if(PM == 0) { Exception("L'event_changePM ne sert à rien car il modifie les PM de 0"); }
}

void event_changePM_detruire(event_changePM_t * e_cPM) { free(e_cPM); }