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

#include "headers/carte.h"
#include "headers/event.h"

static SDL_Rect matriceRectGlobale[TAILLE_CARTE_MAX][TAILLE_CARTE_MAX]; // matrice de rectangles partagée entres TOUTES les cartes (optimisation)
static bool matriceRectGlobaleInitialisee = false;

static void initMatriceRectGlobale(void) {
	for (int i = 0; i < TAILLE_CARTE_MAX; ++i) {
		const int y = i * TAILLE_CASES;
		for (int j = 0; j < TAILLE_CARTE_MAX; ++j) {
			matriceRectGlobale[i][j] = (SDL_Rect){ j * TAILLE_CASES, y, TAILLE_CASES, TAILLE_CASES };
		}
	}
	matriceRectGlobaleInitialisee = true;
}

static inline carte_result_t carte_validerArguments(const char *nom, int largeur, int hauteur, chipset_t *chipset) {
	if (!nom || !*nom) return CARTE_ERR_NULL_OR_EMPTY_NAME;
	if (strlen(nom) >= MAX_TAILLE_STRING) return CARTE_ERR_SIZE_MAX_NAME;
	if (largeur < 1 || largeur > TAILLE_CARTE_MAX) return CARTE_ERR_INVALID_LARGEUR;
	if (hauteur < 1 || hauteur > TAILLE_CARTE_MAX) return CARTE_ERR_INVALID_HAUTEUR;
	if (!chipset) return CARTE_ERR_NULL_CHIPSET;
	return CARTE_OK;
}

carte_t * carte_creer(const char *nom, int largeur, int hauteur, chipset_t *chipset, musique_t *musique, int **c0, int **c1, int**c2, bool**murs, carte_result_t *res) {
	if (!res) { LOG_ERROR("Enum carte_result NULL"); return NULL; }
	*res = CARTE_OK;
	if ((*res = carte_validerArguments(nom, largeur, hauteur, chipset))) return NULL;

	carte_t *carte = calloc(1, sizeof(carte_t));
	if (!carte) { *res = CARTE_ERR_MEMORY_BASE; return NULL; }

	carte->nom = my_strdup(nom); // important : ne pas faire "carte->nom = nom", car cela ne copie que le pointeur, pas le contenu
	if (!carte->nom) { carte_detruire(carte); *res = CARTE_ERR_MEMORY_NAME; return NULL; }

	carte->largeur = largeur;
	carte->hauteur = hauteur;
	carte->chipset = chipset;
	carte->musique = musique;

	int **couches[NB_COUCHES] = { c0, c1, c2 };
	for (int c = 0; c < NB_COUCHES; ++c) {
		if (couches[c]) carte->couches[c] = couches[c];
		else { // Allocation des couches
			carte->couches[c] = creerMatriceInt(hauteur, largeur, TUILE_VIDE);
			if (!carte->couches[c]) { carte_detruire(carte); *res = CARTE_ERR_MEMORY_LAYER; return NULL; }
		}
	}

	// Allocation des autres matrices
	if (murs) carte->murs = murs;
	else {
		carte->murs = creerMatriceBool(hauteur, largeur, false);
		if (!carte->murs) { carte_detruire(carte); *res = CARTE_ERR_MEMORY_WALLS; return NULL; }
	}

	carte->ensembleEvents = malloc(hauteur * sizeof(ensemble_events_t *));
	if (!carte->ensembleEvents) { carte_detruire(carte); *res = CARTE_ERR_MEMORY_SET_EVENTS; return NULL; }

	arraylist_result_t alMs_res;
	carte->monstres = arraylist_creer(AL_MONSTRE, &alMs_res);
	if (!carte->monstres) { carte_detruire(carte); LOG_ERROR("%s", arraylist_strerror(alMs_res)); *res = CARTE_ERR_CREATE_ARRAYLIST_MONSTERS; return NULL; }

	// Initialisation unique et affectation de la matriceRect
	if (!matriceRectGlobaleInitialisee) initMatriceRectGlobale();
	carte->matriceRect = matriceRectGlobale;

	for (int i = 0; i < hauteur; ++i) {
		carte->ensembleEvents[i] = malloc(largeur * sizeof(ensemble_events_t));
		if (!carte->ensembleEvents[i]) { carte_detruire(carte); *res = CARTE_ERR_MEMORY_SET_EVENTS_ROWS; return NULL; }

		for (int j = 0; j < largeur; ++j) {
			for (int p = 0; p < NB_PAGES_EVENT; ++p) {
				carte->ensembleEvents[i][j].events[p] = NULL;
				arraylist_result_t alEv_res;
				carte->ensembleEvents[i][j].events[p] = arraylist_creer(AL_EVENT, &alEv_res);
				if (!carte->ensembleEvents[i][j].events[p]) {
					carte_detruire(carte); LOG_ERROR("%s", arraylist_strerror(alEv_res)); *res = CARTE_ERR_CREATE_ARRAYLIST_EVENTS; return NULL;
				}
			}
		}
	}

	if (!c0 && !c1 && !c2 && !murs) carte_ecrireFichier(carte);
	return carte;
}

