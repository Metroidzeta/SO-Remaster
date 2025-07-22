// @author Alain Barbier alias "Metroidzeta"

#include "event_msg.h"

static void event_msg_validerArguments(const char *msg) {
	if (!msg || !*msg) Exception("Message de l'event_msg NULL ou vide");
}

event_msg_t * event_creerMsg(const char *msg) {
	event_msg_validerArguments(msg);

	event_msg_t * e_msg = malloc(sizeof(event_msg_t));
	if (!e_msg) Exception("Echec allocation event_msg");
	memset(e_msg, 0, sizeof(event_msg_t)); // initialise tout à 0 / NULL pour éviter comportements indifinis en cas d'exception

	e_msg->msg = strdup(msg); // ne pas faire: "e_msg->msg = msg" car on ne copie alors que des adresses
	if (!e_msg->msg) { event_msg_detruire(e_msg); Exception("Echec allocation message e_msg"); }
	return e_msg;
}

void event_msg_detruire(event_msg_t *e_msg) {
	free(e_msg->msg);
	free(e_msg);
}