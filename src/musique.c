// @author Alain Barbier alias "Metroidzeta"

#include "headers/musique.h"

static musique_result_t musique_validerArguments(const char * nomFichier) {
	if (!nomFichier || !*nomFichier) return MUSIQUE_ERR_NULL_OR_EMPTY_FILENAME;
	return MUSIQUE_OK;
}

static musique_result_t musique_chargerPiste(musique_t *musique, const char *nomFichier) {
	musique->piste = creerPiste(nomFichier);
	if (!musique->piste) { LOG_ERROR("Erreur chargement piste : %s", Mix_GetError()); return MUSIQUE_ERR_LOAD_SOUND; }
	musique->enLecture = false;

	return MUSIQUE_OK;
}

musique_result_t musique_creer(musique_t **out_musique, const char * nomFichier) {
	if (!out_musique) return MUSIQUE_ERR_NULL_POINTER;
	*out_musique = NULL;

	musique_result_t res = musique_validerArguments(nomFichier);
	if (res != MUSIQUE_OK) return res;

	musique_t *musique = calloc(1, sizeof(musique_t));
	if (!musique) return MUSIQUE_ERR_MEMORY_BASE;

	musique->nom = my_strdup(nomFichier); // important : ne pas faire "musique->nom = nomFichier", car cela ne copie que le pointeur, pas le contenu
	if (!musique->nom) { musique_detruire(musique); return MUSIQUE_ERR_MEMORY_NAME; }

	if ((res = musique_chargerPiste(musique, nomFichier)) != MUSIQUE_OK) { musique_detruire(musique); return res; }

	*out_musique = musique;
	return MUSIQUE_OK;
}

void musique_play(musique_t *musique) {
	if (!musique || !musique->piste) return;
	Mix_PlayMusic(musique->piste, -1); // -1 = loop infini
	musique->enLecture = true;
}

void musique_stop(musique_t *musique) {
	if (!musique || !musique->piste) return;
	Mix_HaltMusic();
	musique->enLecture = false;
}

void musique_detruire(musique_t *musique) {
	if (!musique) return;
	Mix_FreeMusic(musique->piste);
	free(musique->nom);
	free(musique);
}

const char * musique_strerror(musique_result_t res) {
	switch (res) {
		case MUSIQUE_OK: return "Succes";
		case MUSIQUE_ERR_NULL_POINTER: return "Musique NULL passe en parametre";
		case MUSIQUE_ERR_NULL_OR_EMPTY_FILENAME: return "Nom fichier NULL ou vide";
		case MUSIQUE_ERR_MEMORY_BASE: return "Echec allocation memoire base";
		case MUSIQUE_ERR_MEMORY_NAME: return "Echec allocation memoire nom";
		case MUSIQUE_ERR_LOAD_SOUND: return "Echec chargement du son";
		default: return "Erreur";
	}
}