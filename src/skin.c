// @author Alain Barbier alias "Metroidzeta"

#include "headers/skin.h"

static skin_result_t skin_validerArguments(SDL_Renderer *renderer, const char *nomFichier) {
	if (!renderer) return SKIN_ERR_NULL_RENDERER;
	if (!nomFichier || !*nomFichier) return SKIN_ERR_NULL_OR_EMPTY_FILENAME;
	if (strlen(nomFichier) >= MAX_TAILLE_STRING) return SKIN_ERR_SIZE_MAX_FILENAME;
	return SKIN_OK;
}

static skin_result_t skin_chargerTexture(skin_t *skin, SDL_Renderer *renderer, const char *nomFichier) {
	skin->texture = creerImage(renderer, nomFichier);
	if (!skin->texture) { LOG_ERROR("Echec creerImage : %s", IMG_GetError()); return SKIN_ERR_LOAD_TEXTURE; }
	for (int i = 0; i < ROWS; ++i) {
		for (int j = 0; j < COLS; ++j) {
			skin->textureRegions[i * COLS + j] = (SDL_Rect){ j * REGION_WIDTH, i * REGION_HEIGHT, REGION_WIDTH, REGION_HEIGHT };
		}
	}
	return SKIN_OK;
}

skin_result_t skin_creer(skin_t **out_skin, SDL_Renderer *renderer, const char *nomFichier) {
	if (!out_skin) return SKIN_ERR_NULL_POINTER;
	*out_skin = NULL;

	skin_result_t res = skin_validerArguments(renderer, nomFichier);
	if (res != SKIN_OK) return res;

	skin_t *skin = calloc(1, sizeof(skin_t));
	if (!skin) return SKIN_ERR_MEMORY_BASE;

	skin->nom = my_strdup(nomFichier); // important : ne pas faire "skin->nom = nomFichier", car cela ne copie que le pointeur, pas le contenu
	if (!skin->nom) { skin_detruire(skin); return SKIN_ERR_MEMORY_NAME; }

	if ((res = skin_chargerTexture(skin, renderer, nomFichier)) != SKIN_OK) { skin_detruire(skin); return res; }

	*out_skin = skin;
	return SKIN_OK;
}

void skin_afficher(SDL_Renderer *renderer, skin_t *skin, int numRegion, SDL_Rect *dstRect) {
	if (numRegion < 0 || numRegion >= TOTAL_REGIONS) { LOG_ERROR("Indice de région invalide : %d", numRegion); return; }
	dessinerTexture(renderer, skin->texture, &skin->textureRegions[numRegion], dstRect, "Impossible de dessiner la skin de notre joueur avec SDL_RenderCopy");
}

void skin_detruire(skin_t *skin) {
	if (!skin) return;
	SDL_DestroyTexture(skin->texture);
	free(skin->nom);
	free(skin);
}

const char * skin_strerror(skin_result_t res) {
	switch (res) {
		case SKIN_OK: return "Succes";
		case SKIN_ERR_NULL_POINTER: return "Skin NULL passe en parametre";
		case SKIN_ERR_NULL_RENDERER: return "Renderer NULL passe en parametre";
		case SKIN_ERR_NULL_OR_EMPTY_FILENAME: return "Nom fichier NULL ou vide";
		case SKIN_ERR_SIZE_MAX_FILENAME : return "Nom fichier trop long";
		case SKIN_ERR_MEMORY_BASE: return "Echec allocation memoire base";
		case SKIN_ERR_MEMORY_NAME: return "Echec allocation memoire nom";
		case SKIN_ERR_PATH_TOO_LONG_OR_EMPTY: return "Chemin fichier trop long ou vide";
		case SKIN_ERR_LOAD_TEXTURE: return "Echec chargement texture";
		default: return "Erreur";
	}
}