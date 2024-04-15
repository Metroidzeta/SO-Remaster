// @author Alain Barbier alias "Metroidzeta"

#ifndef EVENT_MSG_H
#define EVENT_MSG_H

#include "base.h"

typedef struct event_msg_s { // Event de type E_MESSAGE, pour afficher un message au joueur
	char * message;
} event_msg_t;

event_msg_t * event_creerMsg(char * message);
void event_msg_verificationsArgs(char * message);
void event_msg_detruire(event_msg_t * e_msg);

#endif