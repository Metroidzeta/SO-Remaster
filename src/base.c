// @author Alain Barbier alias "Metroidzeta"

#include "headers/base.h"

void Exception(const char *msgErr) { fprintf(stderr, "Exception: %s\n", msgErr); exit(EXIT_FAILURE); }
void ExceptionSDL(const char *msgErr) { fprintf(stderr, "Exception %s: %s\n", msgErr, SDL_GetError()); exit(EXIT_FAILURE); }
void ExceptionTTF(const char *msgErr) { fprintf(stderr, "Exception %s: %s\n", msgErr, TTF_GetError()); exit(EXIT_FAILURE); }
void ExceptionMix(const char *msgErr) { fprintf(stderr, "Exception %s: %s\n", msgErr, Mix_GetError()); exit(EXIT_FAILURE); }

void verifAllocSDL(void *ptr, const char *chemin, const char *msgErr) {
	if (!chemin || !msgErr) return;
	if (!ptr) {
		fprintf(stderr, "%s (chemin: %s): %s\n", msgErr, chemin, SDL_GetError());
		exit(EXIT_FAILURE);
	}
}

void verifAllocTTF(void *ptr, const char *chemin, const char *msgErr) {
	if (!chemin || !msgErr) return;
	if (!ptr) {
		fprintf(stderr, "%s (chemin: %s): %s\n", msgErr, chemin, TTF_GetError());
		exit(EXIT_FAILURE);
	}
}

void verifAllocMix(void *ptr, const char *chemin, const char *msgErr) {
	if (!chemin || !msgErr) return;
	if (!ptr) {
		fprintf(stderr, "%s (chemin: %s): %s\n", msgErr, chemin, Mix_GetError());
		exit(EXIT_FAILURE);
	}
}

static void closeTTF_Mix_SDL() {
	TTF_Quit(); Mix_CloseAudio(); SDL_Quit();
}

void initSDL(SDL_Window **window, SDL_Renderer **renderer) {
	if (!window || !renderer) Exception("Pointeur window ou renderer NULL");
	if (*window || *renderer) Exception("Window ou renderer deja initialises");
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) ExceptionSDL("Echec init SDL_INIT_VIDEO | SDL_INIT_AUDIO");
	if (Mix_OpenAudio(44100, AUDIO_S32SYS, 2, 1024) != 0) { SDL_Quit(); ExceptionMix("Echec init Mix_OpenAudio"); }  // init SDL Mixer audio
	if (TTF_Init() != 0) { Mix_CloseAudio(); SDL_Quit(); ExceptionTTF("Echec init TTF_Init"); } // init SDL TTF

	*window = SDL_CreateWindow(TITRE_FENETRE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN); // création fenêtre
	if (!*window) { closeTTF_Mix_SDL(); ExceptionSDL("Echec init SDL_CreateWindow"); }

	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED); // création rendu
	if (!*renderer) { SDL_DestroyWindow(*window); *window = NULL; closeTTF_Mix_SDL(); ExceptionSDL("Echec init SDL_CreateRenderer"); }
}

void freeSDL(SDL_Window *window, SDL_Renderer *renderer) {
	if (renderer) SDL_DestroyRenderer(renderer);
	if (window) SDL_DestroyWindow(window);
	closeTTF_Mix_SDL();
}

char * my_strdup(const char *src) { 
	if (!src) return NULL;
	size_t len = strlen(src) + 1; // +1 pour inclure le caractère nul '\0' de fin de chaîne
	char *dst = malloc(len);
	if (!dst) return NULL;
	memcpy(dst, src, len);
	return dst;
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
	bool **matrice = malloc(lignes * sizeof(bool *) + lignes * colonnes * sizeof(bool)); // allocation contiguë (pointeurs de lignes + données)
	if (!matrice) return NULL;

	bool *data = (bool *)(matrice + lignes); // bloc de données commence juste après le tableau de pointeurs

	for (int i = 0; i < lignes; i++) {
		matrice[i] = data + i * colonnes; // init des pointeurs pour chaque ligne
	}

	// Initialisation des valeurs
	if (!valeurDefaut) memset(data, 0, lignes * colonnes * sizeof(bool)); // + rapide si défaut = false
	else for (int i = 0; i < lignes * colonnes; i++) data[i] = true;

	return matrice;
}

void freeMatriceBOOL(bool **matrice) { if (matrice) free(matrice); }

int ** creerMatriceINT(int lignes, int colonnes, int valeurDefaut) {
	if (lignes < 1 || colonnes < 1) return NULL;
	int **matrice = malloc(lignes * sizeof(int *) + lignes * colonnes * sizeof(int)); // allocation contiguë (pointeurs de lignes + données)
	if (!matrice) return NULL;

	int *data = (int *)(matrice + lignes); // bloc de données commence juste après le tableau de pointeurs

	for (int i = 0; i < lignes; i++) {
		matrice[i] = data + i * colonnes; // init des pointeurs pour chaque ligne
	}

	// Initialisation des valeurs
	if (valeurDefaut == 0) memset(data, 0, lignes * colonnes * sizeof(int)); // + rapide si défaut = 0
	else for (int i = 0; i < lignes * colonnes; i++) data[i] = valeurDefaut;

	return matrice;
}

