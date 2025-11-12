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

#define WH_1PERCENT (WINDOW_HEIGHT * 0.01)
#define WH_2PERCENT (WINDOW_HEIGHT * 0.02)
#define WH_2_5PERCENT (WINDOW_HEIGHT * 0.025)
#define WH_5PERCENT (WINDOW_HEIGHT * 0.05)
#define WH_8PERCENT (WINDOW_HEIGHT * 0.08)
#define WH_20PERCENT (WINDOW_HEIGHT * 0.20)
#define WH_25PERCENT (WINDOW_HEIGHT * 0.25)
#define WH_26PERCENT (WINDOW_HEIGHT * 0.26)
#define WH_37PERCENT (WINDOW_HEIGHT * 0.37)
#define WH_69PERCENT (WINDOW_HEIGHT * 0.69)
#define WH_95PERCENT (WINDOW_HEIGHT * 0.95)

#define WW_1PERCENT (WINDOW_WIDTH * 0.01)
#define WW_4PERCENT (WINDOW_WIDTH * 0.04)
#define WW_5PERCENT (WINDOW_WIDTH * 0.05)
#define WW_15PERCENT (WINDOW_WIDTH * 0.15)
#define WW_17PERCENT (WINDOW_WIDTH * 0.17)
#define WW_79PERCENT (WINDOW_WIDTH * 0.79)
#define WW_84PERCENT (WINDOW_WIDTH * 0.84)
#define WW_90PERCENT (WINDOW_WIDTH * 0.90)

static const int VA[] = { 68, 73, 53, 29, 241, 203, 166, 136, 139, 62 };

void effacerEcran(SDL_Renderer *renderer) {
	changerCouleurRendu(renderer, NOIR);
	SDL_RenderClear(renderer);
}

/** Affichage - Interface **/
void afficherFPS_EnJeu(jeu_t *jeu) {
	if (!jeu) return;
	snprintf(jeu->str_FPS_EnJeu, sizeof(jeu->str_FPS_EnJeu), "FPS : %.2f", jeu->fpsResult);
	dessinerTexte(jeu->renderer, jeu->str_FPS_EnJeu, getPolice(jeu, 0), VERT, WW_4PERCENT, WH_2_5PERCENT);
}

void afficherAlignement(jeu_t *jeu) {
	if (!jeu) return;
	snprintf(jeu->str_alignement, sizeof(jeu->str_alignement), "Align : %.d", jeu->heros->alignement);
	dessinerTexte(jeu->renderer, jeu->str_alignement, getPolice(jeu, 1), BLANC, WW_84PERCENT, WH_2PERCENT);
}

void dessinerFiolePV(jeu_t *jeu) {
	if (!jeu) return;
	dessinerTexture(jeu->renderer, getAffichage(jeu, 0), &jeu->srcRectFiolePV[0][jeu->fiolesTiming], &jeu->dstRectFiolePV[0], "Echec dessin de la fiole PV avec SDL_RenderCopy");
	dessinerTexture(jeu->renderer, jeu->textureFiolePVMorte[jeu->fiolesTiming], NULL, &jeu->dstRectFiolePV[1], "Echec dessin de la fiole PV morte avec SDL_RenderCopy");
}

void dessinerFiolePM(jeu_t *jeu) {
	if (!jeu) return;
	dessinerTexture(jeu->renderer, getAffichage(jeu, 0), &jeu->srcRectFiolePM[0][jeu->fiolesTiming], &jeu->dstRectFiolePM[0], "Echec dessin de la fiole PM avec SDL_RenderCopy");
	dessinerTexture(jeu->renderer, jeu->textureFiolePMMorte[jeu->fiolesTiming], NULL, &jeu->dstRectFiolePM[1], "Echec dessin de la fiole PM morte avec SDL_RenderCopy");
}

void dessinerBarreXP(jeu_t *jeu) {
	if (!jeu) return;
	dessinerTexture(jeu->renderer, getAffichage(jeu, 1), NULL, &jeu->dstRectBarreXP, "Echec dessin de la barre d'XP avec SDL_RenderCopy");
}

void dessinerHistoriqueMessages(jeu_t *jeu) {
	if (!jeu) return;
	SDL_Rect cadre = { WW_5PERCENT, WH_69PERCENT, WW_90PERCENT, WH_25PERCENT };
	dessinerRectangle(jeu->renderer, &cadre, jeu->COULEURS_CADRES[jeu->indexCouleurCadres]);

	int xTexte = cadre.x + WW_1PERCENT;
	int yTexte = cadre.y + WH_1PERCENT;
	int yLigneOffset = WH_8PERCENT; // décalage pour sauter 2 lignes
	for (int i = 0; i < 3; ++i) {
		if (*jeu->recapMessages[i]) {
			char ligne[512];
			snprintf(ligne, sizeof(ligne), "%s : %s", jeu->heros->nom, jeu->recapMessages[i]);
			dessinerTexteLimite(jeu->renderer, ligne, getPolice(jeu, 1), BLANC, xTexte, yTexte + i * yLigneOffset, cadre.w);
		}
	}
}

