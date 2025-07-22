// @author Alain Barbier alias "Metroidzeta"

#ifndef JOUEUR_H
#define JOUEUR_H

#include "base.h"
#include "skin.h"
#include "carte.h"

typedef enum {BAS, GAUCHE, DROITE, HAUT} Directions;
typedef enum {BARBARE, GUERRIER, VOLEUR} Classes;

typedef struct joueur_s {
	char *nom;
	SDL_Texture *textureNom;
	skin_t *skin;
	Classes classe; // BARBARE, GUERRIER ou VOLEUR (enum Classes)
	int niveau;
	int piecesOr;
	SDL_Point position;
	SDL_Rect hitBox;
	SDL_Rect hitBoxEpee[4];
	int force, dexterite, constitution;
	int PV[2]; // PV / PVMax (PV[0] / PV[1])
	int PM[2]; // PM / PMMax (PM[0] / PM[1])
	Directions direction;
	int alignement;
	bool peutAttaquer, attaqueEpee, bloqueTotal, messageTete, ecritMessage, eventEnCours;
	carte_t *carteActuelle;
	float tauxCrit;
	int frameDeplacement;
} joueur_t;

joueur_t * joueur_creer(SDL_Renderer *renderer, const char *nom, skin_t *skin, Classes classe, int niveau, int piecesOr, int xCase, int yCase, TTF_Font *police, carte_t *carte, float tauxCrit);
void joueur_afficherNom(SDL_Renderer *renderer, joueur_t *joueur, SDL_Rect rectPseudo);
void joueur_afficherSkin(SDL_Renderer *renderer, joueur_t *joueur, SDL_Rect *dstRect);
void joueur_modifierAlignement(joueur_t *joueur, int n);
void joueur_modifierPV(joueur_t *joueur, int n);
void joueur_modifierPM(joueur_t *joueur, int n);
void joueur_modifierPosition(joueur_t *joueur, int newX, int newY);
void joueur_updateHitBoxEpee(joueur_t *joueur);
int joueur_getXCase(joueur_t *joueur);
int joueur_getYCase(joueur_t *joueur);
double joueur_getRatioPV(joueur_t *joueur);
double joueur_getRatioPM(joueur_t *joueur);
bool joueur_deplacer(joueur_t *joueur, Directions direction);
const char *joueur_getDirectionToString(joueur_t *joueur);
const char *joueur_getClasseToString(joueur_t *joueur);
void joueur_detruire(joueur_t *joueur);

#endif