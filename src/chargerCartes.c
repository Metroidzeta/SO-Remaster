// @author Alain Barbier alias "Metroidzeta"

#include "headers/chargerCartes.h"

static const carte_info_t tabCartes[] = { // sera remplacé plus tard par la lecture réelle de fichiers (JSON dans le futur)
	// Création des cartes { nom, largeur, hauteur, nomChipset, nomMusique (NULL si pas de musique), depuisFichiers ? (true/false) }
	{ "Sarosa_Milice_Accueil", 20, 10, "BZ.png", "Castle_1.mp3", true },                                    // 0
	{ "Sarosa", 50, 50, "VillageTangaFinal.png", "Sarosa.mp3", true },                                      // 1
	{ "Chateau_Roland_Exterieur", 50, 35, "grey_cas42.png", "bahamut_lagoon.mp3", true },                   // 2
	{ "Chateau_Roland_Cour_Interieure", 15, 20, "PalaisRoland2.png" , "Castle_3.mp3", true },               // 3
	{ "Chateau_Roland_Etage_01", 15, 12, "PalaisRolandInt.png", "Castle_3.mp3", true },                     // 4
	{ "Chateau_Roland_Salle_Trone", 19, 22, "PalaisRoland2.png", "2000_ordeal.mp3", true },                 // 5
	{ "Chateau_Roland_Salle_Trone_Nouveau", 19, 23, "PalaisRolandNouveau.png", "2000_ordeal.mp3", true },   // 6
	{ "Sarosa_Foret_Est", 50 ,30, "VillageTangaFinal.png", "bahamut_lagoon.mp3", true },                    // 7
	{ "Marais_Coacville", 70, 55, "MaraisTanga.png", "cc_viper_manor.mp3", true },                          // 8
	{ "Coacville_Marais_Sud", 20, 18, "marais2.png", "cc_viper_manor.mp3", true },                          // 9
	{ "Coacville_Marecage_Sud", 30, 40, "marais2.png", "cc_viper_manor.mp3", true },                        // 10
	{ "Coacville", 43, 40, "Coacville_exterieur.png", "suikoden-ii-two-rivers.mp3", true },                 // 11
	{ "Coacville_Marecage_Nord", 40, 50, "marais2.png", "cc_viper_manor.mp3", true },                       // 12
	{ "Coacville_Donjon_Exterieur", 25, 40, "chipset173.png", "cc_viper_manor.mp3", true },                 // 13
	{ "Donjon1_Entree", 20, 15, "chipset175.png", "mystery3.ogg", true },                                   // 14
	{ "Donjon1_salle4", 30, 15, "chipset175.png", "mystery3.ogg", true },                                   // 15
	{ "Donjon1_salle5", 15, 15, "chipset175.png", "mystery3.ogg", true },                                   // 16
	{ "Arene_Hunter", 29, 32, "HunterArene.png", "hunter.ogg", true },                                      // 17
	{ "carte18", 20, 20, "grass.png", "illusionary_world.mp3", true },                                      // 18
	{ "Foret_Sud_Sarosa", 50, 31, "chipset5c.png", "chapt1medfill.mp3", true }                              // 19
};

static chipset_t * getChipset(arraylist_t *chipsets, const char *nom) {
	for (int i = 0; i < chipsets->taille; ++i) {
		chipset_t *chipset = arraylist_get(chipsets, i);
		if (strcmp(chipset->nom, nom) == 0) return chipset;
	}
	return NULL;
}

static musique_t * getMusique(arraylist_t *musiques, const char *nom) {
	for (int i = 0; i < musiques->taille; ++i) {
		musique_t *musique = arraylist_get(musiques, i);
		if (strcmp(musique->nom, nom) == 0) return musique;
	}
	return NULL;
}

static chargerCartes_result_t ajouterCarte(const carte_info_t *elem, arraylist_t **cartes, arraylist_t *chipsets, arraylist_t *musiques) {
	chipset_t *chipset = NULL;
	if (elem->nomChipset != NULL) { // un chipset NULL = refusé -> à la création de la carte
		chipset = getChipset(chipsets, elem->nomChipset);
		if (!chipset) { LOG_ERROR("Carte %s, chipset inexistant : %s", elem->nom, elem->nomChipset); return CHARGERCARTES_ERR_GET_CHIPSET; }
	}

	musique_t *musique = NULL;
	if (elem->nomMusique != NULL) { // une musique NULL = accepté -> pas de musique
		musique = getMusique(musiques, elem->nomMusique);
		if (!musique) { LOG_ERROR("Carte %s, musique inexistante : %s", elem->nom, elem->nomMusique); return CHARGERCARTES_ERR_GET_MUSIQUE; }
	}

	carte_t *carte = NULL;
	carte_result_t res = carte_creer(&carte, elem->nom, elem->largeur, elem->hauteur, chipset, musique, elem->depuisFichiers);
	if (res != CARTE_OK) { LOG_ERROR("Echec creation carte %s : %s", elem->nom, carte_strerror(res)); return CHARGERCARTES_ERR_CREATE_CARTE; }

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
		const carte_info_t *elem = &tabCartes[i];
		chargerCartes_result_t resCCA = ajouterCarte(elem, cartes, chipsets, musiques);
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
		case CHARGERCARTES_ERR_GET_CHIPSET: return "Echec obtention chipset";
		case CHARGERCARTES_ERR_GET_MUSIQUE: return "Echec obtention musique";
		case CHARGERCARTES_ERR_CREATE_CARTE: return "Echec creation carte";
		default: return "Erreur";
	}
}