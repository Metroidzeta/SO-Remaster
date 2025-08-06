//	@author Alain Barbier alias "Metroidzeta"
//
//	Pour compiler avec Windows :
//		> gcc -Wall src/*.c -o prog -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image
//	Pour compiler avec Windows (sans console) :
//		> gcc -Wall src/*.c -o prog -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image -mwindows
//	Pour exécuter: ./prog

#include "headers/jeu.h"
#include "headers/controles.h"

void afficherHitBoxJoueur(SDL_Renderer *renderer, jeu_t *jeu) {
	dessinerRectangle(renderer, &jeu->hitBoxHerosEcran, ROUGE);
}

void afficherMonstres(SDL_Renderer *renderer, jeu_t *jeu) {
	SDL_Rect dstRect = { 0, 0, TAILLE_CASES, TAILLE_CASES };
	for (int i = 0; i < jeu->heros->carteActuelle->monstres->taille; ++i) {
		monstre_t *monstre = arraylist_get(jeu->heros->carteActuelle->monstres, i);
		dstRect.x = monstre->position.x + jeu->xOffSetHeros;
		dstRect.y = monstre->position.y + jeu->yOffSetHeros;
		monstre_afficher(renderer, monstre, 28, &dstRect);
	}
}

void afficherCadreEtMessage(SDL_Renderer *renderer, const char *msg, TTF_Font *police, SDL_Rect cadre, SDL_Color couleurCadre) {
	if (!renderer || !msg || !police) return;
	dessinerRectangle(renderer, &cadre, couleurCadre);
	if (*msg) dessinerTexteLimite(renderer, msg, police, BLANC, cadre.x, cadre.y, cadre.w);
}

void afficherMessageEvent(SDL_Renderer *renderer, const char *message, jeu_t *jeu) {
	//                _________.x_________  _________.y_________  ________.w_________  ________.h_________
	SDL_Rect cadre = {WINDOW_WIDTH * 0.175, WINDOW_HEIGHT * 0.02, WINDOW_WIDTH * 0.65, WINDOW_HEIGHT * 0.2};
	afficherCadreEtMessage(renderer, message, getPolice(jeu, 1), cadre, jeu->couleurs_cadres[jeu->numCouleur_cadres]);
}

void afficherCadreEcriture(SDL_Renderer *renderer, jeu_t *jeu) {
	//                ________.x_________  ________.y__________  _________.w________  _________.h_________
	SDL_Rect cadre = {WINDOW_WIDTH * 0.03, WINDOW_HEIGHT * 0.95, WINDOW_WIDTH * 0.94, WINDOW_HEIGHT * 0.04};
	afficherCadreEtMessage(renderer, jeu->message[0], getPolice(jeu, 1), cadre, jeu->couleurs_cadres[jeu->numCouleur_cadres]);
}

void afficherCadreMessageTeteHeros(SDL_Renderer *renderer, jeu_t *jeu) {
	if (*jeu->message[1]) {
		int w, h;
		SDL_Texture *texture = creerTextureLimiteDepuisTexte(renderer, jeu->message[1], getPolice(jeu, 1), BLANC, 7 * TAILLE_CASES);
		if (!texture) Exception("Echec creation texture message sur la tete du heros");
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
		SDL_DestroyTexture(texture);
		SDL_Rect cadre = { jeu->hitBoxHerosEcran.x + TAILLE_CASES / 2 - w / 2, jeu->hitBoxHerosEcran.y - TAILLE_CASES / 4 - h, w, h };
		dessinerRectangle(renderer, &cadre, jeu->couleurs_cadres[jeu->numCouleur_cadres]);
		dessinerTexteLimite(renderer, jeu->message[1], getPolice(jeu, 1), BLANC, cadre.x, cadre.y, 7 * TAILLE_CASES);
	}
}

