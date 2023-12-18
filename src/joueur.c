// @author Metroidzeta

#include "joueur.h"

joueur_t * joueur_creer(SDL_Renderer * renderer, char * nom, Classes classe, int niveau, int piecesOr, char * cheminSprite, int xCase, int yCase, TTF_Font * police, carte_t * carteActuelle, float tauxCoupCritique) {
	joueur_verificationsArgs(nom,niveau,piecesOr,xCase,yCase,carteActuelle,tauxCoupCritique);
	joueur_t * joueur = malloc(sizeof(joueur_t));
	verifAlloc(joueur,"Erreur d'allocation du joueur");

	joueur->nom = strdup(nom); // il ne faut pas écrire : "joueur->nom = nom;" car on ne copie alors que des adresses
	verifAllocStrCopy(joueur->nom,nom);
	joueur->textureNom = creerTextureTexte(renderer,nom,police,BLANC);

	joueur->sprite = creerTextureImage(renderer,cheminSprite);
	joueur->classe = classe;
	joueur->niveau = niveau;
	joueur->piecesOr = piecesOr;
	joueur->x = xCase * TAILLE_CASES;
	joueur->y = yCase * TAILLE_CASES;
	joueur->hitBox = (SDL_Rect) {joueur->x, joueur->y, TAILLE_CASES, TAILLE_CASES};

	// HIT BOX EPEE                       ___.x____  _______________.y__________________  _____.w_____  _______.h_______
	joueur->hitBoxEpee[BAS] = (SDL_Rect) {joueur->x, joueur->y + (TAILLE_CASES / 2) + 10, TAILLE_CASES, TAILLE_CASES / 2};
	joueur->hitBoxEpee[GAUCHE] = (SDL_Rect) {joueur->x - 10, joueur->y, TAILLE_CASES / 2, TAILLE_CASES};
	joueur->hitBoxEpee[DROITE] = (SDL_Rect) {joueur->x + (TAILLE_CASES / 2) + 10, joueur->y, TAILLE_CASES / 2, TAILLE_CASES};
	joueur->hitBoxEpee[HAUT] = (SDL_Rect) {joueur->x, joueur->y - (TAILLE_CASES / 2) + 10, TAILLE_CASES, TAILLE_CASES / 2};

	joueur->force = 12;
	joueur->dexterite = 9;
	joueur->constitution = 10;
	joueur->PV[0] = joueur->PV[1] = 600; // PV / PVMax
	joueur->PM[0] = joueur->PM[1] = 250; // PM / PMMax
	if(joueur->PV[0] < 0 || joueur->PV[0] > joueur->PV[1]) { Exception("Le joueur a des PV < 0 ou PV > PVMax"); }
	if(joueur->PM[0] < 0 || joueur->PM[0] > joueur->PM[1]) { Exception("Le joueur a des PM < 0 ou PM > PMMax"); }
	joueur->direction = BAS;
	joueur->alignement = 50;
	joueur->attaqueEpee = joueur->bloqueTotal = joueur->messageTete = joueur->ecritMessage = joueur->eventEnCours = false;
	joueur->carteActuelle = carteActuelle;
	joueur->tauxCoupCritique = tauxCoupCritique / 100; // on divise par 100 (pour obtenir un pourcentage)
	joueur->frameDeplacement = 7;

	return joueur;
}

void joueur_verificationsArgs(char * nom, int niveau, int piecesOr, int xCase, int yCase, carte_t * carteActuelle, float tauxCoupCritique) {
	if(nom == NULL) { Exception("Le nom du joueur est NULL"); }
	if(nom[0] == '\0') { Exception("Le nom du joueur est vide"); }
	if(niveau < 1 || niveau > NIVEAU_MAX) { Exception("Le niveau du joueur est < 1 ou > NIVEAU_MAX"); }
	if(piecesOr < 0) { Exception("Les piecesOr du joueur est < 0"); }
	if(carteActuelle == NULL) { Exception("La carteActuelle du joueur est NULL"); }
	if(xCase < 0 || xCase > carteActuelle->largeur - 1) { Exception("La xCase du joueur est < 0 ou > largeur - 1 de la carteActuelle"); }
	if(yCase < 0 || yCase > carteActuelle->hauteur - 1) { Exception("La yCase du joueur est < 0 ou > hauteur - 1 de la carteActuelle"); }
	if(tauxCoupCritique < 0 || tauxCoupCritique > 100) { Exception("Le tauxCoupCritique du joueur est < 0 ou > 100"); }
}

void joueur_afficherNom(SDL_Renderer * renderer, joueur_t * joueur, SDL_Rect rectPseudo) {
	dessinerTexture(renderer,joueur->textureNom,NULL,&rectPseudo,"Impossible de dessiner le nom du joueur");
}

