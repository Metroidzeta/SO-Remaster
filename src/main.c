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

//	Pour compiler avec Windows :
//		> gcc -Wall src/*.c -o prog -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image
//	Pour compiler avec Windows (sans console) :
//		> gcc -Wall src/*.c -o prog -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image -mwindows
//	Pour exécuter: ./prog

#include "headers/jeu.h"
#include "headers/controles.h"
#include "headers/event.h"
#include "headers/gestionnaireGraphiques.h"

#define ATTACK_COOLDOWN_NANOS  667000000ULL // 2/3 secondes
#define NANOS_PER_RENDER       (1000000000ULL / FPS)
#define NANOS_PER_TICK         (1000000000ULL / UPS)
#define NANOS_588MS            588000000ULL
#define NANOS_1SEC             1000000000ULL
#define NANOS_1MIN             60000000000ULL
#define MAX_FRAME_SKIP_NANOS   5000000000ULL
#define NO_DELAYS_PER_YIELD    16

void render(jeu_t *jeu);

void afficherCadreEtMessage(SDL_Renderer *renderer, const char *msg, TTF_Font *police, SDL_Rect cadre, SDL_Color couleurCadre) {
	if (!renderer || !msg || !police) return;
	dessinerRectangle(renderer, &cadre, couleurCadre);
	if (*msg) dessinerTexteLimite(renderer, msg, police, BLANC, cadre.x, cadre.y, cadre.w);
}

void afficherMessageEvent(const char *message, jeu_t *jeu) {
	//                _________.x_________  _________.y_________  ________.w_________  ________.h_________
	SDL_Rect cadre = {WINDOW_WIDTH * 0.175, WINDOW_HEIGHT * 0.02, WINDOW_WIDTH * 0.65, WINDOW_HEIGHT * 0.2};
	afficherCadreEtMessage(jeu->renderer, message, getPolice(jeu, 1), cadre, jeu->COULEURS_CADRES[jeu->indexCouleurCadres]);
}

void dessinerCadreEcriture(jeu_t *jeu) {
	//                ________.x_________  ________.y__________  _________.w________  _________.h_________
	SDL_Rect cadre = {WINDOW_WIDTH * 0.03, WINDOW_HEIGHT * 0.95, WINDOW_WIDTH * 0.94, WINDOW_HEIGHT * 0.04};
	afficherCadreEtMessage(jeu->renderer, jeu->message[0], getPolice(jeu, 1), cadre, jeu->COULEURS_CADRES[jeu->indexCouleurCadres]);
}

void afficherFPS_Fenetre(jeu_t *jeu) {
	snprintf(jeu->str_FPS_Fenetre, sizeof(jeu->str_FPS_Fenetre), "%s | FPS : %.2f", TITRE_FENETRE, jeu->fpsResult);
	SDL_SetWindowTitle(jeu->window, jeu->str_FPS_Fenetre);
}

static void changerMusique(musique_t *musique, jeu_t *jeu) {
	if (jeu->musiqueActuelle != musique) {
		musique_stopAndPlay(jeu->musiqueActuelle, musique);
		jeu->musiqueActuelle = musique;
	}
}

static void executeEvent_MSG(jeu_t *jeu) {
	jeu->heros->estEnTrainDEcrire = false;
	jeu_viderMessageHeros(jeu);
}

static void executeEvent_TP(EventData_t *data, jeu_t *jeu) {
	jeu->degatsAffiches = false;
	const int xDst = data->tp.xDst, yDst = data->tp.yDst;
	carte_t *carteDst = data->tp.carteDst;
	changerMusique(carteDst->musique, jeu);
	jeu->heros->carteActuelle = carteDst;
	heros_modifierPosition(jeu->heros, xDst, yDst);
	printf("Teleportation dans la carte %s (%d,%d)\n", carteDst->nom, xDst / TAILLE_CASES, yDst / TAILLE_CASES);

	// --- Synchronisation caméra ---
	camera_setCible(&jeu->camera, &jeu->heros->xOffSet, &jeu->heros->yOffSet);
	camera_update(&jeu->camera); // recalculer immédiatement l'offset caméra
	camera_sync(&jeu->camera); // synchroniser la caméra pour empêcher un glissement

	effacerEcran(jeu->renderer);
	camera_interpolate(&jeu->camera, 1.0); // interpolation complète
	render(jeu); // afficher directement la nouvelle carte
	jeu->refreshNextFrame = true;
}