carte_t * carte_creerVide(const char *nom, int largeur, int hauteur, chipset_t *chipset, musique_t *musique, carte_result_t *res) {
	return carte_creer(nom, largeur, hauteur, chipset, musique, NULL, NULL, NULL, NULL, res);
}

static cJSON *cJSON_matriceInt(int **mat, carte_t *carte) {
	cJSON *arr = cJSON_CreateArray();
	for (int i = 0; i < carte->hauteur; ++i) {
		cJSON *row = cJSON_CreateArray();
		for (int j = 0; j < carte->largeur; ++j) {
			cJSON_AddItemToArray(row, cJSON_CreateNumber(mat[i][j]));
		}
		cJSON_AddItemToArray(arr, row);
	}
	return arr;
}

static cJSON *cJSON_matriceBool(bool **mat, carte_t *carte) {
	cJSON *arr = cJSON_CreateArray();
	for (int i = 0; i < carte->hauteur; ++i) {
		cJSON *row = cJSON_CreateArray();
		for (int j = 0; j < carte->largeur; ++j) {
			cJSON_AddItemToArray(row, cJSON_CreateBool(mat[i][j]));
		}
		cJSON_AddItemToArray(arr, row);
	}
	return arr;
}

static cJSON *cJSON_event_MSG(EventData_t data) {
	cJSON *obj = cJSON_CreateObject();
	cJSON_AddStringToObject(obj, "type", "MSG");
	cJSON_AddStringToObject(obj, "texte", data.msg);
	return obj;
}

static cJSON *cJSON_event_TP(EventData_t data) {
	cJSON *obj = cJSON_CreateObject();
	cJSON_AddStringToObject(obj, "type", "TP");
	cJSON_AddNumberToObject(obj, "xDst", data.tp.xDst / TAILLE_CASES);
	cJSON_AddNumberToObject(obj, "yDst", data.tp.yDst / TAILLE_CASES);
	cJSON_AddStringToObject(obj, "carteDst", data.tp.carteDst->nom);
	return obj;
}

static cJSON *cJSON_event_JM(EventData_t data) {
	cJSON *obj = cJSON_CreateObject();
	cJSON_AddStringToObject(obj, "type", "JouerMusique");
	cJSON_AddStringToObject(obj, "nom", data.musique->nom);
	return obj;
}

static cJSON *cJSON_event_AM(void) {
	cJSON *obj = cJSON_CreateObject();
	cJSON_AddStringToObject(obj, "type", "ArretMusique");
	return obj;
}

static cJSON *cJSON_event_MPV(EventData_t data) {
	cJSON *obj = cJSON_CreateObject();
	cJSON_AddStringToObject(obj, "type", "PV");
	cJSON_AddNumberToObject(obj, "valeur", data.val_PV);
	return obj;
}

static cJSON *cJSON_event_MPM(EventData_t data) {
	cJSON *obj = cJSON_CreateObject();
	cJSON_AddStringToObject(obj, "type", "PM");
	cJSON_AddNumberToObject(obj, "valeur", data.val_PM);
	return obj;
}

static cJSON *cJSON_event_LVLUP(void) {
	cJSON *obj = cJSON_CreateObject();
	cJSON_AddStringToObject(obj, "type", "LVLUP");
	return obj;
}

