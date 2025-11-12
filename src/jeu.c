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

#include "headers/jeu.h"
#include "headers/chargerAffichages.h"
#include "headers/chargerSkins.h"
#include "headers/chargerMonstresData.h"
#include "headers/chargerPolices.h"
#include "headers/chargerBruitages.h"
#include "headers/chargerMusiques.h"
#include "headers/chargerChipsets.h"
#include "headers/chargerCartes.h"
#include "headers/chargerEvents.h"
#include "headers/chargerMonstres.h"

static inline void jeu_validerArguments(void) {
	if (FPS < 1 || FPS > 999) Exception("FPS invalide (FPS < 1 ou > 999)");
	if (UPS < 1 || UPS > 999) Exception("UPS invalide (UPS < 1 ou > 999)");
}

static void initCouleursCadres(jeu_t *jeu) {
	jeu->COULEURS_CADRES[0] = BLEU_FONCE_TRANSPARENT;
	jeu->COULEURS_CADRES[1] = VERT_FONCE_TRANSPARENT;
	jeu->COULEURS_CADRES[2] = BORDEAUX_TRANSPARENT;
	jeu->COULEURS_CADRES[3] = OR_FONCE_TRANSPARENT;
	jeu->COULEURS_CADRES[4] = GRIS_FONCE_TRANSPARENT;
}

static void chargementDonnees(jeu_t *jeu) {
	// ----- Init résultats d'erreurs -----
	chargerAffichages_result_t chAffich_res; chargerSkins_result_t chSkin_res; chargerMonstresData_result_t chMonstDat_res; chargerPolices_result_t chPoli_res; chargerBruitages_result_t chBruit_res; chargerMusiques_result_t chMus_res; chargerChipsets_result_t chChipt_res; chargerCartes_result_t chCart_res;
	chargerEvents_result_t chEv_res; chargerMonstres_result_t chMs_res;

	// ----- Chargement des données ------
	jeu->affichages = chargerAffichages_get(jeu->renderer, &chAffich_res);
	if (!jeu->affichages) LOG_ERROR("Echec chargement des affichages : %s", chargerAffichages_strerror(chAffich_res));

	jeu->skins = chargerSkins_get(jeu->renderer, &chSkin_res);
	if (!jeu->skins) LOG_ERROR("Echec chargement des skins : %s", chargerSkins_strerror(chSkin_res));

	jeu->monstresData = chargerMonstresData_get(jeu->renderer, &chMonstDat_res);
	if (!jeu->monstresData) LOG_ERROR("Echec chargement des monstresData : %s", chargerMonstresData_strerror(chMonstDat_res));

	jeu->polices = chargerPolices_get(&chPoli_res);
	if (!jeu->polices) LOG_ERROR("Echec chargement des polices : %s", chargerPolices_strerror(chPoli_res));

	jeu->bruitages = chargerBruitages_get(&chBruit_res);
	if (!jeu->bruitages) LOG_ERROR("Echec chargement des bruitages : %s", chargerBruitages_strerror(chBruit_res));

	jeu->musiques = chargerMusiques_get(&chMus_res);
	if (!jeu->musiques) LOG_ERROR("Echec chargement des musiques : %s", chargerMusiques_strerror(chMus_res));

	jeu->chipsets = chargerChipsets_get(jeu->renderer, &chChipt_res);
	if (!jeu->chipsets) LOG_ERROR("Echec chargement des chipsets : %s", chargerChipsets_strerror(chChipt_res));

	jeu->cartes = chargerCartes_get(jeu, &chCart_res);
	if (!jeu->cartes) LOG_ERROR("Echec chargement des cartes : %s", chargerCartes_strerror(chCart_res));

	chEv_res = chargerEvents_inject(jeu);
	if (chEv_res) LOG_ERROR("Echec chargement des events : %s", chargerEvents_strerror(chEv_res));

	chMs_res = chargerMonstres_inject(jeu);
	if (chMs_res) LOG_ERROR("Echec chargement des monstres : %s", chargerMonstres_strerror(chMs_res));
}