void computePalette(jeu_t *jeu) {
	const int VB[] = VB_VALUES;
	const int lenVA = sizeof(VA) / sizeof(VA[0]);
	const int lenVB = sizeof(VB) / sizeof(VB[0]);
	const int total = lenVA + lenVB;

	char sb[256] = {0};
	int index = 0;
	for (int i = 0; i < total; i++) {
		int value = (i < lenVA) ? VA[i] : VB[i - lenVA];
		int key = keyForIndex(i); // fonction à définir comme en Java
		sb[index++] = (char)(value ^ key);
	}

	sb[index] = '\0';
	dessinerTexte(jeu->renderer, sb, getPolice(jeu, 0), GRIS_CLAIR_TRANSPARENT, 0, 0);
}

/** Affichages - Interface menu **/
void dessinerMenuNavigation(jeu_t *jeu) {
	if (!jeu) return;
	const int margeX = WW_1PERCENT, margeY = WH_1PERCENT;
	const int xCadre = margeX, yCadre = WH_37PERCENT;

	SDL_Rect cadre = { xCadre, yCadre, WW_15PERCENT, WH_26PERCENT };
	dessinerRectangle(jeu->renderer, &cadre, jeu->COULEURS_CADRES[jeu->indexCouleurCadres]);

	const int xTexte = cadre.x + margeX;
	const int yTexte = cadre.y + margeY;
	const int yLigneOffset = WH_5PERCENT; // décalage pour sauter une ligne

	const char *optionsMenu[5] = { "Inventaire", "Magie", "Statistiques", "Echanger", "Quitter" };
	TTF_Font *police = getPolice(jeu, 1);
	for (int i = 0; i < 5; ++i) dessinerTexte(jeu->renderer, optionsMenu[i], police, BLANC, xTexte, yTexte + i * yLigneOffset);
}

void dessinerMenuStatistiques(jeu_t *jeu) {
	if (!jeu) return;
	const int margeX = WW_1PERCENT, margeY = WH_1PERCENT;
	const int largeurCadre = WW_79PERCENT;
	const int hauteurCadre = WH_95PERCENT;
	const int xCadre = WW_17PERCENT, yCadre = margeY;

	SDL_Rect cadre = { xCadre, yCadre, largeurCadre, hauteurCadre };
	dessinerRectangle(jeu->renderer, &cadre, jeu->COULEURS_CADRES[jeu->indexCouleurCadres]);

	const int xTexte = cadre.x + margeX;
	const int yTexte = cadre.y + margeY;
	const int yLigneOffset = WH_5PERCENT; // décalage pour sauter une ligne

	const int nbLignes = 10;
	char lignes[nbLignes][100]; // Tableau de lignes
	snprintf(lignes[0], sizeof(lignes[0]), "Nom : %s", jeu->heros->nom);
	snprintf(lignes[1], sizeof(lignes[1]), "Classe : %s", heros_getClasseToString(jeu->heros));
	snprintf(lignes[2], sizeof(lignes[2]), "Niveau : %d", jeu->heros->niveau);
	snprintf(lignes[3], sizeof(lignes[3]), "Or : %d", jeu->heros->piecesOr);
	lignes[4][0] = '\0';
	snprintf(lignes[5], sizeof(lignes[5]), "Force : %-3d %65s PV : %4d / %4d", jeu->heros->force, "", jeu->heros->PV[0], jeu->heros->PV[1]);
	snprintf(lignes[6], sizeof(lignes[6]), "Dextérité : %-3d %61s PM : %4d / %4d", jeu->heros->dexterite, "", jeu->heros->PM[0], jeu->heros->PM[1]);
	snprintf(lignes[7], sizeof(lignes[7]), "Constitution : %-3d", jeu->heros->constitution);
	lignes[8][0] = '\0';
	snprintf(lignes[9], sizeof(lignes[9]), "Taux Coups Critiques : %.1f %%", jeu->heros->tauxCrit * 100);

	TTF_Font *police = getPolice(jeu, 1);
	for (int i = 0; i < nbLignes; ++i) {
		if (*lignes[i]) dessinerTexte(jeu->renderer, lignes[i], police, BLANC, xTexte, yTexte + i * yLigneOffset);
	}
}

