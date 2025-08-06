// @author Alain Barbier alias "Metroidzeta"

#include "headers/heros.h"

static heros_result_t heros_validerArguments(SDL_Renderer *renderer, const char *nom, skin_t *skin, int niveau, int piecesOr, int xCase, int yCase, carte_t *carte, float tauxCrit) {
	if (!renderer) return HEROS_ERR_NULL_RENDERER;
	if (!nom || !*nom) return HEROS_ERR_NULL_OR_EMPTY_NAME;
	if (!skin) return HEROS_ERR_NULL_SKIN;
	if (niveau < 1 || niveau > NIVEAU_MAX) return HEROS_ERR_INVALID_LEVEL;
	if (piecesOr < 0) return HEROS_ERR_INVALID_PIECES_OR;
	if (!carte) return HEROS_ERR_NULL_CARTE;
	if (xCase < 0 || xCase >= carte->largeur) return HEROS_ERR_INVALID_XCASE;
	if (yCase < 0 || yCase >= carte->hauteur) return HEROS_ERR_INVALID_YCASE;
	if (tauxCrit < 0 || tauxCrit > 100) return HEROS_ERR_INVALID_TAUXCRIT;
	return HEROS_OK;
}

static void heros_initialiserHitBoxEpee(heros_t *heros) {
	const int x = heros->position.x;
	const int y = heros->position.y;

	heros->hitBoxEpee[BAS] = (SDL_Rect){ x, y + (TAILLE_CASES / 2) + 10, TAILLE_CASES, TAILLE_CASES / 2 };
	heros->hitBoxEpee[GAUCHE] = (SDL_Rect){ x - 10, y, TAILLE_CASES / 2, TAILLE_CASES };
	heros->hitBoxEpee[DROITE] = (SDL_Rect){ x + (TAILLE_CASES / 2) + 10, y, TAILLE_CASES / 2, TAILLE_CASES };
	heros->hitBoxEpee[HAUT] = (SDL_Rect){ x, y - (TAILLE_CASES / 2) + 10, TAILLE_CASES, TAILLE_CASES / 2 };
}

heros_result_t heros_creer(heros_t **out_heros, SDL_Renderer *renderer, const char *nom, skin_t *skin, Classes classe, int niveau, int piecesOr, int xCase, int yCase, TTF_Font *police, carte_t *carte, float tauxCrit) {
	if (!out_heros) return HEROS_ERR_NULL_POINTER;
	*out_heros = NULL;

	heros_result_t res = heros_validerArguments(renderer, nom, skin, niveau, piecesOr, xCase, yCase, carte, tauxCrit);
	if (res != HEROS_OK) return res;

	heros_t *heros = calloc(1, sizeof(heros_t));
	if (!heros) return HEROS_ERR_MEMORY_BASE;

	heros->nom = my_strdup(nom); // important : ne pas faire "heros->nom = nom", car cela ne copie que le pointeur, pas le contenu
	if (!heros->nom) { heros_detruire(heros); return HEROS_ERR_MEMORY_NAME; }

	heros->textureNom = creerTextureDepuisTexte(renderer, nom, police, BLANC);
	if (!heros->textureNom) { heros_detruire(heros); return HEROS_ERR_LOAD_TEXTURE_NAME; }

	heros->skin = skin;
	heros->classe = classe;
	heros->niveau = niveau;
	heros->piecesOr = piecesOr;
	heros->position = (SDL_Point){ xCase * TAILLE_CASES, yCase * TAILLE_CASES };
	heros->hitBox = (SDL_Rect){ heros->position.x, heros->position.y, TAILLE_CASES, TAILLE_CASES };
	heros_initialiserHitBoxEpee(heros);

	heros->force = FORCE_INITIALE;
	heros->dexterite = DEXTERITE_INITIALE;
	heros->constitution = CONSTITUTION_INITIALE;
	heros->PV[0] = heros->PV[1] = PV_INITIAL; // PV / PVMax
	heros->PM[0] = heros->PM[1] = PM_INITIAL; // PM / PMMax
	if (heros->PV[0] < 0 || heros->PV[0] > heros->PV[1]) { Exception("PV Heros < 0 ou > PVMax"); }
	if (heros->PM[0] < 0 || heros->PM[0] > heros->PM[1]) { Exception("PM Heros < 0 ou > PMMax"); }
	heros->direction = BAS;
	heros->alignement = ALIGNEMENT_INITIAL;
	heros->peutAttaquer = heros->attaqueEpee = heros->bloqueTotal = heros->messageTete = heros->ecritMessage = heros->eventEnCours = false;
	heros->carteActuelle = carte;
	heros->tauxCrit = tauxCrit / 100.0f; // diviser par 100 (obtenir pourcentage)
	heros->frameDeplacement = 7;

	*out_heros = heros;
	return HEROS_OK;
}

void heros_afficherNom(SDL_Renderer *renderer, heros_t *heros, SDL_Rect rectPseudo) {
	dessinerTexture(renderer, heros->textureNom, NULL, &rectPseudo, "Impossible de dessiner le nom du joueur");
}