static void executeEvent_JM(EventData_t *data, jeu_t *jeu) {
	changerMusique(data->musique, jeu);
}

static void executeEvent_AM(jeu_t *jeu) {
	if (jeu->musiqueActuelle) musique_stop(jeu->musiqueActuelle);
}

static void executeEvent_MPV(EventData_t *data, jeu_t *jeu) {
	heros_modifierPV(jeu->heros, data->val_PV);
	updateFiolePV(jeu);
}

static void executeEvent_MPM(EventData_t *data, jeu_t *jeu) {
	heros_modifierPM(jeu->heros, data->val_PM);
	updateFiolePM(jeu);
}

static void executeEvent_LVLUP(jeu_t *jeu) {
	heros_lvlup(jeu->heros);
}

void executeEvent(event_t *ev, jeu_t *jeu) {
	if (!ev || !jeu) return;
	jeu->heros->estBloque = true;
	jeu->heros->estDansUnEvent = true;
	switch (ev->type) {
		case EVENT_MSG:              executeEvent_MSG(jeu); break;               // message du jeu pour le héros
		case EVENT_TP:               executeEvent_TP(&ev->data, jeu); break;     // téléportation
		case EVENT_JOUER_MUSIQUE:    executeEvent_JM(&ev->data, jeu); break;     // lancer une musique
		case EVENT_ARRET_MUSIQUE:    executeEvent_AM(jeu); break;                // arrêt de la musique
		case EVENT_MODIF_PV:         executeEvent_MPV(&ev->data, jeu); break;    // modifier PV du héros (- ou +)
		case EVENT_MODIF_PM:         executeEvent_MPM(&ev->data, jeu); break;    // modifier PM du héros (- ou +)
		case EVENT_LVLUP:            executeEvent_LVLUP(jeu); break;             // le héros passe au niveau supérieur
	}
	if (ev->type != EVENT_MSG) {
		jeu->nbEventPass++;
		jeu->heros->estDansUnEvent = false;
	}
}

