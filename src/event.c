// @author Alain Barbier alias "Metroidzeta"

#include "event.h"
#include "event_msg.h"
#include "event_tp.h"
#include "event_jm.h"
#include "event_changePV.h"
#include "event_changePM.h"

static void event_validerArguments(e_type type, void *ptrEvt) {
	if (type != E_ARRETER_MUSIQUE && !ptrEvt) Exception("ptr vers l'event reel NULL");
}

event_t * event_creer(e_type type, void *ptrEvt) {
	event_validerArguments(type, ptrEvt);

	event_t *e = malloc(sizeof(event_t));
	if (!e) Exception("Echec creation event");
	memset(e, 0, sizeof(event_t)); // initialise tout à 0 / NULL pour éviter comportements indifinis en cas d'exception

	*e = (event_t) { type, ptrEvt };
	return e;
}

void event_detruire(event_t *e) {
	switch (e->type) {
		case E_MESSAGE: event_msg_detruire(e->ptr); break;
		case E_TP: event_tp_detruire(e->ptr); break;
		case E_JOUER_MUSIQUE: event_jm_detruire(e->ptr); break;
		case E_CHANGE_PV: event_changePV_detruire(e->ptr); break;
		case E_CHANGE_PM: event_changePM_detruire(e->ptr); break;
		default: break;
	}
	free(e);
}