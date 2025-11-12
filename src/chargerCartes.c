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
#include "headers/chargerCartes.h"
#include "headers/carte.h"

static int ** parseMatriceInt(cJSON *array, int hauteur, int largeur) {
	if (!array || !cJSON_IsArray(array)) { LOG_ERROR("parseMatriceInt: objet JSON invalide"); return NULL; }
	int **matrice = creerMatriceInt(hauteur, largeur, 0);
	if (!matrice) { LOG_ERROR("parseMatriceInt: echec allocation matrice"); return NULL; }

	int i = 0;
	cJSON *ligne;
	cJSON_ArrayForEach(ligne, array) {
		if (i >= hauteur) break; // sécurité
		int j = 0;
		cJSON *val = NULL;
		cJSON_ArrayForEach(val, ligne) {
			if (j >= largeur) break; // sécurité
			if (!cJSON_IsNumber(val)) { freeMatriceInt(matrice); LOG_ERROR("parseMatriceInt: valeur non numerique"); return NULL; }
			matrice[i][j] = val->valueint;
			j++;
		}
		i++;
	}
	return matrice;
}

static bool **parseMatriceBool(cJSON *array, int hauteur, int largeur) {
	if (!array || !cJSON_IsArray(array)) { LOG_ERROR("parseMatriceBool: objet JSON invalide"); return NULL; }
	bool **matrice = creerMatriceBool(hauteur, largeur, false);
	if (!matrice) { LOG_ERROR("parseMatriceBool: echec allocation matrice"); return NULL; }

	int i = 0;
	cJSON *ligne;
	cJSON_ArrayForEach(ligne, array) {
		if (i >= hauteur) break; // sécurité
		int j = 0;
		cJSON *val = NULL;
		cJSON_ArrayForEach(val, ligne) {
			if (j >= largeur) break; // sécurité
			if (cJSON_IsBool(val)) matrice[i][j] = cJSON_IsTrue(val);
			else if (cJSON_IsNumber(val)) {
				if (val->valueint == 0 || val->valueint == 1) (matrice[i][j] = (bool) val->valueint);
				else { freeMatriceBool(matrice); LOG_ERROR("parseMatriceBool: nombre invalide (doit etre 0 ou 1)"); return NULL; }
			}
			else { freeMatriceBool(matrice); LOG_ERROR("parseMatriceBool: valeur non booleenne ou numerique"); return NULL; }
			j++;
		}
		i++;
	}

	return matrice;
}

static void freeMatricesInt(int **m0, int **m1, int ** m2) {
	if (m0) freeMatriceInt(m0); 
	if (m1) freeMatriceInt(m1);
	if (m2) freeMatriceInt(m2);
}

static chargerCartes_result_t ajouterCarte(const char *nom, arraylist_t *cartes, jeu_t *jeu) {
	if (!nom || !*nom) return 0;

	// ---------- Fichier Base + Couches ----------
	char nomFichierBC[MAX_TAILLE_CHEMIN];
	snprintf(nomFichierBC, sizeof(nomFichierBC), "cartes/%s_BC.json", nom);

	cJSON *jsonBC = cJSON_ParseWithFile(nomFichierBC);
	if (!jsonBC) { LOG_ERROR("Echec parsing JSON (src: %s)", nomFichierBC); return CHARGERCARTES_ERR_READ_BASE_COUCHES; }

	// Lecture des champs
	cJSON *largeurItem = cJSON_GetObjectItem(jsonBC, "largeur");
	if (!largeurItem || !cJSON_IsNumber(largeurItem)) { cJSON_Delete(jsonBC); LOG_ERROR("Carte %s, largeur invalide ou manquante", nom); return CHARGERCARTES_ERR_PARSE_LARGEUR; }
	const int largeur = largeurItem->valueint;

	cJSON *hauteurItem = cJSON_GetObjectItem(jsonBC, "hauteur");
	if (!hauteurItem || !cJSON_IsNumber(hauteurItem)) { cJSON_Delete(jsonBC); LOG_ERROR("Carte %s, hauteur invalide ou manquante", nom); return CHARGERCARTES_ERR_PARSE_HAUTEUR; }
	const int hauteur = hauteurItem->valueint;

	cJSON *chipsetItem = cJSON_GetObjectItem(jsonBC, "chipset");
	const char *nomChipset = (chipsetItem && cJSON_IsString(chipsetItem)) ? chipsetItem->valuestring : NULL;
	chipset_t *chipset = getChipset2(jeu, nomChipset);
	if (!chipset) { cJSON_Delete(jsonBC); LOG_ERROR("Carte %s, chipset invalide ou manquant : %s", nom, nomChipset ? nomChipset : ""); return CHARGERCARTES_ERR_PARSE_CHIPSET; }

	cJSON *musiqueItem = cJSON_GetObjectItem(jsonBC, "musique");
	const char *nomMusique = (musiqueItem && cJSON_IsString(musiqueItem)) ? musiqueItem->valuestring : NULL;
	musique_t *musique = getMusique2(jeu, nomMusique); // La musique peut être NULL (pas de musique)

	// Lecture des matrices couches
	int **c0 = parseMatriceInt(cJSON_GetObjectItem(jsonBC, "couche0"), hauteur, largeur);
	if (!c0) { cJSON_Delete(jsonBC); LOG_ERROR("Carte %s, couche0 invalide ou inexistante", nom); return CHARGERCARTES_ERR_PARSE_COUCHE0; }

	int **c1 = parseMatriceInt(cJSON_GetObjectItem(jsonBC, "couche1"), hauteur, largeur);
	if (!c1) { cJSON_Delete(jsonBC); freeMatricesInt(c0, NULL, NULL); LOG_ERROR("Carte %s, couche1 invalide ou inexistante", nom); return CHARGERCARTES_ERR_PARSE_COUCHE1; }

	int **c2 = parseMatriceInt(cJSON_GetObjectItem(jsonBC, "couche2"), hauteur, largeur);
	if (!c2) { cJSON_Delete(jsonBC); freeMatricesInt(c0, c1, NULL); LOG_ERROR("Carte %s, couche2 invalide ou inexistante", nom); return CHARGERCARTES_ERR_PARSE_COUCHE2; }

	cJSON_Delete(jsonBC);
	// ---------- Fichier Murs + Events ----------
	char nomFichierME[MAX_TAILLE_CHEMIN];
	snprintf(nomFichierME, sizeof(nomFichierME), "cartes/%s_ME.json", nom);

	cJSON *jsonME = cJSON_ParseWithFile(nomFichierME);
	if (!jsonME) { LOG_ERROR("Echec parsing JSON (src: %s)", nomFichierME); return CHARGERCARTES_ERR_READ_MURS_EVENTS; }

	// Lecture de la matrice murs
	bool **murs = parseMatriceBool(cJSON_GetObjectItem(jsonME, "murs"), hauteur, largeur);
	if (!murs) { cJSON_Delete(jsonME); freeMatricesInt(c0, c1, c2); LOG_ERROR("Carte %s, murs invalide ou inexistant", nom); return CHARGERCARTES_ERR_PARSE_MURS; }
	cJSON_Delete(jsonME);

	// ---------- Création carte ----------
	carte_result_t cart_res;
	carte_t *carte = carte_creer(nom, largeur, hauteur, chipset, musique, c0, c1, c2, murs, &cart_res);
	if (!carte) { freeMatricesInt(c0, c1, c2); freeMatriceBool(murs); LOG_ERROR("%s (src: %s)", carte_strerror(cart_res), nom); return CHARGERCARTES_ERR_CREATE_CARTE; }

	arraylist_add(cartes, carte);
	return CHARGERCARTES_OK;
}

