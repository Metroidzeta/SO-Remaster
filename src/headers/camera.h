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

#ifndef CAMERA_H
#define CAMERA_H

#include "base.h"

typedef struct {
	double xPrec, yPrec;   // position précédente
	double xInter, yInter; // position interpolée
	int xActuel, yActuel;  // position actuelle
	int *xCible;           // pointeur vers la coordonnée X de la cible
	int *yCible;           // pointeur vers la coordonnée Y de la cible
} camera_t;

void camera_init(camera_t *cam);
void camera_setCible(camera_t *cam, int *xCible, int *yCible);
void camera_update(camera_t *cam);
void camera_interpolate(camera_t *cam, double interpolation);
void camera_sync(camera_t *cam);

#endif