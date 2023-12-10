#include "event.h"
#include "event_msg.h"
#include "event_tp.h"
#include "event_jm.h"

event_t * event_creer(e_type type, void * evtPtr) {
	event_verificationsArgs(type,evtPtr);
	event_t * e = malloc(sizeof(event_t));
	verifAlloc(e,"Erreur d'allocation de l'event");
	*e = (event_t) {type, evtPtr};
	return e;
}

void event_verificationsArgs(e_type type, void * evtPtr) {
	if(type != E_ARRETER_MUSIQUE && evtPtr == NULL) { Exception("Le pointeur vers l'event reel est NULL"); }
}

void event_detruire(event_t * e) {
	switch(e->type) {
		case E_MESSAGE: event_msg_detruire(e->ptr); break;
		case E_TELEPORTATION: event_tp_detruire(e->ptr); break;
		case E_JOUER_MUSIQUE: event_jm_detruire(e->ptr); break;
		default: break;
	}
	free(e);
}