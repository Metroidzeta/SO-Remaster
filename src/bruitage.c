// @author Alain Barbier alias "Metroidzeta"

#include "bruitage.h"

static void bruitage_verificationsArgs(char * nomFichier) {
	if(nomFichier == NULL) { Exception("Le nomFichier du bruitage est NULL"); }
	if(nomFichier[0] == '\0') { Exception("Le nomFichier du bruitage est vide"); }
}

bruitage_t * bruitage_creer(char * nomFichier) {
	bruitage_verificationsArgs(nomFichier);
	bruitage_t * bruitage = malloc(sizeof(bruitage_t)); verifAlloc(bruitage,"Erreur d'allocation du bruitage");
	bruitage->nom = strdup(nomFichier); verifAllocStrCopy(bruitage->nom,nomFichier); // il ne faut pas écrire : "bruitage->nom = nomFichier;" car on ne copie alors que des adresses
	bruitage->son = creerSon(nomFichier);
	return bruitage;
}

void bruitage_play(bruitage_t * bruitage) { Mix_PlayChannel(-1,bruitage->son,0); } // -1 = jouer sur le premier channel disponible

void bruitage_detruire(bruitage_t * bruitage) {
	Mix_FreeChunk(bruitage->son);
	free(bruitage->nom);
	free(bruitage);
}