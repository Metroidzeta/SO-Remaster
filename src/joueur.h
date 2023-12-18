// @author Metroidzeta

#ifndef JOUEUR_H
#define JOUEUR_H

#include "base.h"
#include "carte.h"

//             0     1        2      3
typedef enum {BAS, GAUCHE, DROITE, HAUT} Directions;
//               0         1        2
typedef enum {BARBARE, GUERRIER, VOLEUR} Classes;

typedef struct joueur_s {
	char * nom;
	SDL_Texture * textureNom;
	SDL_Texture * sprite;
	Classes classe; // BARBARE, GUERRIER ou VOLEUR (enum Classes)
	int niveau;
	int piecesOr;
	int x;
	int y;
	SDL_Rect hitBox;
	SDL_Rect hitBoxEpee[4];
	int force;
	int dexterite;
	int constitution;
	int PV[2]; // PV / PVMax (PV[0] / PV[1])
	int PM[2]; // PM / PMMax (PM[0] / PM[1])
	Directions direction;
	int alignement;
	bool attaqueEpee;
	bool bloqueTotal;
	bool messageTete;
	bool ecritMessage;
	bool eventEnCours;
	carte_t * carteActuelle;
	float tauxCoupCritique;
	int frameDeplacement;
} joueur_t;

joueur_t * joueur_creer(SDL_Renderer * renderer, char * nom, Classes classe, int niveau, int piecesOr, char * cheminSprite, int xCase, int yCase, TTF_Font * police, carte_t * carteActuelle, float tauxCoupCritique);
void joueur_verificationsArgs(char * nom, int niveau, int piecesOr, int xCase, int yCase, carte_t * carteActuelle, float tauxCoupCritique);
void joueur_afficherNom(SDL_Renderer * renderer, joueur_t * joueur, SDL_Rect rectPseudo);
int joueur_modifierValeur(int valeur, int n, int minVal, int maxVal);
void joueur_modifierAlignement(joueur_t * joueur, int n);
void joueur_modifierPV(joueur_t * joueur, int n);
void joueur_modifierPM(joueur_t * joueur, int n);
void joueur_modifierPosition(joueur_t * joueur, int newX, int newY);
void joueur_updateHitBoxEpee(joueur_t * joueur);
int joueur_getXCase(joueur_t * joueur);
int joueur_getYCase(joueur_t * joueur);
double joueur_getRatioPV(joueur_t * joueur);
void joueur_deplacer(joueur_t * joueur, Directions direction);
const char * joueur_getDirectionToString(joueur_t * joueur);
const char * joueur_getClasseToString(joueur_t * joueur);
void joueur_detruire(joueur_t * joueur);

#endif