void afficherCadreMessageRecap(SDL_Renderer *renderer, jeu_t *jeu) {
	//                ________.x_________  _________.y_________  _______.w_________  _________.h_________
	SDL_Rect cadre = {WINDOW_WIDTH * 0.05, WINDOW_HEIGHT * 0.69, WINDOW_WIDTH * 0.9, WINDOW_HEIGHT * 0.25};
	dessinerRectangle(renderer, &cadre, jeu->couleurs_cadres[jeu->numCouleur_cadres]);

	int xTexte = cadre.x + WINDOW_WIDTH * 0.01;
	int yTexte = cadre.y + WINDOW_HEIGHT * 0.01;
	int yLigneOffset = WINDOW_HEIGHT * 0.08; // décalage pour sauter une ligne
	for (int i = 0; i < 3; ++i) {
		if (*jeu->recapMessages[i]) {
			size_t taille_max = strlen(jeu->heros->nom) + 3 + strlen(jeu->recapMessages[i]) + 1; // nom + " : " + message + '\0'
			char ligne[taille_max];
			snprintf(ligne, taille_max, "%s : %s", jeu->heros->nom, jeu->recapMessages[i]);
			dessinerTexteLimite(renderer, ligne, getPolice(jeu, 1), BLANC, xTexte, yTexte + i * yLigneOffset, cadre.w);
		}
	}
}

void afficherMenuNavigation(SDL_Renderer *renderer, jeu_t *jeu) {
	if (!renderer || !jeu) return;

	const int margeX = WINDOW_WIDTH * 0.01;
	const int margeY = WINDOW_HEIGHT * 0.01;
	const int largeur = WINDOW_WIDTH * 0.15;
	const int hauteur = WINDOW_HEIGHT * 0.26;
	const int xCadre = margeX;
	const int yCadre = WINDOW_HEIGHT * 0.37;

	SDL_Rect cadre = { xCadre, yCadre, largeur, hauteur };
	dessinerRectangle(renderer, &cadre, jeu->couleurs_cadres[jeu->numCouleur_cadres]);

	const int xTexte = cadre.x + margeX;
	const int yTexte = cadre.y + margeY;
	const int yLigneOffset = WINDOW_HEIGHT * 0.05; // décalage pour sauter une ligne

	const char * const optionsMenu[5] = { "Inventaire", "Magie", "Statistiques", "Echanger", "Quitter" };

	TTF_Font *police = getPolice(jeu, 1);
	for (int i = 0; i < 5; ++i) dessinerTexte(renderer, optionsMenu[i], police, BLANC, xTexte, yTexte + i * yLigneOffset);
}

void afficherMenuStatistiques(SDL_Renderer *renderer, jeu_t *jeu) {
	if (!renderer || !jeu) return;

	const int margeX = WINDOW_WIDTH * 0.01;
	const int margeY = WINDOW_HEIGHT * 0.01;
	const int largeurCadre = WINDOW_WIDTH * 0.79;
	const int hauteurCadre = WINDOW_HEIGHT * 0.95;
	const int xCadre = WINDOW_WIDTH * 0.17;
	const int yCadre = margeY;

	SDL_Rect cadre = { xCadre, yCadre, largeurCadre, hauteurCadre };
	dessinerRectangle(renderer, &cadre, jeu->couleurs_cadres[jeu->numCouleur_cadres]);

	const int xTexte = cadre.x + margeX;
	const int yTexte = cadre.y + margeY;
	const int yLigneOffset = WINDOW_HEIGHT * 0.05; // décalage pour sauter une ligne

	const int nbLignes = 10;
	char lignes[nbLignes][100];  // Tableau de lignes
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
		if (*lignes[i]) dessinerTexte(renderer, lignes[i], police, BLANC, xTexte, yTexte + i * yLigneOffset);
	}
}

void afficherDegats(SDL_Renderer *renderer, int nombre, SDL_Color couleur, double frame, jeu_t *jeu) {
	for (int i = 0; i < jeu->lesHitBoxDesMonstresTouches->taille; ++i) {
		SDL_Rect *hitBox = getHitBoxMonstreTouche(jeu, i);
		dessinerNombre(renderer, nombre, getPolice(jeu, 2), couleur, hitBox->x + jeu->xOffSetHeros, hitBox->y + jeu->yOffSetHeros - TAILLE_CASES / 2 - (WINDOW_HEIGHT * 0.2 * frame));
	}
}

