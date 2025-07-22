// @author Alain Barbier alias "Metroidzeta"

#include "touches.h"

touches_t touches_initialiser() { // Initialiser toutes les touches sur false par défaut
	return (touches_t) { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
}

void touches_detection(SDL_Event *event, touches_t *touches, jeu_t *jeu) {
	if (event->type == SDL_KEYDOWN) { // touche pressée
		switch (event->key.keysym.sym) {
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

	else if (event->type == SDL_KEYUP) { // Quand une touche est relachée
		switch (event->key.keysym.sym) {
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

	else if (event->type == SDL_TEXTINPUT) { // Quand un caractère est tapé (écriture)
		//SDL_PumpEvents();
		//SDL_FlushEvents(SDL_KEYDOWN,SDL_TEXTINPUT);
		if (jeu->joueur->ecritMessage && jeu->compteurLettres < TAILLE_MAX_MSG) {
			const char *src = event->text.text;
			unsigned char c = (unsigned char) src[0];
			int nbOctets = 1; // Caractère invalide → 1 octet par sécurité

			if ((c & 0x80) == 0x00) nbOctets = 1;          // 0xxxxxxx : ASCII
			else if ((c & 0xE0) == 0xC0) nbOctets = 2;     // 110xxxxx
			else if ((c & 0xF0) == 0xE0) nbOctets = 3;     // 1110xxxx
			else if ((c & 0xF8) == 0xF0) nbOctets = 4;     // 11110xxx

			// Vérifie qu'on a assez de place dans le tampon
			if (jeu->compteurLettresReelles + nbOctets < TAILLE_MAX_MSG_REELLE) {
				for (int i = 0; i < nbOctets; ++i) {
					jeu->message[0][jeu->compteurLettresReelles] = src[i];
					jeu->messageCharNbOctets[0][jeu->compteurLettres] = nbOctets;
					jeu->compteurLettresReelles++;
				}
				jeu->compteurLettres++;
				jeu->message[0][jeu->compteurLettresReelles] = '\0';
			}
		}
	}

	else if (event->type == SDL_QUIT) { // clique croix rouge fenêtre
		jeu->enCours = false;
	}
}