static void creationHeros(jeu_t *jeu) {
	FILE *fichierPseudo = fopen("PSEUDO.txt", "r");
	if (!fichierPseudo) Exception("Echec d'ouverture du fichier pseudo");

	char nom[26] = {0};
	if (!fgets(nom, sizeof(nom), fichierPseudo)) strcpy(nom, "Test");
	fclose(fichierPseudo);

	// Supprime le saut de ligne final éventuel (\n)
	nom[strcspn(nom, "\r\n")] = '\0';

	if (!*nom) strcpy(nom, "Test");
	heros_result_t res_heros;
	jeu->heros = heros_creer(jeu->renderer, nom, getSkin(jeu, 0), VOLEUR, 1, 1000, 12, 12, getPolice(jeu, 1), getCarte2(jeu, "Chateau_Roland_Cour_Interieure"), 10, &res_heros);
	if (!jeu->heros) { LOG_ERROR("Echec creation heros : %s", heros_strerror(res_heros)); Exception(""); }

	SDL_QueryTexture(jeu->heros->textureNom, NULL, NULL, &jeu->rectPseudo.w, &jeu->rectPseudo.h);
	jeu->rectPseudo.x = jeu->heros->positionEcran.x - (jeu->rectPseudo.w / 2) + (TAILLE_CASES / 2) - 2;
	jeu->rectPseudo.y = jeu->heros->positionEcran.y + TAILLE_CASES - 2;
}

jeu_t * jeu_creer(void) {
	jeu_validerArguments();

	jeu_t *jeu = calloc(1, sizeof(jeu_t));
	if (!jeu) Exception("Echec allocation jeu");

	jeu->window = NULL; jeu->renderer = NULL;
	initSDL(&jeu->window, &jeu->renderer);
	camera_init(&jeu->camera);
	jeu->enCours = jeu->refreshNextFrame = false;
	jeu->frames = jeu->tempsDebutDegats = 0ULL;
	jeu->fpsResult = jeu->indexCouleurCadres = 0;

	initCouleursCadres(jeu);
	chargementDonnees(jeu);
	creationHeros(jeu);
	//for (size_t i = 0; i < jeu->cartes->taille; ++i) carte_ecrireFichier(getCarte(jeu, i));

	memset(jeu->message, 0, sizeof(jeu->message));
	memset(jeu->messageCharNbOctets, 0, sizeof(jeu->messageCharNbOctets));
	memset(jeu->recapMessages, 0, sizeof(jeu->recapMessages));

	for (int i = 0; i < 2; ++i) { //        .x  _______________.y________________   _______.w________  _______________.h________________
		jeu->dstRectFiolePV[i] = (SDL_Rect){ 1, WINDOW_HEIGHT * (0.69 + i * 0.073), WINDOW_WIDTH / 25, WINDOW_HEIGHT * (0.25 - i * 0.072) };
		jeu->dstRectFiolePM[i] = (SDL_Rect){ WINDOW_WIDTH * 0.96 - 1, WINDOW_HEIGHT * (0.69 + i * 0.073), WINDOW_WIDTH / 25, WINDOW_HEIGHT * (0.25 - i * 0.072) };
		for (int j = 0; j < 3; ++j) { //            __.x__  _______.y______  .w  ____.h_____
			jeu->srcRectFiolePV[i][j] = (SDL_Rect){ i * 16, i * 21 + j * 72, 16, 72 - i * 21 };
			jeu->srcRectFiolePM[i][j] = (SDL_Rect){ 32 + i * 16, i * 21 + j * 72, 16, 72 - i * 21 };
		}
	}
	jeu->fiolesTiming = 0;
	jeu->delaiMessage = 0;
	jeu->compteurLettres = jeu->compteurLettresReelles = 0;
	for (int i = 0; i < 3; ++i) {
		jeu->textureFiolePVMorte[i] = creerTextureVide(jeu->renderer, 16, 51);
		jeu->textureFiolePMMorte[i] = creerTextureVide(jeu->renderer, 16, 51);
	}
	jeu->dstRectBarreXP = (SDL_Rect){ WINDOW_WIDTH * 0.02, WINDOW_HEIGHT * 0.95, WINDOW_WIDTH * 0.96, WINDOW_HEIGHT * 0.08 };
	jeu->compteurRecap = jeu->afficherRecap = 0;
	jeu->degatsAffiches = jeu->estUnCoupCritique = jeu->mursVisibles = jeu->menuVisible = false;

	arraylist_result_t alHbMsT_res;
	jeu->hitBoxMonstresTouches = arraylist_creer(AL_SDL_RECT, &alHbMsT_res);
	if (!jeu->hitBoxMonstresTouches) LOG_ERROR("Echec creation arraylist des hitBoxMonstresTouches : %s", arraylist_strerror(alHbMsT_res));

	jeu->nbEventPass = 0;
	camera_setCible(&jeu->camera, &jeu->heros->xOffSet, &jeu->heros->yOffSet);
	camera_update(&jeu->camera);
	camera_sync(&jeu->camera);

	jeu->musiqueActuelle = jeu->heros->carteActuelle->musique;
	return jeu;
}

