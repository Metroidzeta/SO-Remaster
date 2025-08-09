// @author Alain Barbier alias "Metroidzeta"

#include "headers/chargerEvents.h"

static const event_info_t tabEvents[] = { // sera remplacé plus tard par la lecture réelle de fichiers (JSON dans le futur)
	// Création des messages { nomCarte, numPage, xCase, yCase, E_MSG, { .msg = { msg } } }
	{ "Sarosa", 0, 24, 10, E_MSG, { .msg = { "Les coups critiques sont affichés en rouge et infligent 200% de ton attaque de base." } } },
	{ "Coacville", 0, 11, 6, E_MSG, { .msg = { "Une odeur trop affreuse sort de cette maison... Je préfère rester dehors." } } },
	{ "Donjon1_salle4", 0, 29, 8, E_MSG, { .msg = { "Metroidzeta : Malheureusement l'aventure s'arrête ici." } } },
	{ "Donjon1_salle4", 0, 29, 8, E_MSG, { .msg = { "Metroidzeta : Le jeu est entièrement codé en C avec la bibliothèque SDL2, rien à voir avec RPG Maker." } } },
	{ "Donjon1_salle4", 0, 29, 8, E_MSG, { .msg = { "Metroidzeta : Est-ce que ça te plaît ? Tu peux participer au projet si tu le souhaites ;)" } } },

	// Création des téléportations { nomCarte, numPage, xCase, yCase, E_TP, { .tp = { xCaseDst, yCaseDst, nomCarteDst } } }
	{ "Sarosa_Milice_Accueil", 0, 8, 3, E_TP, { .tp = { 11, 15, "Sarosa" } } },
	{ "Sarosa", 0, 10, 15, E_TP, { .tp = { 8, 4, "Sarosa_Milice_Accueil" } } },
	{ "Sarosa", 0, 23, 0, E_TP, { .tp = { 25, 33, "Chateau_Roland_Exterieur" } } },
	{ "Sarosa", 0, 49, 27, E_TP, { .tp = { 1, 13, "Sarosa_Foret_Est" } } },
	{ "Sarosa", 0, 26, 49, E_TP, { .tp = { 28, 1, "Foret_Sud_Sarosa" } } },
	{ "Chateau_Roland_Exterieur", 0, 25, 34, E_TP, { .tp = { 23, 1, "Sarosa" } } },
	{ "Chateau_Roland_Exterieur", 0, 25, 31, E_TP, { .tp = { 7, 18, "Chateau_Roland_Cour_Interieure" } } },
	{ "Chateau_Roland_Exterieur", 0, 25, 6, E_TP, { .tp = { 7, 5, "Chateau_Roland_Cour_Interieure" } } },
	{ "Chateau_Roland_Exterieur", 0, 25, 21, E_TP, { .tp = { 7, 9, "Chateau_Roland_Etage_01" } } },
	{ "Chateau_Roland_Cour_Interieure", 0, 7, 19, E_TP, { .tp = { 25, 32, "Chateau_Roland_Exterieur" } } },
	{ "Chateau_Roland_Cour_Interieure", 0, 0, 7, E_TP, { .tp = { 1, 9, "Chateau_Roland_Etage_01" } } },
	{ "Chateau_Roland_Cour_Interieure", 0, 0, 8, E_TP, { .tp = { 1, 9, "Chateau_Roland_Etage_01" } } },
	{ "Chateau_Roland_Cour_Interieure", 0, 14, 7, E_TP, { .tp = { 13, 9, "Chateau_Roland_Etage_01" } } },
	{ "Chateau_Roland_Cour_Interieure", 0, 14, 8, E_TP, { .tp = { 13, 9, "Chateau_Roland_Etage_01" } } },
	{ "Chateau_Roland_Cour_Interieure", 0, 14, 13, E_TP, { .tp = { 1, 17, "Arene_Hunter" } } },
	{ "Chateau_Roland_Cour_Interieure", 0, 14, 14, E_TP, { .tp = { 1, 17, "Arene_Hunter" } } },
	{ "Chateau_Roland_Cour_Interieure", 0, 7, 4, E_TP, { .tp = { 25, 5, "Chateau_Roland_Exterieur" } } },
	{ "Chateau_Roland_Etage_01", 0, 0, 10, E_TP, { .tp = { 1, 9, "Chateau_Roland_Cour_Interieure" } } },
	{ "Chateau_Roland_Etage_01", 0, 14, 10, E_TP, { .tp = { 13, 9, "Chateau_Roland_Cour_Interieure" } } },
	{ "Chateau_Roland_Etage_01", 0, 7, 10, E_TP, { .tp = { 25, 22, "Chateau_Roland_Exterieur" } } },
	{ "Chateau_Roland_Etage_01", 0, 6, 3, E_TP, { .tp = { 9, 20, "Chateau_Roland_Salle_Trone_Nouveau" } } },
	{ "Chateau_Roland_Etage_01", 0, 7, 3, E_TP, { .tp = { 9, 20, "Chateau_Roland_Salle_Trone_Nouveau" } } },
	{ "Chateau_Roland_Etage_01", 0, 8, 3, E_TP, { .tp = { 9, 20, "Chateau_Roland_Salle_Trone_Nouveau" } } },
	{ "Chateau_Roland_Salle_Trone", 0, 8, 20, E_TP, { .tp = { 1, 7, "Chateau_Roland_Cour_Interieure" } } },
	{ "Chateau_Roland_Salle_Trone", 0, 9, 20, E_TP, { .tp = { 1, 7, "Chateau_Roland_Cour_Interieure" } } },
	{ "Chateau_Roland_Salle_Trone", 0, 10, 20, E_TP, { .tp = { 1, 7, "Chateau_Roland_Cour_Interieure" } } },
	{ "Chateau_Roland_Salle_Trone_Nouveau", 0, 8, 21, E_TP, { .tp = { 7, 4, "Chateau_Roland_Etage_01" } } },
	{ "Chateau_Roland_Salle_Trone_Nouveau", 0, 9, 21, E_TP, { .tp = { 7, 4, "Chateau_Roland_Etage_01" } } },
	{ "Chateau_Roland_Salle_Trone_Nouveau", 0, 10, 21, E_TP, { .tp = { 7, 4, "Chateau_Roland_Etage_01" } } },
	{ "Sarosa_Foret_Est", 0, 0, 13, E_TP, { .tp = { 48, 27, "Sarosa" } } },
	{ "Sarosa_Foret_Est", 0, 25, 29, E_TP, { .tp = { 19, 1, "Marais_Coacville" } } },
	{ "Marais_Coacville", 0, 19, 0, E_TP, { .tp = { 25, 28, "Sarosa_Foret_Est" } } },
	{ "Marais_Coacville", 0, 69, 42, E_TP, { .tp = { 1, 15, "Coacville_Marais_Sud" } } },
	{ "Coacville_Marais_Sud", 0, 0, 15, E_TP, { .tp = { 68, 42, "Marais_Coacville" } } },
	{ "Coacville_Marais_Sud", 0, 8, 5, E_TP, { .tp = { 17, 38, "Coacville_Marecage_Sud" } } },
	{ "Coacville_Marecage_Sud", 0, 17, 39, E_TP, { .tp = { 8, 6, "Coacville_Marais_Sud" } } },
	{ "Coacville_Marecage_Sud", 0, 15, 0, E_TP, { .tp = { 16, 38, "Coacville" } } },
	{ "Coacville", 0, 16, 39, E_TP, { .tp = { 15, 1, "Coacville_Marecage_Sud" } } },
	{ "Coacville", 0, 18, 0, E_TP, { .tp = { 22, 48, "Coacville_Marecage_Nord" } } },
	{ "Coacville_Marecage_Nord", 0, 22, 49, E_TP, { .tp = { 18, 1, "Coacville" } } },
	{ "Coacville_Marecage_Nord", 0, 20, 0, E_TP, { .tp = { 11, 38, "Coacville_Donjon_Exterieur" } } },
	{ "Coacville_Donjon_Exterieur", 0, 11, 39, E_TP, { .tp = { 20, 1, "Coacville_Marecage_Nord" } } },
	{ "Coacville_Donjon_Exterieur", 0, 11, 5, E_TP, { .tp = { 9, 13, "Donjon1_Entree" } } },
	{ "Coacville_Donjon_Exterieur", 0, 0, 9, E_TP, { .tp = { 18, 9, "carte18" } } },
	{ "Donjon1_Entree", 0, 8, 14, E_TP, { .tp = { 11, 6, "Coacville_Donjon_Exterieur" } } },
	{ "Donjon1_Entree", 0, 9, 14, E_TP, { .tp = { 11, 6, "Coacville_Donjon_Exterieur" } } },
	{ "Donjon1_Entree", 0, 10, 14, E_TP, { .tp = { 11, 6, "Coacville_Donjon_Exterieur" } } },
	{ "Donjon1_Entree", 0, 8, 3, E_TP, { .tp = { 7, 13, "Donjon1_salle5" } } },
	{ "Donjon1_salle4", 0, 13, 14, E_TP, { .tp = { 7, 4, "Donjon1_salle5" } } },
	{ "Donjon1_salle4", 0, 14, 14, E_TP, { .tp = { 7, 4, "Donjon1_salle5" } } },
	{ "Donjon1_salle4", 0, 15, 14, E_TP, { .tp = { 7, 4, "Donjon1_salle5" } } },
	{ "Donjon1_salle5", 0, 7, 3, E_TP, { .tp = { 14, 13, "Donjon1_salle4" } } },
	{ "Donjon1_salle5", 0, 6, 14, E_TP, { .tp = { 8, 4, "Donjon1_Entree" } } },
	{ "Donjon1_salle5", 0, 7, 14, E_TP, { .tp = { 8, 4, "Donjon1_Entree" } } },
	{ "Donjon1_salle5", 0, 8, 14, E_TP, { .tp = { 8, 4, "Donjon1_Entree" } } },
	{ "Arene_Hunter", 0, 0, 17, E_TP, { .tp = { 13, 14, "Chateau_Roland_Cour_Interieure" } } },
	{ "Arene_Hunter", 0, 28, 17, E_TP, { .tp = { 1, 9, "carte18" } } },
	{ "carte18", 0, 19, 9, E_TP, { .tp = { 1, 9, "Coacville_Donjon_Exterieur" } } },
	{ "carte18", 0, 0, 9, E_TP, { .tp = { 27, 17, "Arene_Hunter" } } },
	{ "Foret_Sud_Sarosa", 0, 28, 0, E_TP, { .tp = { 26, 48, "Sarosa" } } },

	// Création des jouer/arrêter musiques (avec message) { nomCarte, numPage, xCase, yCase, E_JM/E_AM, .jm/.am = { nomFichier/rien } }
	{ "Sarosa", 0, 6, 17, E_MSG, { .msg = { "La musique \"Hunter\" sera joué après ce message" } } },
	{ "Sarosa", 0, 6, 17, E_JM, { .jm = { "hunter.ogg" } } },
	{ "Sarosa", 0, 6, 17, E_MSG, { .msg = { "Arrêt de la musique \"Hunter\" après ce message" } } },
	{ "Sarosa", 0, 6, 17, E_AM, { .am = {} } },
	{ "Sarosa", 0, 6, 17, E_MSG, { .msg = { "On remet la musique de \"Sarosa\" après ce message" } } },
	{ "Sarosa", 0, 6, 17, E_JM, { .jm = { "Sarosa.mp3" } } },

	// Création des modifications de PV du héros { nomCarte, numPage, xCase, yCase, E_CPV, { valeur } }
	{ "Arene_Hunter", 0, 6, 8, E_CPV, { .cpv = { -15 } } },
	{ "Arene_Hunter", 0, 22, 8, E_CPV, { .cpv = { 15 } } },

	// Création des modifications de PM du héros { nomCarte, numPage, xCase, yCase, E_CPM, { valeur } }
	{ "Arene_Hunter", 0, 6, 26, E_CPM, { .cpm = { -6 } } },
	{ "Arene_Hunter", 0, 22, 26, E_CPM, { .cpm = { 6 } } }
};

