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

#include "headers/heros.h"

static inline int alignToTile(int value) { return value - (value % TAILLE_CASES); }

static inline heros_result_t heros_validerArguments(SDL_Renderer *renderer, const char *nom, skin_t *skin, int niveau, int piecesOr, int xCase, int yCase, carte_t *carte, float tauxCrit) {
	if (!renderer) return HEROS_ERR_NULL_RENDERER;
	if (!nom || !*nom) return HEROS_ERR_NULL_OR_EMPTY_NAME;
	if (strlen(nom) >= MAX_TAILLE_STRING) return HEROS_ERR_SIZE_MAX_NAME;
	if (!skin) return HEROS_ERR_NULL_SKIN;
	if (niveau < 1 || niveau > NIVEAU_MAX) return HEROS_ERR_INVALID_LEVEL;
	if (piecesOr < 0) return HEROS_ERR_INVALID_PIECES_OR;
	if (!carte) return HEROS_ERR_NULL_CARTE;
	if (xCase < 0 || xCase >= carte->largeur) return HEROS_ERR_INVALID_XCASE;
	if (yCase < 0 || yCase >= carte->hauteur) return HEROS_ERR_INVALID_YCASE;
	if (tauxCrit < 0 || tauxCrit > 100) return HEROS_ERR_INVALID_TAUXCRIT;
	return HEROS_OK;
}

static void heros_creerHitBoxsEpee(SDL_Rect hitBoxEpee[], int x, int y) { // HIT BOX EPEE (REELLE / A L'ECRAN)
	const int taille = TAILLE_CASES;
	hitBoxEpee[BAS] = (SDL_Rect){ x, y + (taille / 2) + OFFSET_EPEE_PX, taille, taille / 2 };    // BAS
	hitBoxEpee[GAUCHE] = (SDL_Rect){ x - OFFSET_EPEE_PX, y, taille / 2, taille };                // GAUCHE
	hitBoxEpee[DROITE] = (SDL_Rect){ x + (taille / 2) + OFFSET_EPEE_PX, y, taille / 2, taille }; // DROITE
	hitBoxEpee[HAUT] = (SDL_Rect){ x, y - (taille / 2) + OFFSET_EPEE_PX, taille, taille / 2 };   // HAUT
}

heros_t * heros_creer(SDL_Renderer *renderer, const char *nom, skin_t *skin, Classes classe, int niveau, int piecesOr, int xCase, int yCase, TTF_Font *police, carte_t *carte, float tauxCrit, heros_result_t *res) {
	if (!res) { LOG_ERROR("Enum heros_result NULL"); return NULL; }
	*res = HEROS_OK;
	if ((*res = heros_validerArguments(renderer, nom, skin, niveau, piecesOr, xCase, yCase, carte, tauxCrit))) return NULL;

	heros_t *heros = calloc(1, sizeof(heros_t));
	if (!heros) { *res = HEROS_ERR_MEMORY_BASE; return NULL; }

	heros->nom = my_strdup(nom); // important : ne pas faire "heros->nom = nom", car cela ne copie que le pointeur, pas le contenu
	if (!heros->nom) { heros_detruire(heros); *res = HEROS_ERR_MEMORY_NAME; return NULL; }

	heros->textureNom = creerTextureDepuisTexte(renderer, nom, police, BLANC);
	if (!heros->textureNom) { heros_detruire(heros); *res = HEROS_ERR_LOAD_TEXTURE_NAME; return NULL; }

	heros->skin = skin;
	heros->classe = classe;
	heros->niveau = niveau;
	heros->piecesOr = piecesOr;
	heros->position = (SDL_Point){ xCase * TAILLE_CASES, yCase * TAILLE_CASES };
	heros->positionEcran = (SDL_Point){ alignToTile(WINDOW_WIDTH / 2), alignToTile(WINDOW_HEIGHT / 2) };
	heros->hitBox = (SDL_Rect){ heros->position.x, heros->position.y, TAILLE_CASES, TAILLE_CASES };
	heros->hitBoxEcran = (SDL_Rect){ heros->positionEcran.x, heros->positionEcran.y, TAILLE_CASES, TAILLE_CASES };
	heros_creerHitBoxsEpee(heros->hitBoxEpee, heros->position.x, heros->position.y);
	heros_creerHitBoxsEpee(heros->hitBoxEpeeEcran, heros->positionEcran.x, heros->positionEcran.y);

	heros->force = FORCE_INITIALE;
	heros->dexterite = DEXTERITE_INITIALE;
	heros->constitution = CONSTITUTION_INITIALE;
	heros->PV[0] = heros->PV[1] = PV_INITIAL; // PV / PVMax
	heros->PM[0] = heros->PM[1] = PM_INITIAL; // PM / PMMax
	if (heros->PV[0] < 0 || heros->PV[0] > heros->PV[1]) { Exception("PV Heros < 0 ou > PVMax"); }
	if (heros->PM[0] < 0 || heros->PM[0] > heros->PM[1]) { Exception("PM Heros < 0 ou > PMMax"); }
	heros->direction = BAS;
	heros->alignement = ALIGNEMENT_INITIAL;
	heros->peutAttaquer = heros->attaqueEpee = heros->estBloque = heros->messageTete = heros->estEnTrainDEcrire = heros->estDansUnEvent = false;
	heros->carteActuelle = carte;
	heros->tauxCrit = tauxCrit / 100.0f; // diviser par 100 (obtenir pourcentage)
	heros->frameDeplacement = 7;
	heros_updateOffSet(heros);

	return heros;
}

