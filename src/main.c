// @author Metroidzeta
//
//	Pour compiler avec Windows :
//		> gcc -Wall src/*.c -o prog -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image
//	Pour compiler avec Windows (sans console) :
//		> gcc -Wall src/*.c -o prog -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image -mwindows
//	Pour exécuter: ./prog

#include "jeu.h"
#include "touches.h"

int min(double a, double b) { return (int) (a <= b ? a : b); }

void afficherHitBoxJoueur(SDL_Renderer * renderer, jeu_t * jeu) {
	dessinerRectangle(renderer,&jeu->hitBoxJoueurEcran,ROUGE);
}

void afficherLesMonstres(SDL_Renderer * renderer, jeu_t * jeu) {
	SDL_Rect dstRect = {0, 0, TAILLE_CASES, TAILLE_CASES};
	for(int i = 0; i < jeu->joueur->carteActuelle->lesMonstres->taille; i++) {
		monstre_t * monstre = arraylist_get(jeu->joueur->carteActuelle->lesMonstres,i);
		dstRect.x = monstre->x + jeu->xOffSetJoueur;
		dstRect.y = monstre->y + jeu->yOffSetJoueur;
		monstre_afficher(renderer,monstre,28,&dstRect);
	}
}

void afficherCadreAvecMessage(SDL_Renderer * renderer, char * message, TTF_Font * police, SDL_Rect cadre, SDL_Color couleurCadre) {
	dessinerRectangle(renderer,&cadre,couleurCadre);
	if(message[0] != '\0') { // Si le message n'est pas vide
		dessinerTexteLimite(renderer,message,police,BLANC,cadre.x,cadre.y,cadre.w);
	}
}

void afficherMessageEvent(SDL_Renderer * renderer, char * message, jeu_t * jeu) {
	//                ________.x_________  _________.y_________  ________.w________  ________.h_________
	SDL_Rect cadre = {WINDOW_WIDTH * 0.15, WINDOW_HEIGHT * 0.02, WINDOW_WIDTH * 0.7, WINDOW_HEIGHT * 0.2};
	afficherCadreAvecMessage(renderer,message,getPolice(jeu,1),cadre,jeu->couleurs_cadres[jeu->numCouleur_cadres]);
}

void afficherCadreEcriture(SDL_Renderer * renderer, jeu_t * jeu) {
	//                ________.x_________  ________.y__________  _________.w________  _________.h_________
	SDL_Rect cadre = {WINDOW_WIDTH * 0.03, WINDOW_HEIGHT * 0.95, WINDOW_WIDTH * 0.94, WINDOW_HEIGHT * 0.04};
	afficherCadreAvecMessage(renderer,jeu->message[0],getPolice(jeu,1),cadre,jeu->couleurs_cadres[jeu->numCouleur_cadres]);
}

void afficherCadreMessageTeteJoueur(SDL_Renderer * renderer, jeu_t * jeu) {
	if(jeu->message[1][0] != '\0') { // Si le save message n'est pas vide
		int w, h;
		SDL_Texture * texture = creerTextureLimiteDepuisTexte(renderer,jeu->message[1],getPolice(jeu,1),BLANC,7 * TAILLE_CASES);
		SDL_QueryTexture(texture,NULL,NULL,&w,&h);
		SDL_DestroyTexture(texture);
		//                           ______________________.x___________________________  ______________________.y_______________________ .w .h
		SDL_Rect cadre = (SDL_Rect) {jeu->hitBoxJoueurEcran.x + TAILLE_CASES / 2 - w / 2, jeu->hitBoxJoueurEcran.y - TAILLE_CASES / 4 - h, w, h};
		dessinerRectangle(renderer,&cadre,jeu->couleurs_cadres[jeu->numCouleur_cadres]);
		dessinerTexteLimite(renderer,jeu->message[1],getPolice(jeu,1),BLANC,cadre.x,cadre.y,7 * TAILLE_CASES);
	}
}

