#include "bruitage.h"

bruitage_t * bruitage_creer(char * nom, const char * chemin) {
	bruitage_verificationsArgs(nom);
	bruitage_t * bruitage = malloc(sizeof(bruitage_t));

	bruitage->nom = strdup(nom); // il ne faut pas écrire : "bruitage->nom = nom;" car on ne copie alors que des adresses
	verifAllocStrCopy(bruitage->nom,nom);

	bruitage->son = Mix_LoadWAV(chemin);
	verifAllocMix(bruitage->son,chemin,"Erreur: impossible de creer le bruitage avec Mix_LoadWAV");

	return bruitage;
}

void bruitage_verificationsArgs(char * nom) {
	if(nom == NULL) { Exception("Le nom du bruitage est NULL"); }
	if(nom[0] == '\0') { Exception("Le nom du bruitage est vide"); }
}

void bruitage_play(bruitage_t * bruitage) {
	Mix_PlayChannel(-1,bruitage->son,0); // -1 = jouer sur le premier channel disponible
}

void bruitage_detruire(bruitage_t * bruitage) {
	Mix_FreeChunk(bruitage->son);
	free(bruitage->nom);
	free(bruitage);
}