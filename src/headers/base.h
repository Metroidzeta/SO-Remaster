/**
 * @author Alain Barbier alias "Metroidzeta"
 * Copyright © 2025 Alain Barbier (Metroidzeta) - All rights reserved.
 *
 * This file is part of the project covered by the
 * "Educational and Personal Use License / Licence d’Utilisation Personnelle et Éducative".
 *
 * Permission is granted to fork and use this code for educational and personal purposes only.
 *
 * Commercial use, redistribution, or public republishing of modified versions
 * is strictly prohibited without the express written consent of the author.
 *
 * Coded with SDL2 (Simple DirectMedia Layer 2).
 *
 * Created by Metroidzeta.
 */

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

#include "cJSON.h"

#define TITRE_FENETRE "Slayers RPG Engine C"

#define WINDOW_WIDTH 1280 // par défaut : 1280
#define WINDOW_HEIGHT 960 // par défaut : 960
#define UPS 30 // par défaut : 30
#define FPS 60 // par défaut : 60
#define NIVEAU_MAX 80 // par défaut : 80

// !! NE PAS TOUCHER CES CONSTANTES !!
#define BASE_CASE_SIZE (WINDOW_HEIGHT / 20)
#define TAILLE_CASES (BASE_CASE_SIZE - (BASE_CASE_SIZE % 4)) // par défaut : 48
#define DEPLACEMENT_HEROS (TAILLE_CASES / 4)
#define WINDOW_WIDTH_CASES ((WINDOW_WIDTH + TAILLE_CASES - 1) / TAILLE_CASES)
#define WINDOW_HEIGHT_CASES ((WINDOW_HEIGHT + TAILLE_CASES - 1) / TAILLE_CASES)
#define VB_VALUES { 112, 57, 15, 232, 214, 177, 147, 108, 80, 32, 18 }

// Couleurs RGBA
#define NOIR                      (SDL_Color) {0, 0, 0, 255}
#define BLANC                     (SDL_Color) {255, 255, 255, 255}
#define BLANC_TRANSPARENT         (SDL_Color) {255, 255, 255, 180}
#define GRIS_CLAIR_TRANSPARENT    (SDL_Color) {180, 190, 200, 48}
#define GRIS_FONCE_TRANSPARENT    (SDL_Color) {58, 58, 58, 180}
#define ROUGE                     (SDL_Color) {255, 0, 0, 255}
#define BORDEAUX_TRANSPARENT      (SDL_Color) {109, 7, 26, 180}
#define VERT                      (SDL_Color) {0, 255, 0, 255}
#define VERT_FONCE                (SDL_Color) {0, 100, 0, 255}
#define VERT_FONCE_TRANSPARENT    (SDL_Color) {0, 100, 0, 180}
#define BLEU                      (SDL_Color) {0, 0, 255, 255}
#define BLEU_FONCE_TRANSPARENT    (SDL_Color) {0, 0, 189, 180}
#define OR_FONCE_TRANSPARENT      (SDL_Color) {181, 148, 16, 180}
#define VIOLET_TRANSPARENT        (SDL_Color) {143, 0, 255, 128}

// Chemins
#define PATH_IMAGES     "img/"
#define PATH_MUSIQUES   "musiques/"
#define PATH_BRUITAGES  "bruitages/"
#define PATH_POLICES    "polices/"

// Limites
#define MAX_TAILLE_STRING 200
#define MAX_TAILLE_CHEMIN 256
#define FIOLES_ANIMATION_FRAMES 3

// Logs
#define LOG_ERROR(fmt, ...) fprintf(stderr, "[ERROR] " fmt "\n", ##__VA_ARGS__)

void Exception(const char *msgErr);
void ExceptionSDL(const char *msgErr);
void ExceptionTTF(const char *msgErr);
void ExceptionMix(const char *msgErr);
void verifAllocSDL(void *ptr, const char *msgErr, const char *src);
void verifAllocTTF(void *ptr, const char *msgErr, const char *src);
void verifAllocMix(void *ptr, const char *msgErr, const char *src);
void initSDL(SDL_Window **window, SDL_Renderer **renderer);
void freeSDL(SDL_Window *window, SDL_Renderer *renderer);
char * my_strdup(const char *src);
int minInt(int a, int b);
int maxInt(int a, int b);
double minDouble(double a, double b);
double maxDouble(double a, double b);
double my_floor(double x);
char * intToString(int x);
int keyForIndex(int i);
void copyIntArray(int *dst, const int *src, int taille);
bool ** creerMatriceBool(int lignes, int colonnes, bool valeurDefaut);
void freeMatriceBool(bool **matrice);
int ** creerMatriceInt(int lignes, int colonnes, int valeurDefaut);
void freeMatriceInt(int **matrice);
void changerCouleurRendu(SDL_Renderer *renderer, SDL_Color couleur);

SDL_Texture * creerTexture(SDL_Renderer *renderer, const char *nomFichier);
TTF_Font * creerPolice(const char *nomFichier, int taille);
Mix_Music * creerPiste(const char *nomFichier);
Mix_Chunk * creerSon(const char *nomFichier);
SDL_Texture * creerTextureVide(SDL_Renderer *renderer, int largeur, int hauteur);
SDL_Texture * creerTextureDepuisTexte(SDL_Renderer *renderer, const char *texte, TTF_Font *police, SDL_Color couleur);
SDL_Texture * creerTextureLimiteDepuisTexte(SDL_Renderer *renderer, const char *texte, TTF_Font *police, SDL_Color couleur, int largeurMax);

void dessinerRectangle(SDL_Renderer *renderer, SDL_Rect *rect, SDL_Color couleur);
void dessinerTexture(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_Rect *srcRect, const SDL_Rect *dstRect, const char *msgErr);
void dessinerTexte(SDL_Renderer *renderer, const char *texte, TTF_Font *police, SDL_Color couleur, int x, int y);
void dessinerNombre(SDL_Renderer *renderer, int nombre, TTF_Font *police, SDL_Color couleur, int x, int y);
void dessinerTexteLimite(SDL_Renderer *renderer, const char *texte, TTF_Font *police, SDL_Color couleur, int x, int y, int largeurMax);

void printJSON_custom(FILE *f, cJSON *item, int indent, bool inline_val);
cJSON * cJSON_ParseWithFile(const char *nomFichier);

#endif