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

#ifndef GESTIONNAIRE_GRAPHIQUES_H
#define GESTIONNAIRE_GRAPHIQUES_H

#include "jeu.h"

void effacerEcran(SDL_Renderer *renderer);
void afficherFPS_EnJeu(jeu_t *jeu);
void afficherAlignement(jeu_t *jeu);
void dessinerFiolePV(jeu_t *jeu);
void dessinerFiolePM(jeu_t *jeu);
void dessinerBarreXP(jeu_t *jeu);
void dessinerHistoriqueMessages(jeu_t *jeu);
void computePalette(jeu_t *jeu);
void dessinerMenuNavigation(jeu_t *jeu);
void dessinerMenuStatistiques(jeu_t *jeu);
void dessinerCadreMessageTeteHeros(jeu_t *jeu);
void dessinerCouche(carte_t *carte, int couche, jeu_t *jeu);
void dessinerMurs(carte_t *carte, jeu_t *jeu);
void dessinerMonstres(jeu_t *jeu);
void dessinerDegats(int nombre, SDL_Color couleur, double t, jeu_t *jeu);

#endif