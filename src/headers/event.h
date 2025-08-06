// @author Alain Barbier alias "Metroidzeta"

#ifndef EVENT_H
#define EVENT_H

#include "base.h"

typedef enum {
	E_MSG, // message du jeu
	E_TP, // téléportation
	E_JM, // jouer musique
	E_AM, // arreter musique
	E_CPV, // modifier PV joueur
	E_CPM // modifier PM joueur
} e_type;

typedef struct { // Structure contenant le ptr vers l'event réel et son type
	e_type type;
	void *ptr;
} event_t;

event_t * event_creer(e_type type, void *ptrEvt);
void event_detruire(event_t *e);

#endif