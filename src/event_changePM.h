// @author Metroidzeta

#ifndef EVENT_CHANGEPM_H
#define EVENT_CHANGEPM_H

#include "base.h"

typedef struct event_changePM_s { // Event de type E_CHANGE_PM, pour modifier les PM (en positif +, comme négatif -) du joueur
	int PM;
} event_changePM_t;

event_changePM_t * event_creerChangePM(int PM);
void event_changePM_verificationsArgs(int PM);
void event_changePM_detruire(event_changePM_t * e_cPM);

#endif