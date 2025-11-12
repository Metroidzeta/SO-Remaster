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

#include "headers/camera.h"

void camera_init(camera_t *cam) {
	if (!cam) return;
	cam->xPrec = cam->yPrec = 0.0;
	cam->xInter = cam->yInter = 0.0;
	cam->xActuel = cam->yActuel = 0;
	cam->xCible = cam->yCible = NULL;
}

// Définit la cible suivie (ex: heros.x, heros.y)
void camera_setCible(camera_t *cam, int *xCible, int *yCible) {
	if (!cam) return;
	cam->xCible = xCible;
	cam->yCible = yCible;

	if (xCible && yCible) {
		cam->xActuel = *xCible;
		cam->yActuel = *yCible;
		camera_sync(cam);
	}
}

// Met à jour les positions préc./actuelles à chaque tick logique
void camera_update(camera_t *cam) {
	if (!cam || !cam->xCible || !cam->yCible) return;
	cam->xPrec = cam->xActuel;
	cam->yPrec = cam->yActuel;
	cam->xActuel = *cam->xCible;
	cam->yActuel = *cam->yCible;
}

// Interpolation fluide entre positions
void camera_interpolate(camera_t *cam, double interpolation) {
	if (!cam) return;
	cam->xInter = cam->xPrec + (cam->xActuel - cam->xPrec) * interpolation;
	cam->yInter = cam->yPrec + (cam->yActuel - cam->yPrec) * interpolation;
}

// Synchronisation complète (utile après TP ou changement de carte)
void camera_sync(camera_t *cam) {
	if (!cam) return;
	cam->xPrec = cam->xActuel;
	cam->yPrec = cam->yActuel;
	cam->xInter = cam->xActuel;
	cam->yInter = cam->yActuel;
}