// @author Alain Barbier alias "Metroidzeta"

#include "headers/carte.h"
#include "headers/chargerEvents.h"

static carte_t * getCarte(arraylist_t *cartes, const char *nom) { // Ne pas libérer les cartes : partagée, allouée ailleurs
	if (!nom || !*nom) return NULL;
	for (int i = 0; i < cartes->taille; ++i) {
		carte_t *carte = arraylist_get(cartes, i);
		if (strcmp(carte->nom, nom) == 0) return carte;
	}
	return NULL;
}

static musique_t * getMusique(arraylist_t *musiques, const char *nom) { // Ne pas libérer les musiques : partagée, allouée ailleurs
	if (!nom || !*nom) return NULL;
	for (int i = 0; i < musiques->taille; ++i) {
		musique_t *musique = arraylist_get(musiques, i);
		if (strcmp(musique->nom, nom) == 0) return musique;
	}
	return NULL;
}

static chargerEvents_result_t ajouterEventMSG(carte_t *carte, EventInfo_t *ev_info, const char *msg) {
	event_result_t res;
	event_t *ev = event_creerMSG(msg, &res);
	carte_ajouterEvent(carte, ev_info->numPage, ev_info->xCase, ev_info->yCase, ev);
	return CHARGEREVENTS_OK;
}

static chargerEvents_result_t ajouterEventTP(arraylist_t *cartes, carte_t *carte, EventInfo_t *ev_info, int xCaseDst, int yCaseDst, const char *nomCarteDst) {
	carte_t *carteDst = NULL;
	if (nomCarteDst != NULL) { // une carteDst NULL = refusé -> à la création de l'event_tp
		carteDst = getCarte(cartes, nomCarteDst);
		if (!carteDst) { LOG_ERROR("Carte de destination inexistante : %s", nomCarteDst); return CHARGEREVENTS_ERR_GET_CARTEDST; }
	}
	event_result_t res;
	event_t *ev = event_creerTP(xCaseDst, yCaseDst, carteDst, &res);
	carte_ajouterEvent(carte, ev_info->numPage, ev_info->xCase, ev_info->yCase, ev);
	return CHARGEREVENTS_OK;
}

static chargerEvents_result_t ajouterEventJM(carte_t *carte, EventInfo_t *ev_info, arraylist_t *musiques, const char *nomMusique) {
	musique_t *musique = NULL;
	if (nomMusique != NULL) { // une musique NULL = refusé -> à la création de l'event_jm
		musique = getMusique(musiques, nomMusique);
		if (!musique) { LOG_ERROR("Musique inexistante : %s", nomMusique); return CHARGEREVENTS_ERR_GET_MUSIQUE; }
	}
	event_result_t res;
	event_t *ev = event_creerJM(musique, &res);
	carte_ajouterEvent(carte, ev_info->numPage, ev_info->xCase, ev_info->yCase, ev);
	return CHARGEREVENTS_OK;
}

static chargerEvents_result_t ajouterEventAM(carte_t *carte, EventInfo_t *ev_info) {
	event_result_t res;
	event_t *ev = event_creerAM(&res);
	carte_ajouterEvent(carte, ev_info->numPage, ev_info->xCase, ev_info->yCase, ev);
	return CHARGEREVENTS_OK;
}

static chargerEvents_result_t ajouterEventCPV(carte_t *carte, EventInfo_t *ev_info, int PV) {
	event_result_t res;
	event_t *ev = event_creerChangePV(PV, &res);
	carte_ajouterEvent(carte, ev_info->numPage, ev_info->xCase, ev_info->yCase, ev);
	return CHARGEREVENTS_OK;
}

static chargerEvents_result_t ajouterEventCPM(carte_t *carte, EventInfo_t *ev_info, int PM) {
	event_result_t res;
	event_t *ev = event_creerChangePM(PM, &res);
	carte_ajouterEvent(carte, ev_info->numPage, ev_info->xCase, ev_info->yCase, ev);
	return CHARGEREVENTS_OK;
}

