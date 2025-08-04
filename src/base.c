// @author Alain Barbier alias "Metroidzeta"

#include "headers/base.h"

void Exception(const char *msgErr) {
	fprintf(stderr, "Exception: %s\n", msgErr);
	exit(EXIT_FAILURE);
}

void ExceptionSDL(const char *msgErr) { fprintf(stderr, "%s: %s\n", msgErr, SDL_GetError()); exit(EXIT_FAILURE); }
void printErrTTF(const char *msgErr) { fprintf(stderr, "%s: %s\n", msgErr, TTF_GetError()); exit(EXIT_FAILURE); }
void printErrMix(const char *msgErr) { fprintf(stderr, "%s: %s\n", msgErr, Mix_GetError()); exit(EXIT_FAILURE); }

void verifAllocSDL(void *ptr, const char *chemin, const char *msgErr) {
	if (!ptr) {
		fprintf(stderr,"%s (chemin: %s): %s\n", msgErr, chemin, SDL_GetError());
		exit(EXIT_FAILURE);
	}
}

void verifAllocTTF(void *ptr, const char *chemin, const char *msgErr) {
	if (!ptr) {
		fprintf(stderr,"%s (chemin: %s): %s\n", msgErr, chemin, TTF_GetError());
		exit(EXIT_FAILURE);
	}
}

void verifAllocMix(void *ptr, const char *chemin, const char *msgErr) {
	if (!ptr) {
		fprintf(stderr,"%s (chemin: %s): %s\n", msgErr, chemin, Mix_GetError());
		exit(EXIT_FAILURE);
	}
}

void initSDL(SDL_Window **window, SDL_Renderer **renderer, char *titre) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) ExceptionSDL("Erreur init SDL_INIT_VIDEO"); // init SDL Vidéo
	if (SDL_Init(SDL_INIT_AUDIO) != 0) ExceptionSDL("Erreur init SDL_INIT_AUDIO"); // init SDL Audio
	if (Mix_OpenAudio(44100, AUDIO_S32SYS, 2, 1024) != 0) printErrMix("Erreur init Mix_OpenAudio"); // init SDL Mixer audio
	if (TTF_Init() != 0) printErrTTF("Erreur init TTF_Init"); // init SDL TTF

	*window = SDL_CreateWindow(titre, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN); // création fenêtre
	if (!*window) ExceptionSDL("Erreur init SDL_CreateWindow");

	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED); // création rendu
	if (!*renderer) ExceptionSDL("Erreur init SDL_CreateRenderer");
}

void freeSDL(SDL_Window *window, SDL_Renderer *renderer) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit(); Mix_CloseAudio(); SDL_Quit();
}

int minDouble(double a, double b) { return (int)(a <= b ? a : b); }
int minInt(int a, int b) { return (a < b) ? a : b; }
int maxInt(int a, int b) { return (a > b) ? a : b; }

static int uintlen(unsigned x) {
	int len = 1; while (x /= 10) len++;
	return len;
}

static int intlen(int x) { return x < 0 ? uintlen(-x) + 1 : uintlen(x); }

char * intToString(int x) {
	int len = intlen(x) + 1;
	char *str = malloc(len);
	if (!str) return NULL;
	snprintf(str, len, "%d", x);
	return str;
}

void copyIntArray(int *dst, const int *src, int taille) { memcpy(dst, src, taille * sizeof(int)); }

bool ** creerMatriceBOOL(int lignes, int colonnes, bool valeurDefaut) {
	if (lignes < 1 || colonnes < 1) return NULL;
	bool **matrice = malloc(lignes * sizeof(bool *));
	if (!matrice) return NULL;

	for (int i = 0; i < lignes; ++i) {
		matrice[i] = malloc(colonnes * sizeof(bool));
		if (!matrice[i]) { for (int j = 0; j < i; j++) free(matrice[j]); free(matrice); return NULL; }
		for (int j = 0; j < colonnes; j++) matrice[i][j] = valeurDefaut;
	}

	return matrice;
}

void freeMatriceBOOL(bool **matrice, int lignes) {
	if (!matrice) return;
	for (int i = 0; i < lignes; ++i) free(matrice[i]);
	free(matrice);
}

int ** creerMatriceINT(int lignes, int colonnes, int valeurDefaut) {
	if (lignes < 1 || colonnes < 1) return NULL;
	int **matrice = malloc(lignes * sizeof(int *));
	if (!matrice) return NULL;

	for (int i = 0; i < lignes; ++i) {
		matrice[i] = malloc(colonnes * sizeof(int));
		if (!matrice[i]) { for (int j = 0; j < i; j++) free(matrice[j]); free(matrice); return NULL; }
		for (int j = 0; j < colonnes; j++) matrice[i][j] = valeurDefaut;
	}

	return matrice;
}

void freeMatriceINT(int **matrice, int lignes) {
	if (!matrice) return;
	for (int i = 0; i < lignes; ++i) free(matrice[i]);
	free(matrice);
}

void changerCouleurRendu(SDL_Renderer *renderer, SDL_Color couleur) {
	if (SDL_SetRenderDrawColor(renderer, couleur.r, couleur.g, couleur.b, couleur.a) != 0) ExceptionSDL("Impossible changerCouleur SDL_SetRenderDrawColor");
}

void effacerEcran(SDL_Renderer *renderer) {
	changerCouleurRendu(renderer, NOIR);
	SDL_RenderClear(renderer);
}