void updateUPS(jeu_t *jeu, controles_t *controles, SDL_Event *event, Uint64 now, Uint64 *lastAttackCooldown) {
	while (SDL_PollEvent(event)) controles_detection(event, controles, jeu);

	bool flechesAppuye = (controles->HAUT || controles->BAS || controles->GAUCHE || controles->DROITE);
	heros_t *heros = jeu->heros;
	if (flechesAppuye && !jeu->heros->estBloque) {
		if (controles->HAUT && !controles->BAS) heros_deplacer(heros, HAUT);
		if (controles->BAS && !controles->HAUT) heros_deplacer(heros, BAS);
		if (controles->GAUCHE && !controles->DROITE) heros_deplacer(heros, GAUCHE);
		if (controles->DROITE && !controles->GAUCHE) heros_deplacer(heros, DROITE);
	} else {
		heros->frameDeplacement = 7;
	}
	camera_update(&jeu->camera);
	if (flechesAppuye && !heros->estBloque && !jeu->alEventsActuels) {
		jeu->alEventsActuels = carte_verifierCollisionsEvents(heros->carteActuelle, &heros->hitBox);
	}
	if (jeu->alEventsActuels && !heros->estDansUnEvent) {
		event_t *e = getEventActuel(jeu);
		if (e) executeEvent(e, jeu);
	}

	if (!heros->estEnTrainDEcrire) {
		if (controles->A) {
			jeu->mursVisibles = !jeu->mursVisibles;
			printf("Vous avez %s l'affichage des murs\n", jeu->mursVisibles ? "active" : "desactive");
			controles->A = false;
		}

		if (controles->B) {
			SDL_SetWindowFullscreen(jeu->window, SDL_WINDOW_FULLSCREEN);
			controles->B = false;
		}

		if (controles->Q) {
			jeu->enCours = false;
			controles->Q = false;
		}

		if (!heros->estBloque) {
			if (controles->S) {
				if (heros->peutAttaquer) {
					bruitage_play(getBruitage(jeu, 0));
					heros_updateHitBoxEpee(heros);
					SDL_Rect *hitBoxEpee = &heros->hitBoxEpee[heros->direction];
					bool premier = true;
					double resultat_aleatoire = 0;
					for (int i = 0; i < heros->carteActuelle->monstres->taille; ++i) {
						monstre_t *monstre = arraylist_get(heros->carteActuelle->monstres, i);
						if (SDL_HasIntersection(hitBoxEpee, &monstre->hitBox)) {
							if (premier) {
								resultat_aleatoire = (double) rand() / RAND_MAX;
								//printf("resultat_aleatoire = %f > tauxCrit = %f\n", resultat_aleatoire, heros->tauxCrit);
								arraylist_clear(jeu->hitBoxMonstresTouches, false);
								premier = false;
							}
							if (resultat_aleatoire > heros->tauxCrit) {
								printf("Coup normal sur le monstre\n");
								bruitage_play(getBruitage(jeu, 1));
								jeu->estUnCoupCritique = false;
							} else {
								printf("Coup critique! sur le monstre\n");
								bruitage_play(getBruitage(jeu, 2));
								jeu->estUnCoupCritique = true;
							}
							arraylist_add(jeu->hitBoxMonstresTouches, &monstre->hitBox);
							jeu->degatsAffiches = true;
							jeu->tempsDebutDegats = now;
						}
					}
					heros->attaqueEpee = true;
					heros->peutAttaquer = false;
					*lastAttackCooldown = now;
				}
				controles->S = false;
			}
		}
	}

	if (controles->RETOUR_ARRIERE) {
		if (heros->estEnTrainDEcrire) {
			jeu_supprimerDernierCaractere(jeu);
		}
		controles->RETOUR_ARRIERE = false;
	}

	if (controles->ENTREE) {
		if (!heros->estBloque) {
			const bool ecritureActive = heros->estEnTrainDEcrire;
			heros->estEnTrainDEcrire = !ecritureActive;   	
			if (ecritureActive && *jeu->message[0]) {
				jeu->delaiMessage = 0;
				heros->messageTete = true;
				printf("%s : %s\n", heros->nom, jeu->message[0]);
				ajouterMessageHistorique(jeu);
				jeu_sauvegarderMessageHeros(jeu);
				jeu_viderMessageHeros(jeu);
			}
		}
		controles->ENTREE = false;
	}

	if (controles->ESPACE) {
		if (heros->estDansUnEvent && getEventActuel(jeu)->type == EVENT_MSG) {
			jeu->nbEventPass++;
			heros->estDansUnEvent = false;
		}
		controles->ESPACE = false;
	}

	if (controles->ECHAP) {
		if (!jeu->alEventsActuels) {
			jeu->afficherRecap = 0;
			jeu->menuVisible = !jeu->menuVisible;
			if (jeu->menuVisible) {
				heros->estEnTrainDEcrire = false;
				jeu_viderMessageHeros(jeu);
				heros->frameDeplacement = 7;
			}
			heros->estBloque = !heros->estBloque;
			controles->ECHAP = false;
		}
	}

	if (!heros->estBloque) {
		if (controles->F1) {
			jeu->afficherRecap = (jeu->afficherRecap + 1) % 2; // % 3 dans le futur
			controles->F1 = false;
		}
		if (controles->F3) {
			if (!heros->estEnTrainDEcrire) heros->estEnTrainDEcrire = true;
			jeu_viderMessageHeros(jeu);
			jeu_restaurerDernierMessageHeros(jeu);
			controles->F3 = false;
		}
	}

	if (controles->F5) {
		jeu->indexCouleurCadres = (jeu->indexCouleurCadres + 1) % 5;
		controles->F5 = false;
	}

	if (jeu->alEventsActuels && jeu->nbEventPass == jeu->alEventsActuels->taille) {
		jeu->nbEventPass = 0;
		jeu->alEventsActuels = NULL;
		heros->estBloque = false;
	}
}

