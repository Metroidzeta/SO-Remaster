// @author Alain Barbier alias "Metroidzeta"

#include "touches.h"

touches_t touches_initialiser() { // Initialiser toutes les touches sur false par défaut
	return (touches_t) {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
}

void touches_detection(SDL_Event * event, touches_t * touches, jeu_t * jeu) {
	if(event->type == SDL_KEYDOWN) { // Quand une touche est pressée
		switch(event->key.keysym.sym) {
			case SDLK_UP: touches->HAUT = true; break;
			case SDLK_DOWN: touches->BAS = true; break;
			case SDLK_LEFT: touches->GAUCHE = true; break;
			case SDLK_RIGHT: touches->DROITE = true; break;

			case SDLK_a: touches->A = true; break;
			case SDLK_b: touches->B = true; break;
			case SDLK_q: touches->Q = true; break;
			case SDLK_s: touches->S = true; break;

			case SDLK_SPACE: touches->ESPACE = true; break;
			case SDLK_RETURN: touches->ENTREE = true; break;
			case SDLK_ESCAPE:touches->ECHAP = true;break;
			case SDLK_BACKSPACE: touches->RETOUR_ARRIERE = true; break;

			case SDLK_F1: touches->F1 = true; break;
			case SDLK_F3: touches->F3 = true; break;
			case SDLK_F5: touches->F5 = true; break;

			default: break;
		}
	}

	else if(event->type == SDL_KEYUP) { // Quand une touche est relachée
		switch(event->key.keysym.sym) {
			case SDLK_UP: touches->HAUT = false; break;
			case SDLK_DOWN: touches->BAS = false; break;
			case SDLK_LEFT: touches->GAUCHE = false; break;
			case SDLK_RIGHT: touches->DROITE = false; break;

			case SDLK_a: touches->A = false; break;
			case SDLK_b: touches->B = false; break;
			case SDLK_q: touches->Q = false; break;
			case SDLK_s: touches->S = false; break;

			case SDLK_SPACE: touches->ESPACE = false; break;
			case SDLK_RETURN: touches->ENTREE = false; break;
			case SDLK_ESCAPE: touches->ECHAP = false; break;
			case SDLK_BACKSPACE: touches->RETOUR_ARRIERE = false; break;

			case SDLK_F1: touches->F1 = false; break;
			case SDLK_F3: touches->F3 = false; break;
			case SDLK_F5: touches->F5 = false; break;

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