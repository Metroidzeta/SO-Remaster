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

#include "headers/base.h"

void Exception(const char *msgErr) { LOG_ERROR("Exception: %s", msgErr ? msgErr : ""); exit(EXIT_FAILURE); }
void ExceptionSDL(const char *msgErr) { LOG_ERROR("Exception: %s (%s)", msgErr ? msgErr : "", SDL_GetError()); exit(EXIT_FAILURE); }
void ExceptionTTF(const char *msgErr) { LOG_ERROR("Exception: %s (%s)", msgErr ? msgErr : "", TTF_GetError()); exit(EXIT_FAILURE); }
void ExceptionMix(const char *msgErr) { LOG_ERROR("Exception: %s (%s)", msgErr ? msgErr : "", Mix_GetError()); exit(EXIT_FAILURE); }

void verifAllocSDL(void *ptr, const char *msgErr, const char *src) {
	if (!msgErr || !src) return;
	if (!ptr) { LOG_ERROR("Exception: %s (src: %s): %s", msgErr, src, SDL_GetError()); exit(EXIT_FAILURE); }
}

void verifAllocTTF(void *ptr, const char *msgErr, const char *src) {
	if (!msgErr || !src) return;
	if (!ptr) { LOG_ERROR("Exception: %s (src: %s): %s", msgErr, src, TTF_GetError()); exit(EXIT_FAILURE); }
}

void verifAllocMix(void *ptr, const char *msgErr, const char *src) {
	if (!msgErr || !src) return;
	if (!ptr) { LOG_ERROR("Exception: %s (src: %s): %s", msgErr, src, Mix_GetError()); exit(EXIT_FAILURE); }
}

static inline void closeMix_TTF_SDL() { Mix_CloseAudio(); TTF_Quit(); SDL_Quit(); }

void initSDL(SDL_Window **window, SDL_Renderer **renderer) {
	if (!window || !renderer) Exception("Window ou renderer NULL");
	if (*window || *renderer) Exception("Window ou renderer deja initialises");
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) ExceptionSDL("Echec init SDL_INIT_VIDEO | SDL_INIT_AUDIO");
	if (TTF_Init() != 0) { Mix_CloseAudio(); SDL_Quit(); ExceptionTTF("Echec init TTF_Init"); } // init SDL TTF
	if (Mix_OpenAudio(44100, AUDIO_S32SYS, 2, 1024) != 0) { SDL_Quit(); ExceptionMix("Echec init Mix_OpenAudio"); }  // init SDL Mixer audio

	*window = SDL_CreateWindow(TITRE_FENETRE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN); // création fenêtre
	if (!*window) { closeMix_TTF_SDL(); ExceptionSDL("Echec init SDL_CreateWindow"); }

	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED); // création rendu
	if (!*renderer) { SDL_DestroyWindow(*window); *window = NULL; closeMix_TTF_SDL(); ExceptionSDL("Echec init SDL_CreateRenderer"); }
}

void freeSDL(SDL_Window *window, SDL_Renderer *renderer) {
	if (renderer) SDL_DestroyRenderer(renderer);
	if (window) SDL_DestroyWindow(window);
	closeMix_TTF_SDL();
}

char * my_strdup(const char *src) { 
	if (!src) return NULL;
	size_t len = strlen(src) + 1; // +1 pour inclure le caractère nul '\0' de fin de chaîne
	char *dst = malloc(len);
	if (!dst) return NULL;
	memcpy(dst, src, len);
	return dst;
}

int minInt(int a, int b) { return (a < b) ? a : b; }
int maxInt(int a, int b) { return (a > b) ? a : b; }
double minDouble(double a, double b) { return (a < b) ? a : b; }
double maxDouble(double a, double b) { return (a > b) ? a : b; }

double my_floor(double x) {
	int i = (int)x;
	return (x < (double)i) ? (double)(i - 1) : (double)i;
}

static inline int uintlen(unsigned x) {
	int len = 1;
	while (x /= 10) len++;
	return len;
}

static inline int intlen(int x) { return x < 0 ? uintlen(-(unsigned)x) + 1 : uintlen((unsigned)x); }

char * intToString(int x) {
	int len = intlen(x) + 1;
	char *str = malloc(len);
	if (!str) return NULL;
	snprintf(str, len, "%d", x);
	return str;
}

int keyForIndex(int i) { return (i * 31 + 7) & 0xFF; }
void copyIntArray(int *dst, const int *src, int taille) { memcpy(dst, src, taille * sizeof(int)); }