void afficherCadreMessageRecap(SDL_Renderer * renderer, jeu_t * jeu) {
	//                ________.x_________  _________.y_________  _______.w_________  _________.h_________
	SDL_Rect cadre = {WINDOW_WIDTH * 0.05, WINDOW_HEIGHT * 0.69, WINDOW_WIDTH * 0.9, WINDOW_HEIGHT * 0.25};
	dessinerRectangle(renderer,&cadre,jeu->couleurs_cadres[jeu->numCouleur_cadres]);

	int xTexte = cadre.x + WINDOW_WIDTH * 0.01;
	int yTexte = cadre.y + WINDOW_HEIGHT * 0.01;
	int yLigneOffset = WINDOW_HEIGHT * 0.08; // Permet de sauter une ligne
	for(int i = 0; i < 3; i++) {
		if(jeu->recapMessages[i][0] != '\0') { // Si le message n'est pas vide
			char ligne[strlen(jeu->joueur->nom) + 3 + strlen(jeu->recapMessages[i]) + 1]; // pseudo + " : " + message + "/0"
			sprintf(ligne,"%s : %s",jeu->joueur->nom,jeu->recapMessages[i]);
			dessinerTexteLimite(renderer,ligne,getPolice(jeu,1),BLANC,xTexte,yTexte + i * yLigneOffset,cadre.w);
		}
	}
}

void afficherMenuNavigation(SDL_Renderer * renderer, jeu_t * jeu) {
	//                ________.x ________  _________.y_________  ________.w_________  _________.h_________
	SDL_Rect cadre = {WINDOW_WIDTH * 0.01, WINDOW_HEIGHT * 0.37, WINDOW_WIDTH * 0.15, WINDOW_HEIGHT * 0.26};
	dessinerRectangle(renderer,&cadre,jeu->couleurs_cadres[jeu->numCouleur_cadres]);
	char * motsMenu[5] = {"Inventaire","Magie","Statistiques","Echanger","Quitter"};
	int xTexte = cadre.x + WINDOW_WIDTH * 0.01;
	int yTexte = cadre.y + WINDOW_HEIGHT * 0.01;
	int yLigneOffset = WINDOW_HEIGHT * 0.05; // Permet de sauter une ligne

	for(int i = 0; i < 5; i++) {
		dessinerTexte(renderer,motsMenu[i],getPolice(jeu,1),BLANC,xTexte,yTexte + i * yLigneOffset);
	}
}

void afficherMenuStatistiques(SDL_Renderer * renderer, jeu_t * jeu) {
	//                ________.x ________  _________.y_________  ________.w_________  _________.h_________
	SDL_Rect cadre = {WINDOW_WIDTH * 0.17, WINDOW_HEIGHT * 0.01, WINDOW_WIDTH * 0.80, WINDOW_HEIGHT * 0.98};
	dessinerRectangle(renderer,&cadre,jeu->couleurs_cadres[jeu->numCouleur_cadres]);
	int xTexte = cadre.x + WINDOW_WIDTH * 0.01;
	int yTexte = cadre.y + WINDOW_HEIGHT * 0.01;
	int yLigneOffset = WINDOW_HEIGHT * 0.05; // Permet de sauter une ligne

	int nbLignes = 10;
	char lignes[nbLignes][100];  // Tableau de lignes
	sprintf(lignes[0],"Nom : %s",jeu->joueur->nom);
	sprintf(lignes[1],"Classe : %s",joueur_getClasseToString(jeu->joueur));
	sprintf(lignes[2],"Niveau : %d",jeu->joueur->niveau);
	sprintf(lignes[3],"Or : %d",jeu->joueur->piecesOr);
	strcpy(lignes[4],"");
	sprintf(lignes[5],"Force : %-3d %65s PV : %4d / %4d",jeu->joueur->force,"",jeu->joueur->PV[0],jeu->joueur->PV[1]);
	sprintf(lignes[6],"Dextérité : %-3d %61s PM : %4d / %4d",jeu->joueur->dexterite,"",jeu->joueur->PM[0],jeu->joueur->PM[1]);
	sprintf(lignes[7],"Constitution : %-3d",jeu->joueur->constitution);
	strcpy(lignes[8],"");
	sprintf(lignes[9],"Taux Coups Critiques : %.1f %c",jeu->joueur->tauxCoupCritique * 100,'%');

	for(int i = 0; i < nbLignes; i++) {
		if(lignes[i][0] != '\0') {
			dessinerTexte(renderer,lignes[i],getPolice(jeu,1),BLANC,xTexte,yTexte + i * yLigneOffset);
		}
	}
}

void afficherDegats(SDL_Renderer * renderer, int nombre, SDL_Color couleur, double frame, jeu_t * jeu) {
	for(int i = 0; i < jeu->lesHitBoxDesMonstresTouches->taille; i++) {
		SDL_Rect * hitBox = getHitBoxMonstreTouche(jeu,i);
		dessinerNombre(renderer,nombre,getPolice(jeu,2),couleur,hitBox->x + jeu->xOffSetJoueur,hitBox->y + jeu->yOffSetJoueur - TAILLE_CASES / 2 - (WINDOW_HEIGHT * 0.2 * frame));
	}
}