Uint64 nowNanos() {
	Uint64 counter = SDL_GetPerformanceCounter();
	Uint64 freq = SDL_GetPerformanceFrequency();

	Uint64 sec = counter / freq;
	Uint64 frac = counter % freq;
	return sec * 1000000000ULL + (frac * 1000000000ULL) / freq;
}

void render(jeu_t *jeu) {
	effacerEcran(jeu->renderer);
	heros_t *heros = jeu->heros;
	dessinerCouche(heros->carteActuelle, 0, jeu); // couche 0 tuiles chipset
	dessinerCouche(heros->carteActuelle, 1, jeu); // couche 1 tuiles chipset
	if (jeu->mursVisibles) dessinerMurs(heros->carteActuelle, jeu); // murs
	heros_afficherNom(jeu->renderer, heros, jeu->rectPseudo); // nom du héros
	dessinerMonstres(jeu);
	heros_afficherSkin(jeu->renderer, heros); // skin du héros
	dessinerCouche(heros->carteActuelle, 2, jeu); // couche 2 tuiles chipset

	if (jeu->degatsAffiches) {
		const double duree = 1.0; // 1 seconde d'affichage
		const double tempsEcoule = (nowNanos() - jeu->tempsDebutDegats) / 1e9;
		double t = tempsEcoule / duree;
		t = maxDouble(0.0, minDouble(1.0, t)); // clamp [0..1]
		if (t >= 1.0) {
			jeu->degatsAffiches = false;
		} else {
			if (jeu->estUnCoupCritique) dessinerDegats(64, ROUGE, t, jeu);
			else dessinerDegats(32, BLANC, t, jeu);
		}
	}

	if (heros->attaqueEpee) { // si notre héros est en train d'attaquer
		afficherHitboxAttaqueEpee(jeu);
		heros->attaqueEpee = false;
	}

	dessinerFiolePV(jeu);
	dessinerFiolePM(jeu);
	dessinerBarreXP(jeu);

	if (heros->estEnTrainDEcrire) dessinerCadreEcriture(jeu);
	if (heros->messageTete) dessinerCadreMessageTeteHeros(jeu);
	if (jeu->afficherRecap == 1) dessinerHistoriqueMessages(jeu);
	if (heros->estDansUnEvent) {
		event_t *evActuel = getEventActuel(jeu);
		if (evActuel && evActuel->type == EVENT_MSG) {
			afficherMessageEvent(evActuel->data.msg, jeu);
		}
	}

	if (jeu->menuVisible) {
		dessinerMenuNavigation(jeu);
		dessinerMenuStatistiques(jeu);
	}

	afficherAlignement(jeu);
	afficherFPS_EnJeu(jeu); // afficher FPS dans le jeu
	if (jeu->refreshNextFrame) {
		computePalette(jeu);
		jeu->refreshNextFrame = false;
	}

	SDL_RenderPresent(jeu->renderer); // mettre à jour l'écran (afficher le rendu de la frame)
}

void updateFPS(jeu_t *jeu) {
	render(jeu);
	jeu->frames++;
}

void updateCooldowns(jeu_t *jeu, Uint64 now, Uint64 *lastAttackCooldown) {
	if (!jeu->heros->peutAttaquer && (now - *lastAttackCooldown) > ATTACK_COOLDOWN_NANOS) jeu->heros->peutAttaquer = true;
}

void updateTimers(jeu_t *jeu, Uint64 now, Uint64 *lastFiolesTime, Uint64 *lastSecond, Uint64 *lastMinute, Uint64 *lastFrameCount) {
	// --- Animation fioles ---
	if (now - *lastFiolesTime >= NANOS_588MS) {
		jeu->fiolesTiming = (jeu->fiolesTiming + 1) % FIOLES_ANIMATION_FRAMES;
		*lastFiolesTime += NANOS_588MS;
	}

	// --- Chaque seconde ---
	if (now - *lastSecond >= NANOS_1SEC) {
		if (jeu->heros->messageTete) { // si il y a déjà un message sur la tête de notre héros
			if (++jeu->delaiMessage == 6) {
				jeu->delaiMessage = 0;
				jeu->heros->messageTete = false;
			}
		}
		Uint64 framesEcoulees = jeu->frames - *lastFrameCount;
		double elapsedSec = (double)(now - *lastSecond) / 1e9;
		jeu->fpsResult = (double)framesEcoulees / elapsedSec;
		afficherFPS_Fenetre(jeu); // afficher FPS dans le titre de la fenetre
		//printf("FPS = %.2lf\n", jeu->FPS_result);
		*lastFrameCount = jeu->frames;
		*lastSecond += NANOS_1SEC;
	}

	// --- Chaque minute ---
	if (now - *lastMinute >= NANOS_1MIN) {
		heros_modifierAlignement(jeu->heros, 1);
		*lastMinute += NANOS_1MIN;
	}
}

