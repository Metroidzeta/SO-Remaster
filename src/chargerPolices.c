// @author Alain Barbier alias "Metroidzeta"

#include "headers/chargerPolices.h"

static const police_info_t tabPolices[] = { // sera remplacé plus tard par la lecture réelle de fichiers (JSON dans le futur)
	// Création des polices { nomFichier, taille }
	{ "arial.ttf", WINDOW_WIDTH / 80 },                   // 0 (FPS)
	{ "arial.ttf", TAILLE_CASES * 0.68},                  // 1 (Texte de base)
	{ "Zelda Breath of the Wild.otf", WINDOW_WIDTH / 28 } // 2 (Coups d'attaques)
};

static chargerPolices_result_t ajouterPolice(const police_info_t *elem, arraylist_t **polices) {
	if (!elem->nomFichier) { LOG_ERROR("Nom fichier police NULL"); return CHARGERPOLICES_ERR_CREATE_POLICE; }
	TTF_Font *police = creerPolice(elem->nomFichier, elem->taille);
	if (!police) { LOG_ERROR("Police : %s (fichier : %s)", TTF_GetError(), elem->nomFichier); return CHARGERPOLICES_ERR_CREATE_POLICE; }
	arraylist_add(*polices, police);
	return CHARGERPOLICES_OK;
}

chargerPolices_result_t chargerPolices_get(arraylist_t **polices) {
	if (!polices) return CHARGERPOLICES_ERR_NULL_POINTER;

	arraylist_result_t resAL = arraylist_creer(polices, AL_POLICE);
	if (resAL != ARRAYLIST_OK) { LOG_ERROR("Arraylist polices : %s", arraylist_strerror(resAL)); return CHARGERPOLICES_ERR_CREATE_ARRAYLIST; }

	const size_t nbPolices = sizeof(tabPolices) / sizeof(tabPolices[0]);
	for (size_t i = 0; i < nbPolices; ++i) {
		const police_info_t *elem = &tabPolices[i];
		chargerPolices_result_t resCPL = ajouterPolice(elem, polices);
		if (resCPL != CHARGERPOLICES_OK) return resCPL;
	}
	return CHARGERPOLICES_OK;
}

const char * chargerPolices_strerror(chargerPolices_result_t res) {
	switch (res) {
		case CHARGERPOLICES_OK: return "Succes";
		case CHARGERPOLICES_ERR_NULL_POINTER: return "Pointeur sur arraylist polices NULL passe en parametre";
		case CHARGERPOLICES_ERR_CREATE_ARRAYLIST: return "Echec creation arraylist polices";
		case CHARGERPOLICES_ERR_CREATE_POLICE: return "Echec creation police";
		default: return "Erreur";
	}
}