void afficherAlignement(SDL_Renderer *renderer, jeu_t *jeu) {
	sprintf(jeu->str_alignement, "Align : %.d", jeu->heros->alignement);
	dessinerTexte(renderer, jeu->str_alignement, getPolice(jeu, 1), BLANC, WINDOW_WIDTH * 0.84, WINDOW_HEIGHT * 0.02);
}

void afficherFPS_Fenetre(SDL_Window *window, jeu_t *jeu) {
	snprintf(jeu->str_FPS_Fenetre, sizeof(jeu->str_FPS_Fenetre), "%s | FPS : %.2lf", TITRE_FENETRE, jeu->FPS_result);
	SDL_SetWindowTitle(window, jeu->str_FPS_Fenetre);
}

void afficherFPS_EnJeu(SDL_Renderer *renderer, jeu_t *jeu) {
	sprintf(jeu->str_FPS_EnJeu, "FPS : %.2lf", jeu->FPS_result);
	dessinerTexte(renderer, jeu->str_FPS_EnJeu, getPolice(jeu, 0), VERT, WINDOW_WIDTH * 0.04, WINDOW_HEIGHT * 0.03);
}

static void changerMusique(musique_t *musique, jeu_t *jeu) {
	if (jeu->musiqueActuelle != musique) {
		musique_stopAndPlay(jeu->musiqueActuelle, musique);
		jeu->musiqueActuelle = musique;
	}
}

static void faireEvent_MSG(jeu_t *jeu) {
	jeu->heros->ecritMessage = false;
	viderMessage(jeu);
}

static void faireEvent_TP(event_tp_t *e_tp, jeu_t *jeu) {
	jeu->degatsAffiches = 0;
	carte_t *carteDst = e_tp->carteDst;
	changerMusique(carteDst->musique, jeu);
	jeu->heros->carteActuelle = carteDst;
	heros_modifierPosition(jeu->heros, e_tp->xDst, e_tp->yDst);
	printf("Teleportation dans la carte %s : (%d,%d)\n", carteDst->nom, e_tp->xDst / TAILLE_CASES, e_tp->yDst / TAILLE_CASES);
}

static void faireEvent_JM(event_jm_t *e_jm, jeu_t *jeu) {
	changerMusique(e_jm->musique, jeu);
}

static void faireEvent_AM(jeu_t *jeu) {
	if (jeu->musiqueActuelle) musique_stop(jeu->musiqueActuelle);
}

static void faireEvent_CPV(event_changePV_t *e_cPV, SDL_Renderer *renderer, jeu_t *jeu) {
	heros_modifierPV(jeu->heros, e_cPV->PV);
	updateFiolePV(renderer, jeu);
}

static void faireEvent_CPM(event_changePM_t *e_cPM, SDL_Renderer *renderer, jeu_t *jeu) {
	heros_modifierPM(jeu->heros, e_cPM->PM);
	updateFiolePM(renderer, jeu);
}

void faireEvent(SDL_Renderer *renderer, event_t *e, jeu_t *jeu) {
	if (e) {
		if (!e->ptr) return;
		jeu->heros->bloqueTotal = true;
		jeu->heros->eventEnCours = true;
		switch (e->type) {
			case E_MSG: faireEvent_MSG(jeu); break; // message du jeu pour le héros
			case E_TP: faireEvent_TP((event_tp_t *) e->ptr, jeu); break; // téléportation
			case E_JM: faireEvent_JM((event_jm_t *) e->ptr, jeu); break; // jouer une musique
			case E_AM: faireEvent_AM(jeu); break; // arrêt de la musique
			case E_CPV: faireEvent_CPV((event_changePV_t *) e->ptr, renderer, jeu); break; // changer PV du héros (- ou +)
			case E_CPM: faireEvent_CPM((event_changePM_t *) e->ptr, renderer, jeu); break; // changer PM du héros (- ou +)
		}
		if (e->type != E_MSG) {
			jeu->nbEventPass++;
			jeu->heros->eventEnCours = false;
		}
	}
}

