#include "musique.h"

musique_t * musique_creer(char * nom, const char * chemin) {
	musique_verificationsArgs(nom);
	musique_t * musique = malloc(sizeof(musique_t));

	musique->nom = strdup(nom); // il ne faut pas écrire : "musique->nom = nom;" car on ne copie alors que des adresses
	verifAllocStrCopy(musique->nom,nom);

	musique->piste = Mix_LoadMUS(chemin);
	verifAllocMix(musique->piste,chemin,"Erreur: impossible de creer la musique avec Mix_LoadMUS");
	musique->enLecture = false;

	return musique;
}

void musique_verificationsArgs(char * nom) {
	if(nom == NULL) { Exception("Le nom de la musique est NULL"); }
	if(nom[0] == '\0') { Exception("Le nom de la musique est vide"); }
}

void musique_play(musique_t * musique) {
	Mix_PlayMusic(musique->piste,-1); // -1 = loop infini
	musique->enLecture = true;
}

void musique_stop(musique_t * musique) {
	Mix_HaltMusic();
	musique->enLecture = false;
}

void musique_detruire(musique_t * musique) {
	Mix_FreeMusic(musique->piste);
	free(musique->nom);
	free(musique);
}