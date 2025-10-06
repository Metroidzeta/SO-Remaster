// @author Alain Barbier alias "Metroidzeta"

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

static chargerEvents_result_t ajouterEventMSG(const event_info_t *elem, carte_t *carte) {
	event_msg_t *evt_msg = event_creerMsg(elem->msg.msg);
	carte_ajouterEvent(carte, elem->numPage, elem->xCase, elem->yCase, E_MSG, evt_msg);
	return CHARGEREVENTS_OK;
}

static chargerEvents_result_t ajouterEventTP(const event_info_t *elem, carte_t *carte, arraylist_t *cartes) {
	carte_t *carteDst = NULL;
	if (elem->tp.nomCarteDst != NULL) { // une carteDst NULL = refusé -> à la création de l'event_tp
		carteDst = getCarte(cartes, elem->tp.nomCarteDst);
		if (!carteDst) { LOG_ERROR("Carte de destination inexistante : %s", elem->tp.nomCarteDst); return CHARGEREVENTS_ERR_GET_CARTEDST; }
	}
	event_tp_t *evt_tp = event_creerTP(elem->tp.xCaseDst, elem->tp.yCaseDst, carteDst);
	carte_ajouterEvent(carte, elem->numPage, elem->xCase, elem->yCase, E_TP, evt_tp);
	return CHARGEREVENTS_OK;
}

static chargerEvents_result_t ajouterEventJM(const event_info_t *elem, carte_t *carte, arraylist_t *musiques) {
	musique_t *musique = NULL;
	if (elem->jm.nomMusique != NULL) { // une musique NULL = refusé -> à la création de l'event_jm
		musique = getMusique(musiques, elem->jm.nomMusique);
		if (!musique) { LOG_ERROR("Musique inexistante : %s", elem->jm.nomMusique); return CHARGEREVENTS_ERR_GET_MUSIQUE; }
	}
	event_jm_t *evt_jm = event_creerJM(musique);
	carte_ajouterEvent(carte, elem->numPage, elem->xCase, elem->yCase, E_JM, evt_jm);
	return CHARGEREVENTS_OK;
}

static chargerEvents_result_t ajouterEventAM(const event_info_t *elem, carte_t *carte) {
	carte_ajouterEvent(carte, elem->numPage, elem->xCase, elem->yCase, E_AM, NULL);
	return CHARGEREVENTS_OK;
}

static chargerEvents_result_t ajouterEventCPV(const event_info_t *elem, carte_t *carte) {
	event_changePV_t *evt_cpv = event_creerChangePV(elem->cpv.valeur);
	carte_ajouterEvent(carte, elem->numPage, elem->xCase, elem->yCase, E_CPV, evt_cpv);
	return CHARGEREVENTS_OK;
}

static chargerEvents_result_t ajouterEventCPM(const event_info_t *elem, carte_t *carte) {
	event_changePM_t *evt_cpm = event_creerChangePM(elem->cpm.valeur);
	carte_ajouterEvent(carte, elem->numPage, elem->xCase, elem->yCase, E_CPM, evt_cpm);
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
			event_info_t info = {0};
			info.numPage = 0;
			info.xCase = valX->valueint;
			info.yCase = valY->valueint;

			// --- Remplissage info selon type ---
			cJSON *msg = cJSON_GetObjectItem(ev, "msg");
			if (msg && cJSON_IsString(msg)) {
				info.type = E_MSG;
				info.msg.msg = msg->valuestring;
				chargerEvents_result_t resE_MSG = ajouterEventMSG(&info, carte);
				if (resE_MSG != CHARGEREVENTS_OK) { cJSON_Delete(jsonME); return resE_MSG; }
				continue;
			}

			cJSON *tp = cJSON_GetObjectItem(ev, "tp");
			if (tp && cJSON_IsObject(tp)) {
				cJSON *xDst = cJSON_GetObjectItem(tp, "xDst");
				cJSON *yDst = cJSON_GetObjectItem(tp, "yDst");
				cJSON *nomCarteDst = cJSON_GetObjectItem(tp, "carteDst");
				if (cJSON_IsNumber(xDst) && cJSON_IsNumber(yDst) && cJSON_IsString(nomCarteDst)) {
					info.type = E_TP;
					info.tp.xCaseDst = xDst->valueint;
					info.tp.yCaseDst = yDst->valueint;
					info.tp.nomCarteDst = nomCarteDst->valuestring;
					chargerEvents_result_t resE_TP = ajouterEventTP(&info, carte, cartes);
					if (resE_TP != CHARGEREVENTS_OK) { cJSON_Delete(jsonME); return resE_TP; }
				}
				continue;
			}

			cJSON *musique = cJSON_GetObjectItem(ev, "musique");
			if (musique) {
				info.type = E_JM;
				info.jm.nomMusique = cJSON_IsString(musique) ? musique->valuestring : NULL;
				chargerEvents_result_t resE_JM = ajouterEventJM(&info, carte, musiques);
				if (resE_JM != CHARGEREVENTS_OK) { cJSON_Delete(jsonME); return resE_JM; }
				continue;
			}

			if (cJSON_GetObjectItem(ev, "arretMusique")) {
				info.type = E_AM;
				chargerEvents_result_t resE_AM = ajouterEventAM(&info, carte);
				if (resE_AM != CHARGEREVENTS_OK) { cJSON_Delete(jsonME); return resE_AM; }
				continue;
			}

			cJSON *pv = cJSON_GetObjectItem(ev, "PV");
			if (pv && cJSON_IsNumber(pv)) {
				info.type = E_CPV;
				info.cpv.valeur = pv->valueint;
				chargerEvents_result_t resE_CPV = ajouterEventCPV(&info, carte);
				if (resE_CPV != CHARGEREVENTS_OK) { cJSON_Delete(jsonME); return resE_CPV; }
				continue;
			}

			cJSON *pm = cJSON_GetObjectItem(ev, "PM");
			if (pm && cJSON_IsNumber(pm)) {
				info.type = E_CPM;
				info.cpm.valeur = pm->valueint;
				 chargerEvents_result_t resE_CPM = ajouterEventCPM(&info, carte);
				if (resE_CPM != CHARGEREVENTS_OK) { cJSON_Delete(jsonME); return resE_CPM; }
				continue;
			}

			LOG_ERROR("Event invalide en (%d, %d) sur la carte %s", info.xCase, info.yCase, carte->nom);
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