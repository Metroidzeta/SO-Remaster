// @author Alain Barbier alias "Metroidzeta"

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
	char * str = malloc(intlen(x) + 1); verifAlloc(str,"Erreur d'allocation du string dans la fonction intToString");
	sprintf(str,"%d",x);
	return str;
}

void bool_array_copy(bool * dstTab, bool * srcTab, int taille) {
	for(int i = 0; i < taille; i++) {
		dstTab[i] = srcTab[i];
	}
}

bool ** creerMatriceBOOL(int lignes, int colonnes, bool valeurParDefaut, const char * msgErr) {
	bool ** matrice = malloc(sizeof(bool*) * lignes); verifAlloc(matrice,msgErr);
	for(int i = 0; i < lignes; i++) {
		matrice[i] = malloc(sizeof(bool) * colonnes); verifAllocLigne(matrice,i,msgErr);
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
	int ** matrice = malloc(sizeof(int*) * lignes); verifAlloc(matrice,msgErr);
	for(int i = 0; i < lignes; i++) {
		matrice[i] = malloc(sizeof(int) * colonnes); verifAllocLigne(matrice,i,msgErr);
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

SDL_Texture * creerAffichage(SDL_Renderer * renderer, char * nomFichier) {
	char chemin[100] = "img/"; // chemin vers l'image de l'affichage
	strcat(chemin,nomFichier);
	return creerTextureDepuisImage(renderer,chemin);
}

TTF_Font * creerPolice(char * nomFichier, int taille) {
	char chemin[100] = "polices/"; // chemin vers la police
	strcat(chemin,nomFichier);
	TTF_Font * police = TTF_OpenFont(chemin, taille); verifAllocTTF(police,chemin,"Erreur: impossible de creer la police avec TTF_OpenFont");
	return police;
}

Mix_Music * creerPiste(char * nomFichier) {
	char chemin[100] = "musiques/"; // chemin vers la musique
	strcat(chemin,nomFichier);
	Mix_Music * piste = Mix_LoadMUS(chemin); verifAllocMix(piste,chemin,"Erreur: impossible de creer la piste de la musique avec Mix_LoadMUS");
	return piste;
}

Mix_Chunk * creerSon(char * nomFichier) {
	char chemin[100] = "bruitages/"; // chemin vers le bruitage
	strcat(chemin,nomFichier);
	Mix_Chunk * son = Mix_LoadWAV(chemin); verifAllocMix(son,chemin,"Erreur: impossible de creer le son du bruitage avec Mix_LoadWAV");
	return son;
}

SDL_Texture * creerTextureVide(SDL_Renderer * renderer, int largeur, int hauteur) {
	SDL_Texture * texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, largeur, hauteur);
	if(!texture) {
		afficherErrSDL("Erreur lors de la création de la texture vide");
	}
	SDL_SetTextureBlendMode(texture,SDL_BLENDMODE_BLEND);
	return texture;
}

SDL_Texture * creerTextureDepuisImage(SDL_Renderer * renderer, const char * chemin) {
	SDL_Texture * texture = IMG_LoadTexture(renderer,chemin); verifAllocSDL(texture,chemin,"Erreur: impossible de creer la texture de l'image avec IMG_LoadTexture");
	return texture;
}

SDL_Texture * creerTextureDepuisTexte(SDL_Renderer * renderer, char * texte, TTF_Font * police, SDL_Color couleur) {
	SDL_Surface * surface = TTF_RenderUTF8_Blended(police,texte,couleur); verifAllocTTF(surface,texte,"Erreur: impossible de creer la surface du texte avec TTF_RenderUTF8_Blended");
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer,surface); verifAllocSDL(texture,texte,"Erreur: impossible de creer la texture du texte avec SDL_CreateTextureFromSurface");
	SDL_FreeSurface(surface);
	return texture;
}

SDL_Texture * creerTextureLimiteDepuisTexte(SDL_Renderer * renderer, char * texte, TTF_Font * police, SDL_Color couleur, int largeurMax) {
	SDL_Surface * surface = TTF_RenderUTF8_Blended_Wrapped(police,texte,couleur,largeurMax); verifAllocTTF(surface,texte,"Erreur: impossible de creer la surface du texte avec TTF_RenderUTF8_Blended_Wrapped");
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer,surface); verifAllocSDL(texture,texte,"Erreur: impossible de creer la texture du texte avec SDL_CreateTextureFromSurface");
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
	SDL_Texture * textureTexte = creerTextureDepuisTexte(renderer,texte,police,couleur);
	SDL_QueryTexture(textureTexte,NULL,NULL,&dstRect.w,&dstRect.h);
	dessinerTexture(renderer,textureTexte,NULL,&dstRect,"Impossible de dessiner la texture du texte avec SDL_RenderCopy");
	SDL_DestroyTexture(textureTexte);
}

void dessinerNombre(SDL_Renderer * renderer, int nombre, TTF_Font * police, SDL_Color couleur, int x, int y) {
	char * strNombre = intToString(nombre);
	dessinerTexte(renderer,strNombre,police,couleur,x,y);
	free(strNombre);
}

void dessinerTexteLimite(SDL_Renderer * renderer, char * texte, TTF_Font * police, SDL_Color couleur, int x, int y, int largeurMax) {
	SDL_Rect dstRect = {x, y, 0, 0};
	SDL_Texture * textureTexte = creerTextureLimiteDepuisTexte(renderer,texte,police,couleur,largeurMax);
	SDL_QueryTexture(textureTexte,NULL,NULL,&dstRect.w,&dstRect.h);
	dessinerTexture(renderer,textureTexte,NULL,&dstRect,"Impossible de dessiner la texture du texte avec SDL_RenderCopy");
	SDL_DestroyTexture(textureTexte);
}