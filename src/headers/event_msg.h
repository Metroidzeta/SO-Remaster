// @author Alain Barbier alias "Metroidzeta"

#ifndef EVENT_MSG_H
#define EVENT_MSG_H

#include "base.h"

typedef struct { // Event de type E_MESSAGE, pour afficher un message du jeu au héros
	char *msg;
} event_msg_t;

event_msg_t * event_creerMsg(const char *msg);
void event_msg_detruire(event_msg_t *e_msg);

#endif