// @author Alain Barbier alias "Metroidzeta"

#ifndef JEU_H
#define JEU_H

#include "heros.h"
#include "bruitage.h"
#include "event_msg.h"
#include "event_tp.h"
#include "event_jm.h"
#include "event_changePV.h"
#include "event_changePM.h"
#include "chargerAffichages.h"
#include "chargerSkins.h"
#include "chargerMonstresData.h"
#include "chargerPolices.h"
#include "chargerBruitages.h"
#include "chargerMusiques.h"
#include "chargerChipsets.h"
#include "chargerCartes.h"
#include "chargerEvents.h"
#include "chargerMonstres.h"

#define TAILLE_MAX_MSG 40
#define TAILLE_MAX_MSG_REELLE ((TAILLE_MAX_MSG * 4) + 1) // taille * 4 si un caractère UTF-8 a une longueur de 4 octets + '\0'

#define DELAI_MS_FIOLES 588
#define DELAI_MS_SECONDE 1000
#define DELAI_MS_MINUTE 60000

#define TAILLE_TITRE_FENETRE (sizeof(TITRE_FENETRE) - 1)  // sizeof inclut le '\0', donc -1

typedef struct jeu_s {
	bool enCours; // programme actif
	int frames;
	double FPS_result;
	int numCouleur_cadres;
	SDL_Color couleurs_cadres[5];
	float deuxTiersSeconde; // 
	char str_FPS_Fenetre[TAILLE_TITRE_FENETRE + 16]; // TAILLE_TITRE_FENETRE + 15 + '\0' (string "TITRE_FENETRE | FPS : XXX.XX" dans la fenêtre, FPS 1-3.2 chiffres)
	char str_FPS_EnJeu[13]; // 12 + '\0' (string "FPS : XXX.XX" dans le jeu, FPS 1-3.2 chiffres)
	char str_alignement[12]; // 11 + '\0' (string "Align : XXX" dans le jeu, Align 1-3 chiffres)
	heros_t *heros;
	SDL_Rect rectPseudo;
	arraylist_t *affichages;
	arraylist_t *skins;
	arraylist_t *monstresData;
	arraylist_t *polices;
	arraylist_t *musiques;
	arraylist_t *bruitages;
	arraylist_t *chipsets;
	arraylist_t *cartes;
	int xOffSetHeros; // Le décalage entre la position réelle x du héros et la position x du héros SUR L'ECRAN
	int yOffSetHeros; // Le décalage entre la position réelle y du héros et la position y du héros SUR L'ECRAN
	SDL_Rect hitBoxHerosEcran; // La hitbox de notre héros SUR L'ECRAN
	SDL_Rect hitBoxEpeeHerosEcran[4]; // La hitbox de l'épée du héros SUR L'ECRAN
	SDL_Rect srcRectFiolePV[2][3];
	SDL_Rect srcRectFiolePM[2][3];
	SDL_Rect dstRectFiolePV[2];
	SDL_Rect dstRectFiolePM[2];
	SDL_Texture * textureFiolePVMorte[3]; // Les 3 textures de la fiolePV morte qui est affichée par dessus la fiole PV vivante
	SDL_Texture * textureFiolePMMorte[3]; // Les 3 textures de la fiolePM morte qui est affichée par dessus la fiole PM vivante
	SDL_Rect dstRectBarreXP;
	int fiolesTiming;
	int delaiMessage;
	char message[2][TAILLE_MAX_MSG_REELLE]; // Le message de notre joueur et sa sauvegarde
	int messageCharNbOctets[2][TAILLE_MAX_MSG]; // tableau pour savoir chaque lettre du message et sa savegarde est codée sur X octets
	int compteurLettres;
	int compteurLettresReelles;
	char recapMessages[3][TAILLE_MAX_MSG_REELLE];
	int compteurRecap;
	int afficherRecap;
	int degatsAffiches;
	bool mursVisibles;
	bool menuVisible;
	bool estCoupCritique;
	arraylist_t * alEventsActuels;
	arraylist_t * lesHitBoxDesMonstresTouches;
	int nbEventPass;
	musique_t * musiqueActuelle;
} jeu_t;

jeu_t * jeu_creer(SDL_Renderer *renderer);

SDL_Texture * getAffichage(jeu_t *jeu, int pos);
skin_t * getSkin(jeu_t *jeu, int pos);
monstreData_t * getMonstreData(jeu_t *jeu, int pos);
TTF_Font * getPolice(jeu_t *jeu, int pos);
musique_t * getMusique(jeu_t *jeu, int pos);
bruitage_t * getBruitage(jeu_t *jeu, int pos);
chipset_t * getChipset(jeu_t *jeu, int pos);
carte_t * getCarte(jeu_t *jeu, int pos);
event_t * getEventActuel(jeu_t *jeu, int pos);
SDL_Rect * getHitBoxMonstreTouche(jeu_t *jeu, int pos);

carte_t * getCarte2(jeu_t *jeu, const char *nom);

void jeu_updateOffSetHeros(jeu_t *jeu);
void jeu_ajouterCarteVide(const char *nom, int largeur, int hauteur, chipset_t *chipset, musique_t *musique, jeu_t *jeu);

void musique_stopAndPlay(musique_t *musiqueActuelle, musique_t *musiqueSuivante);
void viderMessage(jeu_t *jeu);
void sauvegarderMessage(jeu_t *jeu);
void remettreDernierMessage(jeu_t *jeu);
void ajouterMessageHistorique(jeu_t *jeu);
void afficherHitboxAttaqueEpee(SDL_Renderer * renderer, jeu_t *jeu);
void updateFiolePV(SDL_Renderer *renderer, jeu_t *jeu);
void updateFiolePM(SDL_Renderer *renderer, jeu_t *jeu);
void afficherFiolePV(SDL_Renderer *renderer, jeu_t * jeu);
void afficherFiolePM(SDL_Renderer *renderer, jeu_t *jeu);
void afficherBarreXP(SDL_Renderer *renderer, jeu_t *jeu);
void afficherCouche(SDL_Renderer *renderer, carte_t *carte, int couche, jeu_t *jeu);
void afficherMurs(SDL_Renderer *renderer, carte_t *carte, jeu_t *jeu);
void jeu_detruire(jeu_t *jeu);

#endif