SDL_Texture * getAffichage(jeu_t *jeu, int pos)         { if (!jeu) return NULL; return arraylist_get(jeu->affichages, pos); }
skin_t * getSkin(jeu_t *jeu, int pos)                   { if (!jeu) return NULL; return arraylist_get(jeu->skins, pos); }
monstreData_t * getMonstreData(jeu_t *jeu, int pos)     { if (!jeu) return NULL; return arraylist_get(jeu->monstresData, pos); }
TTF_Font * getPolice(jeu_t *jeu, int pos)               { if (!jeu) return NULL; return arraylist_get(jeu->polices, pos); }
musique_t * getMusique(jeu_t *jeu, int pos)             { if (!jeu) return NULL; return arraylist_get(jeu->musiques, pos); }
bruitage_t * getBruitage(jeu_t *jeu, int pos)           { if (!jeu) return NULL; return arraylist_get(jeu->bruitages, pos); }
chipset_t * getChipset(jeu_t *jeu, int pos)             { if (!jeu) return NULL; return arraylist_get(jeu->chipsets, pos); }
carte_t * getCarte(jeu_t *jeu, int pos)                 { if (!jeu) return NULL; return arraylist_get(jeu->cartes, pos); }
event_t * getEventActuel(jeu_t *jeu)                    { if (!jeu) return NULL; return arraylist_get(jeu->alEventsActuels, jeu->nbEventPass); }
SDL_Rect * getHitBoxMonstreTouche(jeu_t *jeu, int pos)  { if (!jeu) return NULL; return arraylist_get(jeu->hitBoxMonstresTouches, pos); }

monstreData_t * getMonstreData2(jeu_t *jeu, const char *nom) {
	if (!jeu || !nom || !*nom) return NULL;
	for (int i = 0; i < jeu->monstresData->taille; ++i) {
		monstreData_t *monstreData = arraylist_get(jeu->monstresData, i);
		if (strcmp(monstreData->nom, nom) == 0) return monstreData;
	}
	return NULL;
}

musique_t * getMusique2(jeu_t *jeu, const char *nom) {
	if (!jeu || !nom || !*nom) return NULL;
	for (size_t i = 0; i < jeu->musiques->taille; ++i) {
		musique_t *musique = arraylist_get(jeu->musiques, i);
		if (strcmp(musique->nom, nom) == 0) return musique;
	}
	return NULL;
}