void heros_updateOffSet(heros_t *heros) {
	heros->xOffSet = heros->positionEcran.x - heros->position.x;
	heros->yOffSet = heros->positionEcran.y - heros->position.y;
}

void heros_afficherNom(SDL_Renderer *renderer, heros_t *heros, SDL_Rect rectPseudo) {
	dessinerTexture(renderer, heros->textureNom, NULL, &rectPseudo, "Echec dessin du nom du heros");
}

void heros_afficherSkin(SDL_Renderer *renderer, heros_t *heros) {
	skin_afficher(renderer, heros->skin, heros->direction * 3 + (heros->frameDeplacement / 4), &heros->hitBoxEcran);
}

static inline int heros_modifierValeur(int val, int minVal, int maxVal) { return minInt(maxInt(val, minVal), maxVal); }

void heros_modifierAlignement(heros_t *heros, int val) { heros->alignement = heros_modifierValeur(heros->alignement + val, 0, 100); }
void heros_modifierPV(heros_t *heros, int val) { heros->PV[0] = heros_modifierValeur(heros->PV[0] + val, 0, heros->PV[1]); }
void heros_modifierPM(heros_t *heros, int val) { heros->PM[0] = heros_modifierValeur(heros->PM[0] + val, 0, heros->PM[1]); }
void heros_lvlup(heros_t *heros) { if (heros->niveau < NIVEAU_MAX) heros->niveau++; }

void heros_modifierPosition(heros_t *heros, int newX, int newY) {
	heros->hitBox.x = heros->position.x = newX;
	heros->hitBox.y = heros->position.y = newY;
	heros_updateOffSet(heros);
}

void heros_updateHitBoxEpee(heros_t *heros) {
	const int taille = TAILLE_CASES;
	const int x = heros->position.x;
	const int y = heros->position.y;
	switch (heros->direction) {
		case BAS: 
			heros->hitBoxEpee[BAS].x = x;
			heros->hitBoxEpee[BAS].y = y + (taille / 2) + OFFSET_EPEE_PX;
			break;
		case GAUCHE:
			heros->hitBoxEpee[GAUCHE].x = x - OFFSET_EPEE_PX;
			heros->hitBoxEpee[GAUCHE].y = y;
			break;
		case DROITE:
			heros->hitBoxEpee[DROITE].x = x + (taille / 2) + OFFSET_EPEE_PX;
			heros->hitBoxEpee[DROITE].y = y;
			break;
		case HAUT:		
			heros->hitBoxEpee[HAUT].x = x;
			heros->hitBoxEpee[HAUT].y = y - (taille / 2) + OFFSET_EPEE_PX;
			break;
	}
}

