// @author Alain Barbier alias "Metroidzeta"

#include "headers/jeu.h"

static void jeu_validerArguments(SDL_Renderer *renderer) {
	if (FPS < 1 || FPS > 999) Exception("FPS invalide < 1 ou > 999");
	if (UPS < 1 || UPS > 999) Exception("UPS invalide < 1 ou > 999");
	if (!renderer) Exception("Renderer NULL");
}

static void chargementDonnees(SDL_Renderer *renderer, jeu_t *jeu) {
	chargerAffichages_result_t resCAF = chargerAffichages_get(renderer, &jeu->affichages);
	if (resCAF != CHARGERAFFICHAGES_OK) LOG_ERROR("Echec chargement affichages : %s", chargerAffichages_strerror(resCAF));

	chargerSkins_result_t resCSK = chargerSkins_get(renderer, &jeu->skins);
	if (resCSK != CHARGERSKINS_OK) LOG_ERROR("Echec chargement skins : %s", chargerSkins_strerror(resCSK));

	chargerMonstresData_result_t resCMD = chargerMonstresData_get(renderer, &jeu->monstresData);
	if (resCMD != CHARGERMONSTRESDATA_OK) LOG_ERROR("Echec chargement monstresData : %s", chargerMonstresData_strerror(resCMD));

	chargerPolices_result_t resCPO = chargerPolices_get(&jeu->polices);
	if (resCPO != CHARGERPOLICES_OK) LOG_ERROR("Echec chargement polices : %s", chargerPolices_strerror(resCPO));

	chargerBruitages_result_t resCBR = chargerBruitages_get(&jeu->bruitages);
	if (resCBR != CHARGERBRUITAGES_OK) LOG_ERROR("Echec chargement bruitages : %s", chargerBruitages_strerror(resCBR));

	chargerMusiques_result_t resCMU = chargerMusiques_get(&jeu->musiques);
	if (resCMU != CHARGERMUSIQUES_OK) LOG_ERROR("Echec chargement musiques : %s", chargerMusiques_strerror(resCMU));

	chargerChipsets_result_t resCCH = chargerChipsets_get(renderer, &jeu->chipsets);
	if (resCCH != CHARGERCHIPSETS_OK) LOG_ERROR("Echec chargement chipsets : %s", chargerChipsets_strerror(resCCH));

	chargerCartes_result_t resCCA = chargerCartes_get(&jeu->cartes, jeu->chipsets, jeu->musiques);
	if (resCCA != CHARGERCARTES_OK) LOG_ERROR("Echec chargement cartes : %s", chargerCartes_strerror(resCCA));

	chargerEvents_result_t resCEV = chargerEvents_get(jeu->cartes, jeu->musiques);
	if (resCEV != CHARGEREVENTS_OK) LOG_ERROR("Echec chargement events : %s", chargerEvents_strerror(resCEV));

	chargerMonstres_result_t resCMO = chargerMonstres_get(jeu->cartes, jeu->monstresData);
	if (resCMO != CHARGERMONSTRES_OK) LOG_ERROR("Echec chargement monstres : %s", chargerMonstres_strerror(resCMO));
}

static void creation_heros(SDL_Renderer *renderer, jeu_t *jeu) {
	FILE *fichierPseudo = fopen("PSEUDO.txt", "r");
	if (!fichierPseudo) { fclose(fichierPseudo); Exception("Echec d'ouverture du fichier pseudo"); }

	char nomHeros[26];
	fscanf(fichierPseudo, "%25s", nomHeros);
	fclose(fichierPseudo);

	if (!*nomHeros) strcpy(nomHeros, "Test");
	heros_result_t res = heros_creer(&jeu->heros, renderer, nomHeros, getSkin(jeu, 0), VOLEUR, 1, 1000, 12, 12, getPolice(jeu, 1), getCarte2(jeu, "Chateau_Roland_Cour_Interieure"), 10);
	if (res != HEROS_OK) { LOG_ERROR("Echec creation heros : %s", heros_strerror(res)); Exception(""); }

	SDL_QueryTexture(jeu->heros->textureNom, NULL, NULL, &jeu->rectPseudo.w, &jeu->rectPseudo.h);
	jeu->rectPseudo.x = jeu->hitBoxHerosEcran.x - (jeu->rectPseudo.w / 2) + (TAILLE_CASES / 2) - 2;
	jeu->rectPseudo.y = jeu->hitBoxHerosEcran.y + TAILLE_CASES - 2;

	jeu_updateOffSetHeros(jeu);
	jeu->musiqueActuelle = jeu->heros->carteActuelle->musique;
}

