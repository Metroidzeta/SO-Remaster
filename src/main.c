//	@author Alain Barbier alias "Metroidzeta"
//
//	Pour compiler avec Windows :
//		> gcc -Wall src/*.c -o prog -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image
//	Pour compiler avec Windows (sans console) :
//		> gcc -Wall src/*.c -o prog -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image -mwindows
//	Pour compiler avec Linux :
//		> gcc -o prog src/*.c `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2main
//	Pour exécuter: ./prog

#include "jeu.h"
#include "touches.h"

void afficherHitBoxJoueur(SDL_Renderer *renderer, jeu_t *jeu) {
	dessinerRectangle(renderer, &jeu->hitBoxJoueurEcran, ROUGE);
}

void afficherLesMonstres(SDL_Renderer *renderer, jeu_t *jeu) {
	SDL_Rect dstRect = { 0, 0, TAILLE_CASES, TAILLE_CASES };
	for (int i = 0; i < jeu->joueur->carteActuelle->lesMonstres->taille; ++i) {
		monstre_t *monstre = arraylist_get(jeu->joueur->carteActuelle->lesMonstres, i);
		dstRect.x = monstre->position.x + jeu->xOffSetJoueur;
		dstRect.y = monstre->position.y + jeu->yOffSetJoueur;
		monstre_afficher(renderer, monstre, 28, &dstRect);
	}
}

void afficherCadreAvecMessage(SDL_Renderer *renderer, const char *message, TTF_Font *police, SDL_Rect cadre, SDL_Color couleurCadre) {
	if (!renderer || !message || !police) return;
	dessinerRectangle(renderer,&cadre,couleurCadre);
	if (*message) dessinerTexteLimite(renderer, message, police, BLANC, cadre.x, cadre.y, cadre.w);
}

void afficherMessageEvent(SDL_Renderer *renderer, const char *message, jeu_t *jeu) {
	//                _________.x_________  _________.y_________  ________.w_________  ________.h_________
	SDL_Rect cadre = {WINDOW_WIDTH * 0.175, WINDOW_HEIGHT * 0.02, WINDOW_WIDTH * 0.65, WINDOW_HEIGHT * 0.2};
	afficherCadreAvecMessage(renderer, message, getPolice(jeu, 1), cadre, jeu->couleurs_cadres[jeu->numCouleur_cadres]);
}

void afficherCadreEcriture(SDL_Renderer *renderer, jeu_t *jeu) {
	//                ________.x_________  ________.y__________  _________.w________  _________.h_________
	SDL_Rect cadre = {WINDOW_WIDTH * 0.03, WINDOW_HEIGHT * 0.95, WINDOW_WIDTH * 0.94, WINDOW_HEIGHT * 0.04};
	afficherCadreAvecMessage(renderer, jeu->message[0], getPolice(jeu, 1), cadre, jeu->couleurs_cadres[jeu->numCouleur_cadres]);
}