int joueur_modifierValeur(int valeur, int n, int minVal, int maxVal) {
	return (valeur + n > maxVal) ? maxVal : (valeur + n < minVal) ? minVal : valeur + n;
}

void joueur_modifierAlignement(joueur_t * joueur, int n) {
	joueur->alignement = joueur_modifierValeur(joueur->alignement,n,0,100);
}

void joueur_modifierPV(joueur_t * joueur, int n) {
	joueur->PV[0] = joueur_modifierValeur(joueur->PV[0],n,0,joueur->PV[1]);
}

void joueur_modifierPM(joueur_t * joueur, int n) {
	joueur->PM[0] = joueur_modifierValeur(joueur->PM[0],n,0,joueur->PM[1]);
}

void joueur_modifierPosition(joueur_t * joueur, int newX, int newY) {
	joueur->hitBox.x = joueur->x = newX;
	joueur->hitBox.y = joueur->y = newY;
}

void joueur_updateHitBoxEpee(joueur_t * joueur) {
	switch(joueur->direction) {
		case BAS: 
			joueur->hitBoxEpee[BAS].x = joueur->x;
			joueur->hitBoxEpee[BAS].y = joueur->y + (TAILLE_CASES / 2) + 10;
			break;
		case GAUCHE:
			joueur->hitBoxEpee[GAUCHE].x = joueur->x - 10;
			joueur->hitBoxEpee[GAUCHE].y = joueur->y;
			break;
		case DROITE:
			joueur->hitBoxEpee[DROITE].x = joueur->x + (TAILLE_CASES / 2) + 10;
			joueur->hitBoxEpee[DROITE].y = joueur->y;
			break;
		case HAUT:		
			joueur->hitBoxEpee[HAUT].x = joueur->x;
			joueur->hitBoxEpee[HAUT].y = joueur->y - (TAILLE_CASES / 2) + 10;
			break;
	}
}

int joueur_getXCase(joueur_t * joueur) { return joueur->x / TAILLE_CASES; }
int joueur_getYCase(joueur_t * joueur) { return joueur->y / TAILLE_CASES; }
double joueur_getRatioPV(joueur_t * joueur) { return (double) joueur->PV[0] / joueur->PV[1]; }
double joueur_getRatioPM(joueur_t * joueur) { return (double) joueur->PM[0] / joueur->PM[1]; }

void joueur_deplacer(joueur_t * joueur, Directions d) {
	SDL_Rect hitBoxTemp = joueur->hitBox;

	switch(d) {
		case HAUT: hitBoxTemp.y -= DEPLACEMENT_JOUEUR; break;
		case BAS: hitBoxTemp.y += DEPLACEMENT_JOUEUR; break;
		case GAUCHE: hitBoxTemp.x -= DEPLACEMENT_JOUEUR; break;
		case DROITE: hitBoxTemp.x += DEPLACEMENT_JOUEUR; break;
	}

	joueur->direction = d;

	if(hitBoxTemp.x >= 0 && hitBoxTemp.y >= 0
		&& hitBoxTemp.x <= (joueur->carteActuelle->largeur - 1) * TAILLE_CASES
		&& hitBoxTemp.y <= (joueur->carteActuelle->hauteur - 1) * TAILLE_CASES
		&& !carte_verifierLesCollisionsMurs(joueur->carteActuelle,&hitBoxTemp)) {

		joueur_modifierPosition(joueur,hitBoxTemp.x,hitBoxTemp.y);
		joueur->frameDeplacement = (joueur->frameDeplacement + 1) % 12;
		printf("Vous avez appuye sur %-6s, Position (x,y) = (%d,%d)\n",joueur_getDirectionToString(joueur),joueur_getXCase(joueur),joueur_getYCase(joueur));
	}
}

const char * joueur_getDirectionToString(joueur_t * joueur) {
	switch(joueur->direction) {
		case HAUT: return "HAUT";
		case BAS: return "BAS";
		case GAUCHE: return "GAUCHE";
		case DROITE: return "DROITE";
	}
	return "";
}

const char * joueur_getClasseToString(joueur_t * joueur) {
	switch(joueur->classe) {
		case BARBARE: return "Barbare";
		case GUERRIER: return "Guerrier";
		case VOLEUR: return "Voleur";
	}
	return "";
}

void joueur_detruire(joueur_t * joueur) { // Pas besoin de free la carteActuelle du joueur car elle est détruite dans l'arraylist lesCartes
	SDL_DestroyTexture(joueur->textureNom);
	SDL_DestroyTexture(joueur->sprite);
	free(joueur->nom);
	free(joueur);
}