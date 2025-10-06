// @author Alain Barbier alias "Metroidzeta"

#include "headers/chargerCartes.h"

static const char * tabCartes[] = { // sera remplacé par une détection des fichiers présents dans le dossier "cartes" à l'avenir
	// Création des cartes { nomCarte,.. }
	"Sarosa_Milice_Accueil",                               // 0
	"Sarosa",                                              // 1
	"Chateau_Roland_Exterieur",                            // 2
	"Chateau_Roland_Cour_Interieure",                      // 3
	"Chateau_Roland_Etage_01",                             // 4
	"Chateau_Roland_Salle_Trone",                          // 5
	"Chateau_Roland_Salle_Trone_Nouveau",                  // 6
	"Sarosa_Foret_Est",                                    // 7
	"Marais_Coacville",                                    // 8
	"Coacville_Marais_Sud",                                // 9
	"Coacville_Marecage_Sud",                              // 10
	"Coacville",                                           // 11
	"Coacville_Marecage_Nord",                             // 12
	"Coacville_Donjon_Exterieur",                          // 13
	"Donjon1_Entree",                                      // 14
	"Donjon1_salle4",                                      // 15
	"Donjon1_salle5",                                      // 16
	"Arene_Hunter",                                        // 17
	"carte18",                                             // 18
	"Foret_Sud_Sarosa"                                     // 19
};

static chipset_t * getChipset(arraylist_t *chipsets, const char *nom) {
	if (!nom || !*nom) return NULL;
	for (int i = 0; i < chipsets->taille; ++i) {
		chipset_t *chipset = arraylist_get(chipsets, i);
		if (strcmp(chipset->nom, nom) == 0) return chipset;
	}
	return NULL;
}

static musique_t * getMusique(arraylist_t *musiques, const char *nom) {
	if (!nom || !*nom) return NULL;
	for (int i = 0; i < musiques->taille; ++i) {
		musique_t *musique = arraylist_get(musiques, i);
		if (strcmp(musique->nom, nom) == 0) return musique;
	}
	return NULL;
}

static int ** parseMatriceInt(cJSON *array, int hauteur, int largeur) {
	if (!array || !cJSON_IsArray(array)) { fprintf(stderr, "parseMatriceInt: objet JSON invalide\n"); return NULL; }
	int **matrice = creerMatriceINT(hauteur, largeur, -1);
	if (!matrice) { fprintf(stderr, "parseMatriceInt: erreur allocation matrice\n"); return NULL; }

	int i = 0;
	cJSON *ligne;
	cJSON_ArrayForEach(ligne, array) {
		if (i >= hauteur) break; // sécurité

		int j = 0;
		cJSON *val = NULL;
		cJSON_ArrayForEach(val, ligne) {
			if (j >= largeur) break; // sécurité
			if (!cJSON_IsNumber(val)) { freeMatriceINT(matrice); Exception("parseMatriceInt: valeur non numérique"); return NULL; }
			matrice[i][j] = val->valueint;
			j++;
		}
		i++;
	}
	return matrice;
}

static bool **parseMatriceBool(cJSON *array, int hauteur, int largeur) {
	if (!array || !cJSON_IsArray(array)) { fprintf(stderr, "parseMatriceBool: objet JSON invalide\n"); return NULL; }

	bool **matrice = creerMatriceBOOL(hauteur, largeur, false);
	if (!matrice) { fprintf(stderr, "parseMatriceBool: erreur allocation matrice\n"); return NULL; }

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
				if (val->valueint == 0 || val->valueint == 1) matrice[i][j] = (bool) val->valueint;
				else { freeMatriceBOOL(matrice); Exception("parseMatriceBOOL: nombre invalide (doit être 0 ou 1)"); return NULL; }
			}
			else { freeMatriceBOOL(matrice); Exception("parseMatriceBOOL: valeur non booléenne ou numérique"); return NULL; }
			j++;
		}
		i++;
	}

	return matrice;
}

static void freeMatricesINT(int **m0, int **m1, int ** m2) {
	if (m0) freeMatriceINT(m0); 
	if (m1) freeMatriceINT(m1);
	if (m2) freeMatriceINT(m2);
}

