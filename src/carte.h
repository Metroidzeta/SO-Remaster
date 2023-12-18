// @author Metroidzeta

#ifndef CARTE_H
#define CARTE_H

#define NB_PAGES_EVENT 5 // par défaut : 5

#include "arraylist.h"
#include "chipset.h"
#include "event.h"
#include "musique.h"

typedef struct ensemble_events_s {
	//int xCase;
	//int yCase;
	arraylist_t * lesEvents[NB_PAGES_EVENT];
} ensemble_events_t;

typedef struct carte_s {
	char * nom;                           // Nom de la carte
	int hauteur;                          // Hauteur de la carte en cases
	int largeur;                          // Largeur de la carte en cases
	int ** couches[3];                    // 3 couches (matrices) de numCaseChipset de la carte (C0 < C1 < Héros < C2)
	bool ** murs;                         // Matrice booléenne représentant les murs de la carte (false = pas de mur, true = mur)
	SDL_Rect *** matriceRect;             // Matrice de rectangles représentant les cases de la carte (dans le jeu, pas sur l'écran)
	ensemble_events_t ** ensembleEvents;  // Matrice d'ensembles d'événements de la carte
	chipset_t * chipset;                  // Le chipset utilisé pour cette carte
	musique_t * musique;                  // La musique utilisée pour cette carte
} carte_t;

carte_t * carte_creer(char * nom, int hauteur, int largeur, chipset_t * chipset, musique_t * musique, bool depuisMatrices);
void carte_verificationsArgs(char * nom, int hauteur, int largeur, chipset_t * chipset, musique_t * musique);
FILE ** ouvrirFichiersMatrices(char * nom, const char * typeOuverture);
carte_t * carte_creerDepuisMatrices(char * nom, int hauteur, int largeur, chipset_t * chipset, musique_t * musique);
carte_t * carte_creerDepuisMatricesTiled(char * nom, int hauteur, int largeur, chipset_t * chipset, musique_t * musique);
void carte_afficherMatriceCouche(carte_t * carte, int numCouche);
void carte_afficherLesMatricesCouches(carte_t * carte);
void carte_afficherMatriceMurs(carte_t * carte);
void carte_ecrireMatrices(carte_t * carte);
bool carte_verifierLesCollisionsMurs(carte_t * carte, SDL_Rect * hitBox);
arraylist_t * carte_verifierLesCollisionsEvents(carte_t * carte, SDL_Rect * hitBox);
void carte_ajouterEvent(carte_t * carte, int numPage, int xCase, int yCase, e_type type, void * evtPtr);
void carte_detruire(carte_t * carte);

#endif