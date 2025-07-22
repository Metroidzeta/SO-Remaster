// @author Alain Barbier alias "Metroidzeta"

#include "chipset.h"

static void chipset_validerArguments(SDL_Renderer * renderer, const char *nomFichier, int tailleTuile) {
	if (!renderer) Exception("Renderer NULL");
	if (!nomFichier || !*nomFichier) Exception("nomFichier chipset NULL ou vide");
	if (tailleTuile < 1) Exception("tailleTuile chipset < 1");
}

static SDL_Rect * extraireTuiles(int nbTuilesLargeur, int nbTuilesHauteur, int tailleTuile) {
	const int nbTuiles = nbTuilesLargeur * nbTuilesHauteur;
	SDL_Rect *tuiles = malloc(nbTuiles * sizeof(SDL_Rect));
	if (!tuiles) return NULL;

	for (int i = 0; i < nbTuiles; ++i) {
		const int x = (i % nbTuilesLargeur) * tailleTuile;
		const int y = (i / nbTuilesLargeur) * tailleTuile;
		tuiles[i] = (SDL_Rect){ x, y, tailleTuile, tailleTuile };
	}

	return tuiles;
}

chipset_t * chipset_creer(SDL_Renderer *renderer, const char *nomFichier, int tailleTuile) {
	chipset_validerArguments(renderer, nomFichier, tailleTuile);

	chipset_t * chipset = malloc(sizeof(chipset_t));
	if (!chipset) Exception("Echec creation chipset");
	memset(chipset, 0, sizeof(chipset_t)); // initialise tout à 0 / NULL pour éviter comportements indifinis en cas d'exception

	chipset->nom = strdup(nomFichier); // ne pas faire: "chipset->nom = nomFichier" car on ne copie alors que des adresses
	if (!chipset->nom) { chipset_detruire(chipset); Exception("Echec creation nom copie chipset"); }
	char chemin[MAX_TAILLE_CHEMIN];
	snprintf(chemin, sizeof(chemin), "%s%s", PATH_IMAGES, nomFichier); // chemin vers l'image de du chipset
	chipset->tailleTuile = tailleTuile;
	chipset->texture = creerTextureDepuisImage(renderer, chemin);
	if (!chipset->texture) { chipset_detruire(chipset); ExceptionSDL("Echec creation texture image chipset"); }

	int largeur, hauteur;
	SDL_QueryTexture(chipset->texture, NULL, NULL, &largeur, &hauteur);
	if (largeur % tailleTuile != 0 || hauteur % tailleTuile != 0) { chipset_detruire(chipset); Exception("Image non divisible par tailleTuile"); }
	chipset->nbTuilesLargeur = largeur / tailleTuile;
	chipset->nbTuilesHauteur = hauteur / tailleTuile;
	chipset->tuiles = extraireTuiles(chipset->nbTuilesLargeur, chipset->nbTuilesHauteur, tailleTuile);
	if (!chipset->tuiles) { chipset_detruire(chipset); Exception("Echec creation tuiles"); }
	return chipset;
}

void chipset_detruire(chipset_t *chipset) {
	if (!chipset) return;
	free(chipset->tuiles);
	SDL_DestroyTexture(chipset->texture);
	free(chipset->nom);
	free(chipset);
}