void heros_afficherSkin(SDL_Renderer *renderer, heros_t *heros, SDL_Rect * dstRect) {
	skin_afficher(renderer, heros->skin, heros->direction * 3 + (heros->frameDeplacement / 4), dstRect);
}

static int heros_modifierValeur(int valeur, int minVal, int maxVal) {
	return valeur > maxVal ? maxVal : (valeur < minVal ? minVal : valeur);
}

void heros_modifierAlignement(heros_t *heros, int n) {
	heros->alignement = heros_modifierValeur(heros->alignement + n, 0, 100);
}

void heros_modifierPV(heros_t *heros, int n) {
	heros->PV[0] = heros_modifierValeur(heros->PV[0] + n, 0, heros->PV[1]);
}

void heros_modifierPM(heros_t *heros, int n) {
	heros->PM[0] = heros_modifierValeur(heros->PM[0] + n, 0, heros->PM[1]);
}

void heros_modifierPosition(heros_t *heros, int newX, int newY) {
	heros->hitBox.x = heros->position.x = newX;
	heros->hitBox.y = heros->position.y = newY;
}

void heros_updateHitBoxEpee(heros_t *heros) {
	const int x = heros->position.x;
	const int y = heros->position.y;
	switch (heros->direction) {
		case BAS: 
			heros->hitBoxEpee[BAS].x = x;
			heros->hitBoxEpee[BAS].y = y + (TAILLE_CASES / 2) + 10;
			break;
		case GAUCHE:
			heros->hitBoxEpee[GAUCHE].x = x - 10;
			heros->hitBoxEpee[GAUCHE].y = y;
			break;
		case DROITE:
			heros->hitBoxEpee[DROITE].x = x + (TAILLE_CASES / 2) + 10;
			heros->hitBoxEpee[DROITE].y = y;
			break;
		case HAUT:		
			heros->hitBoxEpee[HAUT].x = x;
			heros->hitBoxEpee[HAUT].y = y - (TAILLE_CASES / 2) + 10;
			break;
	}
}

int heros_getXCase(heros_t *heros) { return heros->position.x / TAILLE_CASES; }
int heros_getYCase(heros_t *heros) { return heros->position.y / TAILLE_CASES; }
double heros_getRatioPV(heros_t *heros) { return (double) heros->PV[0] / heros->PV[1]; }
double heros_getRatioPM(heros_t *heros) { return (double) heros->PM[0] / heros->PM[1]; }

bool heros_deplacer(heros_t *heros, Directions d) {
	SDL_Rect newHitbox = heros->hitBox;
	switch (d) {
		case HAUT: newHitbox.y -= DEPLACEMENT_HEROS; break;
		case BAS: newHitbox.y += DEPLACEMENT_HEROS; break;
		case GAUCHE: newHitbox.x -= DEPLACEMENT_HEROS; break;
		case DROITE: newHitbox.x += DEPLACEMENT_HEROS; break;
	}

	bool changerDirection = false;
		if (d != heros->direction) {
			heros->direction = d;
			changerDirection = true;
		}

	if (newHitbox.x >= 0 && newHitbox.y >= 0
		&& newHitbox.x <= (heros->carteActuelle->largeur - 1) * TAILLE_CASES
		&& newHitbox.y <= (heros->carteActuelle->hauteur - 1) * TAILLE_CASES
		&& !carte_verifierCollisionsMurs(heros->carteActuelle, &newHitbox)) {

		heros_modifierPosition(heros, newHitbox.x, newHitbox.y);
		heros->frameDeplacement = (heros->frameDeplacement + 1) % 12;
		printf("Vous vous etes deplace vers %-6s : (%d,%d)\n", heros_getDirectionToString(heros), heros_getXCase(heros), heros_getYCase(heros));
		return true;
	}
	return changerDirection;
}

const char * heros_getDirectionToString(heros_t *heros) {
	switch (heros->direction) {
		case HAUT: return "HAUT";
		case BAS: return "BAS";
		case GAUCHE: return "GAUCHE";
		case DROITE: return "DROITE";
	}
	return "";
}

const char * heros_getClasseToString(heros_t *heros) {
	switch (heros->classe) {
		case BARBARE: return "Barbare";
		case GUERRIER: return "Guerrier";
		case VOLEUR: return "Voleur";
	}
	return "";
}

void heros_detruire(heros_t *heros) { // Ne pas libérer heros->skin et heros->carteActuelle : partagée, allouée ailleurs
	if (!heros) return;
	SDL_DestroyTexture(heros->textureNom);
	free(heros->nom);
	free(heros);
}

const char * heros_strerror(heros_result_t res) {
	switch (res) {
		case HEROS_OK: return "Succes";
		case HEROS_ERR_NULL_POINTER: return "Heros NULL passe en parametre";
		case HEROS_ERR_NULL_RENDERER: return "Renderer NULL passe en parametre";
		case HEROS_ERR_NULL_OR_EMPTY_NAME: return "Nom NULL ou vide";
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