void afficherAlignement(SDL_Renderer * renderer, jeu_t * jeu) {
	dessinerNombre(renderer,jeu->joueur->alignement,getPolice(jeu,1),BLANC,WINDOW_WIDTH * 0.04,WINDOW_HEIGHT * 0.06);
}

void afficherFPS_Fenetre(SDL_Window * window, jeu_t * jeu) {
	sprintf(jeu->str_FPS_Fenetre,"Slayers Online 2 | FPS : %.2lf",jeu->FPS_result);
	SDL_SetWindowTitle(window,jeu->str_FPS_Fenetre);
}

void afficherFPS_EnJeu(SDL_Renderer * renderer, jeu_t * jeu) {
	sprintf(jeu->str_FPS_EnJeu,"FPS : %.2lf",jeu->FPS_result);
	dessinerTexte(renderer,jeu->str_FPS_EnJeu,getPolice(jeu,0),VERT,WINDOW_WIDTH * 0.04,WINDOW_HEIGHT * 0.03);
}

void faireEvent(SDL_Renderer * renderer, event_t * e, jeu_t * jeu) {
	if(e) {
		jeu->joueur->bloqueTotal = true;
		jeu->joueur->eventEnCours = true;
		switch(e->type) {
			case E_MESSAGE: // Si c'est un message du jeu
				jeu->joueur->ecritUnMessage = false;
				viderMessage(jeu);
				break;
			case E_TELEPORTATION: // Si c'est une téléportation
				jeu->degatsAffiches = 0;
				event_tp_t * e_tel = (event_tp_t*) e->ptr; // On le cast en event_tp
				carte_t * carteDst = e_tel->carteDst;
				if(jeu->musiqueActuelle != carteDst->musique) {
					musique_stopAndPlay(jeu->musiqueActuelle,carteDst->musique);
					jeu->musiqueActuelle = carteDst->musique;
				}
				jeu->joueur->carteActuelle = carteDst;
				joueur_modifierPosition(jeu->joueur,e_tel->xDst,e_tel->yDst);
				printf("Teleportation! (%d,%d)\n",e_tel->xDst / TAILLE_CASES,e_tel->yDst / TAILLE_CASES);
				jeu->nbEventPass++;
				jeu->joueur->eventEnCours = false;
				break;
			case E_JOUER_MUSIQUE: // Si c'est une lecture de musique
				jeu->joueur->eventEnCours = true; // A ENLEVER !!
				event_jm_t * e_jm = (event_jm_t*) e->ptr; // On le cast en event_jm
				if(jeu->musiqueActuelle != e_jm->musique) {
					musique_stopAndPlay(jeu->musiqueActuelle,e_jm->musique);
					jeu->musiqueActuelle = e_jm->musique;
				}
				jeu->nbEventPass++;
				jeu->joueur->eventEnCours = false;
				break;
			case E_ARRETER_MUSIQUE: // Si c'est un arrêt de musique
				if(jeu->musiqueActuelle != NULL) {
					musique_stop(jeu->musiqueActuelle);
				}
				jeu->nbEventPass++;
				jeu->joueur->eventEnCours = false;
				break;
			case E_CHANGE_PV: // Si c'est pour modifier les PV du joueur
				jeu->joueur->eventEnCours = true; // A ENLEVER !!
				event_changePV_t * e_cPV = (event_changePV_t*) e->ptr; // On le cast en event_changePV
				joueur_modifierPV(jeu->joueur,e_cPV->PV);
				updateFiolePV(renderer,jeu);
				jeu->nbEventPass++;
				jeu->joueur->eventEnCours = false;
				break;
			case E_CHANGE_PM: // Si c'est pour modifier les PM du joueur
				jeu->joueur->eventEnCours = true; // A ENLEVER !!
				event_changePM_t * e_cPM = (event_changePM_t*) e->ptr; // On le cast en event_changePM
				joueur_modifierPM(jeu->joueur,e_cPM->PM);
				updateFiolePM(renderer,jeu);
				jeu->nbEventPass++;
				jeu->joueur->eventEnCours = false;
				break;
		}
	}
}

