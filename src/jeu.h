// @author Metroidzeta

#ifndef JEU_H
#define JEU_H

#include "joueur.h"
#include "bruitage.h"
#include "event_msg.h"
#include "event_tp.h"
#include "event_jm.h"
#include "event_changePV.h"
#include "event_changePM.h"

#define TAILLE_MAX_MSG 40
#define TAILLE_MAX_MSG_REELLE ((TAILLE_MAX_MSG * 2) + 1) // taille * 2 si un caractère UTF-8 a une longueur de 2 octets (pour les caractères accentués) + '\0'

typedef struct jeu_s {
	bool programme_actif;
	int frames;
	double FPS_result;
	int numCouleur_cadres;
	SDL_Color couleurs_cadres[5];
	float deuxTiersSeconde;
	char str_FPS_Fenetre[36]; // 25 + (maxIntLength: 10) + '\0' (le string du titre de la fenetre)
	char str_FPS_EnJeu[13]; // 6 + 6 + '\0' (le string "FPS : xxx.xx" dans le jeu)
	char str_alignement[19]; // 8 + (maxIntLength: 10) + '\0' (le string "Align : XXX" dans le jeu
	joueur_t * joueur;
	SDL_Rect rectPseudo;
	arraylist_t * lesAffichages;
	arraylist_t * lesSkins;
	arraylist_t * lesMonstresData;
	arraylist_t * lesPolices;
	arraylist_t * lesMusiques;
	arraylist_t * lesBruitages;
	arraylist_t * lesChipsets;
	arraylist_t * lesCartes;
	int xOffSetJoueur; // Le décalage entre la position réelle x de notre joueur et la position x de notre joueur SUR L'ECRAN
	int yOffSetJoueur; // Le décalage entre la position réelle y de notre joueur et la position y de notre joueur SUR L'ECRAN
	SDL_Rect hitBoxJoueurEcran; // La hitbox de notre joueur SUR L'ECRAN
	SDL_Rect hitBoxEpeeJoueurEcran[4]; // La hitbox de l'épée de notre joueur SUR L'ECRAN
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
	bool messageChar2octets[2][TAILLE_MAX_MSG_REELLE - 1]; // tableau de bool pour savoir si chaque lettre est codée sur 2 caractères, - 1 car on enlève '\0'
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

jeu_t * jeu_creer(SDL_Renderer * renderer);
void creation_donnees(SDL_Renderer * renderer, jeu_t * jeu);

monstre_data_t * getMonstreData2(jeu_t * jeu, char * nom);
chipset_t * getChipset2(jeu_t * jeu, char * nom);
carte_t * getCarte2(jeu_t * jeu, char * nom);
musique_t * getMusique2(jeu_t * jeu, char * nom);

void creation_events(jeu_t * jeu);
void creation_monstres(jeu_t * jeu);
void jeu_updateOffSetJoueur(jeu_t * jeu);
void creation_notreJoueur(SDL_Renderer * renderer, jeu_t * jeu);

void ajouterAffichage(SDL_Renderer * renderer, char * nomFichier, jeu_t * jeu);
void ajouterSkin(SDL_Renderer * renderer, char * nomFichier, jeu_t * jeu);
void ajouterMonstreData(SDL_Renderer * renderer, char * nomFichier, char * nom, int PVMax, int xp, int piecesOr, jeu_t * jeu);
void ajouterPolice(char * nomFichier, int taille, jeu_t * jeu);
void ajouterMusique(char * nomFichier, jeu_t * jeu);
void ajouterBruitage(char * nomFichier, jeu_t * jeu);
void ajouterChipset(SDL_Renderer * renderer, char * nomFichier, int tailleTuile, jeu_t * jeu);
void ajouterCarte(char * nom, int hauteur, int largeur, chipset_t * chipset, musique_t * musique, jeu_t * jeu);
void ajouterCarteVide(char * nom, int hauteur, int largeur, chipset_t * chipset, musique_t * musique, jeu_t * jeu);

SDL_Texture * getAffichage(jeu_t * jeu, int pos);
skin_t * getSkin(jeu_t * jeu, int pos);
monstre_data_t * getMonstreData(jeu_t * jeu, int pos);
TTF_Font * getPolice(jeu_t * jeu, int pos);
musique_t * getMusique(jeu_t * jeu, int pos);
bruitage_t * getBruitage(jeu_t * jeu, int pos);
chipset_t * getChipset(jeu_t * jeu, int pos);
carte_t * getCarte(jeu_t * jeu, int pos);
event_t * getEventActuel(jeu_t * jeu, int pos);
SDL_Rect * getHitBoxMonstreTouche(jeu_t * jeu, int pos);

void musique_stopAndPlay(musique_t * musiqueActuelle, musique_t * musiqueSuivante);
void viderMessage(jeu_t * jeu);
void sauvegarderMessage(jeu_t * jeu);
void remettreDernierMessage(jeu_t * jeu);
void ajouterMessageHistorique(jeu_t * jeu);
void afficherHitboxAttaqueEpee(SDL_Renderer * renderer, jeu_t * jeu);
void updateFiolePV(SDL_Renderer * renderer, jeu_t * jeu);
void updateFiolePM(SDL_Renderer * renderer, jeu_t * jeu);
void afficherFiolePV(SDL_Renderer * renderer, jeu_t * jeu);
void afficherFiolePM(SDL_Renderer * renderer, jeu_t * jeu);
void afficherBarreXP(SDL_Renderer * renderer, jeu_t * jeu);
void afficherCouche(SDL_Renderer * renderer, carte_t * carte, int couche, jeu_t * jeu);
void afficherMurs(SDL_Renderer * renderer, carte_t * carte, jeu_t * jeu);
void jeu_detruire(jeu_t * jeu);

#endif