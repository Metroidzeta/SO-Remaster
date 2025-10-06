// @author Alain Barbier alias "Metroidzeta"

#include "headers/bruitage.h"

static bruitage_result_t bruitage_validerArguments(const char *nomFichier) {
	if (!nomFichier || !*nomFichier) return BRUITAGE_ERR_NULL_OR_EMPTY_FILENAME;
	if (strlen(nomFichier) >= MAX_TAILLE_STRING) return BRUITAGE_ERR_SIZE_MAX_FILENAME;
	return BRUITAGE_OK;
}

static bruitage_result_t bruitage_chargerSon(bruitage_t *bruitage, const char *nomFichier) {
	bruitage->son = creerSon(nomFichier);
	if (!bruitage->son) { LOG_ERROR("Erreur chargement son : %s", Mix_GetError()); return BRUITAGE_ERR_LOAD_SOUND; }
	return BRUITAGE_OK;
}

bruitage_result_t bruitage_creer(bruitage_t **out_bruitage, const char *nomFichier) {
	if (!out_bruitage) return BRUITAGE_ERR_NULL_POINTER;
	*out_bruitage = NULL;

	bruitage_result_t res = bruitage_validerArguments(nomFichier);
	if (res != BRUITAGE_OK) return res;

	bruitage_t *bruitage = calloc(1, sizeof(bruitage_t));
	if (!bruitage) return BRUITAGE_ERR_MEMORY_BASE;

	bruitage->nom = my_strdup(nomFichier); // important : ne pas faire "bruitage->nom = nomFichier", car cela ne copie que le pointeur, pas le contenu
	if (!bruitage->nom) { bruitage_detruire(bruitage); return BRUITAGE_ERR_MEMORY_NAME; }

	if ((res = bruitage_chargerSon(bruitage, nomFichier)) != BRUITAGE_OK) { bruitage_detruire(bruitage); return res; }

	*out_bruitage = bruitage;
	return BRUITAGE_OK;
}

void bruitage_play(bruitage_t *bruitage) {
	if (!bruitage || !bruitage->son) return;
	Mix_PlayChannel(-1, bruitage->son, 0); // -1 = jouer sur le premier channel disponible
}

void bruitage_detruire(bruitage_t *bruitage) {
	if (!bruitage) return;
	Mix_FreeChunk(bruitage->son);
	free(bruitage->nom);
	free(bruitage);
}

const char * bruitage_strerror(bruitage_result_t res) {
	switch (res) {
		case BRUITAGE_OK: return "Succes";
		case BRUITAGE_ERR_NULL_POINTER: return "Bruitage NULL passe en parametre";
		case BRUITAGE_ERR_NULL_OR_EMPTY_FILENAME: return "Nom fichier NULL ou vide";
		case BRUITAGE_ERR_SIZE_MAX_FILENAME: return "Nom fichier trop long";
		case BRUITAGE_ERR_MEMORY_BASE: return "Echec allocation memoire base";
		case BRUITAGE_ERR_MEMORY_NAME: return "Echec allocation memoire nom";
		case BRUITAGE_ERR_LOAD_SOUND: return "Echec chargement du son";
		default: return "Erreur inconnue";
	}
}