// @author Alain Barbier alias "Metroidzeta"

#include "joueur.h"

static void joueur_validerArguments(SDL_Renderer *renderer, const char *nom, skin_t *skin, int niveau, int piecesOr, int xCase, int yCase, carte_t *carte, float tauxCrit) {
	if (!renderer) Exception("Renderer NULL");
	if (!nom || !*nom) Exception("Nom joueur NULL ou vide");
	if (!skin) Exception("Skin joueur NULL");
	if (niveau < 1 || niveau > NIVEAU_MAX) Exception("Niveau joueur < 1 ou > NIVEAU_MAX");
	if (piecesOr < 0) Exception("PiecesOr joueur < 0");
	if (!carte) Exception("Carte joueur NULL");
	if (xCase < 0 || xCase >= carte->largeur) Exception("xCase joueur < 0 ou >= largeur carte");
	if (yCase < 0 || yCase >= carte->hauteur) Exception("yCase joueur < 0 ou >= hauteur carte");
	if (tauxCrit < 0 || tauxCrit > 100) Exception("TauxCrit joueur < 0 ou > 100");
}

joueur_t * joueur_creer(SDL_Renderer *renderer, const char *nom, skin_t *skin, Classes classe, int niveau, int piecesOr, int xCase, int yCase, TTF_Font *police, carte_t *carte, float tauxCrit) {
	joueur_validerArguments(renderer, nom, skin, niveau, piecesOr, xCase, yCase, carte, tauxCrit);

	joueur_t *joueur = malloc(sizeof(joueur_t));
	if (!joueur) Exception("Echec d'allocation joueur");
	memset(joueur, 0, sizeof(joueur_t)); // initialise tout à 0 / NULL pour éviter comportements indifinis en cas d'exception

	joueur->nom = strdup(nom); // ne pas faire: "joueur->nom = nom" car on ne copie alors que des adresses
	if (!joueur->nom) { joueur_detruire(joueur); Exception("Échec allocation nom joueur"); }
	joueur->textureNom = creerTextureDepuisTexte(renderer, nom, police, BLANC);
	if (!joueur->textureNom) { joueur_detruire(joueur); ExceptionSDL(""); }

	joueur->skin = skin;
	joueur->classe = classe;
	joueur->niveau = niveau;
	joueur->piecesOr = piecesOr;
	joueur->position = (SDL_Point){ xCase * TAILLE_CASES, yCase * TAILLE_CASES };
	joueur->hitBox = (SDL_Rect){ joueur->position.x, joueur->position.y, TAILLE_CASES, TAILLE_CASES };

	// HIT BOX EPEE                      ________.x________  ___________________.y_______________________  _____.w_____  _______.h_______
	joueur->hitBoxEpee[BAS] = (SDL_Rect){joueur->position.x, joueur->position.y + (TAILLE_CASES / 2) + 10, TAILLE_CASES, TAILLE_CASES / 2};
	joueur->hitBoxEpee[GAUCHE] = (SDL_Rect){joueur->position.x - 10, joueur->position.y, TAILLE_CASES / 2, TAILLE_CASES};
	joueur->hitBoxEpee[DROITE] = (SDL_Rect){joueur->position.x + (TAILLE_CASES / 2) + 10, joueur->position.y, TAILLE_CASES / 2, TAILLE_CASES};
	joueur->hitBoxEpee[HAUT] = (SDL_Rect){joueur->position.x, joueur->position.y - (TAILLE_CASES / 2) + 10, TAILLE_CASES, TAILLE_CASES / 2};

	joueur->force = 12;
	joueur->dexterite = 9;
	joueur->constitution = 10;
	joueur->PV[0] = joueur->PV[1] = 600; // PV / PVMax
	joueur->PM[0] = joueur->PM[1] = 250; // PM / PMMax
	if (joueur->PV[0] < 0 || joueur->PV[0] > joueur->PV[1]) { Exception("Le joueur a des PV < 0 ou PV > PVMax"); }
	if (joueur->PM[0] < 0 || joueur->PM[0] > joueur->PM[1]) { Exception("Le joueur a des PM < 0 ou PM > PMMax"); }
	joueur->direction = BAS;
	joueur->alignement = 50;
	joueur->peutAttaquer = joueur->attaqueEpee = joueur->bloqueTotal = joueur->messageTete = joueur->ecritMessage = joueur->eventEnCours = false;
	joueur->carteActuelle = carte;
	joueur->tauxCrit = tauxCrit / 100; // on divise par 100 (pour obtenir un pourcentage)
	joueur->frameDeplacement = 7;

	return joueur;
}

void joueur_afficherNom(SDL_Renderer *renderer, joueur_t *joueur, SDL_Rect rectPseudo) {
	dessinerTexture(renderer, joueur->textureNom, NULL, &rectPseudo, "Impossible de dessiner le nom du joueur");
}

