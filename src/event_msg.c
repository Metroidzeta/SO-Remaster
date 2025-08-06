// @author Alain Barbier alias "Metroidzeta"

#include "headers/event_msg.h"

static void event_msg_validerArguments(const char *msg) {
	if (!msg || !*msg) Exception("Message de l'event_msg NULL ou vide");
}

event_msg_t * event_creerMsg(const char *msg) {
	event_msg_validerArguments(msg);

	event_msg_t * e_msg = calloc(1, sizeof(event_msg_t));
	if (!e_msg) Exception("Echec allocation event_msg");

	e_msg->msg = my_strdup(msg);  // important : ne pas faire "e_msg->msg = msg", car cela ne copie que le pointeur, pas le contenu
	if (!e_msg->msg) { event_msg_detruire(e_msg); Exception("Echec allocation message e_msg"); }

	return e_msg;
}

void event_msg_detruire(event_msg_t *e_msg) {
	if (!e_msg) return;
	free(e_msg->msg);
	free(e_msg);
}