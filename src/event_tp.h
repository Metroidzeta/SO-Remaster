// @author Alain Barbier alias "Metroidzeta"

#ifndef EVENT_TP_H
#define EVENT_TP_H

#include "base.h"
#include "carte.h"

typedef struct event_tp_s { // Event de type E_TELEPORTATION, pour téléporter le joueur vers un {xCaseDst, yCaseDst} d'une carte
	int xDst;
	int yDst;
	carte_t *carteDst;
} event_tp_t;

event_tp_t * event_creerTP(int xCaseDst, int yCaseDst, carte_t *carteDst);
void event_tp_detruire(event_tp_t *e_tel);

#endif