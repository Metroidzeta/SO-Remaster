// @author Alain Barbier alias "Metroidzeta"

#ifndef CARTE_H
#define CARTE_H

#define NB_PAGES_EVENT 5 // par défaut : 5

#include "arraylist.h"
#include "event.h"
#include "chipset.h"
#include "musique.h"
#include "monstre.h"

typedef enum {
	CARTE_OK = 0,
	CARTE_ERR_NULL_POINTER,
	CARTE_ERR_NULL_OR_EMPTY_NAME,
	CARTE_ERR_SIZE_MAX_NAME,
	CARTE_ERR_INVALID_LARGEUR,
	CARTE_ERR_INVALID_HAUTEUR,
	CARTE_ERR_NULL_CHIPSET,
	CARTE_ERR_MEMORY_BASE,
	CARTE_ERR_MEMORY_NAME
} carte_result_t;

typedef struct {
	//int xCase;
	//int yCase;
	arraylist_t *lesEvents[NB_PAGES_EVENT];
} ensemble_events_t;

typedef struct {
	char *nom;                            // Nom de la carte
	int largeur;                          // Largeur de la carte (en cases)
	int hauteur;                          // Hauteur de la carte (en cases)
	chipset_t *chipset;                   // Chipset utilisé
	musique_t *musique;                   // Musique utilisée
	int **couches[3];                     // 3 couches (matrices) de numTuileChipset (C0 < C1 < Héros < C2) de la carte
	bool **murs;                          // Matrice booléenne représentant les murs sur chaque case (false = pas de mur, true = mur) de la carte
	SDL_Rect **matriceRect;               // Matrice de rectangles représentant les cases de la carte
	ensemble_events_t **ensembleEvents;   // Matrice d'ensembles d'événements de la carte
	arraylist_t *monstres;                // Monstres présents sur la carte
} carte_t;

carte_result_t carte_creer(carte_t **out_carte, const char *nom, int largeur, int hauteur, chipset_t *chipset, musique_t *musique, bool depuisFichiers);
void initMatricesDepuisFichiers(carte_t *carte);
carte_result_t carte_creerDepuisMatricesTiled(carte_t **out_carte, const char *nom, int largeur, int hauteur, chipset_t *chipset, musique_t *musique);
void carte_ecrireMatrices(carte_t *carte);
bool carte_verifierCollisionsMurs(carte_t *carte, SDL_Rect *hitBox);
arraylist_t * carte_verifierCollisionsEvents(carte_t *carte, SDL_Rect *hitBox);
void carte_ajouterEvent(carte_t *carte, int numPage, int xCase, int yCase, e_type type, void *evtPtr);
void carte_ajouterMonstre(carte_t *carte, monstre_t *monstre);
void carte_detruire(carte_t *carte);
const char * carte_strerror(carte_result_t res);

#endif