void updateUPS(SDL_Window *window, SDL_Renderer *renderer, controles_t *controles, SDL_Event *event, jeu_t *jeu, Uint32 now, int *lastAttackCooldown) {
	while (SDL_PollEvent(event)) controles_detection(event, controles, jeu);

	bool flechesAppuye = (controles->HAUT || controles->BAS || controles->GAUCHE || controles->DROITE);
	if (flechesAppuye && !jeu->heros->bloqueTotal) {
		if(controles->HAUT && !controles->BAS) heros_deplacer(jeu->heros, HAUT);
		if (controles->BAS && !controles->HAUT) heros_deplacer(jeu->heros, BAS);
		if (controles->GAUCHE && !controles->DROITE) heros_deplacer(jeu->heros, GAUCHE);
		if (controles->DROITE && !controles->GAUCHE) heros_deplacer(jeu->heros, DROITE);
	} else {
		jeu->heros->frameDeplacement = 7;
	}
	if (flechesAppuye && !jeu->heros->bloqueTotal && !jeu->alEventsActuels) {
		jeu->alEventsActuels = carte_verifierCollisionsEvents(jeu->heros->carteActuelle, &jeu->heros->hitBox);
	}
	if (jeu->alEventsActuels && !jeu->heros->eventEnCours) {
		event_t *e = getEventActuel(jeu, jeu->nbEventPass);
		faireEvent(renderer, e, jeu);
	}

	if (!jeu->heros->ecritMessage) {
		if (controles->A) {
			jeu->mursVisibles = !jeu->mursVisibles;
			printf("Vous avez %s l'affichage des murs\n", jeu->mursVisibles ? "active" : "desactive");
			controles->A = false;
		}

		if (controles->B) {
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
			controles->B = false;
		}

		if (controles->Q) {
			jeu->enCours = false;
			controles->Q = false;
		}

		if (!jeu->heros->bloqueTotal) {
			if (controles->S) {
				if (jeu->heros->peutAttaquer) {
					bruitage_play(getBruitage(jeu, 0));
					heros_updateHitBoxEpee(jeu->heros);
					SDL_Rect *hitBoxEpee = &jeu->heros->hitBoxEpee[jeu->heros->direction];
					bool premier = true;
					double resultat_aleatoire = 0;
					for (int i = 0; i < jeu->heros->carteActuelle->monstres->taille; ++i) {
						monstre_t *monstre = arraylist_get(jeu->heros->carteActuelle->monstres, i);
						if (SDL_HasIntersection(hitBoxEpee, &monstre->hitBox)) {
							if (premier) {
								resultat_aleatoire = (double) rand() / RAND_MAX;
								//printf("resultat_aleatoire = %f > tauxCrit = %f\n", resultat_aleatoire, jeu->heros->tauxCrit);
								arraylist_clear(jeu->lesHitBoxDesMonstresTouches, false);
								premier = false;
							}
							if (resultat_aleatoire > jeu->heros->tauxCrit) {
								printf("Coup normal sur le monstre\n");
								bruitage_play(getBruitage(jeu,1));
								jeu->estCoupCritique = false;
							} else {
								printf("Coup critique! sur le monstre\n");
								bruitage_play(getBruitage(jeu, 2));
								jeu->estCoupCritique = true;
							}
							arraylist_add(jeu->lesHitBoxDesMonstresTouches, &monstre->hitBox);
							jeu->degatsAffiches = 1;
						}
					}
					jeu->heros->attaqueEpee = true;
					jeu->heros->peutAttaquer = false;
					*lastAttackCooldown = now;
				}
				controles->S = false;
			}
		}
	}

	if (controles->RETOUR_ARRIERE) {
		if (jeu->heros->ecritMessage && jeu->compteurLettres > 0) {
			const int nbOctets = jeu->messageCharNbOctets[0][jeu->compteurLettres - 1];
			for (int i = 0; i < nbOctets; ++i) jeu->message[0][--(jeu->compteurLettresReelles)] = 0;
			jeu->messageCharNbOctets[0][jeu->compteurLettres - 1] = 0;
			jeu->compteurLettres--;
		}
		controles->RETOUR_ARRIERE = false;
	}

	if (controles->ENTREE) {
		if (!jeu->heros->bloqueTotal) {
			if (!jeu->heros->ecritMessage) {
				jeu->heros->ecritMessage = true;
			} else {
				jeu->heros->ecritMessage = false;		
				if (*jeu->message[0]) {
					jeu->delaiMessage = 0;
					jeu->heros->messageTete = true;
					printf("%s : %s\n",jeu->heros->nom, jeu->message[0]);
					ajouterMessageHistorique(jeu);
					sauvegarderMessage(jeu);
					viderMessage(jeu);
				}
			}
		}
		controles->ENTREE = false;
	}

	if (controles->ESPACE) {
		if (jeu->heros->eventEnCours && getEventActuel(jeu, jeu->nbEventPass)->type == E_MSG) {
			jeu->nbEventPass++;
			jeu->heros->eventEnCours = false;
		}
		controles->ESPACE = false;
	}

	if (controles->ECHAP) {
		if (!jeu->alEventsActuels) {
			jeu->afficherRecap = 0;
			jeu->menuVisible = !jeu->menuVisible;
			if (jeu->menuVisible) jeu->heros->frameDeplacement = 7;
			jeu->heros->bloqueTotal = !jeu->heros->bloqueTotal;
			controles->ECHAP = false;
		}
	}

	if (!jeu->heros->bloqueTotal) {
		if (controles->F1) {
			jeu->afficherRecap = (jeu->afficherRecap + 1) % 2; // % 3 dans le futur
			controles->F1 = false;
		}
		if (controles->F3) {
			if (!jeu->heros->ecritMessage) jeu->heros->ecritMessage = true;
			viderMessage(jeu);
			remettreDernierMessage(jeu);
			controles->F3 = false;
		}
	}

	if (controles->F5) {
		jeu->numCouleur_cadres = (jeu->numCouleur_cadres + 1) % 5;
		controles->F5 = false;
	}

	if (jeu->alEventsActuels && jeu->nbEventPass == jeu->alEventsActuels->taille) {
		jeu->nbEventPass = 0;
		jeu->alEventsActuels = NULL;
		jeu->heros->bloqueTotal = false;
	}
}