bool ** creerMatriceBool(int lignes, int colonnes, bool valDefaut) {
	if (lignes < 1 || colonnes < 1) return NULL;
	bool **matrice = malloc(lignes * sizeof(bool *) + lignes * colonnes * sizeof(bool)); // allocation contiguë (pointeurs de lignes + données)
	if (!matrice) return NULL;

	bool *data = (bool *)(matrice + lignes); // bloc de données commence juste après le tableau de pointeurs
	for (int i = 0; i < lignes; i++) matrice[i] = data + i * colonnes; // init des pointeurs pour chaque ligne

	// Initialisation des valeurs
	if (!valDefaut) memset(data, 0, lignes * colonnes * sizeof(bool)); // + rapide si défaut = false
	else for (int i = 0; i < lignes * colonnes; i++) data[i] = true;

	return matrice;
}

void freeMatriceBool(bool **matrice) { if (matrice) free(matrice); }

int ** creerMatriceInt(int lignes, int colonnes, int valDefaut) {
	if (lignes < 1 || colonnes < 1) return NULL;
	int **matrice = malloc(lignes * sizeof(int *) + lignes * colonnes * sizeof(int)); // allocation contiguë (pointeurs de lignes + données)
	if (!matrice) return NULL;

	int *data = (int *)(matrice + lignes); // bloc de données commence juste après le tableau de pointeurs
	for (int i = 0; i < lignes; i++) matrice[i] = data + i * colonnes; // init des pointeurs pour chaque ligne

	// Initialisation des valeurs
	if (valDefaut == 0) memset(data, 0, lignes * colonnes * sizeof(int)); // + rapide si défaut = 0
	else for (int i = 0; i < lignes * colonnes; i++) data[i] = valDefaut;

	return matrice;
}

void freeMatriceInt(int **matrice) { if (matrice) free(matrice); }

void changerCouleurRendu(SDL_Renderer *renderer, SDL_Color couleur) {
	if (SDL_SetRenderDrawColor(renderer, couleur.r, couleur.g, couleur.b, couleur.a) != 0) ExceptionSDL("Impossible changerCouleur SDL_SetRenderDrawColor");
}

static bool construireChemin(char *dest, size_t tailleDst, const char *prefixe, const char *nomFichier) {
	int nbChars = snprintf(dest, tailleDst, "%s%s", prefixe, nomFichier);
	return (nbChars > 0 && nbChars < (int)tailleDst);
}

SDL_Texture * creerTexture(SDL_Renderer *renderer, const char *nomFichier) {
	if (!renderer || !nomFichier) return NULL;
	char chemin[MAX_TAILLE_CHEMIN];
	if (!construireChemin(chemin, sizeof(chemin), PATH_IMAGES, nomFichier)) return NULL; // chemin vers l'image
	return IMG_LoadTexture(renderer, chemin);
}

TTF_Font * creerPolice(const char *nomFichier, int taille) {
	if (!nomFichier) return NULL;
	char chemin[MAX_TAILLE_CHEMIN];
	if (!construireChemin(chemin, sizeof(chemin), PATH_POLICES, nomFichier)) return NULL; // chemin vers la police
	return TTF_OpenFont(chemin, taille);
}

Mix_Music * creerPiste(const char *nomFichier) {
	if (!nomFichier) return NULL;
	char chemin[MAX_TAILLE_CHEMIN];
	if (!construireChemin(chemin, sizeof(chemin), PATH_MUSIQUES, nomFichier)) return NULL; // chemin vers la piste
	return Mix_LoadMUS(chemin);
}

Mix_Chunk * creerSon(const char *nomFichier) {
	if (!nomFichier) return NULL;
	char chemin[MAX_TAILLE_CHEMIN];
	if (!construireChemin(chemin, sizeof(chemin), PATH_BRUITAGES, nomFichier)) return NULL; // chemin vers le bruitage
	return Mix_LoadWAV(chemin);
}

SDL_Texture * creerTextureVide(SDL_Renderer *renderer, int largeur, int hauteur) {
	if (!renderer) return NULL;
	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, largeur, hauteur);
	if (!texture) ExceptionSDL("Echec creation de la texture vide");
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	return texture;
}

