// @author Alain Barbier alias "Metroidzeta"

#ifndef BASE_H
#define BASE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <time.h>
#include <limits.h>

#define TITRE_FENETRE "Slayers Online 2"

#define WINDOW_WIDTH 1280 // par défaut : 1280
#define WINDOW_HEIGHT 960 // par défaut : 960
#define UPS 30 // par défaut : 30
#define FPS 60 // par défaut : 60
#define NIVEAU_MAX 80 // par défaut : 80
#define TAILLE_CARTE_MAX 100 // par défaut : 100

// !! NE PAS TOUCHER CES CONSTANTES !!
#define BASE_CASE_SIZE (WINDOW_HEIGHT / 20)
#define TAILLE_CASES (BASE_CASE_SIZE - (BASE_CASE_SIZE % 4)) // par défaut : 48
#define DEPLACEMENT_HEROS (TAILLE_CASES / 4)
#define WINDOW_WIDTH_CASES_DIV2 ((WINDOW_WIDTH / TAILLE_CASES) / 2)
#define WINDOW_HEIGHT_CASES_DIV2 ((WINDOW_HEIGHT / TAILLE_CASES) / 2)

// Couleurs RGBA
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

#define MAX_TAILLE_STRING 200
#define MAX_TAILLE_CHEMIN 256

// Chemins
#define PATH_IMAGES "img/"
#define PATH_MUSIQUES "musiques/"
#define PATH_BRUITAGES "bruitages/"
#define PATH_POLICES "polices/"

// Délais
#define DELAI_MS_FIOLES 588
#define DELAI_MS_SECONDE 1000
#define DELAI_MS_MINUTE 60000

#define LOG_ERROR(fmt, ...) fprintf(stderr, "[ERROR] " fmt "\n", ##__VA_ARGS__)

void Exception(const char *msgErr);
void ExceptionSDL(const char *msgErr);
void ExceptionTTF(const char *msgErr);
void ExceptionMix(const char *msgErr);
void verifAllocSDL(void *ptr, const char *chemin, const char *msgErr);
void verifAllocTTF(void *ptr, const char *chemin, const char *msgErr);
void verifAllocMix(void *ptr, const char *chemin, const char *msgErr);
void initSDL(SDL_Window **window, SDL_Renderer **renderer);
void freeSDL(SDL_Window * window, SDL_Renderer * renderer);
char *my_strdup(const char *src);
int minDouble(double a, double b);
int minInt(int a, int b);
int maxInt(int a, int b);
char * intToString(int x);
void copyIntArray(int *dst, const int *src, int taille);
bool ** creerMatriceBOOL(int lignes, int colonnes, bool valeurDefaut);
void freeMatriceBOOL(bool ** matrice, int lignes);
int ** creerMatriceINT(int lignes, int colonnes, int valeurDefaut);
void freeMatriceINT(int ** matrice, int lignes);
void changerCouleurRendu(SDL_Renderer *renderer, SDL_Color couleur);
void effacerEcran(SDL_Renderer *renderer);
void dessinerRectangle(SDL_Renderer *renderer, SDL_Rect *rect, SDL_Color couleur);
SDL_Texture * creerImage(SDL_Renderer * renderer, const char *nomFichier);
TTF_Font * creerPolice(const char *nomFichier, int taille);
Mix_Music * creerPiste(const char *nomFichier);
Mix_Chunk * creerSon(const char *nomFichier);
SDL_Texture * creerTextureVide(SDL_Renderer *renderer, int largeur, int hauteur);
SDL_Texture * creerTextureDepuisTexte(SDL_Renderer *renderer, const char *texte, TTF_Font *police, SDL_Color couleur);
SDL_Texture * creerTextureLimiteDepuisTexte(SDL_Renderer *renderer, const char *texte, TTF_Font *police, SDL_Color couleur, int largeurMax);
void dessinerTexture(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_Rect * srcRect, const SDL_Rect * dstRect, const char * msgErr);
void dessinerTexte(SDL_Renderer *renderer, const char * texte, TTF_Font *police, SDL_Color couleur, int x, int y);
void dessinerNombre(SDL_Renderer *renderer, int nombre, TTF_Font *police, SDL_Color couleur, int x, int y);
void dessinerTexteLimite(SDL_Renderer *renderer, const char *texte, TTF_Font *police, SDL_Color couleur, int x, int y, int largeurMax);

#endif