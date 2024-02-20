// @author Metroidzeta

#include "touches.h"

touches_t touches_initialiser() { // Initialiser toutes les touches sur false par défaut
	return (touches_t) {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
}

void touches_detection(SDL_Event * event, touches_t * touches, jeu_t * jeu) {
	if(event->type == SDL_KEYDOWN) { // Quand une touche est pressée
		switch(event->key.keysym.sym) {
			case SDLK_UP: touches->bouton_HAUT = true; break;
			case SDLK_DOWN: touches->bouton_BAS = true; break;
			case SDLK_LEFT: touches->bouton_GAUCHE = true; break;
			case SDLK_RIGHT: touches->bouton_DROITE = true; break;

			case SDLK_a: touches->bouton_A = true; break;
			case SDLK_b: touches->bouton_B = true; break;
			case SDLK_q: touches->bouton_Q = true; break;
			case SDLK_s: touches->bouton_S = true; break;

			case SDLK_SPACE: touches->bouton_ESPACE = true; break;
			case SDLK_RETURN: touches->bouton_ENTREE = true; break;
			case SDLK_ESCAPE:touches->bouton_ECHAP = true;break;
			case SDLK_BACKSPACE: touches->bouton_RETOUR_ARRIERE = true; break;

			case SDLK_F1: touches->bouton_F1 = true; break;
			case SDLK_F3: touches->bouton_F3 = true; break;
			case SDLK_F5: touches->bouton_F5 = true; break;

			default: break;
		}
	}

	else if(event->type == SDL_KEYUP) { // Quand une touche est relachée
		switch(event->key.keysym.sym) {
			case SDLK_UP: touches->bouton_HAUT = false; break;
			case SDLK_DOWN: touches->bouton_BAS = false; break;
			case SDLK_LEFT: touches->bouton_GAUCHE = false; break;
			case SDLK_RIGHT: touches->bouton_DROITE = false; break;

			case SDLK_a: touches->bouton_A = false; break;
			case SDLK_b: touches->bouton_B = false; break;
			case SDLK_q: touches->bouton_Q = false; break;
			case SDLK_s: touches->bouton_S = false; break;

			case SDLK_SPACE: touches->bouton_ESPACE = false; break;
			case SDLK_RETURN: touches->bouton_ENTREE = false; break;
			case SDLK_ESCAPE: touches->bouton_ECHAP = false; break;
			case SDLK_BACKSPACE: touches->bouton_RETOUR_ARRIERE = false; break;

			case SDLK_F1: touches->bouton_F1 = false; break;
			case SDLK_F3: touches->bouton_F3 = false; break;
			case SDLK_F5: touches->bouton_F5 = false; break;

			default: break;
		}
	}

	else if(event->type == SDL_TEXTINPUT) { // Quand un caractère est tapé (écriture)
		//SDL_PumpEvents();
		//SDL_FlushEvents(SDL_KEYDOWN,SDL_TEXTINPUT);
		if(jeu->joueur->ecritUnMessage && jeu->compteurLettres < TAILLE_MAX_MSG) {
			int nbOctets = strlen(event->text.text);
			jeu->message[0][jeu->compteurLettresReelles] = event->text.text[0];
			jeu->messageChar2octets[0][jeu->compteurLettresReelles] = false;
			jeu->compteurLettresReelles++;

			if(nbOctets == 2) { // Si le caractère UTF-8 est codé sur 2 octets, donc 2 cases (typique pour les caractères accentués)
				jeu->message[0][jeu->compteurLettresReelles] = event->text.text[1];
				jeu->messageChar2octets[0][jeu->compteurLettresReelles] = true;
				jeu->compteurLettresReelles++;
			}
			jeu->compteurLettres++;
			jeu->message[0][jeu->compteurLettresReelles] = '\0';
		}
	}

	else if(event->type == SDL_QUIT) { // Click sur la croix rouge de la fenêtre
		jeu->programme_actif = false;
	}
}