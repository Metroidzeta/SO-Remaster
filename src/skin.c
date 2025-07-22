// @author Alain Barbier alias "Metroidzeta"

#include "skin.h"

static void skin_validerArguments(SDL_Renderer *renderer, const char *nomFichier) {
	if (!renderer) Exception("Renderer NULL");
	if (!nomFichier || !*nomFichier) Exception("nomFichier skin NULL ou vide");
}

skin_t * skin_creer(SDL_Renderer *renderer, const char *nomFichier) {
	skin_validerArguments(renderer, nomFichier);

	skin_t *skin = malloc(sizeof(skin_t));
	if (!skin) Exception("Echec creation skin");
	memset(skin, 0, sizeof(skin_t)); // initialise tout à 0 / NULL pour éviter comportements indifinis en cas d'exception

	skin->nom = strdup(nomFichier); // ne pas faire: "skin->nom = nomFichier;" car on ne copie alors que des adresses
	if (!skin->nom) { skin_detruire(skin); Exception("Echec creation copie nom skin"); }
	char chemin[100]; // chemin vers l'image de la skin
	snprintf(chemin, sizeof(chemin), "img/%s", nomFichier);
	skin->texture = creerTextureDepuisImage(renderer, chemin);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 3; ++j) { //                  __.x__  __.y__  .w  .h
			skin->textureRegions[i * 3 + j] = (SDL_Rect){ j * 48, i * 48, 48, 48 };
		}
	}
	return skin;
}

void skin_afficher(SDL_Renderer *renderer, skin_t *skin, int numRegion, SDL_Rect *dstRect) {
	dessinerTexture(renderer, skin->texture, &skin->textureRegions[numRegion], dstRect, "Impossible de dessiner la skin de notre joueur avec SDL_RenderCopy");
}

void skin_detruire(skin_t *skin) {
	if (!skin) return;
	SDL_DestroyTexture(skin->texture);
	free(skin->nom);
	free(skin);
}