static chargerCartes_result_t ajouterCarte(const char *nom, arraylist_t **cartes, arraylist_t *chipsets, arraylist_t *musiques) {
	if (!nom || !*nom) return 0;

	// ---------- Fichier Base + Couches ----------
	size_t len = strlen("cartes/") + strlen(nom) + strlen("_BC.json") + 1; // + 1: \0
	char nomFichierBC[len];
	snprintf(nomFichierBC, sizeof(nomFichierBC), "cartes/%s_BC.json", nom);

	cJSON *jsonBC = cJSON_ParseWithFile(nomFichierBC);
	if (!jsonBC) { LOG_ERROR("Erreur de parsing JSON pour %s", nomFichierBC); return CHARGERCARTES_ERR_READ_BASE_COUCHES; }

	// Lecture des champs
	cJSON *val;
	val = cJSON_GetObjectItem(jsonBC, "largeur");
	if (!cJSON_IsNumber(val)) { cJSON_Delete(jsonBC); LOG_ERROR("Carte %s, largeur invalide ou manquante", nom); return CHARGERCARTES_ERR_PARSE_LARGEUR; }
	const int largeur = val->valueint;

	val = cJSON_GetObjectItem(jsonBC, "hauteur");
	if (!cJSON_IsNumber(val)) { cJSON_Delete(jsonBC); LOG_ERROR("Carte %s, hauteur invalide ou manquante", nom); return CHARGERCARTES_ERR_PARSE_HAUTEUR; }
	const int hauteur = val->valueint;

	val = cJSON_GetObjectItem(jsonBC, "chipset");
	const char *nomChipset = cJSON_IsString(val) ? val->valuestring : NULL;
	chipset_t *chipset = getChipset(chipsets, nomChipset);
	if (!chipset) { cJSON_Delete(jsonBC); LOG_ERROR("Carte %s, chipset invalide ou manquant : %s", nom, nomChipset ? nomChipset : ""); return CHARGERCARTES_ERR_PARSE_CHIPSET; }

	val = cJSON_GetObjectItem(jsonBC, "musique");
	const char *nomMusique = cJSON_IsString(val) ? val->valuestring : NULL;
	musique_t *musique = getMusique(musiques, nomMusique); // La musique peut être NULL (pas de musique)

	// Couches
	int **c0 = parseMatriceInt(cJSON_GetObjectItem(jsonBC, "couche0"), hauteur, largeur);
	if (!c0) { cJSON_Delete(jsonBC); LOG_ERROR("Carte %s, couche0 invalide ou inexistante", nom); return CHARGERCARTES_ERR_PARSE_COUCHE0; }

	int **c1 = parseMatriceInt(cJSON_GetObjectItem(jsonBC, "couche1"), hauteur, largeur);
	if (!c1) { cJSON_Delete(jsonBC); freeMatricesINT(c0, NULL, NULL); LOG_ERROR("Carte %s, couche1 invalide ou inexistante", nom); return CHARGERCARTES_ERR_PARSE_COUCHE1; }

	int **c2 = parseMatriceInt(cJSON_GetObjectItem(jsonBC, "couche2"), hauteur, largeur);
	if (!c2) { cJSON_Delete(jsonBC); freeMatricesINT(c0, c1, NULL); LOG_ERROR("Carte %s, couche2 invalide ou inexistante", nom); return CHARGERCARTES_ERR_PARSE_COUCHE2; }

	cJSON_Delete(jsonBC);

	// ---------- Fichier Murs + Events ----------
	len = strlen("cartes/") + strlen(nom) + strlen("_ME.json") + 1; // + 1: \0
	char nomFichierME[len];
	snprintf(nomFichierME, sizeof(nomFichierME), "cartes/%s_ME.json", nom);

	cJSON *jsonME = cJSON_ParseWithFile(nomFichierME);
	if (!jsonME) { LOG_ERROR("Erreur de parsing JSON pour %s", nomFichierME); return CHARGERCARTES_ERR_READ_MURS_EVENTS; }

	// Lecture des champs
	bool **murs = parseMatriceBool(cJSON_GetObjectItem(jsonME, "murs"), hauteur, largeur);
	if (!murs) { cJSON_Delete(jsonME); freeMatricesINT(c0, c1, c2); LOG_ERROR("Carte %s, murs invalide ou inexistant", nom); return CHARGERCARTES_ERR_PARSE_MURS; }
	cJSON_Delete(jsonME);

	carte_t *carte = NULL;
	carte_result_t res = carte_creer(&carte, nom, largeur, hauteur, chipset, musique, c0, c1, c2, murs);
	if (res != CARTE_OK) { freeMatricesINT(c0, c1, c2); freeMatriceBOOL(murs); LOG_ERROR("Echec creation carte %s : %s", nom, carte_strerror(res)); return CHARGERCARTES_ERR_CREATE_CARTE; }

	arraylist_add(*cartes, carte);
	return CHARGERCARTES_OK;
}

