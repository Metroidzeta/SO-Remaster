// @author Alain Barbier alias "Metroidzeta"

#ifndef EVENT_H
#define EVENT_H

#include "base.h"

typedef enum { E_MESSAGE, E_TELEPORTATION, E_JOUER_MUSIQUE, E_ARRETER_MUSIQUE, E_CHANGE_PV, E_CHANGE_PM } e_type;

typedef struct event_s { // Structure contenant l'event (pointeur) et son type
	e_type type;
	void * ptr;
} event_t;

event_t * event_creer(e_type type, void * evtPtr);
void event_verificationsArgs(e_type type, void * evtPtr);
void event_detruire(event_t * e);

#endif