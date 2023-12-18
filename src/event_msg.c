#include "event_msg.h"

event_msg_t * event_creerMsg(char * message) {
	event_msg_verificationsArgs(message);
	event_msg_t * e_msg = malloc(sizeof(event_msg_t));
	verifAlloc(e_msg,"Erreur d'allocation de l'event_msg");
	e_msg->message = strdup(message); // il ne faut pas écrire : "e->message = message;" car on ne copie alors que des adresses
	verifAllocStrCopy(e_msg->message,message);
	return e_msg;
}

void event_msg_verificationsArgs(char * message) {
	if(message == NULL) { Exception("Le message de l'event_msg est NULL"); }
	if(message[0] == '\0') { Exception("Le message de l'event_msg est vide"); }
}

void event_msg_detruire(event_msg_t * e_msg) {
	free(e_msg->message);
	free(e_msg);
}