arraylist_t * chargerCartes_get(jeu_t *jeu, chargerCartes_result_t *res) {
	if (!res) { LOG_ERROR("Enum chargerCartes_result NULL"); return NULL; }
	*res = CHARGERCARTES_OK;
	if (!jeu) { *res = CHARGERCARTES_ERR_NULL_POINTER_JEU; return NULL; }
	DIR *dir = opendir("cartes");
	if (!dir) { *res = CHARGERCARTES_ERR_NULL_POINTER_DIRECTORY; return NULL; }

	arraylist_result_t al_res;
	arraylist_t *cartes = arraylist_creer(AL_CARTE, &al_res);
	if (!cartes) { LOG_ERROR("%s", arraylist_strerror(al_res)); closedir(dir); *res = CHARGERCARTES_ERR_CREATE_ARRAYLIST; return NULL; }

	// ----- Détection automatique des cartes -----
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		if (!strstr(entry->d_name, "_BC.json")) { continue; }
		char nomCarte[128];
		strncpy(nomCarte, entry->d_name, sizeof(nomCarte) - 1);
		nomCarte[sizeof(nomCarte) - 1] = '\0';
		char *pos = strstr(nomCarte, "_BC.json");
		if (pos) *pos = '\0';

		chargerCartes_result_t chCart_res = ajouterCarte(nomCarte, cartes, jeu);
		if (chCart_res) { closedir(dir); arraylist_detruire(cartes, true); *res = chCart_res; return NULL; }
	}
	closedir(dir);
	return cartes;
}

const char * chargerCartes_strerror(chargerCartes_result_t res) {
	switch (res) {
		case CHARGERCARTES_OK: return "Succes";
		case CHARGERCARTES_ERR_NULL_POINTER_JEU: return "Jeu NULL passe en parametre";
		case CHARGERCARTES_ERR_NULL_POINTER_DIRECTORY: return "Impossible d'ouvrir le dossier cartes";
		case CHARGERCARTES_ERR_CREATE_ARRAYLIST: return "Echec creation arraylist des cartes";
		case CHARGERCARTES_ERR_READ_BASE_COUCHES: return "Echec lecture du fichier json BC (BaseCouches)";
		case CHARGERCARTES_ERR_READ_MURS_EVENTS: return "Echec lecture du fichier json ME (MursEvents)";
		case CHARGERCARTES_ERR_PARSE_LARGEUR: return "Echec recuperation largeur depuis json";
		case CHARGERCARTES_ERR_PARSE_HAUTEUR: return "Echec recuperation hauteur depuis json";
		case CHARGERCARTES_ERR_PARSE_CHIPSET: return "Echec recuperation chipset depuis json";
		case CHARGERCARTES_ERR_PARSE_COUCHE0: return "Echec recuperation couche0 depuis json";
		case CHARGERCARTES_ERR_PARSE_COUCHE1: return "Echec recuperation couche1 depuis json";
		case CHARGERCARTES_ERR_PARSE_COUCHE2: return "Echec recuperation couche2 depuis json";
		case CHARGERCARTES_ERR_PARSE_MURS: return "Echec recuperation murs depuis json";
		case CHARGERCARTES_ERR_CREATE_CARTE: return "Echec creation de la carte";
		default: return "Erreur inconnue";
	}
}