SDL_Texture * creerTextureDepuisTexte(SDL_Renderer *renderer, const char *texte, TTF_Font *police, SDL_Color couleur) {
	SDL_Surface *surface = TTF_RenderUTF8_Blended(police, texte, couleur);
	verifAllocTTF(surface, "Echec creation de la surface du texte avec TTF_RenderUTF8_Blended", texte);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	verifAllocSDL(texture, "Echec creation de la texture du texte avec SDL_CreateTextureFromSurface", texte);
	SDL_FreeSurface(surface);
	return texture;
}

SDL_Texture * creerTextureLimiteDepuisTexte(SDL_Renderer *renderer, const char *texte, TTF_Font *police, SDL_Color couleur, int largeurMax) {
	SDL_Surface *surface = TTF_RenderUTF8_Blended_Wrapped(police, texte, couleur, largeurMax);
	verifAllocTTF(surface, "Echec creation de la surface du texte avec TTF_RenderUTF8_Blended_Wrapped", texte);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	verifAllocSDL(texture, "Echec cration de la texture du texte avec SDL_CreateTextureFromSurface", texte);
	SDL_FreeSurface(surface);
	return texture;
}

void dessinerRectangle(SDL_Renderer *renderer, SDL_Rect *rect, SDL_Color couleur) {
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	changerCouleurRendu(renderer, couleur);
	if (SDL_RenderFillRect(renderer, rect) != 0) ExceptionSDL("Impossible dessiner rectangle SDL_RenderFillRect");
}

void dessinerTexture(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_Rect *srcRect, const SDL_Rect *dstRect, const char *msgErr) {
	if (SDL_RenderCopy(renderer, texture, srcRect, dstRect) != 0) ExceptionSDL(msgErr);
}

void dessinerTexte(SDL_Renderer *renderer, const char *texte, TTF_Font *police, SDL_Color couleur, int x, int y) {
	if (!renderer || !texte || !police) return;
	SDL_Rect dstRect = {x, y, 0, 0};
	SDL_Texture *texture = creerTextureDepuisTexte(renderer, texte, police, couleur);
	SDL_QueryTexture(texture, NULL, NULL, &dstRect.w, &dstRect.h);
	dessinerTexture(renderer, texture, NULL, &dstRect, "Echec dessin texture du texte avec SDL_RenderCopy");
	SDL_DestroyTexture(texture);
}

void dessinerNombre(SDL_Renderer *renderer, int nombre, TTF_Font *police, SDL_Color couleur, int x, int y) {
	if (!renderer || !police) return;
	char *strNum = intToString(nombre);
	if (!strNum) Exception("Echec allocation strNum intToString");
	dessinerTexte(renderer, strNum, police, couleur, x, y);
	free(strNum);
}

void dessinerTexteLimite(SDL_Renderer *renderer, const char *texte, TTF_Font *police, SDL_Color couleur, int x, int y, int largeurMax) {
	if (!renderer || !texte || !police) return;
	SDL_Rect dstRect = {x, y, 0, 0};
	SDL_Texture *texture = creerTextureLimiteDepuisTexte(renderer, texte, police, couleur, largeurMax);
	SDL_QueryTexture(texture, NULL, NULL, &dstRect.w, &dstRect.h);
	dessinerTexture(renderer, texture, NULL, &dstRect, "Echec dessin texture du texte avec SDL_RenderCopy");
	SDL_DestroyTexture(texture);
}

static inline void printIndentJSON(FILE *f, int indent) {
	for (int i = 0; i < indent * 2; ++i) fprintf(f, " ");
}

static void printEscapedString(FILE *f, const char *str) {
	fputc('"', f);
	for (; *str; ++str) {
		switch (*str) {
			case '\"': fputs("\\\"", f); break;
			case '\\': fputs("\\\\", f); break;
			case '\b': fputs("\\b", f); break;
			case '\f': fputs("\\f", f); break;
			case '\n': fputs("\\n", f); break;
			case '\r': fputs("\\r", f); break;
			case '\t': fputs("\\t", f); break;
			default:
				if ((unsigned char)*str < 0x20) fprintf(f, "\\u%04x", (unsigned char)*str);
				else fputc(*str, f);
		}
	}
	fputc('"', f);
}

static void printPrimitive(FILE *f, cJSON *item) {
	if (cJSON_IsString(item))       printEscapedString(f, item->valuestring);
	else if (cJSON_IsNumber(item))  fprintf(f, "%g", item->valuedouble);
	else if (cJSON_IsBool(item))    fprintf(f, cJSON_IsTrue(item) ? "true" : "false");
	else if (cJSON_IsNull(item))    fprintf(f, "null");
}