void freeMatriceINT(int **matrice) { if (matrice) free(matrice); }

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
	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, largeur, hauteur);
	if (!texture) ExceptionSDL("Echec creation texture vide");
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	return texture;
}

SDL_Texture * creerTextureDepuisTexte(SDL_Renderer *renderer, const char *texte, TTF_Font *police, SDL_Color couleur) {
	SDL_Surface *surface = TTF_RenderUTF8_Blended(police, texte, couleur);
	verifAllocTTF(surface, texte, "Echec creation surface du texte avec TTF_RenderUTF8_Blended");
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	verifAllocSDL(texture, texte, "Echec creation texture du texte avec SDL_CreateTextureFromSurface");
	SDL_FreeSurface(surface);
	return texture;
}

SDL_Texture * creerTextureLimiteDepuisTexte(SDL_Renderer *renderer, const char *texte, TTF_Font *police, SDL_Color couleur, int largeurMax) {
	SDL_Surface *surface = TTF_RenderUTF8_Blended_Wrapped(police, texte, couleur, largeurMax);
	verifAllocTTF(surface, texte, "Echec creation surface du texte avec TTF_RenderUTF8_Blended_Wrapped");
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	verifAllocSDL(texture, texte, "Echec cration texture du texte avec SDL_CreateTextureFromSurface");
	SDL_FreeSurface(surface);
	return texture;
}

void dessinerTexture(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_Rect *srcRect, const SDL_Rect *dstRect, const char *msgErr) {
	if (SDL_RenderCopy(renderer, texture, srcRect, dstRect) != 0) ExceptionSDL(msgErr);
}

void dessinerTexte(SDL_Renderer *renderer, const char *texte, TTF_Font *police, SDL_Color couleur, int x, int y) {
	SDL_Rect dstRect = {x, y, 0, 0};
	SDL_Texture *texture = creerTextureDepuisTexte(renderer, texte, police, couleur);
	SDL_QueryTexture(texture, NULL, NULL, &dstRect.w, &dstRect.h);
	dessinerTexture(renderer, texture, NULL, &dstRect, "Echec dessin texture du texte avec SDL_RenderCopy");
	SDL_DestroyTexture(texture);
}

void dessinerNombre(SDL_Renderer *renderer, int nombre, TTF_Font *police, SDL_Color couleur, int x, int y) {
	char *strNum = intToString(nombre);
	if (!strNum) Exception("Echec allocation strNum intToString");
	dessinerTexte(renderer, strNum, police, couleur, x, y);
	free(strNum);
}

void dessinerTexteLimite(SDL_Renderer *renderer, const char *texte, TTF_Font *police, SDL_Color couleur, int x, int y, int largeurMax) {
	SDL_Rect dstRect = {x, y, 0, 0};
	SDL_Texture *texture = creerTextureLimiteDepuisTexte(renderer, texte, police, couleur, largeurMax);
	SDL_QueryTexture(texture, NULL, NULL, &dstRect.w, &dstRect.h);
	dessinerTexture(renderer, texture, NULL, &dstRect, "Echec dessin texture du texte avec SDL_RenderCopy");
	SDL_DestroyTexture(texture);
}

static void printIndentJSON(FILE *f, int indent) {
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
	if (!item) return;

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
			} else {
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

cJSON * cJSON_ParseWithFile(const char *nomFichier) {
	FILE *f = fopen(nomFichier, "rb");
	if (!f) { fprintf(stderr, "Impossible d'ouvrir le fichier"); return NULL; }

	// Aller à la fin pour connaître la taille
	fseek(f, 0, SEEK_END);
	long taille = ftell(f);
	rewind(f);

	if (taille <= 0) { fclose(f); fprintf(stderr, "Fichier vide ou erreur taille\n"); return NULL; }

	// Allouer le buffer
	char *buffer = malloc(taille + 1);
	if (!buffer) { perror("Erreur allocation mémoire"); fclose(f); return NULL; }

	// Lire le fichier
	size_t lu = fread(buffer, 1, taille, f);
	fclose(f);

	if (lu != taille) { fprintf(stderr, "Erreur lecture fichier\n"); free(buffer); return NULL; }
	buffer[taille] = '\0'; // Terminer la chaîne

	// Parser le JSON
	cJSON *json = cJSON_Parse(buffer);
	if (!json) fprintf(stderr, "Erreur parsing JSON: %s\n", cJSON_GetErrorPtr());

	free(buffer);
	return json;
}