static inline int alignToTile(int value) {
	return value - (value % TAILLE_CASES);
}

jeu_t * jeu_creer(SDL_Renderer *renderer) {
	jeu_validerArguments(renderer);

	jeu_t *jeu = calloc(1, sizeof(jeu_t));
	if (!jeu) Exception("Echec allocation jeu");

	jeu->enCours = false;
	jeu->frames = jeu->FPS_result = jeu->numCouleur_cadres = 0;

	jeu->couleurs_cadres[0] = BLEU_FONCE_TRANSPARENT;
	jeu->couleurs_cadres[1] = VERT_FONCE_TRANSPARENT;
	jeu->couleurs_cadres[2] = BORDEAUX_TRANSPARENT;
	jeu->couleurs_cadres[3] = OR_FONCE_TRANSPARENT;
	jeu->couleurs_cadres[4] = GRIS_FONCE_TRANSPARENT;
	jeu->deuxTiersSeconde = 1000.0f * 2.0f / 3.0f;
	chargementDonnees(renderer, jeu);

	const int xHerosEcran = alignToTile(WINDOW_WIDTH / 2);
	const int yHerosEcran = alignToTile(WINDOW_HEIGHT / 2);
	// HIT BOX HEROS (A L'ECRAN)        ____.x_____  ____.y_____  _____.w_____  _____.h_____
	jeu->hitBoxHerosEcran = (SDL_Rect) {xHerosEcran, yHerosEcran, TAILLE_CASES, TAILLE_CASES};
	creation_heros(renderer, jeu);

	// HIT BOX EPEE (A L'ECRAN)                  ____.x_____  ________________.y___________________  _____.w_____  ______.h________
	jeu->hitBoxEpeeHerosEcran[BAS] = (SDL_Rect){ xHerosEcran, yHerosEcran + (TAILLE_CASES / 2) + 10, TAILLE_CASES, TAILLE_CASES / 2 };
	jeu->hitBoxEpeeHerosEcran[GAUCHE] = (SDL_Rect){ xHerosEcran - 10, yHerosEcran, TAILLE_CASES / 2, TAILLE_CASES };
	jeu->hitBoxEpeeHerosEcran[DROITE] = (SDL_Rect){ xHerosEcran + (TAILLE_CASES / 2) + 10, yHerosEcran, TAILLE_CASES / 2, TAILLE_CASES };
	jeu->hitBoxEpeeHerosEcran[HAUT] = (SDL_Rect){ xHerosEcran, yHerosEcran - (TAILLE_CASES / 2) + 10, TAILLE_CASES, TAILLE_CASES / 2 };

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
		jeu->textureFiolePVMorte[i] = creerTextureVide(renderer, 16, 51);
		jeu->textureFiolePMMorte[i] = creerTextureVide(renderer, 16, 51);
	}
	jeu->dstRectBarreXP = (SDL_Rect) { WINDOW_WIDTH * 0.02, WINDOW_HEIGHT * 0.95, WINDOW_WIDTH * 0.96, WINDOW_HEIGHT * 0.08 };
	jeu->compteurRecap = 0;
	jeu->afficherRecap = 0;
	jeu->degatsAffiches = 0;
	jeu->estCoupCritique = jeu->mursVisibles = jeu->menuVisible = false;
	jeu->alEventsActuels = NULL;
	jeu->lesHitBoxDesMonstresTouches = NULL;

	arraylist_result_t resAL = arraylist_creer(&jeu->lesHitBoxDesMonstresTouches, AL_SDL_RECT);
	if (resAL != ARRAYLIST_OK) LOG_ERROR("Echec creation arraylist hitBoxDesMonstresTouches : %s", arraylist_strerror(resAL));

	jeu->nbEventPass = 0;

	return jeu;
}

