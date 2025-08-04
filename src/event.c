// @author Alain Barbier alias "Metroidzeta"

#include "headers/event.h"

#include "headers/event_msg.h"
#include "headers/event_tp.h"
#include "headers/event_jm.h"
#include "headers/event_changePV.h"
#include "headers/event_changePM.h"

static void event_validerArguments(e_type type, void *ptrEvt) {
	if (type != E_AM && !ptrEvt) Exception("ptr vers l'event reel NULL");
}

event_t * event_creer(e_type type, void *ptrEvt) {
	event_validerArguments(type, ptrEvt);

	event_t *e = calloc(1, sizeof(event_t));
	if (!e) Exception("Echec creation event");

	*e = (event_t) { type, ptrEvt };
	return e;
}

void event_detruire(event_t *e) {
	switch (e->type) {
		case E_MSG: event_msg_detruire(e->ptr); break;
		case E_TP: event_tp_detruire(e->ptr); break;
		case E_JM: event_jm_detruire(e->ptr); break;
		case E_CPV: event_changePV_detruire(e->ptr); break;
		case E_CPM: event_changePM_detruire(e->ptr); break;
		default: break;
	}
	free(e);
}