void joueur_afficherSkin(SDL_Renderer *renderer, joueur_t *joueur, SDL_Rect * dstRect) {
	skin_afficher(renderer, joueur->skin, joueur->direction * 3 + (joueur->frameDeplacement / 4), dstRect);
}

static int joueur_modifierValeur(int valeur, int minVal, int maxVal) {
	return valeur > maxVal ? maxVal : (valeur < minVal ? minVal : valeur);
}

void joueur_modifierAlignement(joueur_t *joueur, int n) {
	joueur->alignement = joueur_modifierValeur(joueur->alignement + n, 0, 100);
}

void joueur_modifierPV(joueur_t *joueur, int n) {
	joueur->PV[0] = joueur_modifierValeur(joueur->PV[0] + n, 0, joueur->PV[1]);
}

void joueur_modifierPM(joueur_t *joueur, int n) {
	joueur->PM[0] = joueur_modifierValeur(joueur->PM[0] + n, 0, joueur->PM[1]);
}

void joueur_modifierPosition(joueur_t *joueur, int newX, int newY) {
	joueur->hitBox.x = joueur->position.x = newX;
	joueur->hitBox.y = joueur->position.y = newY;
}

void joueur_updateHitBoxEpee(joueur_t *joueur) {
	const int x = joueur->position.x;
	const int y = joueur->position.y;
	switch (joueur->direction) {
		case BAS: 
			joueur->hitBoxEpee[BAS].x = x;
			joueur->hitBoxEpee[BAS].y = y + (TAILLE_CASES / 2) + 10;
			break;
		case GAUCHE:
			joueur->hitBoxEpee[GAUCHE].x = x - 10;
			joueur->hitBoxEpee[GAUCHE].y = y;
			break;
		case DROITE:
			joueur->hitBoxEpee[DROITE].x = x + (TAILLE_CASES / 2) + 10;
			joueur->hitBoxEpee[DROITE].y = y;
			break;
		case HAUT:		
			joueur->hitBoxEpee[HAUT].x = x;
			joueur->hitBoxEpee[HAUT].y = y - (TAILLE_CASES / 2) + 10;
			break;
	}
}

int joueur_getXCase(joueur_t *joueur) { return joueur->position.x / TAILLE_CASES; }
int joueur_getYCase(joueur_t *joueur) { return joueur->position.y / TAILLE_CASES; }
double joueur_getRatioPV(joueur_t *joueur) { return (double) joueur->PV[0] / joueur->PV[1]; }
double joueur_getRatioPM(joueur_t *joueur) { return (double) joueur->PM[0] / joueur->PM[1]; }

bool joueur_deplacer(joueur_t *joueur, Directions d) {
	SDL_Rect newHitbox = joueur->hitBox;
	switch (d) {
		case HAUT: newHitbox.y -= DEPLACEMENT_JOUEUR; break;
		case BAS: newHitbox.y += DEPLACEMENT_JOUEUR; break;
		case GAUCHE: newHitbox.x -= DEPLACEMENT_JOUEUR; break;
		case DROITE: newHitbox.x += DEPLACEMENT_JOUEUR; break;
	}

	bool changerDirection = false;
		if (d != joueur->direction) {
			joueur->direction = d;
			changerDirection = true;
		}

	if (newHitbox.x >= 0 && newHitbox.y >= 0
		&& newHitbox.x <= (joueur->carteActuelle->largeur - 1) * TAILLE_CASES
		&& newHitbox.y <= (joueur->carteActuelle->hauteur - 1) * TAILLE_CASES
		&& !carte_verifierCollisionsMurs(joueur->carteActuelle, &newHitbox)) {

		joueur_modifierPosition(joueur, newHitbox.x, newHitbox.y);
		joueur->frameDeplacement = (joueur->frameDeplacement + 1) % 12;
		printf("Vous vous etes deplace vers %-6s : (%d,%d)\n", joueur_getDirectionToString(joueur), joueur_getXCase(joueur), joueur_getYCase(joueur));
		return true;
	}
	return changerDirection;
}

const char * joueur_getDirectionToString(joueur_t *joueur) {
	switch (joueur->direction) {
		case HAUT: return "HAUT";
		case BAS: return "BAS";
		case GAUCHE: return "GAUCHE";
		case DROITE: return "DROITE";
	}
	return "";
}

const char * joueur_getClasseToString(joueur_t *joueur) {
	switch (joueur->classe) {
		case BARBARE: return "Barbare";
		case GUERRIER: return "Guerrier";
		case VOLEUR: return "Voleur";
	}
	return "";
}

void joueur_detruire(joueur_t *joueur) { // Pas besoin de free la skin ou la carteActuelle du joueur car elle est détruite dans l'arraylist lesSkins/lesCartes
	SDL_DestroyTexture(joueur->textureNom);
	free(joueur->nom);
	free(joueur);
}