// @author Alain Barbier alias "Metroidzeta"

#ifndef CHARGEREVENTS_H
#define CHARGEREVENTS_H

#include "base.h"
#include "arraylist.h"
#include "event.h"
#include "event_msg.h"
#include "event_tp.h"
#include "event_jm.h"
#include "event_changePV.h"
#include "event_changePM.h"

typedef enum {
	CHARGEREVENTS_OK = 0,
	CHARGEREVENTS_ERR_NULL_POINTER_CARTE,
	CHARGEREVENTS_ERR_NULL_POINTER_CARTES,
	CHARGEREVENTS_ERR_NULL_POINTER_MUSIQUES,
	CHARGEREVENTS_ERR_READ_MURS_EVENTS,
	CHARGEREVENTS_ERR_GET_CARTEDST,
	CHARGEREVENTS_ERR_GET_MUSIQUE
} chargerEvents_result_t;

typedef struct {
	const char *msg;              // message du jeu à afficher
} event_u_msg_t;

typedef struct {
	int xCaseDst;                 // xCase de destination
	int yCaseDst;                 // yCase de destination
	const char *nomCarteDst;      // nom carte de destination
} event_u_tp_t;

typedef struct {
	const char *nomMusique;
} event_u_jm_t;

typedef struct {} event_u_am_t;   // rien

typedef struct {
	int valeur;                   // changer les PV de cette valeur
} event_u_cpv_t;

typedef struct {
	int valeur;                   // changer les PM de cette valeur
} event_u_cpm_t;

typedef struct {
	const char *nomCarte;         // nom de la carte où est l'event
	int numPage;                  // numPage de l'event (par défaut : 0)
	int xCase;                    // xCase de l'event
	int yCase;                    // yCase de l'event
	e_type type;                  // type de l'event (enum e_type dans event.h)
	union {
		event_u_msg_t msg;
		event_u_tp_t tp;
		event_u_jm_t jm;
		event_u_am_t am;
		event_u_cpv_t cpv;
		event_u_cpm_t cpm;
	};
} event_info_t;

chargerEvents_result_t chargerEvents_get(arraylist_t *cartes, arraylist_t *musiques);
const char * chargerEvents_strerror(chargerEvents_result_t res);

#endif