void updateFPS(SDL_Renderer *renderer, jeu_t *jeu) {
	effacerEcran(renderer);

	afficherCouche(renderer, jeu->heros->carteActuelle, 0, jeu); // afficher couche 0 tuiles chipset
	afficherCouche(renderer, jeu->heros->carteActuelle, 1, jeu); // afficher couche 1 tuiles chipset
	if (jeu->mursVisibles) afficherMurs(renderer, jeu->heros->carteActuelle, jeu); // afficher murs
	heros_afficherNom(renderer, jeu->heros, jeu->rectPseudo); // afficher nom du héros
	afficherMonstres(renderer, jeu);
	heros_afficherSkin(renderer, jeu->heros, &jeu->hitBoxHerosEcran); // afficher sprite du héros
	afficherCouche(renderer, jeu->heros->carteActuelle, 2, jeu); // afficher couche 2 tuiles chipset

	if (jeu->degatsAffiches > 0) {
		if (jeu->estCoupCritique) afficherDegats(renderer, 64, ROUGE, (double) jeu->degatsAffiches / FPS, jeu);
		else afficherDegats(renderer, 32, BLANC, (double) jeu->degatsAffiches / FPS, jeu);
		jeu->degatsAffiches++;
		if (jeu->degatsAffiches >= FPS) jeu->degatsAffiches = 0;
	}

	if (jeu->heros->attaqueEpee) { // si notre héros est en train d'attaquer
		afficherHitboxAttaqueEpee(renderer, jeu);
		jeu->heros->attaqueEpee = false;
	}

	afficherFiolePV(renderer, jeu);
	afficherFiolePM(renderer, jeu);
	afficherBarreXP(renderer, jeu);

	if (jeu->heros->ecritMessage) afficherCadreEcriture(renderer, jeu);
	if (jeu->heros->messageTete) afficherCadreMessageTeteHeros(renderer, jeu);
	if (jeu->afficherRecap == 1) afficherCadreMessageRecap(renderer, jeu);
	if (jeu->heros->eventEnCours && getEventActuel(jeu, jeu->nbEventPass)->type == E_MSG) {
		event_msg_t *e_msg = (event_msg_t *) getEventActuel(jeu, jeu->nbEventPass)->ptr;
		afficherMessageEvent(renderer, e_msg->msg, jeu);
	}

	if (jeu->menuVisible) {
		afficherMenuNavigation(renderer, jeu);
		afficherMenuStatistiques(renderer, jeu);
	}

	afficherAlignement(renderer, jeu);
	afficherFPS_EnJeu(renderer, jeu); // afficher FPS dans le jeu

	SDL_RenderPresent(renderer); // mettre à jour l'écran (afficher le rendu de la frame)
	jeu->frames++;
}