void afficherCadreMessageTeteJoueur(SDL_Renderer *renderer, jeu_t *jeu) {
	if (jeu->message[1][0] != '\0') {
		int w, h;
		SDL_Texture * texture = creerTextureLimiteDepuisTexte(renderer, jeu->message[1], getPolice(jeu, 1), BLANC, 7 * TAILLE_CASES);
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
		SDL_DestroyTexture(texture);
		SDL_Rect cadre = { jeu->hitBoxJoueurEcran.x + TAILLE_CASES / 2 - w / 2, jeu->hitBoxJoueurEcran.y - TAILLE_CASES / 4 - h, w, h };
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
		if (jeu->recapMessages[i][0] != '\0') {
			char ligne[strlen(jeu->joueur->nom) + 3 + strlen(jeu->recapMessages[i]) + 1]; // pseudo + " : " + message + "/0"
			sprintf(ligne,"%s : %s",jeu->joueur->nom,jeu->recapMessages[i]);
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
	snprintf(lignes[0], sizeof(lignes[0]), "Nom : %s", jeu->joueur->nom);
	snprintf(lignes[1], sizeof(lignes[1]), "Classe : %s", joueur_getClasseToString(jeu->joueur));
	snprintf(lignes[2], sizeof(lignes[2]), "Niveau : %d", jeu->joueur->niveau);
	snprintf(lignes[3], sizeof(lignes[3]), "Or : %d", jeu->joueur->piecesOr);
	lignes[4][0] = '\0';
	snprintf(lignes[5], sizeof(lignes[5]), "Force : %-3d %65s PV : %4d / %4d", jeu->joueur->force, "", jeu->joueur->PV[0], jeu->joueur->PV[1]);
	snprintf(lignes[6], sizeof(lignes[6]), "Dextérité : %-3d %61s PM : %4d / %4d", jeu->joueur->dexterite, "", jeu->joueur->PM[0], jeu->joueur->PM[1]);
	snprintf(lignes[7], sizeof(lignes[7]), "Constitution : %-3d", jeu->joueur->constitution);
	lignes[8][0] = '\0';
	snprintf(lignes[9], sizeof(lignes[9]), "Taux Coups Critiques : %.1f %%", jeu->joueur->tauxCrit * 100);

	TTF_Font *police = getPolice(jeu, 1);
	for (int i = 0; i < nbLignes; ++i) {
		if (*lignes[i]) dessinerTexte(renderer, lignes[i], police, BLANC, xTexte, yTexte + i * yLigneOffset);
	}
}

void afficherDegats(SDL_Renderer *renderer, int nombre, SDL_Color couleur, double frame, jeu_t *jeu) {
	for (int i = 0; i < jeu->lesHitBoxDesMonstresTouches->taille; ++i) {
		SDL_Rect * hitBox = getHitBoxMonstreTouche(jeu, i);
		dessinerNombre(renderer, nombre, getPolice(jeu,2), couleur, hitBox->x + jeu->xOffSetJoueur, hitBox->y + jeu->yOffSetJoueur - TAILLE_CASES / 2 - (WINDOW_HEIGHT * 0.2 * frame));
	}
}

void afficherAlignement(SDL_Renderer *renderer, jeu_t *jeu) {
	sprintf(jeu->str_alignement, "Align : %.d", jeu->joueur->alignement);
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

void faireEvent(SDL_Renderer *renderer, event_t *e, jeu_t *jeu) {
	if (e) {
		jeu->joueur->bloqueTotal = true;
		jeu->joueur->eventEnCours = true;
		switch(e->type) {
			case E_MESSAGE: // message du jeu
				jeu->joueur->ecritMessage = false;
				viderMessage(jeu);
				break;
			case E_TP: // téléportation
				jeu->degatsAffiches = 0;
				event_tp_t *e_tel = (event_tp_t*) e->ptr;
				carte_t *carteDst = e_tel->carteDst;
				if (jeu->musiqueActuelle != carteDst->musique) {
					musique_stopAndPlay(jeu->musiqueActuelle, carteDst->musique);
					jeu->musiqueActuelle = carteDst->musique;
				}
				jeu->joueur->carteActuelle = carteDst;
				joueur_modifierPosition(jeu->joueur, e_tel->xDst, e_tel->yDst);
				printf("Teleportation! (%d,%d)\n", e_tel->xDst / TAILLE_CASES, e_tel->yDst / TAILLE_CASES);
				jeu->nbEventPass++;
				jeu->joueur->eventEnCours = false;
				break;
			case E_JOUER_MUSIQUE: // lire musique
				jeu->joueur->eventEnCours = true; // A ENLEVER !!
				event_jm_t * e_jm = (event_jm_t*) e->ptr; // On le cast en event_jm
				if (jeu->musiqueActuelle != e_jm->musique) {
					musique_stopAndPlay(jeu->musiqueActuelle, e_jm->musique);
					jeu->musiqueActuelle = e_jm->musique;
				}
				jeu->nbEventPass++;
				jeu->joueur->eventEnCours = false;
				break;
			case E_ARRETER_MUSIQUE: // arrêt musique
				if(jeu->musiqueActuelle != NULL) {
					musique_stop(jeu->musiqueActuelle);
				}
				jeu->nbEventPass++;
				jeu->joueur->eventEnCours = false;
				break;
			case E_CHANGE_PV: // modifier PV joueur
				jeu->joueur->eventEnCours = true; // A ENLEVER !!
				event_changePV_t * e_cPV = (event_changePV_t*) e->ptr; // On le cast en event_changePV
				joueur_modifierPV(jeu->joueur, e_cPV->PV);
				updateFiolePV(renderer, jeu);
				jeu->nbEventPass++;
				jeu->joueur->eventEnCours = false;
				break;
			case E_CHANGE_PM: // modifier PM joueur
				jeu->joueur->eventEnCours = true; // A ENLEVER !!
				event_changePM_t * e_cPM = (event_changePM_t*) e->ptr; // On le cast en event_changePM
				joueur_modifierPM(jeu->joueur, e_cPM->PM);
				updateFiolePM(renderer, jeu);
				jeu->nbEventPass++;
				jeu->joueur->eventEnCours = false;
				break;
		}
	}
}

void updateUPS(SDL_Window *window, SDL_Renderer *renderer, touches_t *touches, SDL_Event *event, jeu_t *jeu, Uint32 now, int *lastAttackCooldown) {
	while (SDL_PollEvent(event)) {
		touches_detection(event, touches, jeu);
	}

	bool flechesAppuye = (touches->HAUT || touches->BAS || touches->GAUCHE || touches->DROITE);
	if (flechesAppuye && !jeu->joueur->bloqueTotal) {
		if(touches->HAUT && !touches->BAS) joueur_deplacer(jeu->joueur, HAUT);
		if (touches->BAS && !touches->HAUT) joueur_deplacer(jeu->joueur, BAS);
		if (touches->GAUCHE && !touches->DROITE) joueur_deplacer(jeu->joueur, GAUCHE);
		if (touches->DROITE && !touches->GAUCHE) joueur_deplacer(jeu->joueur, DROITE);
	} else {
		jeu->joueur->frameDeplacement = 7;
	}
	if (flechesAppuye && !jeu->joueur->bloqueTotal && !jeu->alEventsActuels) {
		jeu->alEventsActuels = carte_verifierLesCollisionsEvents(jeu->joueur->carteActuelle, &jeu->joueur->hitBox);
	}
	if (jeu->alEventsActuels && !jeu->joueur->eventEnCours) {
		event_t *e = getEventActuel(jeu, jeu->nbEventPass);
		faireEvent(renderer, e, jeu);
	}

	if (!jeu->joueur->ecritMessage) {
		if (touches->A) {
			jeu->mursVisibles = !jeu->mursVisibles;
			printf("Vous avez %s l'affichage des murs\n", jeu->mursVisibles ? "active" : "desactive");
			touches->A = false;
		}

		if (touches->B) {
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
			touches->B = false;
		}

		if (touches->Q) {
			jeu->enCours = false;
			touches->Q = false;
		}

		if (!jeu->joueur->bloqueTotal) {
			if (touches->S) {
				if (jeu->joueur->peutAttaquer) {
					bruitage_play(getBruitage(jeu, 0));
					joueur_updateHitBoxEpee(jeu->joueur);
					SDL_Rect * hitBoxEpee = &jeu->joueur->hitBoxEpee[jeu->joueur->direction];
					bool premier = true;
					double resultat_aleatoire = 0;
					for (int i = 0; i < jeu->joueur->carteActuelle->lesMonstres->taille; ++i) {
						monstre_t * monstre = arraylist_get(jeu->joueur->carteActuelle->lesMonstres, i);
						if (SDL_HasIntersection(hitBoxEpee, &monstre->hitBox)) {
							if (premier) {
								resultat_aleatoire = (double) rand() / RAND_MAX;
								//printf("resultat_aleatoire = %f > tauxCrit = %f\n", resultat_aleatoire, jeu->joueur->tauxCrit);
								arraylist_clear(jeu->lesHitBoxDesMonstresTouches, false);
								premier = false;
							}
							if (resultat_aleatoire > jeu->joueur->tauxCrit) {
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
					jeu->joueur->attaqueEpee = true;
					jeu->joueur->peutAttaquer = false;
					*lastAttackCooldown = now;
				}
				touches->S = false;
			}
		}
	}

	if (touches->RETOUR_ARRIERE) {
		if (jeu->joueur->ecritMessage && jeu->compteurLettres > 0) {
			const int nbOctets = jeu->messageCharNbOctets[0][jeu->compteurLettres - 1];
			for (int i = 0; i < nbOctets; i++) jeu->message[0][--(jeu->compteurLettresReelles)] = 0;
			jeu->messageCharNbOctets[0][jeu->compteurLettres - 1] = 0;
			jeu->compteurLettres--;
		}
		touches->RETOUR_ARRIERE = false;
	}

	if (touches->ENTREE) {
		if (!jeu->joueur->bloqueTotal) {
			if (!jeu->joueur->ecritMessage) {
				jeu->joueur->ecritMessage = true;
			} else {
				jeu->joueur->ecritMessage = false;		
				if (jeu->message[0][0] != '\0') {
					jeu->delaiMessage = 0;
					jeu->joueur->messageTete = true;
					printf("%s : %s\n",jeu->joueur->nom, jeu->message[0]);
					ajouterMessageHistorique(jeu);
					sauvegarderMessage(jeu);
					viderMessage(jeu);
				}
			}
		}
		touches->ENTREE = false;
	}

	if (touches->ESPACE) {
		if (jeu->joueur->eventEnCours && getEventActuel(jeu, jeu->nbEventPass)->type == E_MESSAGE) {
			jeu->nbEventPass++;
			jeu->joueur->eventEnCours = false;
		}
		touches->ESPACE = false;
	}

	if (touches->ECHAP) {
		if (!jeu->alEventsActuels) {
			jeu->afficherRecap = 0;
			jeu->menuVisible = !jeu->menuVisible;
			if (jeu->menuVisible) jeu->joueur->frameDeplacement = 7;
			jeu->joueur->bloqueTotal = !jeu->joueur->bloqueTotal;
			touches->ECHAP = false;
		}
	}

	if (!jeu->joueur->bloqueTotal) {
		if (touches->F1) {
			jeu->afficherRecap = (jeu->afficherRecap + 1) % 2; // % 3 dans le futur
			touches->F1 = false;
		}
		if (touches->F3) {
			if (!jeu->joueur->ecritMessage) jeu->joueur->ecritMessage = true;
			viderMessage(jeu);
			remettreDernierMessage(jeu);
			touches->F3 = false;
		}
	}

	if (touches->F5) {
		jeu->numCouleur_cadres = (jeu->numCouleur_cadres + 1) % 5;
		touches->F5 = false;
	}

	if (jeu->alEventsActuels && jeu->nbEventPass == jeu->alEventsActuels->taille) {
		jeu->nbEventPass = 0;
		jeu->alEventsActuels = NULL;
		jeu->joueur->bloqueTotal = false;
	}
}

void updateFPS(SDL_Renderer *renderer, jeu_t *jeu) {
	effacerEcran(renderer);

	afficherCouche(renderer, jeu->joueur->carteActuelle, 0, jeu); // couche 0 chipset
	afficherCouche(renderer, jeu->joueur->carteActuelle, 1, jeu); // couche 1 chipset
	if (jeu->mursVisibles) afficherMurs(renderer, jeu->joueur->carteActuelle, jeu); // afficher murs
	joueur_afficherNom(renderer, jeu->joueur, jeu->rectPseudo); // affiche le nom de notre joueur
	afficherLesMonstres(renderer, jeu);
	joueur_afficherSkin(renderer, jeu->joueur, &jeu->hitBoxJoueurEcran); // affiche le sprite de notre joueur
	afficherCouche(renderer, jeu->joueur->carteActuelle, 2, jeu); // couche 2 du chipset

	if (jeu->degatsAffiches > 0) {
		if (jeu->estCoupCritique) afficherDegats(renderer, 64, ROUGE, (double) jeu->degatsAffiches / FPS, jeu);
		else afficherDegats(renderer, 32, BLANC, (double) jeu->degatsAffiches / FPS, jeu);
		jeu->degatsAffiches++;
		if (jeu->degatsAffiches >= FPS) jeu->degatsAffiches = 0;
	}

	if (jeu->joueur->attaqueEpee) { // Si notre joueur est en train d'attaquer
		afficherHitboxAttaqueEpee(renderer, jeu);
		jeu->joueur->attaqueEpee = false;
	}

	afficherFiolePV(renderer, jeu);
	afficherFiolePM(renderer, jeu);
	afficherBarreXP(renderer, jeu);

	if (jeu->joueur->ecritMessage) afficherCadreEcriture(renderer, jeu);
	if (jeu->joueur->messageTete) afficherCadreMessageTeteJoueur(renderer, jeu);
	if (jeu->afficherRecap == 1) afficherCadreMessageRecap(renderer, jeu);
	if (jeu->joueur->eventEnCours && getEventActuel(jeu, jeu->nbEventPass)->type == E_MESSAGE) {
		event_msg_t *e_msg = (event_msg_t *) getEventActuel(jeu, jeu->nbEventPass)->ptr; // Cast en event_msg
		afficherMessageEvent(renderer, e_msg->msg, jeu);
	}

	if (jeu->menuVisible) {
		afficherMenuNavigation(renderer, jeu);
		afficherMenuStatistiques(renderer, jeu);
	}

	afficherAlignement(renderer, jeu);
	afficherFPS_EnJeu(renderer, jeu); // afficher FPS dans le jeu

	SDL_RenderPresent(renderer); // Mettre à jour l'écran (afficher le rendu de la frame)
	jeu->frames++;
}

void updateCooldowns(jeu_t *jeu, Uint32 now, int *lastAttackCooldown) {
	if (!jeu->joueur->peutAttaquer && (now - *lastAttackCooldown) > jeu->deuxTiersSeconde) jeu->joueur->peutAttaquer = true;
}

void updateTimers(SDL_Window *window, jeu_t *jeu, Uint32 now, Uint32 *lastFiolesTime, Uint32 *lastSecond, Uint32 *lastMinute, int *lastFrame) {
	if (now - *lastFiolesTime >= DELAI_FIOLES_MS) {
		jeu->fiolesTiming = (jeu->fiolesTiming + 1) % 3;
		*lastFiolesTime = now;
	}

	if (now - *lastSecond >= DELAI_SECONDE) { // Chaque seconde
		if (jeu->joueur->messageTete) { // Si il y a déjà un message sur la tête du joueur
			if (++jeu->delaiMessage == 6) {
				jeu->delaiMessage = 0;
				jeu->joueur->messageTete = false;
			}
		}
		int framesEcoulees = jeu->frames - *lastFrame;
		double elapsedSec = (now - *lastSecond) / DELAI_SECONDE;
		jeu->FPS_result = framesEcoulees / elapsedSec;
		//printf("FPS = %.2lf\n", jeu->FPS_result);
		afficherFPS_Fenetre(window, jeu); // Affiche les FPS dans le titre de la fenetre
		*lastFrame = jeu->frames;
		*lastSecond = now;
	}

	if (now - *lastMinute >= DELAI_MINUTE) { // Chaque minute
		joueur_modifierAlignement(jeu->joueur, 1);
		*lastMinute = now;
	}
}

int main(int argc, char *argv[]) {
	SDL_Window * window = NULL;
	SDL_Renderer * renderer = NULL;

	srand(time(NULL)); // init aléatoire
	touches_t touches = touches_initialiser();
	initSDL(&window, &renderer, TITRE_FENETRE);

	// Création des données du jeu
	jeu_t * jeu = jeu_creer(renderer);

	SDL_Event event;

	const double millisPerRender = 1000.0 / FPS;
	const double millisPerTick = 1000.0 / UPS;

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
		jeu_updateOffSetJoueur(jeu);

		updateCooldowns(jeu, now, &lastAttackCooldown);
		updateTimers(window, jeu, now, &lastFiolesTime, &lastSecond, &lastMinute, &lastFrame);

		if (now >= nextTick) {
			updateUPS(window, renderer, &touches, &event, jeu, now, &lastAttackCooldown);
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