static carte_t * getCarte(arraylist_t *cartes, const char *nom) { // Ne pas libérer les cartes : partagée, allouée ailleurs
	for (int i = 0; i < cartes->taille; ++i) {
		carte_t *carte = arraylist_get(cartes, i);
		if (strcmp(carte->nom, nom) == 0) return carte;
	}
	return NULL;
}

static musique_t * getMusique(arraylist_t *musiques, const char *nom) { // Ne pas libérer les musiques : partagée, allouée ailleurs
	for (int i = 0; i < musiques->taille; ++i) {
		musique_t *musique = arraylist_get(musiques, i);
		if (strcmp(musique->nom, nom) == 0) return musique;
	}
	return NULL;
}

static chargerEvents_result_t ajouterEventMsg(const event_info_t *elem, carte_t *carte) {
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

static chargerEvents_result_t ajouterEvent(const event_info_t *elem, arraylist_t *cartes, arraylist_t *musiques) {
	if (!elem) return CHARGEREVENTS_ERR_NULL_ELEM;
	carte_t *carte = NULL;
	if (elem->nomCarte != NULL) { // une carte NULL = refusé -> à l'ajout de l'event dans la carte
		carte = getCarte(cartes, elem->nomCarte);
		if (!carte) { LOG_ERROR("Carte inexistante : %s", elem->nomCarte); return CHARGEREVENTS_ERR_GET_CARTE; }
	}

	switch (elem->type) {
		case E_MSG: return ajouterEventMsg(elem, carte);
		case E_TP: return ajouterEventTP(elem, carte, cartes);
		case E_JM: return ajouterEventJM(elem, carte, musiques);
		case E_AM: return ajouterEventAM(elem, carte);
		case E_CPV: return ajouterEventCPV(elem, carte);
		case E_CPM: return ajouterEventCPM(elem, carte);
		default: { LOG_ERROR("Type d'evenement inconnu : %d", elem->type); return CHARGEREVENTS_ERR_CREATE_EVENT; }
	}
}

chargerEvents_result_t chargerEvents_get(arraylist_t *cartes, arraylist_t *musiques) {
	if (!cartes) return CHARGEREVENTS_ERR_NULL_POINTER_CARTES;
	if (!musiques) return CHARGEREVENTS_ERR_NULL_POINTER_MUSIQUES;

	const size_t nbEvents = sizeof(tabEvents) / sizeof(tabEvents[0]);
	for (size_t i = 0; i < nbEvents; ++i) {
		const event_info_t *elem = &tabEvents[i];
		chargerEvents_result_t resCEV = ajouterEvent(elem, cartes, musiques);
		if (resCEV != CHARGEREVENTS_OK) return resCEV;
	}
	return CHARGEREVENTS_OK;
}

const char * chargerEvents_strerror(chargerEvents_result_t res) {
	switch (res) {
		case CHARGEREVENTS_OK: return "Succes";
		case CHARGEREVENTS_ERR_NULL_POINTER_CARTES: return "Arraylist cartes NULL passe en parametre";
		case CHARGEREVENTS_ERR_NULL_POINTER_MUSIQUES: return "Arraylist musiques NULL passe en parametre";
		case CHARGEREVENTS_ERR_NULL_ELEM: return "Element NULL passe en parametre";
		case CHARGEREVENTS_ERR_GET_CARTE: return "Echec obtention carte";
		case CHARGEREVENTS_ERR_GET_CARTEDST: return "Echec obtention carte de destination pour event_tp";
		case CHARGEREVENTS_ERR_GET_MUSIQUE: return "Echec obtention musique";
		case CHARGEREVENTS_ERR_CREATE_EVENT: return "Echec creation event";
		default: return "Erreur";
	}
}