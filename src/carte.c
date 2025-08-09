// @author Alain Barbier alias "Metroidzeta"

#include "headers/carte.h"

static carte_result_t carte_validerArguments(const char *nom, int largeur, int hauteur, chipset_t *chipset) {
    if (!nom || !*nom) return CARTE_ERR_NULL_OR_EMPTY_NAME;
	if (strlen(nom) >= MAX_TAILLE_STRING) return CARTE_ERR_SIZE_MAX_NAME;
    if (largeur < 1 || largeur > TAILLE_CARTE_MAX) return CARTE_ERR_INVALID_LARGEUR;
    if (hauteur < 1 || hauteur > TAILLE_CARTE_MAX) return CARTE_ERR_INVALID_HAUTEUR;
    if (!chipset) return CARTE_ERR_NULL_CHIPSET;
	return CARTE_OK;
}

carte_result_t carte_creer(carte_t **out_carte, const char *nom, int largeur, int hauteur, chipset_t *chipset, musique_t *musique, bool depuisFichiers) {
	if (!out_carte) return CARTE_ERR_NULL_POINTER;
	*out_carte = NULL;

	carte_result_t res = carte_validerArguments(nom, largeur, hauteur, chipset);
	if (res != CARTE_OK) return res;

	carte_t *carte = calloc(1, sizeof(carte_t));
	if (!carte) return CARTE_ERR_MEMORY_BASE;

	carte->nom = my_strdup(nom); // important : ne pas faire "carte->nom = nom", car cela ne copie que le pointeur, pas le contenu
	if (!carte->nom) { carte_detruire(carte); return CARTE_ERR_MEMORY_NAME; }

	carte->largeur = largeur;
	carte->hauteur = hauteur;
	carte->chipset = chipset;
	carte->musique = musique;

	// Allocation des couches
	for (int c = 0; c < 3; ++c) {
		carte->couches[c] = creerMatriceINT(hauteur, largeur, -1);
		if (!carte->couches[c]) { carte_detruire(carte); Exception("Échec creation matriceINT couches"); }
	}

	// Allocation des autres matrices
	carte->murs = creerMatriceBOOL(hauteur, largeur, false);
	if (!carte->murs) { carte_detruire(carte); Exception("Echec creation matriceBOOL murs"); }

	carte->matriceRect = malloc(hauteur * sizeof(SDL_Rect *));
	if (!carte->matriceRect) { carte_detruire(carte); Exception("Echec creation matrice rectangles"); }

	carte->ensembleEvents = malloc(hauteur * sizeof(ensemble_events_t *));
	if (!carte->ensembleEvents) { carte_detruire(carte); Exception("Echec creation matrice ensemblesEvents"); }

	arraylist_result_t resAL = arraylist_creer(&carte->monstres, AL_MONSTRE);
	if (resAL != ARRAYLIST_OK) { carte_detruire(carte); Exception("Echec creation arraylist monstres"); }

	for (int i = 0; i < hauteur; ++i) {
		carte->matriceRect[i] = malloc(largeur * sizeof(SDL_Rect)); 
		if (!carte->matriceRect[i]) { carte_detruire(carte); Exception("Echec creation lignes matrice rectangles"); }

		carte->ensembleEvents[i] = malloc(largeur * sizeof(ensemble_events_t));
		if (!carte->ensembleEvents[i]) { carte_detruire(carte); Exception("Echec creation lignes matrice ensemblesEvents"); }
		for (int j = 0; j < largeur; ++j) { //      _______.x_______  _______.y_______  _____.w_____  _____.h_____
			carte->matriceRect[i][j] = (SDL_Rect) { j * TAILLE_CASES, i * TAILLE_CASES, TAILLE_CASES, TAILLE_CASES };
			for (int p = 0; p < NB_PAGES_EVENT; ++p) {
				carte->ensembleEvents[i][j].lesEvents[p] = NULL;
				resAL = arraylist_creer(&carte->ensembleEvents[i][j].lesEvents[p], AL_EVENT);
				if (resAL != ARRAYLIST_OK) { carte_detruire(carte); Exception("Echec creation arraylist d'une page d'events"); }
			}
		}
	}
	if (depuisFichiers) initMatricesDepuisFichiers(carte);
	else carte_ecrireMatrices(carte);

	*out_carte = carte;
	return CARTE_OK;
}

