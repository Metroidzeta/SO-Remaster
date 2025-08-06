// @author Alain Barbier alias "Metroidzeta"

#ifndef EVENT_TP_H
#define EVENT_TP_H

#include "base.h"
#include "carte.h"

typedef struct { // Event de type E_TP, pour téléporter le joueur vers un {xCaseDst, yCaseDst} d'une carte de destination
	int xDst;
	int yDst;
	carte_t *carteDst;
} event_tp_t;

event_tp_t * event_creerTP(int xCaseDst, int yCaseDst, carte_t *carteDst);
void event_tp_detruire(event_tp_t *e_tel);

#endif