// @author Alain Barbier alias "Metroidzeta"

#include "headers/event.h"
#include "headers/carte.h"

static event_result_t event_validerArguments(EventData_t *data) {
	if (!data) return EVENT_ERR_NULL_POINTER_DATA;
	return EVENT_OK;
}

static event_t * event_creer(EventType type, EventData_t *data, event_result_t *res) {
	event_result_t code = event_validerArguments(data);
	if (code != EVENT_OK) { if (res) *res = code; return NULL; }

	event_t *ev = calloc(1, sizeof(event_t));
	if (!ev) { if (res) *res = EVENT_ERR_MEMORY_BASE; return NULL; }

	*ev = (event_t){ type, *data };
	return ev;
}

event_t * event_creerMSG(const char *msg, event_result_t *res) {
	if (!msg || !*msg) { if (res) *res = EVENT_ERR_NULL_OR_EMPTY_MSG; return NULL; }
	if (strlen(msg) >= MAX_TAILLE_STRING) { if (res) *res = EVENT_ERR_SIZE_MAX_MSG; return NULL; }

	EventData_t data = {0};
	data.msg =  my_strdup(msg); // important : ne pas faire "e_msg->msg = msg", car cela ne copie que le pointeur, pas le contenu
	if (!data.msg) { if (res) *res = EVENT_ERR_MEMORY_MSG; return NULL; }
	return event_creer(EVENT_MSG, &data, res);
}

event_t * event_creerTP(int xCaseDst, int yCaseDst, carte_t *carteDst, event_result_t *res) {
	if (!carteDst) { if (res) *res = EVENT_ERR_NULL_POINTER_CARTEDST; return NULL; }
	if (xCaseDst < 0 || xCaseDst >= carteDst->largeur || yCaseDst < 0 || yCaseDst >= carteDst->hauteur) { if (res) *res = EVENT_ERR_INVALID_COORD; return NULL; }

	EventData_t data = {0};
	data.tp.xDst = xCaseDst * TAILLE_CASES; // vraie valeur de x : il faut multiplier par TAILLE_CASES
	data.tp.yDst = yCaseDst * TAILLE_CASES; // vraie valeur de y : il faut multiplier par TAILLE_CASES
	data.tp.carteDst = carteDst;
	return event_creer(EVENT_TP, &data, res);
}

event_t * event_creerJM(musique_t *musique, event_result_t *res) {
	if (!musique) { if (res) *res = EVENT_ERR_NULL_POINTER_MUSIQUE; return NULL; }

	EventData_t data = {0};
	data.musique = musique;
	return event_creer(EVENT_JOUER_MUSIQUE, &data, res);
}

event_t * event_creerAM(event_result_t *res) {
	EventData_t data = {0};
	return event_creer(EVENT_ARRET_MUSIQUE, &data, res);
}

event_t * event_creerChangePV(int PV, event_result_t *res) {
	if (PV == 0) { if (res) *res = EVENT_ERR_ARGS_PV; return NULL; }

	EventData_t data = {0};
	data.val_PV = PV;
	return event_creer(EVENT_CHANGE_PV, &data, res);
}

event_t * event_creerChangePM(int PM, event_result_t *res) {
	if (PM == 0) { if (res) *res = EVENT_ERR_ARGS_PM; return NULL; }

	EventData_t data = {0};
	data.val_PM = PM;
	return event_creer(EVENT_CHANGE_PM, &data, res);
}

void event_detruire(event_t *ev) {
	if (!ev) return;

	if (ev->type == EVENT_MSG && ev->data.msg) free((void*)ev->data.msg);
	free(ev);
}

const char * event_strerror(event_result_t res) {
	switch (res) {
		case EVENT_OK: return "Succes";
		case EVENT_ERR_NULL_POINTER: return "Event NULL passe en parametre";
		case EVENT_ERR_NULL_POINTER_DATA: return "EventData NULL passe en parametre";
		case EVENT_ERR_MEMORY_BASE: return "Echec allocation memoire base";
		case EVENT_ERR_NULL_OR_EMPTY_MSG: return "Event_msg: Message NULL ou vide";
		case EVENT_ERR_SIZE_MAX_MSG: return "Event_msg: Message trop long";
		case EVENT_ERR_MEMORY_MSG: return "Event_msg: Echec allocation memoire message";
		case EVENT_ERR_NULL_POINTER_CARTEDST: return "Event_tp: CarteDst NULL passe en parametre";
		case EVENT_ERR_INVALID_COORD: return "Event_tp: Coordonnées hors limite carteDst (< 0 ou > limite)";
		case EVENT_ERR_NULL_POINTER_MUSIQUE: return "Event_jm: Musique NULL passe en parametre";
		case EVENT_ERR_ARGS_PV: return "Event_cPV: inutile car modifie les PV de 0";
		case EVENT_ERR_ARGS_PM: return "Event_cPM: inutile car modifie les PM de 0";
		default: return "Erreur inconnue";
	}
}