void updateUPS(SDL_Window * window, SDL_Renderer * renderer, touches_t * touches, SDL_Event * event, jeu_t * jeu, int * now, int * lastFrame2) {
	while(SDL_PollEvent(event)) {
		touches_detection(event,touches,jeu);
	}

	bool flechesAppuye = (touches->bouton_HAUT || touches->bouton_BAS || touches->bouton_GAUCHE || touches->bouton_DROITE);
	if(flechesAppuye && !jeu->joueur->bloqueTotal) {
		if(touches->bouton_HAUT && !touches->bouton_BAS) {
			joueur_deplacer(jeu->joueur,HAUT);
		}
		if(touches->bouton_BAS && !touches->bouton_HAUT) {
			joueur_deplacer(jeu->joueur,BAS);
		}
		if(touches->bouton_GAUCHE && !touches->bouton_DROITE) {
			joueur_deplacer(jeu->joueur,GAUCHE);
		}
		if(touches->bouton_DROITE && !touches->bouton_GAUCHE) {
			joueur_deplacer(jeu->joueur,DROITE);
		}
	} else {
		jeu->joueur->frameDeplacement = 7;
	}
	if(flechesAppuye && !jeu->joueur->bloqueTotal && !jeu->alEventsActuels) {
		jeu->alEventsActuels = carte_verifierLesCollisionsEvents(jeu->joueur->carteActuelle,&jeu->joueur->hitBox);
	}
	if(jeu->alEventsActuels && !jeu->joueur->eventEnCours) {
		event_t * e = getEventActuel(jeu,jeu->nbEventPass);
		faireEvent(renderer,e,jeu);
	}

	if(!jeu->joueur->ecritUnMessage) { // Si le joueur n'est pas entrain d'écrire un message
		if(touches->bouton_A) {
			jeu->mursVisibles = !jeu->mursVisibles;
			printf("Vous avez %s l'affichage des murs\n",jeu->mursVisibles ? "active" : "desactive");
			touches->bouton_A = false;
		}

		if(touches->bouton_B) {
			SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN);
			touches->bouton_B = false;
		}

		if(touches->bouton_Q) {
			jeu->programme_actif = false;
			touches->bouton_Q = false;
		}
		if(!jeu->joueur->bloqueTotal) {
			if(touches->bouton_S) {
				if(jeu->joueur->peutAttaquer) {
					bruitage_play(getBruitage(jeu,0));
					joueur_updateHitBoxEpee(jeu->joueur);
					SDL_Rect * hitBoxEpee = &jeu->joueur->hitBoxEpee[jeu->joueur->direction];
					bool premier = true;
					double resultat_aleatoire;
					for(int i = 0; i < jeu->joueur->carteActuelle->lesMonstres->taille; i++) {
						monstre_t * monstre = arraylist_get(jeu->joueur->carteActuelle->lesMonstres,i);
						if(SDL_HasIntersection(hitBoxEpee,&monstre->hitBox)) {
							if(premier) {
								resultat_aleatoire = (double) rand() / RAND_MAX;
								//printf("resultat_aleatoire = %f > tauxCoupCritique = %f\n",resultat_aleatoire,jeu->joueur->tauxCoupCritique);
								arraylist_clear(jeu->lesHitBoxDesMonstresTouches,false);
								premier = false;
							}
							if(resultat_aleatoire > jeu->joueur->tauxCoupCritique) {
								printf("Coup normal sur le monstre\n");
								bruitage_play(getBruitage(jeu,1));
								jeu->estCoupCritique = false;
							} else {
								printf("Coup critique! sur le monstre\n");
								bruitage_play(getBruitage(jeu,2));
								jeu->estCoupCritique = true;
							}
							arraylist_add(jeu->lesHitBoxDesMonstresTouches,&monstre->hitBox);
							jeu->degatsAffiches = 1;
						}
					}
					jeu->joueur->attaqueEpee = true;
					jeu->joueur->peutAttaquer = false;
					*lastFrame2 = *now;
				}
				touches->bouton_S = false;
			}
		}
	}

	if(touches->bouton_RETOUR_ARRIERE) {
		if(jeu->joueur->ecritUnMessage && jeu->compteurLettres > 0) {
			if(jeu->messageChar2octets[0][jeu->compteurLettresReelles - 1]) { // Si le dernier caractère UTF-8 a une longueur de 2 octets (pour les caractères accentués)
				jeu->message[0][--(jeu->compteurLettresReelles)] = 0;
				jeu->messageChar2octets[0][jeu->compteurLettresReelles] = false;
			}
			jeu->message[0][--(jeu->compteurLettresReelles)] = 0;
			jeu->compteurLettres--;
		}
		touches->bouton_RETOUR_ARRIERE = false;
	}

	if(touches->bouton_ENTREE) {
		if(!jeu->joueur->bloqueTotal) {
			if(!jeu->joueur->ecritUnMessage) {
				jeu->joueur->ecritUnMessage = true;
			} else {
				jeu->joueur->ecritUnMessage = false;		
				if(jeu->message[0][0] != '\0') {
					jeu->delaiMessage = 0;
					jeu->joueur->messageTete = true;
					printf("%s : %s\n",jeu->joueur->nom,jeu->message[0]);
					ajouterMessageHistorique(jeu);
					sauvegarderMessage(jeu);
					viderMessage(jeu);
				}
			}
		}
		touches->bouton_ENTREE = false;
	}

	if(touches->bouton_ESPACE) {
		if(jeu->joueur->eventEnCours && getEventActuel(jeu,jeu->nbEventPass)->type == E_MESSAGE) {
			jeu->nbEventPass++;
			jeu->joueur->eventEnCours = false;
		}
		touches->bouton_ESPACE = false;
	}

	if(touches->bouton_ECHAP) {
		if(!jeu->alEventsActuels) {
			jeu->afficherRecap = 0;
			jeu->menuVisible = !jeu->menuVisible;
			if(jeu->menuVisible) {
				jeu->joueur->frameDeplacement = 7;
			}
			jeu->joueur->bloqueTotal = !jeu->joueur->bloqueTotal;
			touches->bouton_ECHAP = false;
		}
	}

	if(!jeu->joueur->bloqueTotal) {
		if(touches->bouton_F1) {
			jeu->afficherRecap = (jeu->afficherRecap + 1) % 2; // % 3 dans le futur
			touches->bouton_F1 = false;
		}
		if(touches->bouton_F3) {
			if(!jeu->joueur->ecritUnMessage) { jeu->joueur->ecritUnMessage = true; }
			viderMessage(jeu);
			remettreDernierMessage(jeu);
			touches->bouton_F3 = false;
		}
	}

	if(touches->bouton_F5) {
		jeu->numCouleur_cadres = (jeu->numCouleur_cadres + 1) % 5;
		touches->bouton_F5 = false;
	}

	if(jeu->alEventsActuels && jeu->nbEventPass == jeu->alEventsActuels->taille) {
		jeu->nbEventPass = 0;
		jeu->alEventsActuels = NULL;
		jeu->joueur->bloqueTotal = false;
	}
}

