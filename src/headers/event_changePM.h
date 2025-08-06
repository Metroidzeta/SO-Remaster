// @author Alain Barbier alias "Metroidzeta"

#ifndef EVENT_CHANGEPM_H
#define EVENT_CHANGEPM_H

#include "base.h"

typedef struct { // Event de type E_CPM, pour modifier les PM (en positif +, comme négatif -) du joueur
	int PM;
} event_changePM_t;

event_changePM_t * event_creerChangePM(int PM);
void event_changePM_detruire(event_changePM_t *e_cPM);

#endif