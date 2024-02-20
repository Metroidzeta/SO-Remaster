// @author Metroidzeta

#ifndef BASE_H
#define BASE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <time.h>

#define WINDOW_WIDTH 1280 // par défaut : 1280
#define WINDOW_HEIGHT 960 // par défaut : 960
#define UPS 30 // par défaut : 30
#define FPS 60 // par défaut : 60
#define NIVEAU_MAX 80 // par défaut : 80

// /!\ NE PAS TOUCHER CES CONSTANTES /!\ :
#define TAILLE_CASES ((WINDOW_HEIGHT / 20) % 4 == 0 ? WINDOW_HEIGHT / 20 : (WINDOW_HEIGHT / 20) - ((WINDOW_HEIGHT / 20) % 4)) // par défaut : 48
#define DEPLACEMENT_JOUEUR (TAILLE_CASES / 4)
#define WINDOW_WIDTH_CASES_DIV2 ((WINDOW_WIDTH / TAILLE_CASES) / 2)
#define WINDOW_HEIGHT_CASES_DIV2 ((WINDOW_HEIGHT / TAILLE_CASES) / 2)

// Couleurs RVB :
#define BLANC (SDL_Color) {255, 255, 255, 255}
#define BLANC_TRANSPARENT (SDL_Color) {255, 255, 255, 180}
#define GRIS_FONCE_TRANSPARENT (SDL_Color) {58, 58, 58, 180}
#define ROUGE (SDL_Color) {255, 0, 0, 255}
#define BORDEAUX_TRANSPARENT (SDL_Color) {109, 7, 26, 180}
#define VERT (SDL_Color) {0, 255, 0, 255}
#define VERT_FONCE (SDL_Color) {0, 100, 0, 255}
#define VERT_FONCE_TRANSPARENT (SDL_Color) {0, 100, 0, 180}
#define BLEU (SDL_Color) {0, 0, 255, 255}
#define BLEU_FONCE_TRANSPARENT (SDL_Color) {0, 0, 189, 180}
#define OR_FONCE_TRANSPARENT (SDL_Color) {181, 148, 16, 180}
#define VIOLET_TRANSPARENT (SDL_Color) {143, 0, 255, 128}
#define NOIR (SDL_Color) {0, 0, 0, 255}

void Exception(const char * msgErr);
void verifAlloc(void * ptr, const char * msgErr);
void verifAllocLigne(void * ptr, int i, const char * msgErr);
void verifAllocStrCopy(void * ptr, char * strSrc);
void afficherErrSDL(const char * msgErr);
void afficherErrTTF(const char * msgErr);
void afficherErrMix(const char * msgErr);
void verifAllocSDL(void * ptr, const char * chemin, const char * msgErr);
void verifAllocTTF(void * ptr, const char * chemin, const char * msgErr);
void verifAllocMix(void * ptr, const char * chemin, const char * msgErr);
void initialiserSDL(SDL_Window ** window, SDL_Renderer ** renderer, char * titre_fenetre);
void freeSDL(SDL_Window * window, SDL_Renderer * renderer);
int uintlen(unsigned x);
int intlen(int x);
char * intToString(int x);
void bool_array_copy(bool * dstTab, bool * srcTab, int taille);
bool ** creerMatriceBOOL(int lignes, int colonnes, bool valeurParDefaut, const char * msgErr);
void freeMatriceBOOL(bool ** matrice, int lignes);
int ** creerMatriceINT(int lignes, int colonnes, int valeurParDefaut, const char * msgErr);
void freeMatriceINT(int ** matrice, int lignes);
FILE * ouvrirFichier(const char * chemin, const char * typeOuverture, const char * msgErr);
void changerCouleurRendu(SDL_Renderer * renderer, SDL_Color couleur);
void effacerEcran(SDL_Renderer * renderer);
void dessinerRectangle(SDL_Renderer * renderer, SDL_Rect * rectangle, SDL_Color couleur);
SDL_Texture * creerAffichage(SDL_Renderer * renderer, char * nomFichier);
TTF_Font * creerPolice(char * nomFichier, int taille);
Mix_Music * creerPiste(char * nomFichier);
Mix_Chunk * creerSon(char * nomFichier);
SDL_Texture * creerTextureVide(SDL_Renderer * renderer, int largeur, int hauteur);
SDL_Texture * creerTextureDepuisImage(SDL_Renderer * renderer, const char * chemin);
SDL_Texture * creerTextureDepuisTexte(SDL_Renderer * renderer, char * texte, TTF_Font * police, SDL_Color couleur);
SDL_Texture * creerTextureLimiteDepuisTexte(SDL_Renderer * renderer, char * texte, TTF_Font * police, SDL_Color couleur, int largeurMax);
void dessinerTexture(SDL_Renderer * renderer, SDL_Texture * texture, const SDL_Rect * srcRect, const SDL_Rect * dstRect, const char * msgErr);
void dessinerTexte(SDL_Renderer * renderer, char * texte, TTF_Font * police, SDL_Color couleur, int x, int y);
void dessinerNombre(SDL_Renderer * renderer, int nombre, TTF_Font * police, SDL_Color couleur, int x, int y);
void dessinerTexteLimite(SDL_Renderer * renderer, char * texte, TTF_Font * police, SDL_Color couleur, int x, int y, int largeurMax);

#endif