static chargerEvents_result_t json_lireEvents(carte_t *carte, arraylist_t *cartes, arraylist_t *musiques) {
	if (!carte) return CHARGEREVENTS_ERR_NULL_POINTER_CARTE;
	if (!cartes) return CHARGEREVENTS_ERR_NULL_POINTER_CARTES;
	if (!musiques) return CHARGEREVENTS_ERR_NULL_POINTER_MUSIQUES;

	// ---------- Fichier Murs + Events ----------
	size_t len = strlen("cartes/") + strlen(carte->nom) + strlen("_ME.json") + 1; // + 1: \0
	char nomFichierME[len];
	snprintf(nomFichierME, sizeof(nomFichierME), "cartes/%s_ME.json", carte->nom);

	cJSON *jsonME = cJSON_ParseWithFile(nomFichierME);
	if (!jsonME) { LOG_ERROR("Erreur de parsing JSON pour %s", nomFichierME); return CHARGEREVENTS_ERR_READ_MURS_EVENTS; }

	cJSON *jsonEnsemblesEvents = cJSON_GetObjectItem(jsonME, "ensemblesEvents");
	cJSON *evBloc = NULL;
	cJSON_ArrayForEach(evBloc, jsonEnsemblesEvents) {
		cJSON *valX = cJSON_GetObjectItem(evBloc, "x");
		cJSON *valY = cJSON_GetObjectItem(evBloc, "y");
		cJSON *arrEvents = cJSON_GetObjectItem(evBloc, "events");

		if (!cJSON_IsNumber(valX) || !cJSON_IsNumber(valY) || !cJSON_IsArray(arrEvents))
			continue;

		cJSON *ev = NULL;
		cJSON_ArrayForEach(ev, arrEvents) {
			EventInfo_t ev_info = {0};
			ev_info.numPage = 0;
			ev_info.xCase = valX->valueint;
			ev_info.yCase = valY->valueint;

			// --- Remplissage info selon type ---
			cJSON *msg = cJSON_GetObjectItem(ev, "msg");
			if (msg && cJSON_IsString(msg)) {
				chargerEvents_result_t resE_MSG = ajouterEventMSG(carte, &ev_info, msg->valuestring);
				if (resE_MSG != CHARGEREVENTS_OK) { cJSON_Delete(jsonME); return resE_MSG; }
				continue;
			}

			cJSON *tp = cJSON_GetObjectItem(ev, "tp");
			if (tp && cJSON_IsObject(tp)) {
				cJSON *xDst = cJSON_GetObjectItem(tp, "xDst");
				cJSON *yDst = cJSON_GetObjectItem(tp, "yDst");
				cJSON *nomCarteDst = cJSON_GetObjectItem(tp, "carteDst");
				if (cJSON_IsNumber(xDst) && cJSON_IsNumber(yDst) && cJSON_IsString(nomCarteDst)) {
					chargerEvents_result_t resE_TP = ajouterEventTP(cartes, carte, &ev_info, xDst->valueint, yDst->valueint, nomCarteDst->valuestring);
					if (resE_TP != CHARGEREVENTS_OK) { cJSON_Delete(jsonME); return resE_TP; }
				}
				continue;
			}

			cJSON *musique = cJSON_GetObjectItem(ev, "musique");
			if (musique) {
				const char *nomMusique = cJSON_IsString(musique) ? musique->valuestring : NULL;
				chargerEvents_result_t resE_JM = ajouterEventJM(carte, &ev_info, musiques, nomMusique);
				if (resE_JM != CHARGEREVENTS_OK) { cJSON_Delete(jsonME); return resE_JM; }
				continue;
			}

			if (cJSON_GetObjectItem(ev, "arretMusique")) {
				chargerEvents_result_t resE_AM = ajouterEventAM(carte, &ev_info);
				if (resE_AM != CHARGEREVENTS_OK) { cJSON_Delete(jsonME); return resE_AM; }
				continue;
			}

			cJSON *pv = cJSON_GetObjectItem(ev, "PV");
			if (pv && cJSON_IsNumber(pv)) {
				chargerEvents_result_t resE_CPV = ajouterEventCPV(carte, &ev_info, pv->valueint);
				if (resE_CPV != CHARGEREVENTS_OK) { cJSON_Delete(jsonME); return resE_CPV; }
				continue;
			}

			cJSON *pm = cJSON_GetObjectItem(ev, "PM");
			if (pm && cJSON_IsNumber(pm)) {
				chargerEvents_result_t resE_CPM = ajouterEventCPM(carte, &ev_info, pm->valueint);
				if (resE_CPM != CHARGEREVENTS_OK) { cJSON_Delete(jsonME); return resE_CPM; }
				continue;
			}

			LOG_ERROR("Event invalide en (%d, %d) sur la carte %s", ev_info.xCase, ev_info.yCase, carte->nom);
		}
	}
	cJSON_Delete(jsonME);
	return CHARGEREVENTS_OK;
}

chargerEvents_result_t chargerEvents_get(arraylist_t *cartes, arraylist_t *musiques) {
	if (!cartes) return CHARGEREVENTS_ERR_NULL_POINTER_CARTES;
	if (!musiques) return CHARGEREVENTS_ERR_NULL_POINTER_MUSIQUES;

	for (int i = 0; i < cartes->taille; ++i) {
		carte_t *carte = arraylist_get(cartes, i);
		chargerEvents_result_t resCEV = json_lireEvents(carte, cartes, musiques);
		if (resCEV != CHARGEREVENTS_OK) return resCEV;
	}
	return CHARGEREVENTS_OK;
}

const char * chargerEvents_strerror(chargerEvents_result_t res) {
	switch (res) {
		case CHARGEREVENTS_OK: return "Succes";
		case CHARGEREVENTS_ERR_NULL_POINTER_CARTE: return "Carte NULL passe en parametre";
		case CHARGEREVENTS_ERR_NULL_POINTER_CARTES: return "Arraylist cartes NULL passe en parametre";
		case CHARGEREVENTS_ERR_NULL_POINTER_MUSIQUES: return "Arraylist musiques NULL passe en parametre";
		case CHARGEREVENTS_ERR_READ_MURS_EVENTS: return "Echec lecture fichier Murs_Events";
		case CHARGEREVENTS_ERR_GET_CARTEDST: return "Echec obtention carte de destination pour event_tp";
		case CHARGEREVENTS_ERR_GET_MUSIQUE: return "Echec obtention musique";
		default: return "Erreur inconnue";
	}
}