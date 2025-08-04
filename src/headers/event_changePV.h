// @author Alain Barbier alias "Metroidzeta"

#ifndef EVENT_CHANGEPV_H
#define EVENT_CHANGEPV_H

#include "base.h"

typedef struct event_changePV_s { // Event de type E_CHANGE_PV, pour modifier les PV (en positif +, comme négatif -) du joueur
	int PV;
} event_changePV_t;

event_changePV_t * event_creerChangePV(int PV);
void event_changePV_detruire(event_changePV_t *e_cPV);

#endif