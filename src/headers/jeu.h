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

#ifndef JEU_H
#define JEU_H

#include "heros.h"
#include "bruitage.h"
#include "camera.h"

#define TAILLE_MAX_MSG 45
#define TAILLE_MAX_MSG_REELLE ((TAILLE_MAX_MSG * 4) + 1) // taille * 4 si un caractère UTF-8 a une longueur de 4 octets + '\0'
#define TAILLE_TITRE_FENETRE (sizeof(TITRE_FENETRE) - 1)  // sizeof inclut le '\0', donc -1

typedef struct jeu_s {
	SDL_Window *window;
	SDL_Renderer *renderer;
	camera_t camera;
	bool enCours; // programme actif
	bool refreshNextFrame;
	Uint64 frames;
	double fpsResult;
	int indexCouleurCadres;
	SDL_Color COULEURS_CADRES[5];
	char str_FPS_Fenetre[TAILLE_TITRE_FENETRE + 16]; // TAILLE_TITRE_FENETRE + 15 + '\0' (string "TITRE_FENETRE | FPS : XXX.XX" dans la fenêtre, 1-3.2 chiffres)
	char str_FPS_EnJeu[13]; // 12 + '\0' (string "FPS : XXX.XX", 1-3.2 chiffres)
	char str_alignement[12]; // 11 + '\0' (string "Align : XXX", 1-3 chiffres)
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
	SDL_Rect srcRectFiolePV[2][3], srcRectFiolePM[2][3];
	SDL_Rect dstRectFiolePV[2], dstRectFiolePM[2];
	SDL_Texture *textureFiolePVMorte[3]; // Les 3 textures de la fiolePV morte qui est affichée par dessus la fiole PV vivante
	SDL_Texture *textureFiolePMMorte[3]; // Les 3 textures de la fiolePM morte qui est affichée par dessus la fiole PM vivante
	SDL_Rect dstRectBarreXP;
	int fiolesTiming;
	int delaiMessage;
	char message[2][TAILLE_MAX_MSG_REELLE]; // Le message du héros et sa sauvegarde
	int messageCharNbOctets[2][TAILLE_MAX_MSG]; // tableau pour savoir combien d'octets (1-4) chaque lettre du message du héros et sa savegarde est codée (UTF-8)
	int compteurLettres;
	int compteurLettresReelles;
	char recapMessages[3][TAILLE_MAX_MSG_REELLE];
	int compteurRecap;
	int afficherRecap;
	bool degatsAffiches;       // indique si des dégâts sont en cours d’affichage
	bool estUnCoupCritique;    // indique si le dernier coup était critique
	Uint64 tempsDebutDegats;   // temps du début de l’animation de dégâts
	bool mursVisibles, menuVisible;
	arraylist_t *alEventsActuels;
	arraylist_t *hitBoxMonstresTouches;
	int nbEventPass;
	musique_t *musiqueActuelle;
} jeu_t;

jeu_t * jeu_creer(void);

SDL_Texture * getAffichage(jeu_t *jeu, int pos);
skin_t * getSkin(jeu_t *jeu, int pos);
monstreData_t * getMonstreData(jeu_t *jeu, int pos);
TTF_Font * getPolice(jeu_t *jeu, int pos);
musique_t * getMusique(jeu_t *jeu, int pos);
bruitage_t * getBruitage(jeu_t *jeu, int pos);
chipset_t * getChipset(jeu_t *jeu, int pos);
carte_t * getCarte(jeu_t *jeu, int pos);
event_t * getEventActuel(jeu_t *jeu);
SDL_Rect * getHitBoxMonstreTouche(jeu_t *jeu, int pos);

monstreData_t * getMonstreData2(jeu_t *jeu, const char *nom);
musique_t * getMusique2(jeu_t *jeu, const char *nom);
chipset_t * getChipset2(jeu_t *jeu, const char *nom);
carte_t * getCarte2(jeu_t *jeu, const char *nom);

void jeu_ajouterCarteVide(const char *nom, int largeur, int hauteur, chipset_t *chipset, musique_t *musique, jeu_t *jeu);
void musique_stopAndPlay(musique_t *musiqueActuelle, musique_t *musiqueSuivante);
void jeu_viderMessageHeros(jeu_t *jeu);
void jeu_supprimerDernierCaractere(jeu_t *jeu);
void jeu_sauvegarderMessageHeros(jeu_t *jeu);
void jeu_restaurerDernierMessageHeros(jeu_t *jeu);
void ajouterMessageHistorique(jeu_t *jeu);
void afficherHitboxAttaqueEpee(jeu_t *jeu);
void updateFiolePV(jeu_t *jeu);
void updateFiolePM(jeu_t *jeu);

void jeu_detruire(jeu_t *jeu);

#endif