void updateCooldowns(jeu_t *jeu, Uint32 now, int *lastAttackCooldown) {
	if (!jeu->heros->peutAttaquer && (now - *lastAttackCooldown) > jeu->deuxTiersSeconde) jeu->heros->peutAttaquer = true;
}

void updateTimers(SDL_Window *window, jeu_t *jeu, Uint32 now, Uint32 *lastFiolesTime, Uint32 *lastSecond, Uint32 *lastMinute, int *lastFrame) {
	if (now - *lastFiolesTime >= DELAI_FIOLES_MS) {
		jeu->fiolesTiming = (jeu->fiolesTiming + 1) % 3;
		*lastFiolesTime = now;
	}

	if (now - *lastSecond >= DELAI_SECONDE) { // chaque seconde
		if (jeu->heros->messageTete) { // si il y a déjà un message sur la tête de notre héros
			if (++jeu->delaiMessage == 6) {
				jeu->delaiMessage = 0;
				jeu->heros->messageTete = false;
			}
		}
		int framesEcoulees = jeu->frames - *lastFrame;
		double elapsedSec = (now - *lastSecond) / DELAI_SECONDE;
		jeu->FPS_result = framesEcoulees / elapsedSec;
		//printf("FPS = %.2lf\n", jeu->FPS_result);
		afficherFPS_Fenetre(window, jeu); // afficher FPS dans le titre de la fenetre
		*lastFrame = jeu->frames;
		*lastSecond = now;
	}

	if (now - *lastMinute >= DELAI_MINUTE) { // chaque minute
		heros_modifierAlignement(jeu->heros, 1);
		*lastMinute = now;
	}
}

int main(int argc, char *argv[]) {
	SDL_Window * window = NULL;
	SDL_Renderer * renderer = NULL;

	srand(time(NULL)); // init graine aléatoire
	controles_t controles = controles_init(); // init des contrôles (touches)
	initSDL(&window, &renderer, TITRE_FENETRE);

	jeu_t *jeu = jeu_creer(renderer);
	SDL_Event event;

	const double millisPerRender = 1e3 / FPS;
	const double millisPerTick = 1e3 / UPS;

	Uint32 now = SDL_GetTicks();
	double nextRender = (double) now + millisPerRender;
	double nextTick = (double) now + millisPerTick;

	int lastFrame = 0;
	int lastAttackCooldown = 0;

	Uint32 lastFiolesTime = now, lastMinute = now, lastSecond = now;
	Uint32 afterTime;

	musique_play(jeu->musiqueActuelle);
	jeu->enCours = true;

	while (jeu->enCours) {
		now = SDL_GetTicks();
		jeu_updateOffSetHeros(jeu);

		updateCooldowns(jeu, now, &lastAttackCooldown);
		updateTimers(window, jeu, now, &lastFiolesTime, &lastSecond, &lastMinute, &lastFrame);

		if (now >= nextTick) {
			updateUPS(window, renderer, &controles, &event, jeu, now, &lastAttackCooldown);
			nextTick += millisPerTick;
		}

		if (now >= nextRender) {
			updateFPS(renderer, jeu);
			nextRender += millisPerRender;
		}

		afterTime = SDL_GetTicks();
		int sleepTime = minDouble(nextTick - (double)afterTime, nextRender - (double)afterTime);
		if (sleepTime > 0) {
			SDL_Delay(sleepTime);
		} else { // en retard : on resynchronise proprement pour éviter le décalage
			while (now > nextRender) nextRender += millisPerRender;
			while (now > nextTick) nextTick += millisPerTick;
		}
	}
	jeu_detruire(jeu);
	freeSDL(window, renderer);

	return EXIT_SUCCESS;
}