int main(int argc, char *argv[]) {
	srand(time(NULL)); // init graine aléatoire
	controles_t controles = controles_init(); // init des contrôles (touches)

	jeu_t *jeu = jeu_creer();
	SDL_Event event;

	// --- Variables de synchronisation ---
	Uint64 now = nowNanos();
	Uint64 nextRender = now, nextTick = now;

	// --- Timers secondaires ---
	Uint64 lastFiolesTime = now, lastMinute = now, lastSecond = now;
	Uint64 overSleepNanos = 0ULL;
	int noDelays = 0;

	// --- Initialisation ---
	Uint64 lastFrameCount = 0ULL;
	musique_play(jeu->musiqueActuelle);
	jeu->enCours = true;

	Uint64 lastAttackCooldown = 0ULL;
	while (jeu->enCours) {
		now = nowNanos();

		// --- Détection d'une longue pause (veille, freeze, etc.) ---
		if ((Sint64)(now - nextTick) > (Sint64) MAX_FRAME_SKIP_NANOS) {
			nextTick = now + NANOS_PER_TICK;
			nextRender = now + NANOS_PER_RENDER;
			overSleepNanos = 0ULL;
			noDelays = 0;
		}

		// --- Logique globale ---
		updateCooldowns(jeu, now, &lastAttackCooldown);
		updateTimers(jeu, now, &lastFiolesTime, &lastSecond, &lastMinute, &lastFrameCount);

		// --- Updates logiques (UPS) ---
		while (now >= nextTick) {
			updateUPS(jeu, &controles, &event, now, &lastAttackCooldown);
			nextTick += NANOS_PER_TICK;
			now = nowNanos(); // anti dérive
		}

		// --- Rendu graphique (FPS) ---
		if (now >= nextRender) {
			// calcul du facteur d'interpolation entre deux updates logiques
			double interpolation = (double)(now - (nextTick - NANOS_PER_TICK)) / (double)NANOS_PER_TICK;
			interpolation = maxDouble(0.0, minDouble(1.0, interpolation)); // intervalle [0..1]
			camera_interpolate(&jeu->camera, interpolation);
			updateFPS(jeu);
			nextRender += NANOS_PER_RENDER;
		}

		// --- Gestion CPU ---
		Uint64 nextAction = (nextTick < nextRender) ? nextTick : nextRender;
		Sint64 diff = (Sint64)nextAction - (Sint64)nowNanos();
		if (diff < 0) diff = 0;
		Sint64 sleepNanos = diff - (Sint64)overSleepNanos;

		if (sleepNanos > 0) {
			Uint32 sleepMillis = (Uint32)(sleepNanos / 1000000ULL);
			Uint64 before = nowNanos();
			if (sleepMillis > 0) SDL_Delay(sleepMillis);

			// Micro-attente propre sans 100% CPU
			Uint64 remain = sleepNanos % 1000000ULL;
			if (remain > 0) {
				Uint64 start = nowNanos();
				while (nowNanos() - start < remain) SDL_Delay(0); // yield CPU
			}

			Uint64 after = nowNanos();
			overSleepNanos = (after - before) - (Uint64)sleepNanos; // léger ajustement possible
			noDelays = 0;
		} else {
			overSleepNanos = 0ULL;
			if (++noDelays >= NO_DELAYS_PER_YIELD) {
				SDL_Delay(1); // yield
				noDelays = 0;
			}
		}
	}
	jeu_detruire(jeu);
	jeu = NULL;
	return EXIT_SUCCESS;
}