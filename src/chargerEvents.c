/**
 * @author Alain Barbier alias "Metroidzeta"
 * Copyright © 2025 Alain Barbier (Metroidzeta) - All rights reserved.
 *
 * This file is part of the project covered by the
 * "Educational and Personal Use License / Licence d’Utilisation Personnelle et Éducative".
 *
 * Permission is granted to fork and use this code for educational and personal purposes only.
 *
 * Commercial use, redistribution, or public republishing of modified versions
 * is strictly prohibited without the express written consent of the author.
 *
 * Coded with SDL2 (Simple DirectMedia Layer 2).
 *
 * Created by Metroidzeta.
 */

#include <dirent.h>
#include "headers/chargerEvents.h"
#include "headers/event.h"

static chargerEvents_result_t json_lireEvents(const char *nomCarte, jeu_t *jeu) {
	// ---------- Fichier Murs + Events ----------
	char nomFichierME[MAX_TAILLE_CHEMIN];
	snprintf(nomFichierME, sizeof(nomFichierME), "cartes/%s_ME.json", nomCarte);

	cJSON *jsonME = cJSON_ParseWithFile(nomFichierME);
	if (!jsonME) { LOG_ERROR("Echec parsing JSON (src: %s)", nomFichierME); return CHARGEREVENTS_ERR_READ_FILE_MURS_EVENTS; }

	carte_t *carte = getCarte2(jeu, nomCarte);
	if (!carte) { LOG_ERROR("Nom carte: %s", nomCarte); cJSON_Delete(jsonME); return CHARGEREVENTS_ERR_NULL_POINTER_CARTE; }

	cJSON *ensemblesEvents = cJSON_GetObjectItem(jsonME, "ensemblesEvents");
	if (!ensemblesEvents || !cJSON_IsArray(ensemblesEvents)) { cJSON_Delete(jsonME); return CHARGEREVENTS_OK; } // pas d'événements
	cJSON *evBloc = NULL;
	cJSON_ArrayForEach(evBloc, ensemblesEvents) {
		cJSON *valX = cJSON_GetObjectItem(evBloc, "x");
		cJSON *valY = cJSON_GetObjectItem(evBloc, "y");
		cJSON *arrEvents = cJSON_GetObjectItem(evBloc, "events");

		if (!valX || !valY || !arrEvents || !cJSON_IsNumber(valX) || !cJSON_IsNumber(valY) || !cJSON_IsArray(arrEvents)) {
			LOG_ERROR("[ERREUR] Bloc d'evenement incomplet ignore dans %s", nomFichierME);
			continue;
		}

		cJSON *evElem = NULL;
		cJSON_ArrayForEach(evElem, arrEvents) { // On lit les events un par un
			const int numPage = 0;
			const int xCase = valX->valueint;
			const int yCase = valY->valueint;
			event_result_t ev_res;

			cJSON *typeItem = cJSON_GetObjectItem(evElem, "type");
			if (!typeItem || !cJSON_IsString(typeItem)) { LOG_ERROR("[AVERTISSEMENT] Evenement sans type ignore (%s [%d,%d])", nomFichierME, xCase, yCase); continue; }
			const char *type = typeItem->valuestring;

			// --- Remplissage selon type ---
			if (!strcmp(type, "MSG")) {
				cJSON *texteItem = cJSON_GetObjectItem(evElem, "texte");
				if (!texteItem || !cJSON_IsString(texteItem)) { LOG_ERROR("[AVERTISSEMENT] MSG sans texte ignore dans %s", nomFichierME); continue; }
				const char *texte = texteItem->valuestring;

				event_t *ev_msg = event_creerMSG(texte, &ev_res);
				if (!ev_msg) { LOG_ERROR("Echec creation event_msg: %s", event_strerror(ev_res)); cJSON_Delete(jsonME); return CHARGEREVENTS_ERR_CREATE_EVENT; }
				carte_ajouterEvent(carte, numPage, xCase, yCase, ev_msg);
			}

			else if (!strcmp(type, "TP")) {
				cJSON *xDstItem = cJSON_GetObjectItem(evElem, "xDst");
				cJSON *yDstItem = cJSON_GetObjectItem(evElem, "yDst");
				cJSON *carteDstItem = cJSON_GetObjectItem(evElem, "carteDst");
				if (!xDstItem || !yDstItem || !carteDstItem || !cJSON_IsNumber(xDstItem) || !cJSON_IsNumber(yDstItem) || !cJSON_IsString(carteDstItem)) {
					LOG_ERROR("[AVERTISSEMENT] TP incomplet ignore dans %s", nomFichierME);
					continue;
				}
				const int xCaseDst = xDstItem->valueint, yCaseDst = yDstItem->valueint;
				const char *nomCarteDst = carteDstItem->valuestring;
				carte_t *carteDst = getCarte2(jeu, nomCarteDst);
				if (!carteDst) { LOG_ERROR("Carte de destination introuvable: %s", nomCarteDst); continue; }

				event_t *ev_tp = event_creerTP(xCaseDst, yCaseDst, carteDst, &ev_res);
				if (!ev_tp) { LOG_ERROR("Echec creation event_tp: %s", event_strerror(ev_res)); cJSON_Delete(jsonME); return CHARGEREVENTS_ERR_CREATE_EVENT; }
				carte_ajouterEvent(carte, numPage, xCase, yCase, ev_tp);
			}

			else if (!strcmp(type, "JouerMusique")) {
				cJSON *nomItem = cJSON_GetObjectItem(evElem, "nom");
				if (!nomItem || !cJSON_IsString(nomItem)) { LOG_ERROR("[AVERTISSEMENT] JouerMusique sans nom de musique ignore dans %s", nomFichierME); continue; }
				const char *nomMusique = nomItem->valuestring;

				musique_t *musique = getMusique2(jeu, nomMusique);
				if (!musique) { LOG_ERROR("Musique introuvable: %s", nomMusique); continue; }

				event_t *ev_jm = event_creerJM(musique, &ev_res);
				if (!ev_jm) { LOG_ERROR("Echec creation event_jm: %s", event_strerror(ev_res)); cJSON_Delete(jsonME); return CHARGEREVENTS_ERR_CREATE_EVENT; }
				carte_ajouterEvent(carte, numPage, xCase, yCase, ev_jm);
			}

			else if (!strcmp(type, "ArretMusique")) {
				event_t *ev_am = event_creerAM(&ev_res);
				if (!ev_am) { LOG_ERROR("Echec creation event_am: %s", event_strerror(ev_res)); cJSON_Delete(jsonME); return CHARGEREVENTS_ERR_CREATE_EVENT; }
				carte_ajouterEvent(carte, numPage, xCase, yCase, ev_am);
			}

			else if (!strcmp(type, "PV")) {
				cJSON *valItem = cJSON_GetObjectItem(evElem, "valeur");
				if (!valItem || !cJSON_IsNumber(valItem)) { LOG_ERROR("[AVERTISSEMENT] Modif PV sans valeur ignore dans %s", nomFichierME); continue; }
				const int val = valItem->valueint;

				event_t *ev_mpv = event_creerModifPV(val, &ev_res);
				if (!ev_mpv) { LOG_ERROR("Echec creation event_mpv: %s", event_strerror(ev_res)); cJSON_Delete(jsonME); return CHARGEREVENTS_ERR_CREATE_EVENT; }
				carte_ajouterEvent(carte, numPage, xCase, yCase, ev_mpv);
			}

			else if (!strcmp(type, "PM")) {
				cJSON *valItem = cJSON_GetObjectItem(evElem, "valeur");
				if (!valItem || !cJSON_IsNumber(valItem)) { LOG_ERROR("[AVERTISSEMENT] Modif PM sans valeur ignore dans %s", nomFichierME); continue; }
				const int val = valItem->valueint;

				event_t *ev_mpm = event_creerModifPM(val, &ev_res);
				if (!ev_mpm) { LOG_ERROR("Echec creation event_mpm: %s", event_strerror(ev_res)); cJSON_Delete(jsonME); return CHARGEREVENTS_ERR_CREATE_EVENT; }
				carte_ajouterEvent(carte, numPage, xCase, yCase, ev_mpm);
			}

			else if (!strcmp(type, "LVLUP")) {
				event_t *ev_lvlup = event_creerLVLUP(&ev_res);
				if (!ev_lvlup) { LOG_ERROR("Echec creation event_lvlup: %s", event_strerror(ev_res)); cJSON_Delete(jsonME); return CHARGEREVENTS_ERR_CREATE_EVENT; }
				carte_ajouterEvent(carte, numPage, xCase, yCase, ev_lvlup);
			}

			else {
				LOG_ERROR("[AVERTISSEMENT] Type inconnu \"%s\" ignore dans %s", type, nomFichierME);
			}
		}
	}
	cJSON_Delete(jsonME);
	return CHARGEREVENTS_OK;
}