static FILE ** ouvrirFichiersMatrices(const char *nom, const char *mode) {
	if (!nom || !*nom) Exception("Nom NULL ou vide dans ouvrirFichiersMatrices");
	if (!mode|| !*mode) Exception("Mode d'ouverture NULL ou vide dans ouvrirFichiersMatrices");

	FILE **fichiers = malloc(4 * sizeof(FILE *));
	if (!fichiers) Exception("Echec creation tableau fichiers contenant matrices carte");

	char nomFichier_couches[15 + strlen(nom)]; // "cartes/" + (le nom de la carte) + "_Ci.txt" + "\0"
	for (int i = 0; i < 3; ++i) {
		snprintf(nomFichier_couches, sizeof(nomFichier_couches), "cartes/%s_C%d.txt", nom, i);
		fichiers[i] = fopen(nomFichier_couches, mode);
		if (!fichiers[i]) { for (int j = 0; j < i; ++j) fclose(fichiers[j]); free(fichiers); Exception("Echec ouverture d'un fichier matrice couche"); }
	}

	char nomFichier_murs[17 + strlen(nom)]; // "cartes/" + (le nom de la carte) + "_Murs.txt" + "\0"
	snprintf(nomFichier_murs, sizeof(nomFichier_murs), "cartes/%s_Murs.txt", nom);
	fichiers[3] = fopen(nomFichier_murs, mode);
	if (!fichiers[3]) { for (int i = 0; i < 3; ++i) fclose(fichiers[i]); free(fichiers); Exception("Echec ouverture d'un fichier matrice murs"); }

	return fichiers;
}

void initMatricesDepuisFichiers(carte_t *carte) {
	FILE **fichiers = ouvrirFichiersMatrices(carte->nom, "r");

	for (int i = 0; i < carte->hauteur; ++i) {
		for (int j = 0; j < carte->largeur; ++j) {
			for (int c = 0; c < 3; ++c) {
				if (fscanf(fichiers[c], "%d", &carte->couches[c][i][j]) != 1) Exception("Lecture entier fichier couche échouée");
			}
			int temp;
			if (fscanf(fichiers[3], "%d", &temp) != 1) Exception("Lecture entier fichier murs échouée");
			carte->murs[i][j] = (bool)temp;
		}
	}

	for (int f = 0; f < 4; ++f) fclose(fichiers[f]); // fermeture fichiers
	free(fichiers);
}

carte_result_t carte_creerDepuisMatricesTiled(carte_t **out_carte, const char *nom, int largeur, int hauteur, chipset_t *chipset, musique_t *musique) {
	carte_result_t res = carte_creer(out_carte, nom, largeur, hauteur, chipset, musique, true);
	if (res != CARTE_OK) return res;
	carte_t *carte = *out_carte;
	for (int i = 0; i < carte->hauteur; ++i) {
		for (int j = 0; j < carte->largeur; ++j) {
			for (int c = 0; c < 3; ++c) carte->couches[c][i][j]--;
		}
	}
	return CARTE_OK;
}

void carte_ecrireMatrices(carte_t *carte) {
	FILE **fichiers = ouvrirFichiersMatrices(carte->nom, "w");

	for (int i = 0; i < carte->hauteur; ++i) {
		for (int j = 0; j < carte->largeur; ++j) {
			for (int c = 0; c < 3; ++c) fprintf(fichiers[c], "%4d ", carte->couches[c][i][j]); // écrit avec largeur fixe de 4 caratères
			fprintf(fichiers[3], "%d ", carte->murs[i][j]);
		}
		if (i < carte->hauteur - 1) {
			for (int f = 0; f < 4; ++f) fprintf(fichiers[f], "\n");
		}
	}

	for(int f = 0; f < 4; ++f) fclose(fichiers[f]); // fermeture fichiers
	free(fichiers);
}

static void calculerBornesCollisions(const carte_t *carte, const SDL_Rect *rect, int *x0, int *x1, int *y0, int *y1) {
	*x0 = maxInt(0, rect->x / TAILLE_CASES);
	*x1 = minInt(carte->largeur - 1, (rect->x + rect->w) / TAILLE_CASES);
	*y0 = maxInt(0, rect->y / TAILLE_CASES);
	*y1 = minInt(carte->hauteur - 1, (rect->y + rect->h) / TAILLE_CASES);
	//printf("x0 = %d, x1 = %d, y0 = %d, y1 = %d\n", x0, x1, y0, y1);
}