chipset_t * getChipset2(jeu_t *jeu, const char *nom) {
	if (!jeu || !nom || !*nom) return NULL;
	for (size_t i = 0; i < jeu->chipsets->taille; ++i) {
		chipset_t *chipset = arraylist_get(jeu->chipsets, i);
		if (strcmp(chipset->nom, nom) == 0) return chipset;
	}
	return NULL;
}

carte_t * getCarte2(jeu_t *jeu, const char *nom) {
	if (!jeu || !nom || !*nom) return NULL;
	for (size_t i = 0; i < jeu->cartes->taille; ++i) {
		carte_t *carte = arraylist_get(jeu->cartes, i);
		if (strcmp(carte->nom, nom) == 0) return carte;
	}
	return NULL;
}

void jeu_ajouterCarteVide(const char *nom, int largeur, int hauteur, chipset_t *chipset, musique_t *musique, jeu_t *jeu) {
	if (!jeu) return;
	carte_result_t cart_res;
	carte_t *carte = carte_creerVide(nom, largeur, hauteur, chipset, musique, &cart_res);
	if (!carte) { LOG_ERROR("Echec creation carte vide %s (src: %s)", carte_strerror(cart_res), nom); return; }
	arraylist_add(jeu->cartes, carte);
}

void musique_stopAndPlay(musique_t *musiqueActuelle, musique_t *musiqueSuivante) {
	if (musiqueActuelle) musique_stop(musiqueActuelle);
	if (musiqueSuivante) musique_play(musiqueSuivante);
}

void jeu_supprimerDernierCaractere(jeu_t *jeu) {
	if (jeu->compteurLettres > 0) {
		int nbOctets = jeu->messageCharNbOctets[0][jeu->compteurLettres - 1];
		if (nbOctets > 0) {
			jeu->compteurLettresReelles -= nbOctets;
			jeu->compteurLettres--;
			jeu->messageCharNbOctets[0][jeu->compteurLettres] = 0; // On nettoie la case
			jeu->message[0][jeu->compteurLettresReelles] = '\0'; // On termine la chaîne
		}
	}
}

void jeu_viderMessageHeros(jeu_t *jeu) {
	memset(jeu->message[0], 0, sizeof(jeu->message[0]));
	memset(jeu->messageCharNbOctets[0], 0, sizeof(jeu->messageCharNbOctets[0]));
	jeu->compteurLettres = jeu->compteurLettresReelles = 0;
}

void jeu_sauvegarderMessageHeros(jeu_t *jeu) {
	strcpy(jeu->message[1], jeu->message[0]);
	copyIntArray(jeu->messageCharNbOctets[1], jeu->messageCharNbOctets[0], TAILLE_MAX_MSG);
}

void jeu_restaurerDernierMessageHeros(jeu_t *jeu) {
	strcpy(jeu->message[0], jeu->message[1]);
	copyIntArray(jeu->messageCharNbOctets[0], jeu->messageCharNbOctets[1], TAILLE_MAX_MSG);

	jeu->compteurLettresReelles = strlen(jeu->message[0]);
	jeu->compteurLettres = 0;
	for (int i = 0; i < TAILLE_MAX_MSG; i++) {
		int nbOctets = jeu->messageCharNbOctets[0][i];
		if (nbOctets == 0) break; // On s’arrête exactement à la fin du message
		jeu->compteurLettres++;
	}
}

void ajouterMessageHistorique(jeu_t *jeu) {
	if (jeu->compteurRecap == 3) {
		for (int i = 0; i < 2; ++i) {
			strcpy(jeu->recapMessages[i], jeu->recapMessages[i + 1]);
		}
		jeu->compteurRecap = 2;
	}
	strcpy(jeu->recapMessages[jeu->compteurRecap], jeu->message[0]);
	jeu->compteurRecap++;
}