int main(int argc, char *argv[]) {
	SDL_Window * window = NULL;
	SDL_Renderer * renderer = NULL;

	srand(time(NULL)); // Initialiser l'aléatoire
	touches_t touches = touches_initialiser();
	initialiserSDL(&window,&renderer,"Slayers Online 2");

	// Création des données du jeu
	jeu_t * jeu = jeu_creer(renderer);

	SDL_Event event;

	int nb_frames_passees = 0;
	int lastFrame = 0;
	int lastFrame2 = 0;

	int now = SDL_GetTicks();
	int lastFiolesUpdateTime = now;
	int lastSeconde = now;
	int lastMinute = now;
	const double MILLIS_PAR_RENDER = 1000.0f / FPS;
	const double MILLIS_PAR_TICK = 1000.0f / UPS;
	double nextRender = (double) now + MILLIS_PAR_RENDER;
	double nextTick = (double) now + MILLIS_PAR_TICK;
	int afterTime, millisToWait;

	musique_play(jeu->musiqueActuelle);
	jeu->programme_actif = true;

	while(jeu->programme_actif) {
		now = SDL_GetTicks();
		jeu_updateOffSetJoueur(jeu);

		if(now - lastFiolesUpdateTime >= 588) {
			jeu->fiolesTiming = (jeu->fiolesTiming + 1) % 3;
			lastFiolesUpdateTime = now;
		}

		if(!jeu->joueur->peutAttaquer && (double) (now - lastFrame2) > jeu->deuxTiersSeconde) {
			jeu->joueur->peutAttaquer = true;
		}

		if(now - lastSeconde >= 1000) { // Chaque seconde
			if(jeu->joueur->messageTete) { // Si il y a déjà un message sur la tête du joueur
				if(++jeu->delaiMessage == 6) { // Pré-incrémentation
					jeu->delaiMessage = 0;
					jeu->joueur->messageTete = false;
				}
			}
			nb_frames_passees = jeu->frames - lastFrame;
			jeu->FPS_result = nb_frames_passees * 1e3 / (now - lastSeconde);
			//printf("FPS = %.2lf\n",jeu->FPS_result);
			afficherFPS_Fenetre(window,jeu); // Afficher les FPS dans le titre de la fenetre
			lastFrame = jeu->frames;
			lastSeconde = now;
		}

		if(now - lastMinute >= 60000) { // Chaque minute
			joueur_modifierAlignement(jeu->joueur,1);
			lastMinute = now;
		}

		if(now >= nextTick) {
			updateUPS(window,renderer,&touches,&event,jeu,&now,&lastFrame2);
			nextTick += MILLIS_PAR_TICK;
		}

		if(now >= nextRender) {
			/************************ DESSIN chaque FRAME ************************/
			effacerEcran(renderer); // On efface l'écran
			afficherCouche(renderer,jeu->joueur->carteActuelle,0,jeu); // Afficher la couche 0 du chipset
			afficherCouche(renderer,jeu->joueur->carteActuelle,1,jeu); // Afficher la couche 1 du chipset
			if(jeu->mursVisibles) {
				afficherMurs(renderer,jeu->joueur->carteActuelle,jeu); // Afficher les murs
			}
			joueur_afficherNom(renderer,jeu->joueur,jeu->rectPseudo); // Afficher le nom (pseudo) de notre joueur
			afficherLesMonstres(renderer,jeu);
			joueur_afficherSkin(renderer,jeu->joueur,&jeu->hitBoxJoueurEcran); // Afficher le sprite de notre joueur
			afficherCouche(renderer,jeu->joueur->carteActuelle,2,jeu); // Afficher la couche 2 du chipset
			if(jeu->degatsAffiches > 0) {
				if(jeu->estCoupCritique) {
					afficherDegats(renderer,64,ROUGE,(double) jeu->degatsAffiches / FPS,jeu);
				} else {
					afficherDegats(renderer,32,BLANC,(double) jeu->degatsAffiches / FPS,jeu);
				}
				jeu->degatsAffiches++;
				if(jeu->degatsAffiches >= FPS) {
					jeu->degatsAffiches = 0;
				}
			}
			if(jeu->joueur->attaqueEpee) { // Si notre joueur est en train d'attaquer
				afficherHitboxAttaqueEpee(renderer,jeu);
				jeu->joueur->attaqueEpee = false;
			}
			afficherFiolePV(renderer,jeu); // Afficher la fiole PV de notre joueur
			afficherFiolePM(renderer,jeu); // Afficher la fiole PM de notre joueur
			afficherBarreXP(renderer,jeu); // Afficher la barre XP de notre joueur
			if(jeu->joueur->ecritUnMessage) { // Si notre joueur est en train d'écrire un message
				afficherCadreEcriture(renderer,jeu);
			}
			if(jeu->joueur->messageTete) {
				afficherCadreMessageTeteJoueur(renderer,jeu);
			}
			if(jeu->afficherRecap == 1) {
				afficherCadreMessageRecap(renderer,jeu);
			}
			if(jeu->joueur->eventEnCours && getEventActuel(jeu,jeu->nbEventPass)->type == E_MESSAGE) {
				event_msg_t * e_msg = (event_msg_t*) getEventActuel(jeu,jeu->nbEventPass)->ptr; // Cast en event_msg
				afficherMessageEvent(renderer,e_msg->message,jeu);
			}
			if(jeu->menuVisible) {
				afficherMenuNavigation(renderer,jeu); // Afficher le menu du jeu
				afficherMenuStatistiques(renderer,jeu);
			}
			afficherAlignement(renderer,jeu); // Afficher l'alignement de notre joueur
			afficherFPS_EnJeu(renderer,jeu); // Afficher les FPS dans le jeu

			SDL_RenderPresent(renderer); // Mettre à jour l'écran (afficher le rendu de la frame)
			/************************ FIN DESSIN chaque FRAME ************************/
			jeu->frames++;
			nextRender += MILLIS_PAR_RENDER;
		}

		afterTime = SDL_GetTicks();
		millisToWait = min(nextTick - (double) afterTime, nextRender - (double) afterTime);
		if(millisToWait > 0) {
			SDL_Delay(millisToWait); // attendre les millis secondes nécessaires pour respecter les UPS/FPS
		}
	}
	jeu_detruire(jeu);
	freeSDL(window,renderer);

    return EXIT_SUCCESS;
}