chargerCartes_result_t chargerCartes_get(arraylist_t **cartes, arraylist_t *chipsets, arraylist_t *musiques) {
	if (!cartes) return CHARGERCARTES_ERR_NULL_POINTER_CARTES;
	if (!chipsets) return CHARGERCARTES_ERR_NULL_POINTER_CHIPSETS;
	if (!musiques) return CHARGERCARTES_ERR_NULL_POINTER_MUSIQUES;

	arraylist_result_t resAL = arraylist_creer(cartes, AL_CARTE);
	if (resAL != ARRAYLIST_OK) { LOG_ERROR("Arraylist cartes : %s", arraylist_strerror(resAL)); return CHARGERCARTES_ERR_CREATE_ARRAYLIST; }

	const size_t nbCartes = sizeof(tabCartes) / sizeof(tabCartes[0]);
	for (size_t i = 0; i < nbCartes; ++i) {
		chargerCartes_result_t resCCA = ajouterCarte(tabCartes[i], cartes, chipsets, musiques);
		if (resCCA != CHARGERCARTES_OK) return resCCA;
	}
	return CHARGERCARTES_OK;
}

const char * chargerCartes_strerror(chargerCartes_result_t res) {
	switch (res) {
		case CHARGERCARTES_OK: return "Succes";
		case CHARGERCARTES_ERR_NULL_POINTER_CARTES: return "Pointeur sur arraylist cartes NULL passe en parametre";
		case CHARGERCARTES_ERR_NULL_POINTER_CHIPSETS: return "Arraylist chipsets NULL passe en paramatre";
		case CHARGERCARTES_ERR_NULL_POINTER_MUSIQUES: return "Arraylist musiques NULL passe en parametre";
		case CHARGERCARTES_ERR_CREATE_ARRAYLIST: return "Echec creation arraylist cartes";
		case CHARGERCARTES_ERR_READ_BASE_COUCHES: return "Echec lecture fichier Base_Couches";
		case CHARGERCARTES_ERR_READ_MURS_EVENTS: return "Echec lecture fichier Murs_Events";
		case CHARGERCARTES_ERR_PARSE_LARGEUR: return "Echec recuperation largeur depuis JSON";
		case CHARGERCARTES_ERR_PARSE_HAUTEUR: return "Echec recuperation hauteur depuis JSON";
		case CHARGERCARTES_ERR_PARSE_CHIPSET: return "Echec recuperation chipset depuis JSON";
		case CHARGERCARTES_ERR_PARSE_COUCHE0: return "Echec recuperation couche0 depuis JSON";
		case CHARGERCARTES_ERR_PARSE_COUCHE1: return "Echec recuperation couche1 depuis JSON";
		case CHARGERCARTES_ERR_PARSE_COUCHE2: return "Echec recuperation couche2 depuis JSON";
		case CHARGERCARTES_ERR_PARSE_MURS: return "Echec recuperation murs depuis JSON";
		case CHARGERCARTES_ERR_CREATE_CARTE: return "Echec creation carte";
		default: return "Erreur inconnue";
	}
}