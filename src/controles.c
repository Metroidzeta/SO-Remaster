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

#include "headers/controles.h"

controles_t controles_init(void) { return (controles_t){0}; } // initialiser toutes les touches sur false par défaut

void controles_detection(SDL_Event *event, controles_t *controles, jeu_t *jeu) {
	if (!event || !controles || !jeu) return;
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
		//SDL_FlushEvents(SDL_KEYDOWN, SDL_TEXTINPUT);
		if (jeu->heros->estEnTrainDEcrire && jeu->compteurLettres < TAILLE_MAX_MSG) {
			const char *src = event->text.text;
			int i = 0;

			while (src[i] != '\0') {
				unsigned char c = (unsigned char) src[i];
				int nbOctets = 1;

				if ((c & 0x80) == 0x00) nbOctets = 1;       // ASCII
				else if ((c & 0xE0) == 0xC0) nbOctets = 2;  // UTF-8 2 octets
				else if ((c & 0xF0) == 0xE0) nbOctets = 3;  // UTF-8 3 octets
				else if ((c & 0xF8) == 0xF0) nbOctets = 4;  // UTF-8 4 octets

				// Vérifie place disponible (caractères + '\0')
				if (jeu->compteurLettresReelles + nbOctets > TAILLE_MAX_MSG_REELLE - 1 || jeu->compteurLettres >= TAILLE_MAX_MSG) break;

				// Copier les octets dans le buffer
				for (int j = 0; j < nbOctets; j++) jeu->message[0][jeu->compteurLettresReelles + j] = src[i + j];

				// Stocker le nombre d’octets du caractère
				jeu->messageCharNbOctets[0][jeu->compteurLettres] = nbOctets;

				jeu->compteurLettres++;
				jeu->compteurLettresReelles += nbOctets;

				i += nbOctets; // passer au caractère suivant
			}

			// Terminer correctement la chaîne
			jeu->message[0][jeu->compteurLettresReelles] = '\0';

			// Nettoyer le reste du buffer
			memset(jeu->message[0] + jeu->compteurLettresReelles, 0, TAILLE_MAX_MSG_REELLE - jeu->compteurLettresReelles);
		}
	}

	else if (event->type == SDL_QUIT) { // le clic sur la croix rouge de la fenêtre
		jeu->enCours = false;
	}
}