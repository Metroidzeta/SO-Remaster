// @author Alain Barbier alias "Metroidzeta"

#include "headers/carte.h"

#include "headers/event_msg.h"
#include "headers/event_tp.h"
#include "headers/event_jm.h"
#include "headers/event_changePV.h"
#include "headers/event_changePM.h"

static carte_result_t carte_validerArguments(const char *nom, int largeur, int hauteur, chipset_t *chipset) {
    if (!nom || !*nom) return CARTE_ERR_NULL_OR_EMPTY_NAME;
	if (strlen(nom) >= MAX_TAILLE_STRING) return CARTE_ERR_SIZE_MAX_NAME;
    if (largeur < 1 || largeur > TAILLE_CARTE_MAX) return CARTE_ERR_INVALID_LARGEUR;
    if (hauteur < 1 || hauteur > TAILLE_CARTE_MAX) return CARTE_ERR_INVALID_HAUTEUR;
    if (!chipset) return CARTE_ERR_NULL_CHIPSET;
	return CARTE_OK;
}

static SDL_Rect matriceRectGlobale[TAILLE_CARTE_MAX][TAILLE_CARTE_MAX]; // matrice de rectangles partagée entres TOUTES les cartes (optimisation)
static bool matriceRectInitialisee = false;

static void initMatriceRectGlobale() {
    for (int i = 0; i < TAILLE_CARTE_MAX; ++i) {
		const int y = i * TAILLE_CASES;
        for (int j = 0; j < TAILLE_CARTE_MAX; ++j) {
            matriceRectGlobale[i][j] = (SDL_Rect){ j * TAILLE_CASES, y, TAILLE_CASES, TAILLE_CASES };
        }
    }
    matriceRectInitialisee = true;
}

carte_result_t carte_creer(carte_t **out_carte, const char *nom, int largeur, int hauteur, chipset_t *chipset, musique_t *musique, int **c0, int **c1, int**c2, bool**murs) {
	if (!out_carte) return CARTE_ERR_NULL_POINTER;
	*out_carte = NULL;

	carte_result_t res = carte_validerArguments(nom, largeur, hauteur, chipset);
	if (res != CARTE_OK) return res;

	carte_t *carte = calloc(1, sizeof(carte_t));
	if (!carte) return CARTE_ERR_MEMORY_BASE;

	carte->nom = my_strdup(nom); // important : ne pas faire "carte->nom = nom", car cela ne copie que le pointeur, pas le contenu
	if (!carte->nom) { carte_detruire(carte); return CARTE_ERR_MEMORY_NAME; }

	carte->largeur = largeur;
	carte->hauteur = hauteur;
	carte->chipset = chipset;
	carte->musique = musique;

	int **couches[3] = { c0, c1, c2 };
	// Allocation des couches
	for (int c = 0; c < 3; ++c) {
		if (couches[c]) carte->couches[c] = couches[c];
		else {
			carte->couches[c] = creerMatriceINT(hauteur, largeur, -1);
			if (!carte->couches[c]) { carte_detruire(carte); return CARTE_ERR_MEMORY_LAYER; }
		}
	}

	// Allocation des autres matrices
	if (murs) carte->murs = murs;
	else {
		carte->murs = creerMatriceBOOL(hauteur, largeur, false);
		if (!carte->murs) { carte_detruire(carte); return CARTE_ERR_MEMORY_WALLS; }
	}

	carte->ensembleEvents = malloc(hauteur * sizeof(ensemble_events_t *));
	if (!carte->ensembleEvents) { carte_detruire(carte); return CARTE_ERR_MEMORY_SET_EVENTS; }

	arraylist_result_t resAL = arraylist_creer(&carte->monstres, AL_MONSTRE);
	if (resAL != ARRAYLIST_OK) { carte_detruire(carte); return CARTE_ERR_CREATE_ARRAYLIST_MONSTERS; }

	// Initialisation unique et affectation de la matriceRect
    if (!matriceRectInitialisee) initMatriceRectGlobale();
    carte->matriceRect = matriceRectGlobale;

	for (int i = 0; i < hauteur; ++i) {
		carte->ensembleEvents[i] = malloc(largeur * sizeof(ensemble_events_t));
		if (!carte->ensembleEvents[i]) { carte_detruire(carte); return CARTE_ERR_MEMORY_SET_EVENTS_ROWS; }

		for (int j = 0; j < largeur; ++j) {
			for (int p = 0; p < NB_PAGES_EVENT; ++p) {
				carte->ensembleEvents[i][j].events[p] = NULL;
				resAL = arraylist_creer(&carte->ensembleEvents[i][j].events[p], AL_EVENT);
				if (resAL != ARRAYLIST_OK) { carte_detruire(carte); return CARTE_ERR_CREATE_ARRAYLIST_EVENTS; }
			}
		}
	}

	if (!c0 && !c1 && !c2 && !murs) carte_ecrireFichier(carte);
	*out_carte = carte;
	return CARTE_OK;
}

