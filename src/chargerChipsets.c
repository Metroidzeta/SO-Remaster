// @author Alain Barbier alias "Metroidzeta"

#include "headers/chargerChipsets.h"

static const chipset_info_t tabChipsets[] = { // sera remplacé plus tard par la lecture réelle de fichiers (JSON dans le futur)
	// Création des chipsets { nomFichier, tailleTuile (par défaut : 16) }
	{ "BZ.png", 16 },                       // 0
	{ "VillageTangaFinal.png", 16 },        // 1
	{ "grey_cas42.png", 16 },               // 2
	{ "PalaisRoland2.png", 16 },            // 3
	{ "PalaisRolandInt.png", 16 },          // 4
	{ "PalaisRolandNouveau.png", 48 },      // 5
	{ "MaraisTanga.png", 16 },              // 6
	{ "marais2.png", 16 },                  // 7
	{ "Coacville_exterieur.png", 16 },      // 8
	{ "chipset173.png", 16 },               // 9
	{ "chipset175.png", 16 },               // 10
	{ "HunterArene.png", 16 },              // 11
	{ "grass.png", 32 },                    // 12
	{ "chipset5c.png", 16 }                 // 13
};

static chargerChipsets_result_t ajouterChipset(SDL_Renderer *renderer, const chipset_info_t *elem, arraylist_t **chipsets) {
	chipset_t *chipset = NULL;
	chipset_result_t res = chipset_creer(&chipset, renderer, elem->nomFichier, elem->tailleTuile);
	if (res != CHIPSET_OK) { LOG_ERROR("Chipset : %s (fichier : %s)", chipset_strerror(res), elem->nomFichier); return CHARGERCHIPSETS_ERR_CREATE_CHIPSET; }
	arraylist_add(*chipsets, chipset);
	return CHARGERCHIPSETS_OK;
}

chargerChipsets_result_t chargerChipsets_get(SDL_Renderer *renderer, arraylist_t **chipsets) {
	if (!chipsets) return CHARGERCHIPSETS_ERR_NULL_POINTER;

	arraylist_result_t resAL = arraylist_creer(chipsets, AL_CHIPSET);
	if (resAL != ARRAYLIST_OK) { LOG_ERROR("Arraylist chipsets : %s", arraylist_strerror(resAL)); return CHARGERCHIPSETS_ERR_CREATE_ARRAYLIST; }

	const size_t nbChipsets = sizeof(tabChipsets) / sizeof(tabChipsets[0]);
	for (size_t i = 0; i < nbChipsets; ++i) {
		const chipset_info_t *elem = &tabChipsets[i];
		chargerChipsets_result_t resCCH = ajouterChipset(renderer, elem, chipsets);
		if (resCCH != CHARGERCHIPSETS_OK) return resCCH;
	}
	return CHARGERCHIPSETS_OK;
}

const char * chargerChipsets_strerror(chargerChipsets_result_t res) {
	switch (res) {
		case CHARGERCHIPSETS_OK: return "Succes";
		case CHARGERCHIPSETS_ERR_NULL_POINTER: return "Pointeur sur arraylist chipsets NULL passe en parametre";
		case CHARGERCHIPSETS_ERR_CREATE_ARRAYLIST: return "Echec creation arraylist chipsets";
		case CHARGERCHIPSETS_ERR_CREATE_CHIPSET: return "Echec creation chipset";
		default: return "Erreur";
	}
}