SDL_Texture * getAffichage(jeu_t *jeu, int pos) { return arraylist_get(jeu->affichages, pos); }
skin_t * getSkin(jeu_t *jeu, int pos) { return arraylist_get(jeu->skins, pos); }
monstreData_t * getMonstreData(jeu_t *jeu, int pos) { return arraylist_get(jeu->monstresData, pos); }
TTF_Font * getPolice(jeu_t *jeu, int pos) { return arraylist_get(jeu->polices, pos); }
musique_t * getMusique(jeu_t *jeu, int pos) { return arraylist_get(jeu->musiques, pos); }
bruitage_t * getBruitage(jeu_t *jeu, int pos) { return arraylist_get(jeu->bruitages, pos); }
chipset_t * getChipset(jeu_t *jeu, int pos) { return arraylist_get(jeu->chipsets, pos); }
carte_t * getCarte(jeu_t *jeu, int pos) { return arraylist_get(jeu->cartes, pos); }
event_t * getEventActuel(jeu_t *jeu, int pos) { return arraylist_get(jeu->alEventsActuels, pos); }
SDL_Rect * getHitBoxMonstreTouche(jeu_t *jeu, int pos) { return arraylist_get(jeu->lesHitBoxDesMonstresTouches, pos); }

carte_t * getCarte2(jeu_t *jeu, const char *nom) {
	for (int i = 0; i < jeu->cartes->taille; ++i) {
		carte_t *carte = arraylist_get(jeu->cartes, i);
		if (strcmp(carte->nom, nom) == 0) return carte;
	}
	return NULL;
}

void jeu_updateOffSetHeros(jeu_t *jeu) {
	jeu->xOffSetHeros = jeu->hitBoxHerosEcran.x - jeu->heros->position.x;
	jeu->yOffSetHeros = jeu->hitBoxHerosEcran.y - jeu->heros->position.y;
}

void jeu_ajouterCarteVide(const char *nom, int largeur, int hauteur, chipset_t *chipset, musique_t *musique, jeu_t *jeu) {
	carte_t *carte = NULL;
	carte_result_t res = carte_creer(&carte, nom, largeur, hauteur, chipset, musique, false);
	if (res != CARTE_OK) { LOG_ERROR("Carte : %s", carte_strerror(res)); Exception(""); }
	arraylist_add(jeu->cartes, carte);
}

void musique_stopAndPlay(musique_t *musiqueActuelle, musique_t *musiqueSuivante) {
	if (musiqueActuelle) musique_stop(musiqueActuelle);
	if (musiqueSuivante) musique_play(musiqueSuivante);
}

void viderMessage(jeu_t *jeu) {
	memset(jeu->message[0], 0, TAILLE_MAX_MSG_REELLE);
	memset(jeu->messageCharNbOctets[0], 0, TAILLE_MAX_MSG);
	jeu->compteurLettres = jeu->compteurLettresReelles = 0;
}

void sauvegarderMessage(jeu_t *jeu) {
	strcpy(jeu->message[1], jeu->message[0]);
	copyIntArray(jeu->messageCharNbOctets[1], jeu->messageCharNbOctets[0], TAILLE_MAX_MSG);
}

