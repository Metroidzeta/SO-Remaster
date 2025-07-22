// @author Alain Barbier alias "Metroidzeta"

#include "musique.h"

static void musique_validerArguments(const char * nomFichier) {
	if (!nomFichier || !*nomFichier) Exception("nomFichier musique NULL ou vide");
}

musique_t * musique_creer(const char * nomFichier) {
	musique_validerArguments(nomFichier);

	musique_t *musique = malloc(sizeof(musique_t));
	if (!musique) Exception("Echec creation musique");
	memset(musique, 0, sizeof(musique_t)); // initialise tout à 0 / NULL pour éviter comportements indifinis en cas d'exception

	musique->nom = strdup(nomFichier); // ne pas faire: "musique->nom = nomFichier;" car on ne copie alors que des adresses
	if (!musique->nom) { musique_detruire(musique); Exception("Echec creation copie nom musique"); }
	musique->piste = creerPiste(nomFichier);
	musique->enLecture = false;
	return musique;
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