// @author Alain Barbier alias "Metroidzeta"

#include "headers/controles.h"

controles_t controles_init() { // initialiser toutes les touches sur false par défaut
	return (controles_t){0};
}

void controles_detection(SDL_Event *event, controles_t *controles, jeu_t *jeu) {
	if (event->type == SDL_KEYDOWN) { // si une touche est pressée
		switch (event->key.keysym.sym) {
			case SDLK_UP: controles->HAUT = true; break;
			case SDLK_DOWN: controles->BAS = true; break;
			case SDLK_LEFT: controles->GAUCHE = true; break;
			case SDLK_RIGHT: controles->DROITE = true; break;

			case SDLK_a: controles->A = true; break;
			case SDLK_b: controles->B = true; break;
			case SDLK_q: controles->Q = true; break;
			case SDLK_s: controles->S = true; break;

			case SDLK_SPACE: controles->ESPACE = true; break;
			case SDLK_RETURN: controles->ENTREE = true; break;
			case SDLK_ESCAPE:controles->ECHAP = true;break;
			case SDLK_BACKSPACE: controles->RETOUR_ARRIERE = true; break;

			case SDLK_F1: controles->F1 = true; break;
			case SDLK_F3: controles->F3 = true; break;
			case SDLK_F5: controles->F5 = true; break;

			default: break;
		}
	}

	else if (event->type == SDL_KEYUP) { // si une touche est relachée
		switch (event->key.keysym.sym) {
			case SDLK_UP: controles->HAUT = false; break;
			case SDLK_DOWN: controles->BAS = false; break;
			case SDLK_LEFT: controles->GAUCHE = false; break;
			case SDLK_RIGHT: controles->DROITE = false; break;

			case SDLK_a: controles->A = false; break;
			case SDLK_b: controles->B = false; break;
			case SDLK_q: controles->Q = false; break;
			case SDLK_s: controles->S = false; break;

			case SDLK_SPACE: controles->ESPACE = false; break;
			case SDLK_RETURN: controles->ENTREE = false; break;
			case SDLK_ESCAPE: controles->ECHAP = false; break;
			case SDLK_BACKSPACE: controles->RETOUR_ARRIERE = false; break;

			case SDLK_F1: controles->F1 = false; break;
			case SDLK_F3: controles->F3 = false; break;
			case SDLK_F5: controles->F5 = false; break;

			default: break;
		}
	}

	else if (event->type == SDL_TEXTINPUT) { // quand un caractère est tapé (écriture)
		//SDL_PumpEvents();
		//SDL_FlushEvents(SDL_KEYDOWN,SDL_TEXTINPUT);
		if (jeu->heros->ecritMessage && jeu->compteurLettres < TAILLE_MAX_MSG) {
			const char *src = event->text.text;
			unsigned char c = (unsigned char) src[0];
			int nbOctets = 1; // Caractère invalide -> 1 octet par sécurité

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

	else if (event->type == SDL_QUIT) { // clique sur la croix rouge fenêtre
		jeu->enCours = false;
	}
}