static cJSON *cJSON_events(carte_t *carte) {
	cJSON *arr = cJSON_CreateArray();

	for (int i = 0; i < carte->hauteur; ++i) {
		for (int j = 0; j < carte->largeur; ++j) {
			arraylist_t *listeEvents = carte->ensembleEvents[i][j].events[0];
			if (!arraylist_isEmpty(listeEvents)) {
				cJSON *eventsObj = cJSON_CreateObject();
				cJSON_AddNumberToObject(eventsObj, "x", j);
				cJSON_AddNumberToObject(eventsObj, "y", i);

				cJSON *eventsArray = cJSON_CreateArray();
				for (int k = 0; k < listeEvents->taille; ++k) {
					event_t *ev = arraylist_get(listeEvents, k);
					cJSON *evJson = NULL;
					switch (ev->type) {
						case EVENT_MSG:            evJson = cJSON_event_MSG(ev->data); break;
						case EVENT_TP:             evJson = cJSON_event_TP(ev->data); break;
						case EVENT_JOUER_MUSIQUE:  evJson = cJSON_event_JM(ev->data); break;
						case EVENT_ARRET_MUSIQUE:  evJson = cJSON_event_AM(); break;
						case EVENT_MODIF_PV:       evJson = cJSON_event_MPV(ev->data); break;
						case EVENT_MODIF_PM:       evJson = cJSON_event_MPM(ev->data); break;
						case EVENT_LVLUP:          evJson = cJSON_event_LVLUP(); break;
						default: break;
					}
					if (evJson) cJSON_AddItemToArray(eventsArray, evJson);
				}

				cJSON_AddItemToObject(eventsObj, "events", eventsArray);
				cJSON_AddItemToArray(arr, eventsObj);
			}
		}
	}

	return arr;
}

void carte_ecrireFichier(carte_t *carte) {
	if (!carte) return;
	// ---------- Fichier Base + Couches ----------
	size_t len = strlen("cartes/") + strlen(carte->nom) + strlen("_BC.json") + 1; // + 1: \0
	char nomFichierBC[len];
	snprintf(nomFichierBC, sizeof(nomFichierBC), "cartes/%s_BC.json", carte->nom);

	cJSON *jsonBC = cJSON_CreateObject();
	cJSON_AddNumberToObject(jsonBC, "largeur", carte->largeur);
	cJSON_AddNumberToObject(jsonBC, "hauteur", carte->hauteur);
	cJSON_AddStringToObject(jsonBC, "chipset", carte->chipset->nom);
	cJSON_AddStringToObject(jsonBC, "musique", carte->musique->nom);
	cJSON_AddItemToObject(jsonBC, "couche0", cJSON_matriceInt(carte->couches[0], carte));
	cJSON_AddItemToObject(jsonBC, "couche1", cJSON_matriceInt(carte->couches[1], carte));
	cJSON_AddItemToObject(jsonBC, "couche2", cJSON_matriceInt(carte->couches[2], carte));

	FILE *f1 = fopen(nomFichierBC, "w");
	if (!f1) { perror("Erreur ouverture fichier couches"); cJSON_Delete(jsonBC); return; }

	printJSON_custom(f1, jsonBC, 0, false);
	fclose(f1);
	cJSON_Delete(jsonBC);

	// ---------- Fichier Murs + Events ----------
	len = strlen("cartes/") + strlen(carte->nom) + strlen("_ME.json") + 1; // + 1: \0
	char nomFichierME[len];
	snprintf(nomFichierME, sizeof(nomFichierME), "cartes/%s_ME.json", carte->nom);

	cJSON *jsonME = cJSON_CreateObject();
	cJSON_AddItemToObject(jsonME, "murs", cJSON_matriceBool(carte->murs, carte));
	cJSON_AddItemToObject(jsonME, "ensemblesEvents", cJSON_events(carte));

	FILE *f2 = fopen(nomFichierME, "w");
	if (!f2) { perror("Erreur ouverture fichier murs"); cJSON_Delete(jsonME); return; }

	printJSON_custom(f2, jsonME, 0, false);
	fclose(f2);
	cJSON_Delete(jsonME);
}

static void calculerBornesCollisions(const carte_t *carte, const SDL_Rect *rect, int *x0, int *x1, int *y0, int *y1) {
	*x0 = maxInt(0, rect->x / TAILLE_CASES);
	*x1 = minInt(carte->largeur, (rect->x + rect->w - 1) / TAILLE_CASES + 1);
	*y0 = maxInt(0, rect->y / TAILLE_CASES);
	*y1 = minInt(carte->hauteur, (rect->y + rect->h - 1) / TAILLE_CASES + 1);
	//printf("x0 = %d, x1 = %d, y0 = %d, y1 = %d\n", x0, x1, y0, y1);
}

bool carte_verifierCollisionsMurs(carte_t *carte, SDL_Rect *hitBox) {
	if (!carte || !hitBox) return false;
	int x0, x1, y0, y1;
	calculerBornesCollisions(carte, hitBox, &x0, &x1, &y0, &y1);
	for (int i = y0; i < y1; ++i) {
		for (int j = x0; j < x1; ++j) {
			if (carte->murs[i][j] && SDL_HasIntersection(hitBox, &carte->matriceRect[i][j])) return true;
		}
	}
	return false;
}