void dessinerRectangle(SDL_Renderer *renderer, SDL_Rect *rect, SDL_Color couleur) {
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	changerCouleurRendu(renderer, couleur);
	if (SDL_RenderFillRect(renderer, rect) != 0) ExceptionSDL("Impossible dessiner rectangle SDL_RenderFillRect");
}

SDL_Texture * creerImage(SDL_Renderer *renderer, const char *nomFichier) {
	char chemin[MAX_TAILLE_CHEMIN];
	int nbChars = snprintf(chemin, sizeof(chemin), "%s%s", PATH_IMAGES, nomFichier); // chemin vers l'image
	if (nbChars >= MAX_TAILLE_CHEMIN || nbChars < 0) return NULL;

	return IMG_LoadTexture(renderer, chemin);
}

TTF_Font * creerPolice(const char *nomFichier, int taille) {
	char chemin[MAX_TAILLE_CHEMIN];
	int nbChars = snprintf(chemin, sizeof(chemin), "%s%s", PATH_POLICES, nomFichier); // chemin vers la police
	if (nbChars >= MAX_TAILLE_CHEMIN || nbChars < 0) return NULL;

	return TTF_OpenFont(chemin, taille);
}

Mix_Music * creerPiste(const char *nomFichier) {
	char chemin[MAX_TAILLE_CHEMIN];
	snprintf(chemin, sizeof(chemin), "%s%s", PATH_MUSIQUES, nomFichier); // chemin vers la piste
	Mix_Music *piste = Mix_LoadMUS(chemin);
	verifAllocMix(piste, chemin, "Erreur: impossible creer piste Mix_LoadMUS");
	return piste;
}

Mix_Chunk * creerSon(const char *nomFichier) {
	char chemin[MAX_TAILLE_CHEMIN];
	snprintf(chemin, sizeof(chemin), "%s%s", PATH_BRUITAGES, nomFichier); // chemin vers le bruitage
	Mix_Chunk *son = Mix_LoadWAV(chemin);
	verifAllocMix(son, chemin, "Erreur: impossible creer bruitage Mix_LoadWAV");
	return son;
}

SDL_Texture * creerTextureVide(SDL_Renderer *renderer, int largeur, int hauteur) {
	SDL_Texture * texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, largeur, hauteur);
	if (!texture) ExceptionSDL("Impossible créer texture vide");
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	return texture;
}

SDL_Texture * creerTextureDepuisTexte(SDL_Renderer *renderer, const char *texte, TTF_Font *police, SDL_Color couleur) {
	SDL_Surface * surface = TTF_RenderUTF8_Blended(police, texte, couleur);
	verifAllocTTF(surface, texte, "Erreur: impossible de creer la surface du texte avec TTF_RenderUTF8_Blended");
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
	verifAllocSDL(texture, texte, "Erreur: impossible de creer la texture du texte avec SDL_CreateTextureFromSurface");
	SDL_FreeSurface(surface);
	return texture;
}

SDL_Texture * creerTextureLimiteDepuisTexte(SDL_Renderer * renderer, const char * texte, TTF_Font * police, SDL_Color couleur, int largeurMax) {
	SDL_Surface * surface = TTF_RenderUTF8_Blended_Wrapped(police, texte, couleur, largeurMax);
	verifAllocTTF(surface, texte, "Erreur: impossible de creer la surface du texte avec TTF_RenderUTF8_Blended_Wrapped");
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
	verifAllocSDL(texture, texte, "Erreur: impossible creer texture texte SDL_CreateTextureFromSurface");
	SDL_FreeSurface(surface);
	return texture;
}

void dessinerTexture(SDL_Renderer * renderer, SDL_Texture * texture, const SDL_Rect * srcRect, const SDL_Rect * dstRect, const char * msgErr) {
	if (SDL_RenderCopy(renderer, texture, srcRect, dstRect) != 0) ExceptionSDL(msgErr);
}

void dessinerTexte(SDL_Renderer * renderer, const char *texte, TTF_Font *police, SDL_Color couleur, int x, int y) {
	SDL_Rect dstRect = {x, y, 0, 0};
	SDL_Texture * texture = creerTextureDepuisTexte(renderer, texte, police, couleur);
	SDL_QueryTexture(texture, NULL, NULL, &dstRect.w, &dstRect.h);
	dessinerTexture(renderer, texture, NULL, &dstRect,"Impossible de dessiner texture texte SDL_RenderCopy");
	SDL_DestroyTexture(texture);
}

void dessinerNombre(SDL_Renderer * renderer, int nombre, TTF_Font *police, SDL_Color couleur, int x, int y) {
	char *strNum = intToString(nombre);
	if (!strNum) Exception("Echec création string intToString");
	dessinerTexte(renderer, strNum, police, couleur, x, y);
	free(strNum);
}

void dessinerTexteLimite(SDL_Renderer *renderer, const char *texte, TTF_Font *police, SDL_Color couleur, int x, int y, int largeurMax) {
	SDL_Rect dstRect = {x, y, 0, 0};
	SDL_Texture * texture = creerTextureLimiteDepuisTexte(renderer, texte, police, couleur, largeurMax);
	SDL_QueryTexture(texture, NULL, NULL, &dstRect.w, &dstRect.h);
	dessinerTexture(renderer, texture, NULL, &dstRect, "Impossible de dessiner texture texte SDL_RenderCopy");
	SDL_DestroyTexture(texture);
}