bool carte_verifierCollisionsMurs(carte_t *carte, SDL_Rect *hitBox) {
	int x0, x1, y0, y1;
	calculerBornesCollisions(carte, hitBox, &x0, &x1, &y0, &y1);
	for (int i = y0; i <= y1; ++i) {
		for (int j = x0; j <= x1; ++j) {
			if (carte->murs[i][j] && SDL_HasIntersection(hitBox, &carte->matriceRect[i][j])) return true;
		}
	}
	return false;
}

arraylist_t * carte_verifierCollisionsEvents(carte_t *carte, SDL_Rect *hitBox) {
	int x0, x1, y0, y1;
	calculerBornesCollisions(carte, hitBox, &x0, &x1, &y0, &y1);
	for (int i = y0; i <= y1; ++i) {
		for (int j = x0; j <= x1; ++j) {
			if (!arraylist_isEmpty(carte->ensembleEvents[i][j].lesEvents[0]) && SDL_HasIntersection(hitBox, &carte->matriceRect[i][j])) {
				return carte->ensembleEvents[i][j].lesEvents[0];
			}
		}
	}
	return NULL;
}

void carte_ajouterEvent(carte_t *carte, int numPage, int xCase, int yCase, e_type type, void *evtPtr) {
	if (!carte) Exception("Event ajoute sur une carte NULL");
	if (xCase < 0 || xCase >= carte->largeur) Exception("xCase event ajoute < 0 ou >= largeur carte");
	if (yCase < 0 || yCase >= carte->hauteur) Exception("yCase event ajoute < 0 ou >= hauteur carte");
	if (numPage < 0 || numPage >= NB_PAGES_EVENT) Exception("NumPage event ajoute < 0 ou >= NB_PAGES_EVENT");

	event_t *e = event_creer(type, evtPtr);
	arraylist_add(carte->ensembleEvents[yCase][xCase].lesEvents[numPage], e);
}

void carte_ajouterMonstre(carte_t *carte, monstre_t *monstre) {
	if (!monstre) Exception("Monstre NULL ajoute");
	if (monstre_getXCase(monstre) >= carte->largeur) Exception("xCase monstre >= largeur carte");
	if (monstre_getYCase(monstre) >= carte->hauteur) Exception("yCase monstre >= hauteur carte");

	arraylist_add(carte->monstres, monstre);
}

void carte_detruire(carte_t *carte) { // Ne pas libérer carte->chipset et carte->musique : partagée, allouée ailleurs
	if (!carte) return;
	for (int i = 0; i < carte->hauteur; ++i) {
		for (int j = 0; j < carte->largeur; ++j) {
			for (int p = 0; p < NB_PAGES_EVENT; ++p) {
				arraylist_detruire(carte->ensembleEvents[i][j].lesEvents[p], true);
			}
		}
		free(carte->ensembleEvents[i]);
		free(carte->matriceRect[i]);
	}
	free(carte->ensembleEvents);
	arraylist_detruire(carte->monstres, true);
	free(carte->matriceRect);
	freeMatriceBOOL(carte->murs, carte->hauteur);
	for (int c = 0; c < 3; ++c) freeMatriceINT(carte->couches[c], carte->hauteur);
	free(carte->nom);
	free(carte);
}

const char * carte_strerror(carte_result_t res) {
	switch (res) {
		case CARTE_OK: return "Succes";
		case CARTE_ERR_NULL_POINTER: return "Carte NULL passe en parametre";
		case CARTE_ERR_NULL_OR_EMPTY_NAME: return "Nom NULL ou vide";
		case CARTE_ERR_SIZE_MAX_NAME: return "Nom trop long";
		case CARTE_ERR_INVALID_LARGEUR: return "Largeur < 1 ou > TAILLE_CARTE_MAX";
		case CARTE_ERR_INVALID_HAUTEUR: return "Hauteur < 1 ou > TAILLE_CARTE_MAX";
		case CARTE_ERR_NULL_CHIPSET: return "Chipset NULL passe en parametre";
		case CARTE_ERR_MEMORY_BASE: return "Echec allocation memoire base";
		case CARTE_ERR_MEMORY_NAME: return "Echec allocation memoire nom";
		default: return "Erreur";
	}
}