chargerEvents_result_t chargerEvents_inject(jeu_t *jeu) {
	if (!jeu) return CHARGEREVENTS_ERR_NULL_POINTER_JEU;
	DIR *dir = opendir("cartes");
	if (!dir) return CHARGEREVENTS_ERR_NULL_POINTER_DIRECTORY;

	// ----- Détection automatique des cartes -----
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		if (!strstr(entry->d_name, "_ME.json")) { continue; }
		char nomCarte[128];
		strncpy(nomCarte, entry->d_name, sizeof(nomCarte) - 1);
		nomCarte[sizeof(nomCarte) - 1] = '\0';
		char *pos = strstr(nomCarte, "_ME.json");
		if (pos) *pos = '\0';

		chargerEvents_result_t chEv_res = json_lireEvents(nomCarte, jeu);
		if (chEv_res) { closedir(dir); return chEv_res; }
	}
	closedir(dir);
	return CHARGEREVENTS_OK;
}

const char * chargerEvents_strerror(chargerEvents_result_t res) {
	switch (res) {
		case CHARGEREVENTS_OK: return "Succes";
		case CHARGEREVENTS_ERR_NULL_POINTER_JEU: return "Jeu NULL passe en parametre";
		case CHARGEREVENTS_ERR_NULL_POINTER_DIRECTORY: return "Impossible d'ouvrir le dossier cartes";
		case CHARGEREVENTS_ERR_NULL_POINTER_CARTE: return "Echec obtention de la carte (introuvable dans le jeu)";
		case CHARGEREVENTS_ERR_READ_FILE_MURS_EVENTS: return "Echec lecture du fichier json ME (MursEvents)";
		case CHARGEREVENTS_ERR_CREATE_EVENT: return "Echec creation event";
		default: return "Erreur inconnue";
	}
}