carte_result_t carte_creerVide(carte_t **out_carte, const char *nom, int largeur, int hauteur, chipset_t *chipset, musique_t *musique) {
	return carte_creer(out_carte, nom, largeur, hauteur, chipset, musique, NULL, NULL, NULL, NULL);
}

carte_result_t carte_creerTiled(carte_t **out_carte, const char *nom, int largeur, int hauteur, chipset_t *chipset, musique_t *musique, int **c0, int **c1, int**c2, bool**murs) {
	carte_result_t res = carte_creer(out_carte, nom, largeur, hauteur, chipset, musique, c0, c1, c2, murs);
	if (res != CARTE_OK) return res;
	carte_t *carte = *out_carte;
	for (int c = 0; c < 3; ++c) {
		for (int i = 0; i < carte->hauteur; ++i) {
			for (int j = 0; j < carte->largeur; ++j) {
				carte->couches[c][i][j]--;
			}
		}
	}
	return CARTE_OK;
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

static cJSON *cJSON_event_MSG(event_msg_t *ev_msg) {
	cJSON *obj = cJSON_CreateObject();
	cJSON_AddStringToObject(obj, "msg", ev_msg->msg);
	return obj;
}

static cJSON *cJSON_event_TP(event_tp_t *ev_tp) {
	cJSON *obj = cJSON_CreateObject();
	cJSON *tp = cJSON_CreateObject();
	cJSON_AddNumberToObject(tp, "xDst", ev_tp->xDst / TAILLE_CASES);
	cJSON_AddNumberToObject(tp, "yDst", ev_tp->yDst / TAILLE_CASES);
	cJSON_AddStringToObject(tp, "carteDst", ev_tp->carteDst->nom);
	cJSON_AddItemToObject(obj, "tp", tp);
	return obj;
}

static cJSON *cJSON_event_JM(event_jm_t *ev_jm) {
	cJSON *obj = cJSON_CreateObject();
	cJSON_AddStringToObject(obj, "musique", ev_jm->musique->nom);
	return obj;
}

static cJSON *cJSON_event_AM() {
	cJSON *obj = cJSON_CreateObject();
	cJSON_AddStringToObject(obj, "arretMusique", "");
	return obj;
}

static cJSON *cJSON_event_CPV(event_changePV_t *ev_cPV) {
	cJSON *obj = cJSON_CreateObject();
	cJSON_AddNumberToObject(obj, "PV", ev_cPV->PV);
	return obj;
}

static cJSON *cJSON_event_CPM(event_changePM_t *ev_cPM) {
	cJSON *obj = cJSON_CreateObject();
	cJSON_AddNumberToObject(obj, "PM", ev_cPM->PM);
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
						case E_MSG: evJson = cJSON_event_MSG((event_msg_t *)ev->ptr); break;
						case E_TP:  evJson = cJSON_event_TP((event_tp_t *)ev->ptr); break;
						case E_JM:  evJson = cJSON_event_JM((event_jm_t *)ev->ptr); break;
						case E_AM:  evJson = cJSON_event_AM(); break;
						case E_CPV: evJson = cJSON_event_CPV((event_changePV_t *)ev->ptr); break;
						case E_CPM: evJson = cJSON_event_CPM((event_changePM_t *)ev->ptr); break;
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
	*x1 = minInt(carte->largeur - 1, (rect->x + rect->w) / TAILLE_CASES);
	*y0 = maxInt(0, rect->y / TAILLE_CASES);
	*y1 = minInt(carte->hauteur - 1, (rect->y + rect->h) / TAILLE_CASES);
	//printf("x0 = %d, x1 = %d, y0 = %d, y1 = %d\n", x0, x1, y0, y1);
}

bool carte_verifierCollisionsMurs(carte_t *carte, SDL_Rect *hitBox) {
	int x0, x1, y0, y1;
	calculerBornesCollisions(carte, hitBox, &x0, &x1, &y0, &y1);
	for (int i = y0; i <= y1; ++i) {
		for (int j = x0; j <= x1; ++j) {
			if (carte->murs[i][j] && SDL_HasIntersection(hitBox, &carte->matriceRect[i][j])) return true;
		}
	}
	return false;
}

arraylist_t * carte_verifierCollisionsEvents(carte_t *carte, SDL_Rect *hitBox) {
	int x0, x1, y0, y1;
	calculerBornesCollisions(carte, hitBox, &x0, &x1, &y0, &y1);
	for (int i = y0; i <= y1; ++i) {
		for (int j = x0; j <= x1; ++j) {
			if (!arraylist_isEmpty(carte->ensembleEvents[i][j].events[0]) && SDL_HasIntersection(hitBox, &carte->matriceRect[i][j])) {
				return carte->ensembleEvents[i][j].events[0];
			}
		}
	}
	return NULL;
}

void carte_ajouterEvent(carte_t *carte, int numPage, int xCase, int yCase, e_type type, void *evtPtr) {
	if (!carte) Exception("Event ajoute sur une carte NULL");
	if (xCase < 0 || xCase >= carte->largeur) Exception("xCase event ajoute < 0 ou >= largeur carte");
	if (yCase < 0 || yCase >= carte->hauteur) Exception("yCase event ajoute < 0 ou >= hauteur carte");
	if (numPage < 0 || numPage >= NB_PAGES_EVENT) Exception("NumPage event ajoute < 0 ou >= NB_PAGES_EVENT");

	event_t *e = event_creer(type, evtPtr);
	arraylist_add(carte->ensembleEvents[yCase][xCase].events[numPage], e);
}

void carte_ajouterMonstre(carte_t *carte, monstre_t *monstre) {
	if (!monstre) Exception("Monstre NULL ajoute");
	if (monstre_getXCase(monstre) >= carte->largeur) Exception("xCase monstre >= largeur carte");
	if (monstre_getYCase(monstre) >= carte->hauteur) Exception("yCase monstre >= hauteur carte");

	arraylist_add(carte->monstres, monstre);
}

void carte_detruire(carte_t *carte) { // Ne pas libérer carte->chipset et carte->musique : partagée, allouée ailleurs
	if (!carte) return;
	for (int i = 0; i < carte->hauteur; ++i) {
		for (int j = 0; j < carte->largeur; ++j) {
			for (int p = 0; p < NB_PAGES_EVENT; ++p) {
				arraylist_detruire(carte->ensembleEvents[i][j].events[p], true);
			}
		}
		free(carte->ensembleEvents[i]);
	}
	free(carte->ensembleEvents);
	arraylist_detruire(carte->monstres, true);
	freeMatriceBOOL(carte->murs);
	for (int c = 0; c < 3; ++c) freeMatriceINT(carte->couches[c]);
	free(carte->nom);
	free(carte);
}

const char * carte_strerror(carte_result_t res) {
	switch (res) {
		case CARTE_OK: return "Succes";
		case CARTE_ERR_NULL_POINTER: return "Carte NULL passe en parametre";
		case CARTE_ERR_NULL_OR_EMPTY_NAME: return "Nom NULL ou vide";
		case CARTE_ERR_SIZE_MAX_NAME: return "Nom trop long";
		case CARTE_ERR_INVALID_LARGEUR: return "Largeur < 1 ou > TAILLE_CARTE_MAX";
		case CARTE_ERR_INVALID_HAUTEUR: return "Hauteur < 1 ou > TAILLE_CARTE_MAX";
		case CARTE_ERR_NULL_CHIPSET: return "Chipset NULL passe en parametre";
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