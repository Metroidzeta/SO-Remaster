// @author Metroidzeta

#include "base.h"

void Exception(const char * msgErr) {
	fprintf(stderr,"Exception: %s\n",msgErr);
	exit(EXIT_FAILURE);
}

void verifAlloc(void * ptr, const char * msgErr) {
	if(!ptr) {
		fprintf(stderr,"%s: %s\n",msgErr,strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void verifAllocLigne(void * ptr, int i, const char * msgErr) {
	if(!ptr) {
		fprintf(stderr,"%s (erreur ligne: %d): %s\n",msgErr,i,strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void verifAllocStrCopy(void * ptr, char * strSrc) {
	if(!ptr) {
		fprintf(stderr,"Erreur d'allocation du string %s: %s\n",strSrc,strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void afficherErrSDL(const char * msgErr) {
	fprintf(stderr,"%s: %s\n",msgErr,SDL_GetError());
	exit(EXIT_FAILURE);
}

void afficherErrTTF(const char * msgErr) {
	fprintf(stderr,"%s: %s\n",msgErr,TTF_GetError());
	exit(EXIT_FAILURE);
}

void afficherErrMix(const char * msgErr) {
	fprintf(stderr,"%s: %s\n",msgErr,Mix_GetError());
	exit(EXIT_FAILURE);
}

void verifAllocSDL(void * ptr, const char * chemin, const char * msgErr) {
	if(!ptr) {
		fprintf(stderr,"%s (chemin: %s): %s\n",msgErr,chemin,SDL_GetError());
		exit(EXIT_FAILURE);
	}
}

void verifAllocTTF(void * ptr, const char * chemin, const char * msgErr) {
	if(!ptr) {
		fprintf(stderr,"%s (chemin: %s): %s\n",msgErr,chemin,TTF_GetError());
		exit(EXIT_FAILURE);
	}
}

void verifAllocMix(void * ptr, const char * chemin, const char * msgErr) {
	if(!ptr) {
		fprintf(stderr,"%s (chemin: %s): %s\n",msgErr,chemin,Mix_GetError());
		exit(EXIT_FAILURE);
	}
}

void initialiserSDL(SDL_Window ** window, SDL_Renderer ** renderer, char * titre_fenetre) {
	// Initialisation SDL Vidéo
	if(SDL_Init(SDL_INIT_VIDEO) != 0) { afficherErrSDL("Erreur d'initialisation SDL_INIT_VIDEO"); }
	// Initialisation SDL Audio
	if(SDL_Init(SDL_INIT_AUDIO) != 0) { afficherErrSDL("Erreur d'initialisation SDL_INIT_AUDIO"); }
	// Initialisation SDL Mixer audio
	if(Mix_OpenAudio(44100, AUDIO_S32SYS, 2, 1024) != 0) { afficherErrMix("Erreur d'initialisation Mix_OpenAudio"); }
	// Initialisation SDL TTF
	if(TTF_Init() != 0) { afficherErrTTF("Erreur d'initialisation TTF_Init"); }
	// Création de la fenêtre
	*window = SDL_CreateWindow(titre_fenetre, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if(!*window) { 
		afficherErrSDL("Erreur: impossible de creer la fenetre avec SDL_CreateWindow"); 
	}
	// Création du rendu
	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
	if(!*renderer) { 
		afficherErrSDL("Erreur: impossible de creer le rendu avec SDL_CreateRenderer"); 
	}
}

void freeSDL(SDL_Window * window, SDL_Renderer * renderer) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	Mix_CloseAudio();
	SDL_Quit();
}

int uintlen(unsigned x) {
	int len = 1;
	while(x /= 10) {
		len++;
	}
	return len;
}

int intlen(int x) {
	return x < 0 ? uintlen(-x) + 1 : uintlen(x);
}

char * intToString(int x) {
	char * str = malloc(intlen(x) + 1);
	verifAlloc(str,"Erreur d'allocation du string dans la fonction intToString");
	sprintf(str,"%d",x);
	return str;
}

void bool_array_copy(bool * dstTab, bool * srcTab, int taille) {
	for(int i = 0; i < taille; i++) {
		dstTab[i] = srcTab[i];
	}
}

bool ** creerMatriceBOOL(int lignes, int colonnes, bool valeurParDefaut, const char * msgErr) {
	bool ** matrice = malloc(sizeof(bool*) * lignes);
	verifAlloc(matrice,msgErr);

	for(int i = 0; i < lignes; i++) {
		matrice[i] = malloc(sizeof(bool) * colonnes);
		verifAllocLigne(matrice,i,msgErr);
		memset(matrice[i],valeurParDefaut,sizeof(bool) * colonnes);
	}
	return matrice;
}

void freeMatriceBOOL(bool ** matrice, int lignes) {
	for(int i = 0; i < lignes; i++) {
		free(matrice[i]);
	}
	free(matrice);
}

int ** creerMatriceINT(int lignes, int colonnes, int valeurParDefaut, const char * msgErr) {
	int ** matrice = malloc(sizeof(int*) * lignes);
	verifAlloc(matrice,msgErr);

	for(int i = 0; i < lignes; i++) {
		matrice[i] = malloc(sizeof(int) * colonnes);
		verifAllocLigne(matrice,i,msgErr);
		memset(matrice[i],valeurParDefaut,sizeof(int) * colonnes);
	}
	return matrice;
}

void freeMatriceINT(int ** matrice, int lignes) {
	for(int i = 0; i < lignes; i++) {
		free(matrice[i]);
	}
	free(matrice);
}

FILE * ouvrirFichier(const char * chemin, const char * typeOuverture, const char * msgErr) {
	FILE * f = fopen(chemin,typeOuverture);
	if(!f) {
		fprintf(stderr,"%s (typeOuverture: %s , chemin: %s) : %s\n",msgErr,typeOuverture,chemin,strerror(errno));
		exit(EXIT_FAILURE);
	}
	return f;
}

void changerCouleurRendu(SDL_Renderer * renderer, SDL_Color couleur) {
	if(SDL_SetRenderDrawColor(renderer, couleur.r, couleur.g, couleur.b, couleur.a) != 0) {
		afficherErrSDL("Erreur: impossible de changer la couleur du rendu avec SDL_SetRenderDrawColor");
	}
}

void effacerEcran(SDL_Renderer * renderer) {
	changerCouleurRendu(renderer,NOIR);
	SDL_RenderClear(renderer);
}

void dessinerRectangle(SDL_Renderer * renderer, SDL_Rect * rectangle, SDL_Color couleur) {
	SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
	changerCouleurRendu(renderer,couleur);
	if(SDL_RenderFillRect(renderer,rectangle) != 0) {
		afficherErrSDL("Erreur: impossible de dessiner le rectangle avec SDL_RenderFillRect");
	}
}

bool CollisionRect(SDL_Rect * R1,SDL_Rect * R2) {
	return (R1->x + R1->w > R2->x) && (R1->y + R1->h > R2->y) && (R1->x < R2->x + R2->w) && (R1->y < R2->y + R2->h);
}

TTF_Font * creerPolice(const char * chemin, int taille) {
	TTF_Font * police = TTF_OpenFont(chemin, taille);
	verifAllocTTF(police,chemin,"Erreur: impossible de creer la police avec TTF_OpenFont");

	return police;
}

SDL_Texture * creerTextureImage(SDL_Renderer * renderer, const char * chemin) {
	SDL_Texture * texture = IMG_LoadTexture(renderer,chemin);
	verifAllocSDL(texture,chemin,"Erreur: impossible de creer la texture de l'image avec IMG_LoadTexture");

	return texture;
}

SDL_Texture * creerTextureTexte(SDL_Renderer * renderer, char * texte, TTF_Font * police, SDL_Color couleur) {
	SDL_Surface * surface = TTF_RenderUTF8_Blended(police,texte,couleur);
	verifAllocTTF(surface,texte,"Erreur: impossible de creer la surface du texte avec TTF_RenderUTF8_Blended");

	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer,surface);
	verifAllocSDL(texture,texte,"Erreur: impossible de creer la texture du texte avec SDL_CreateTextureFromSurface");

	SDL_FreeSurface(surface);
	return texture;
}

SDL_Texture * creerTextureTexteLimite(SDL_Renderer * renderer, char * texte, TTF_Font * police, SDL_Color couleur, int largeurMax) {
	SDL_Surface * surface = TTF_RenderUTF8_Blended_Wrapped(police,texte,couleur,largeurMax);
	verifAllocTTF(surface,texte,"Erreur: impossible de creer la surface du texte avec TTF_RenderUTF8_Blended_Wrapped");

	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer,surface);
	verifAllocSDL(texture,texte,"Erreur: impossible de creer la texture du texte avec SDL_CreateTextureFromSurface");

	SDL_FreeSurface(surface);
	return texture;
}

void dessinerTexture(SDL_Renderer * renderer, SDL_Texture * texture, const SDL_Rect * srcRect, const SDL_Rect * dstRect, const char * msgErr) {
	if(SDL_RenderCopy(renderer,texture,srcRect,dstRect) != 0) {
		afficherErrSDL(msgErr);
	}
}

void dessinerTexte(SDL_Renderer * renderer, char * texte, TTF_Font * police, SDL_Color couleur, int x, int y) {
	SDL_Rect dstRect = {x, y, 0, 0};
	SDL_Texture * textureTexte = creerTextureTexte(renderer,texte,police,couleur);
	SDL_QueryTexture(textureTexte,NULL,NULL,&dstRect.w,&dstRect.h);
	dessinerTexture(renderer,textureTexte,NULL,&dstRect,"Impossible de dessiner la texture du texte avec SDL_RenderCopy");
	SDL_DestroyTexture(textureTexte);
}

void dessinerNombre(SDL_Renderer * renderer, int nombre, TTF_Font * police, SDL_Color couleur, int x, int y) {
	char * nombreToString = intToString(nombre);
	dessinerTexte(renderer,nombreToString,police,couleur,x,y);
	free(nombreToString);
}

void dessinerTexteLimite(SDL_Renderer * renderer, char * texte, TTF_Font * police, SDL_Color couleur, int x, int y, int largeurMax) {
	SDL_Rect dstRect = {x, y, 0, 0};
	SDL_Texture * textureTexte = creerTextureTexteLimite(renderer,texte,police,couleur,largeurMax);
	SDL_QueryTexture(textureTexte,NULL,NULL,&dstRect.w,&dstRect.h);
	dessinerTexture(renderer,textureTexte,NULL,&dstRect,"Impossible de dessiner la texture du texte avec SDL_RenderCopy");
	SDL_DestroyTexture(textureTexte);
}