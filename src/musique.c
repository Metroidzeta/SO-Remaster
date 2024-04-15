// @author Alain Barbier alias "Metroidzeta"

#include "musique.h"

musique_t * musique_creer(char * nomFichier) {
	musique_verificationsArgs(nomFichier);
	musique_t * musique = malloc(sizeof(musique_t)); verifAlloc(musique,"Erreur d'allocation de la musique");
	musique->nom = strdup(nomFichier); verifAllocStrCopy(musique->nom,nomFichier); // il ne faut pas écrire : "musique->nom = nomFichier;" car on ne copie alors que des adresses
	musique->piste = creerPiste(nomFichier);
	musique->enLecture = false;
	return musique;
}

void musique_verificationsArgs(char * nomFichier) {
	if(nomFichier == NULL) { Exception("Le nomFichier de la musique est NULL"); }
	if(nomFichier[0] == '\0') { Exception("Le nomFichier de la musique est vide"); }
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