int heros_getXCase(heros_t *heros) { if (!heros) return -1; return heros->position.x / TAILLE_CASES; }
int heros_getYCase(heros_t *heros) { if (!heros) return -1; return heros->position.y / TAILLE_CASES; }
double heros_getRatioPV(heros_t *heros) { return (double) heros->PV[0] / heros->PV[1]; }
double heros_getRatioPM(heros_t *heros) { return (double) heros->PM[0] / heros->PM[1]; }

bool heros_deplacer(heros_t *heros, Directions d) {
	if (!heros) return false;
	SDL_Rect tempHitBox = heros->hitBox;
	switch (d) {
		case HAUT: tempHitBox.y -= DEPLACEMENT_HEROS; break;
		case BAS: tempHitBox.y += DEPLACEMENT_HEROS; break;
		case GAUCHE: tempHitBox.x -= DEPLACEMENT_HEROS; break;
		case DROITE: tempHitBox.x += DEPLACEMENT_HEROS; break;
	}

	bool changerDirection = false;
	if (d != heros->direction) {
		heros->direction = d;
		changerDirection = true;
	}

	if (tempHitBox.x >= 0 && tempHitBox.y >= 0
		&& tempHitBox.x <= (heros->carteActuelle->largeur - 1) * TAILLE_CASES
		&& tempHitBox.y <= (heros->carteActuelle->hauteur - 1) * TAILLE_CASES
		&& !carte_verifierCollisionsMurs(heros->carteActuelle, &tempHitBox)) {

		heros_modifierPosition(heros, tempHitBox.x, tempHitBox.y);
		heros->frameDeplacement = (heros->frameDeplacement + 1) % 12;
		printf("Vous vous etes deplace vers %-6s : (%d,%d)\n", heros_getDirectionToString(heros), heros_getXCase(heros), heros_getYCase(heros));
		return true;
	}
	return changerDirection;
}

const char * heros_getDirectionToString(heros_t *heros) {
	if (!heros) return NULL;
	switch (heros->direction) {
		case HAUT: return "HAUT";
		case BAS: return "BAS";
		case GAUCHE: return "GAUCHE";
		case DROITE: return "DROITE";
	}
	return "";
}

const char * heros_getClasseToString(heros_t *heros) {
	if (!heros) return NULL;
	switch (heros->classe) {
		case BARBARE: return "Barbare";
		case GUERRIER: return "Guerrier";
		case VOLEUR: return "Voleur";
	}
	return "";
}

void heros_detruire(heros_t *heros) { // Ne pas libérer heros->skin et heros->carteActuelle : partagée, allouée ailleurs
	if (!heros) return;
	SDL_DestroyTexture(heros->textureNom); heros->textureNom = NULL;
	free(heros->nom); heros->nom = NULL;
	free(heros);
}

const char * heros_strerror(heros_result_t res) {
	switch (res) {
		case HEROS_OK: return "Succes";
		case HEROS_ERR_NULL_RENDERER: return "Renderer NULL passe en parametre";
		case HEROS_ERR_NULL_OR_EMPTY_NAME: return "Nom NULL ou vide";
		case HEROS_ERR_SIZE_MAX_NAME: return "Nom trop long";
		case HEROS_ERR_NULL_SKIN: return "Skin NULL passe en parametre";
		case HEROS_ERR_INVALID_LEVEL: return "Niveau < 1 ou > NIVEAU_MAX";
		case HEROS_ERR_INVALID_PIECES_OR: return "PiecesOr < 0";
		case HEROS_ERR_NULL_CARTE: return "Carte NULL passe en parametre";
		case HEROS_ERR_INVALID_XCASE: return "xCase < 0 ou >= largeur carte";
		case HEROS_ERR_INVALID_YCASE: return "yCase < 0 ou >= hauteur carte";
		case HEROS_ERR_INVALID_TAUXCRIT: return "TauxCrit < 0 ou > 100";
		case HEROS_ERR_MEMORY_BASE: return "Echec allocation memoire base";
		case HEROS_ERR_MEMORY_NAME: return "Echec allocation memoire nom";
		case HEROS_ERR_LOAD_TEXTURE_NAME: return "Echec chargement texture du nom";
		default: return "Erreur";
	}
}