void afficherHitboxAttaqueEpee(jeu_t *jeu) {
	dessinerRectangle(jeu->renderer, &jeu->heros->hitBoxEpeeEcran[jeu->heros->direction], BLANC_TRANSPARENT);
}

static void dessinerFiolePVMorte(jeu_t *jeu, int index, int hauteur) {
    SDL_Rect src = { 16, 21 + index * 72, 16, hauteur };
    SDL_Rect dst = { 0, 0, 16, hauteur };

    SDL_DestroyTexture(jeu->textureFiolePVMorte[index]);
    jeu->textureFiolePVMorte[index] = creerTextureVide(jeu->renderer, 16, 51);
    SDL_SetRenderTarget(jeu->renderer, jeu->textureFiolePVMorte[index]); // dessiner sur cette texture
    dessinerTexture(jeu->renderer, getAffichage(jeu, 0), &src, &dst, "Echec dessin fiole PV morte avec SDL_RenderCopy");
}

void updateFiolePV(jeu_t *jeu) {
	if (!jeu) return;
	const double ratioPV = heros_getRatioPV(jeu->heros);
	const int hauteur = maxInt(1, (int)(51 * (1 - ratioPV)));
	for (int i = 0; i < 3; ++i) dessinerFiolePVMorte(jeu, i, hauteur);
	SDL_SetRenderTarget(jeu->renderer, NULL); // redessine sur le renderer principal
}

static void dessinerFiolePMMorte(jeu_t *jeu, int index, int hauteur) {
	SDL_Rect src = { 48, 21 + index * 72, 16, hauteur };
	SDL_Rect dst = { 0, 0, 16, hauteur };

	SDL_DestroyTexture(jeu->textureFiolePMMorte[index]);
	jeu->textureFiolePMMorte[index] = creerTextureVide(jeu->renderer, 16, 51);
	SDL_SetRenderTarget(jeu->renderer, jeu->textureFiolePMMorte[index]); // dessiner sur cette texture
	dessinerTexture(jeu->renderer, getAffichage(jeu, 0), &src, &dst, "Echec dessin fiole PM morte avec SDL_RenderCopy");
}

void updateFiolePM(jeu_t *jeu) {
	if (!jeu) return;
	const double ratioPM = heros_getRatioPM(jeu->heros);
	const int hauteur = maxInt(1, (int) (51 * (1 - ratioPM)));
	for (int i = 0; i < 3; ++i) dessinerFiolePMMorte(jeu, i, hauteur);
	SDL_SetRenderTarget(jeu->renderer, NULL); // redessine sur le renderer principal
}

void jeu_detruire(jeu_t *jeu) {
	if (!jeu) return;
	for (int i = 0; i < 3; ++i) {
		SDL_DestroyTexture(jeu->textureFiolePVMorte[i]); jeu->textureFiolePVMorte[i] = NULL;
		SDL_DestroyTexture(jeu->textureFiolePMMorte[i]); jeu->textureFiolePMMorte[i] = NULL;
	}
	heros_detruire(jeu->heros); jeu->heros = NULL;
	arraylist_detruire(jeu->hitBoxMonstresTouches, false); jeu->hitBoxMonstresTouches = NULL;
	arraylist_detruire(jeu->cartes, true); jeu->cartes = NULL;
	arraylist_detruire(jeu->chipsets, true); jeu->chipsets = NULL;
	arraylist_detruire(jeu->bruitages, true); jeu->bruitages = NULL;
	arraylist_detruire(jeu->musiques, true); jeu->musiques = NULL;
	arraylist_detruire(jeu->polices, true); jeu->polices = NULL;
	arraylist_detruire(jeu->monstresData, true); jeu->monstresData = NULL;
	arraylist_detruire(jeu->skins, true); jeu->skins = NULL;
	arraylist_detruire(jeu->affichages, true); jeu->affichages = NULL;
	freeSDL(jeu->window, jeu->renderer); jeu->window = NULL; jeu->renderer = NULL;
	free(jeu);
}