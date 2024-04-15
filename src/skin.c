// @author Alain Barbier alias "Metroidzeta"

#include "skin.h"

skin_t * skin_creer(SDL_Renderer * renderer, char * nomFichier) {
	skin_verificationsArgs(nomFichier);
	skin_t * skin = malloc(sizeof(skin_t)); verifAlloc(skin,"Erreur d'allocation du skin");
	skin->nom = strdup(nomFichier); verifAllocStrCopy(skin->nom,nomFichier); // il ne faut pas écrire : "skin->nom = nomFichier;" car on ne copie alors que des adresses
	char chemin[100] = "img/"; // chemin vers l'image de la skin
	strcat(chemin,nomFichier);
	skin->texture = creerTextureDepuisImage(renderer,chemin);
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 3; j++) { //                   __.x__  __.y__  .w  .h
			skin->textureRegions[i * 3 + j] = (SDL_Rect) {j * 48, i * 48, 48, 48};
		}
	}
	return skin;
}

void skin_verificationsArgs(char * nomFichier) {
	if(nomFichier == NULL) { Exception("Le nomFichier de la skin est NULL"); }
	if(nomFichier[0] == '\0') { Exception("Le nomFichier de la skin est vide"); }
}

void skin_afficher(SDL_Renderer * renderer, skin_t * skin, int numRegion, SDL_Rect * dstRect) {
	dessinerTexture(renderer,skin->texture,&skin->textureRegions[numRegion],dstRect,"Impossible de dessiner la skin de notre joueur avec SDL_RenderCopy");
}

void skin_detruire(skin_t * skin) {
	SDL_DestroyTexture(skin->texture);
	free(skin->nom);
	free(skin);
}