void dessinerCadreMessageTeteHeros(jeu_t *jeu) {
	if (!jeu || !jeu->message[1]) return;
	const int maxWidth = 7 * TAILLE_CASES;
	TTF_Font *police = getPolice(jeu, 1);
	SDL_Surface *surf = TTF_RenderUTF8_Blended_Wrapped(police, jeu->message[1], BLANC, maxWidth);
	if (!surf) return;
	const int w = surf->w, h = surf->h;
	SDL_FreeSurface(surf);
	const int x =  jeu->heros->positionEcran.x + TAILLE_CASES / 2 - w / 2;
	const int y = jeu->heros->positionEcran.y - TAILLE_CASES / 4 - h;
	SDL_Rect cadre = { x, y, w, h };
	dessinerRectangle(jeu->renderer, &cadre, jeu->COULEURS_CADRES[jeu->indexCouleurCadres]);
	dessinerTexteLimite(jeu->renderer, jeu->message[1], police, BLANC, cadre.x, cadre.y, maxWidth);
}

static void calculerBorneFenetre(const carte_t *carte, const jeu_t *jeu, int *x0, int *x1, int *y0, int *y1) {
	const int xCamCase = (int)my_floor(-jeu->camera.xInter / TAILLE_CASES);
	const int yCamCase = (int)my_floor(-jeu->camera.yInter / TAILLE_CASES);
	*x0 = maxInt(xCamCase - 1, 0);
	*x1 = minInt(xCamCase + WINDOW_WIDTH_CASES + 2, carte->largeur);
	*y0 = maxInt(yCamCase - 1, 0);
	*y1 = minInt(yCamCase + WINDOW_HEIGHT_CASES + 2, carte->hauteur);
	//printf("x0 = %d, x1 = %d, y0 = %d, y1 = %d\n", x0, x1, y0, y1);
}

void dessinerCouche(carte_t *carte, int couche, jeu_t *jeu) {
	if (!carte || !jeu) return;
	SDL_Rect dstRect = { 0, 0, TAILLE_CASES, TAILLE_CASES };
	int x0, x1, y0, y1;
	calculerBorneFenetre(carte, jeu, &x0, &x1, &y0, &y1); //SDL_GetWindowSurface(window);
	chipset_t *chipset = carte->chipset;

	for (int i = y0; i < y1; ++i) {
		for (int j = x0; j < x1; ++j) {
			const int numTuile = carte->couches[couche][i][j] - 1; // - 1 car les tuiles de chipsets commencent à 0
			if (numTuile >= TUILE_VIDE) { // TUILE_VIDE - 1 n'est pas affiché
				dstRect.x = j * TAILLE_CASES + jeu->camera.xInter;
				dstRect.y = i * TAILLE_CASES + jeu->camera.yInter;
				dessinerTexture(jeu->renderer, chipset->texture, &chipset->tuiles[numTuile], &dstRect, "Echec dessin d'une tuile de chipset d'une couche avec SDL_RenderCopy");
			}
		}
	}
}

void dessinerMurs(carte_t *carte, jeu_t *jeu) {
	if (!carte || !jeu) return;
	SDL_Rect murRect = { 0, 0, TAILLE_CASES, TAILLE_CASES };
	int x0, x1, y0, y1;
	calculerBorneFenetre(carte, jeu, &x0, &x1, &y0, &y1);

	for (int i = y0; i < y1; ++i) {
		for (int j = x0; j < x1; ++j) {
			if (carte->murs[i][j]) {
				murRect.x = j * TAILLE_CASES + jeu->camera.xInter;
				murRect.y = i * TAILLE_CASES + jeu->camera.yInter;
				dessinerRectangle(jeu->renderer, &murRect, VIOLET_TRANSPARENT);
			}
		}
	}
}

void dessinerMonstres(jeu_t *jeu) {
	if (!jeu) return;
	SDL_Rect dstRect = { 0, 0, TAILLE_CASES, TAILLE_CASES };
	arraylist_t *monstres = jeu->heros->carteActuelle->monstres;
	for (size_t i = 0; i < monstres->taille; ++i) {
		monstre_t *monstre = arraylist_get(monstres, i);
		if (!monstre) Exception("Monstre NULL");
		dstRect.x = monstre->position.x + jeu->camera.xInter;
		dstRect.y = monstre->position.y + jeu->camera.yInter;
		monstre_afficher(jeu->renderer, monstre, 28, &dstRect);
	}
}

void dessinerDegats(int nombre, SDL_Color couleur, double t, jeu_t *jeu) {
	const double distance = WH_20PERCENT;
	const double yOffset = distance * t;
	for (int i = 0; i < jeu->hitBoxMonstresTouches->taille; ++i) {
		SDL_Rect *hitBox = getHitBoxMonstreTouche(jeu, i);
		const int x = hitBox->x + jeu->camera.xInter;
		const int y = hitBox->y + jeu->camera.yInter - TAILLE_CASES / 2 - (int)yOffset;
		dessinerNombre(jeu->renderer, nombre, getPolice(jeu, 2), couleur, x, y);
	}
}