arraylist_t * carte_verifierCollisionsEvents(carte_t *carte, SDL_Rect *hitBox) {
	if (!carte || !hitBox) return NULL;
	int x0, x1, y0, y1;
	calculerBornesCollisions(carte, hitBox, &x0, &x1, &y0, &y1);
	for (int i = y0; i < y1; ++i) {
		for (int j = x0; j < x1; ++j) {
			if (!arraylist_isEmpty(carte->ensembleEvents[i][j].events[0]) && SDL_HasIntersection(hitBox, &carte->matriceRect[i][j])) {
				return carte->ensembleEvents[i][j].events[0];
			}
		}
	}
	return NULL;
}

void carte_ajouterEvent(carte_t *carte, int numPage, int xCase, int yCase, event_t *ev) {
	if (!carte || !ev) return;
	if (numPage < 0 || numPage >= NB_PAGES_EVENT) Exception("numPage event ajoute < 0 ou >= NB_PAGES_EVENT");
	if (xCase < 0 || xCase >= carte->largeur) Exception("xCase event ajoute < 0 ou >= largeur carte");
	if (yCase < 0 || yCase >= carte->hauteur) Exception("yCase event ajoute < 0 ou >= hauteur carte");

	arraylist_add(carte->ensembleEvents[yCase][xCase].events[numPage], ev);
}

void carte_ajouterMonstre(carte_t *carte, monstre_t *monstre) {
	if (!carte || !monstre) return;
	if (monstre_getXCase(monstre) >= carte->largeur) Exception("xCase monstre >= largeur carte");
	if (monstre_getYCase(monstre) >= carte->hauteur) Exception("yCase monstre >= hauteur carte");

	arraylist_add(carte->monstres, monstre);
}

void carte_detruire(carte_t *carte) { // Ne pas libérer carte->chipset et carte->musique : partagée, allouée ailleurs
	if (!carte) return;
	for (int i = 0; i < carte->hauteur; ++i) {
		for (int j = 0; j < carte->largeur; ++j) {
			for (int p = 0; p < NB_PAGES_EVENT; ++p) {
				arraylist_detruire(carte->ensembleEvents[i][j].events[p], true); carte->ensembleEvents[i][j].events[p] = NULL;
			}
		}
		free(carte->ensembleEvents[i]); carte->ensembleEvents[i] = NULL;
	}
	free(carte->ensembleEvents); carte->ensembleEvents = NULL;
	arraylist_detruire(carte->monstres, true); carte->monstres = NULL;
	freeMatriceBool(carte->murs); carte->murs = NULL;
	for (int c = 0; c < NB_COUCHES; ++c) { freeMatriceInt(carte->couches[c]); carte->couches[c] = NULL; }
	free(carte->nom); carte->nom = NULL;
	free(carte);
}

const char * carte_strerror(carte_result_t res) {
	switch (res) {
		case CARTE_OK: return "Succes";
		case CARTE_ERR_NULL_OR_EMPTY_NAME: return "Nom NULL ou vide";
		case CARTE_ERR_SIZE_MAX_NAME: return "Nom trop long";
		case CARTE_ERR_INVALID_LARGEUR: return "Largeur < 1 ou > TAILLE_CARTE_MAX";
		case CARTE_ERR_INVALID_HAUTEUR: return "Hauteur < 1 ou > TAILLE_CARTE_MAX";
		case CARTE_ERR_NULL_CHIPSET: return "Chipset NULL en argument";
		case CARTE_ERR_MEMORY_BASE: return "Echec allocation memoire base";
		case CARTE_ERR_MEMORY_NAME: return "Echec allocation memoire nom";
		case CARTE_ERR_MEMORY_LAYER: return "Echec allocation memoire matrice d'une couche";
		case CARTE_ERR_MEMORY_WALLS: return "Echec allocation memoire matrice des murs";
		case CARTE_ERR_MEMORY_SET_EVENTS: return "Echec allocation memoire matrice ensembleEvents";
		case CARTE_ERR_CREATE_ARRAYLIST_MONSTERS: return "Echec creation arraylist des monstres";
		case CARTE_ERR_MEMORY_SET_EVENTS_ROWS: return "Echec allocation memoire des lignes de la matrice ensembleEvents";
		case CARTE_ERR_CREATE_ARRAYLIST_EVENTS: return "Echec creation arraylist des events";
		default: return "Erreur inconnue";
	}
}