void remettreDernierMessage(jeu_t *jeu) {
	strcpy(jeu->message[0], jeu->message[1]);
	copyIntArray(jeu->messageCharNbOctets[0], jeu->messageCharNbOctets[1], TAILLE_MAX_MSG);
	jeu->compteurLettresReelles = strlen(jeu->message[0]);
	int compteur = 0;
	while (compteur < TAILLE_MAX_MSG && jeu->messageCharNbOctets[0][compteur] != 0) compteur++;
	jeu->compteurLettres = compteur;
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

void afficherHitboxAttaqueEpee(SDL_Renderer *renderer, jeu_t *jeu) {
	dessinerRectangle(renderer ,&jeu->hitBoxEpeeHerosEcran[jeu->heros->direction], BLANC_TRANSPARENT);
}

static void dessinerFiolePVMorte(SDL_Renderer *renderer, jeu_t *jeu, int index, int hauteur) {
    SDL_Rect src = { 16, 21 + index * 72, 16, hauteur };
    SDL_Rect dst = { 0, 0, 16, hauteur };

    SDL_DestroyTexture(jeu->textureFiolePVMorte[index]);
    jeu->textureFiolePVMorte[index] = creerTextureVide(renderer, 16, 51);
    SDL_SetRenderTarget(renderer, jeu->textureFiolePVMorte[index]); // dessiner sur cette texture
    dessinerTexture(renderer, getAffichage(jeu, 0), &src, &dst, "Echec dessin fiole PV morte avec SDL_RenderCopy");
}

void updateFiolePV(SDL_Renderer *renderer, jeu_t *jeu) {
	const double ratioPV = heros_getRatioPV(jeu->heros);
	const int hauteur = maxInt(1, (int)(51 * (1 - ratioPV)));
	for (int i = 0; i < 3; ++i) dessinerFiolePVMorte(renderer, jeu, i, hauteur);
	SDL_SetRenderTarget(renderer, NULL); // redessine sur le renderer principal
}

static void dessinerFiolePMMorte(SDL_Renderer *renderer, jeu_t *jeu, int index, int hauteur) {
	SDL_Rect src = { 48, 21 + index * 72, 16, hauteur };
	SDL_Rect dst = { 0, 0, 16, hauteur };

	SDL_DestroyTexture(jeu->textureFiolePMMorte[index]);
	jeu->textureFiolePMMorte[index] = creerTextureVide(renderer, 16, 51);
	SDL_SetRenderTarget(renderer, jeu->textureFiolePMMorte[index]); // dessiner sur cette texture
	dessinerTexture(renderer, getAffichage(jeu, 0), &src, &dst, "Echec dessin fiole PM morte avec SDL_RenderCopy");
}

void updateFiolePM(SDL_Renderer *renderer, jeu_t *jeu) {
	const double ratioPM = heros_getRatioPM(jeu->heros);
	const int hauteur = maxInt(1, (int) (51 * (1 - ratioPM)));
	for (int i = 0; i < 3; ++i) dessinerFiolePMMorte(renderer, jeu, i, hauteur);
	SDL_SetRenderTarget(renderer, NULL); // redessine sur le renderer principal
}

void afficherFiolePV(SDL_Renderer *renderer, jeu_t *jeu) {
	dessinerTexture(renderer, getAffichage(jeu, 0), &jeu->srcRectFiolePV[0][jeu->fiolesTiming], &jeu->dstRectFiolePV[0], "Echec dessin fiole PV vivante avec SDL_RenderCopy");
	dessinerTexture(renderer, jeu->textureFiolePVMorte[jeu->fiolesTiming], NULL, &jeu->dstRectFiolePV[1], "Echec dessin fiole PV morte avec SDL_RenderCopy");
}

void afficherFiolePM(SDL_Renderer *renderer, jeu_t *jeu) {
	dessinerTexture(renderer, getAffichage(jeu, 0), &jeu->srcRectFiolePM[0][jeu->fiolesTiming], &jeu->dstRectFiolePM[0], "Echec dessin fiole PM vivante avec SDL_RenderCopy");
	dessinerTexture(renderer, jeu->textureFiolePMMorte[jeu->fiolesTiming], NULL, &jeu->dstRectFiolePM[1], "Echec dessin fiole PM morte avec SDL_RenderCopy");
}

void afficherBarreXP(SDL_Renderer *renderer, jeu_t *jeu) {
	dessinerTexture(renderer, getAffichage(jeu, 1), NULL, &jeu->dstRectBarreXP, "Echec dessin barre XP avec SDL_RenderCopy");
}

static void calculerBorneFenetre(const carte_t *carte, const jeu_t *jeu, int *x0, int *x1, int *y0, int *y1) {
	const int herosXCase = heros_getXCase(jeu->heros);
	const int herosYCase = heros_getYCase(jeu->heros);

	*x0 = maxInt(herosXCase - WINDOW_WIDTH_CASES_DIV2 - 1, 0);
	*x1 = minInt(herosXCase + WINDOW_WIDTH_CASES_DIV2 + 2, carte->largeur);
	*y0 = maxInt(herosYCase - WINDOW_HEIGHT_CASES_DIV2 - 1, 0);
	*y1 = minInt(herosYCase + WINDOW_HEIGHT_CASES_DIV2 + 2, carte->hauteur);
	//printf("x0 = %d, x1 = %d, y0 = %d, y1 = %d\n", x0, x1, y0, y1);
}

void afficherCouche(SDL_Renderer *renderer, carte_t *carte, int couche, jeu_t *jeu) {
	SDL_Rect dstRect = { 0, 0, TAILLE_CASES, TAILLE_CASES };
	int x0, x1, y0, y1;
	calculerBorneFenetre(carte, jeu, &x0, &x1, &y0, &y1);
	//SDL_GetWindowSurface(window);

	for (int i = y0; i < y1; ++i) {
		for (int j = x0; j < x1; ++j) {
			const int numTuile = carte->couches[couche][i][j];
			if (numTuile != -1) {
				dstRect.x = j * TAILLE_CASES + jeu->xOffSetHeros;
				dstRect.y = i * TAILLE_CASES + jeu->yOffSetHeros;

				dessinerTexture(renderer, carte->chipset->texture, &carte->chipset->tuiles[numTuile], &dstRect, "Echec dessin texture d'une tuile chipset sur une couche avec SDL_RenderCopy");
			}
		}
	}
}

void afficherMurs(SDL_Renderer *renderer, carte_t *carte, jeu_t *jeu) {
	SDL_Rect murRect = { 0, 0, TAILLE_CASES, TAILLE_CASES };
	int x0, x1, y0, y1;
	calculerBorneFenetre(carte, jeu, &x0, &x1, &y0, &y1);

	for (int i = y0; i < y1; ++i) {
		for (int j = x0; j < x1; ++j) {
			if (carte->murs[i][j]) {
				murRect.x = j * TAILLE_CASES + jeu->xOffSetHeros;
				murRect.y = i * TAILLE_CASES + jeu->yOffSetHeros;

				dessinerRectangle(renderer, &murRect, VIOLET_TRANSPARENT);
			}
		}
	}
}

void jeu_detruire(jeu_t *jeu) {
	if (!jeu) return;
	for (int i = 0; i < 3; ++i) {
		SDL_DestroyTexture(jeu->textureFiolePVMorte[i]);
		SDL_DestroyTexture(jeu->textureFiolePMMorte[i]);
	}
	heros_detruire(jeu->heros);
	arraylist_detruire(jeu->lesHitBoxDesMonstresTouches, false);
	arraylist_detruire(jeu->cartes, true);
	arraylist_detruire(jeu->chipsets, true);
	arraylist_detruire(jeu->bruitages, true);
	arraylist_detruire(jeu->musiques, true);
	arraylist_detruire(jeu->polices, true);
	arraylist_detruire(jeu->monstresData, true);
	arraylist_detruire(jeu->skins, true);
	arraylist_detruire(jeu->affichages, true);
	free(jeu);
}