// @author Alain Barbier alias "Metroidzeta"

#include "bruitage.h"

static void bruitage_validerArguments(const char *nomFichier) {
	if (!nomFichier || !*nomFichier) Exception("nomFichier bruitage NULL ou vide");
}

bruitage_t * bruitage_creer(const char *nomFichier) {
	bruitage_validerArguments(nomFichier);

	bruitage_t * bruitage = malloc(sizeof(bruitage_t));
	if (!bruitage) Exception("Echec creation bruitage");
	memset(bruitage, 0, sizeof(bruitage_t)); // initialise tout à 0 / NULL pour éviter comportements indifinis en cas d'exception

	bruitage->nom = strdup(nomFichier); // ne pas faire: "bruitage->nom = nomFichier" car on ne copie alors que des adresses
	if (!bruitage->nom) { bruitage_detruire(bruitage); Exception("Echec creation copie nom bruitage"); }
	bruitage->son = creerSon(nomFichier);
	return bruitage;
}

void bruitage_play(bruitage_t * bruitage) {
	if (!bruitage) return;
	Mix_PlayChannel(-1, bruitage->son, 0); // -1 = jouer sur le premier channel disponible
}

void bruitage_detruire(bruitage_t *bruitage) {
	if (!bruitage) return;
	Mix_FreeChunk(bruitage->son);
	free(bruitage->nom);
	free(bruitage);
}