static bool isMatrix(cJSON *array) {
	int size = cJSON_GetArraySize(array);
	if (size == 0) return false;
	for (int i = 0; i < size; ++i)
		if (!cJSON_IsArray(cJSON_GetArrayItem(array, i))) return false;
	return true;
}

void printJSON_custom(FILE *f, cJSON *item, int indent, bool inline_val) {
	if (!f || !item) return;

	if (cJSON_IsObject(item)) {
		if (!inline_val) printIndentJSON(f, indent);
		fprintf(f, "{\n");

		for (cJSON *child = item->child; child; child = child->next) {
			printIndentJSON(f, indent + 1);
			printEscapedString(f, child->string);
			fprintf(f, ": ");

			bool childInline = cJSON_IsObject(child) || cJSON_IsArray(child);
			printJSON_custom(f, child, indent + 1, childInline);

			if (child->next) fprintf(f, ",");
			fprintf(f, "\n");
		}

		printIndentJSON(f, indent);
		fprintf(f, "}");
	}
	else if (cJSON_IsArray(item)) {
		if (!inline_val) printIndentJSON(f, indent);
		fprintf(f, "[\n");

		bool matrix = isMatrix(item);
		int size = cJSON_GetArraySize(item);

		for (int i = 0; i < size; ++i) {
			cJSON *elem = cJSON_GetArrayItem(item, i);

			// si c’est un tableau d’events
			bool isEventArray = (item->string && strcmp(item->string, "events") == 0);

			if (matrix) {
				printIndentJSON(f, indent + 1);
				fprintf(f, "[");
				int rowSize = cJSON_GetArraySize(elem);
				for (int j = 0; j < rowSize; ++j) {
					cJSON *cell = cJSON_GetArrayItem(elem, j);
					if (cJSON_IsNumber(cell)) fprintf(f, "%4g", cell->valuedouble);
					else if (cJSON_IsBool(cell)) fprintf(f, "%d", cJSON_IsTrue(cell) ? 1 : 0);
					else printPrimitive(f, cell);
					if (j < rowSize - 1) fprintf(f, ", ");
				}
				fprintf(f, "]");
			}
			// objets d'un tableau "events" -> inline
			else if (isEventArray && cJSON_IsObject(elem)) {
				printIndentJSON(f, indent + 1);
				fprintf(f, "{ ");
				for (cJSON *child = elem->child; child; child = child->next) {
					fprintf(f, "\"%s\": ", child->string);
					printPrimitive(f, child);
					if (child->next) fprintf(f, ", ");
				}
				fprintf(f, " }");
			}
			else {
				printJSON_custom(f, elem, indent + 1, false);
			}

			if (i < size - 1) fprintf(f, ",");
			fprintf(f, "\n");
		}

		printIndentJSON(f, indent);
		fprintf(f, "]");
	}
	else {
		printPrimitive(f, item);
	}
}

cJSON* cJSON_ParseWithFile(const char *nomFichier) {
	if (!nomFichier || !*nomFichier) { LOG_ERROR("Nom de fichier json vide ou null"); return NULL; }

	FILE *f = fopen(nomFichier, "rb");
	if (!f) { perror("Impossible d'ouvrir le fichier"); return NULL; }

	// Aller à la fin pour connaître la taille
	if (fseek(f, 0, SEEK_END) != 0) { perror("Erreur fseek"); fclose(f); return NULL; }

	long taille_long = ftell(f);
	if (taille_long < 0) { perror("Erreur ftell"); fclose(f); return NULL; }
	if (taille_long == 0) { LOG_ERROR("Fichier vide"); fclose(f); return NULL; }

	size_t taille = (size_t)taille_long;
	rewind(f);

	// Allouer le buffer
	char *buffer = (char*)malloc(taille + 1);
	if (!buffer) { perror("Erreur allocation mémoire"); fclose(f); return NULL; }

	// Lire le fichier
	size_t lu = fread(buffer, 1, taille, f);
	if (lu != taille) { fprintf(stderr, "Erreur lecture fichier\n"); free(buffer); fclose(f); return NULL; }
	buffer[taille] = '\0'; // Terminer la chaîne
	fclose(f);

	// Parser le JSON
	cJSON *json = cJSON_Parse(buffer);
	if (!json) {
		const char* err = cJSON_GetErrorPtr();
		if (err) fprintf(stderr, "Erreur parsing JSON à la position %d\n", err - buffer);
		else LOG_ERROR("Parsing JSON inconnu");
	}

	free(buffer);
	return json;
}