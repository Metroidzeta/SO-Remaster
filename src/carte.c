// @author Alain Barbier alias "Metroidzeta"

#include "carte.h"

static void carte_verificationsArgs(const char * nom, int largeur, int hauteur, chipset_t * chipset) {
	if(nom == NULL) { Exception("Le nom de la carte est NULL"); }
	if(nom[0] == '\0') { Exception("Le nom de la carte est vide"); }
	if(largeur < 1) { Exception("Le largeur de la carte est < 1"); }
	if(hauteur < 1) { Exception("Le hauteur de la carte est < 1"); }
	if(chipset == NULL) { Exception("Le chipset de la carte est NULL"); }
}

carte_t * carte_creer(const char * nom, int largeur, int hauteur, chipset_t * chipset, musique_t * musique, bool depuisMatrices) {
	carte_verificationsArgs(nom,largeur,hauteur,chipset);
	carte_t * carte = malloc(sizeof(carte_t)); verifAlloc(carte,"Erreur d'allocation de la carte");
	carte->nom = strdup(nom); verifAllocStrCopy(carte->nom,nom); // il ne faut pas écrire : "carte->nom = nom;" car on ne copie alors que des adresses
	carte->largeur = largeur;
	carte->hauteur = hauteur;
	for(int c = 0; c < 3; c++) {
		carte->couches[c] = creerMatriceINT(hauteur,largeur,-1,"Erreur d'allocation de la matrice INT d'une des couches de la carte!");
	}
	carte->murs = creerMatriceBOOL(hauteur,largeur,false,"Erreur d'allocation de la matrice BOOL des murs de la carte!");
	carte->matriceRect = malloc(sizeof(SDL_Rect*) * hauteur); verifAlloc(carte->matriceRect,"Erreur d'allocation de la matrice des rectangles correspondant aux cases de la carte");
	carte->ensembleEvents = malloc(sizeof(ensemble_events_t*) * hauteur); verifAlloc(carte->ensembleEvents,"Erreur d'allocation de la matrice de l'ensemble des events da la carte");
	carte->lesMonstres = arraylist_creer(AL_MONSTRE);

	for(int i = 0; i < hauteur; i++) {
		carte->matriceRect[i] = malloc(sizeof(SDL_Rect) * largeur); verifAllocLigne(carte->matriceRect[i],i,"Erreur d'allocation de la matrice des rectangles des cases de la carte");
		carte->ensembleEvents[i] = malloc(sizeof(ensemble_events_t) * largeur); verifAllocLigne(carte->ensembleEvents[i],i,"Erreur d'allocation de la matrice de l'ensemble des events de la carte");
		for(int j = 0; j < largeur; j++) { //      _______.x_______  _______.y_______  _____.w_____  _____.h_____
			carte->matriceRect[i][j] = (SDL_Rect) {j * TAILLE_CASES, i * TAILLE_CASES, TAILLE_CASES, TAILLE_CASES};
			for(int p = 0; p < NB_PAGES_EVENT; p++) {
				carte->ensembleEvents[i][j].lesEvents[p] = arraylist_creer(AL_EVENT);
			}
		}
	}
	carte->chipset = chipset;
	carte->musique = musique;
	if(!depuisMatrices) { // Si la carte n'a été créé à partir de fichiers de matrices
		carte_ecrireMatrices(carte);
	}
	return carte;
}

static FILE ** ouvrirFichiersMatrices(const char * nom, const char * typeOuverture) {
	FILE ** tab_fichiers = malloc(sizeof(FILE*) * 4); verifAlloc(tab_fichiers,"Erreur d'allocation du tableau des fichiers contenant les matrices");

	char nom_fichier_couches[7 + strlen(nom) + 7 + 1]; // "cartes/" + (le nom de la carte) + "_Ci.txt" + "\0"
	for(int i = 0; i < 3; i++) {
		snprintf(nom_fichier_couches,sizeof(nom_fichier_couches),"cartes/%s_C%d.txt",nom,i);
		tab_fichiers[i] = ouvrirFichier(nom_fichier_couches,typeOuverture,"Impossible d'ouvrir le fichier de la matrice INT d'une des couches");
	}

	char nom_fichier_murs[7 + strlen(nom) + 9 + 1]; // "cartes/" + (le nom de la carte) + "_Murs.txt" + "\0"
	snprintf(nom_fichier_murs,sizeof(nom_fichier_murs),"cartes/%s_Murs.txt",nom);
	tab_fichiers[3] = ouvrirFichier(nom_fichier_murs,typeOuverture,"Impossible d'ouvrir le fichier de la matrice BOOL des murs");

	return tab_fichiers;
}

carte_t * carte_creerDepuisMatrices(const char * nom, int largeur, int hauteur, chipset_t * chipset, musique_t * musique) {
	carte_t * carte = carte_creer(nom,largeur,hauteur,chipset,musique,true);
	FILE ** tab_fichiers = ouvrirFichiersMatrices(nom,"r");
	int temp;

	for(int i = 0; i < carte->hauteur; i++) {
		for(int j = 0; j < carte->largeur; j++) {
			for(int c = 0; c < 3; c++) {
				fscanf(tab_fichiers[c],"%d",&carte->couches[c][i][j]);
			}
			fscanf(tab_fichiers[3],"%d",&temp);
			carte->murs[i][j] = (bool) temp;
		}
	}

	for(int f = 0; f < 4; f++) { fclose(tab_fichiers[f]); } // Fermeture des fichiers
	free(tab_fichiers); // Libération du tableau de pointeurs de fichiers

	return carte;
}

carte_t * carte_creerDepuisMatricesTiled(const char * nom, int largeur, int hauteur, chipset_t * chipset, musique_t * musique) {
	carte_t * carte = carte_creerDepuisMatrices(nom,largeur,hauteur,chipset,musique);
	for(int i = 0; i < carte->hauteur; i++) {
		for(int j = 0; j < carte->largeur; j++) {
			for(int c = 0; c < 3; c++) {
				carte->couches[c][i][j]--;
			}
		}
	}

	return carte;
}

void carte_ecrireMatrices(carte_t * carte) {
	FILE ** tab_fichiers = ouvrirFichiersMatrices(carte->nom,"w");

	for(int i = 0; i < carte->hauteur; i++) {
		for(int j = 0; j < carte->largeur; j++) {
			for(int c = 0; c < 3; c++) {
				fprintf(tab_fichiers[c],"%4d ",carte->couches[c][i][j]); // Ecrit avec largeur fixe de 4 caratères
			}
			fprintf(tab_fichiers[3],"%d ",carte->murs[i][j]);
		}
		if(i < carte->hauteur - 1) {
			for(int a = 0; a < 4; a++) {
				fprintf(tab_fichiers[a],"\n");
			}
		}
	}

	for(int f = 0; f < 4; f++) { fclose(tab_fichiers[f]); } // Fermeture des fichiers
	free(tab_fichiers); // Libération du tableau de pointeurs de fichiers
}

bool carte_verifierLesCollisionsMurs(carte_t * carte, SDL_Rect * hitBox) {
	for(int i = 0; i < carte->hauteur; i++) {
		for(int j = 0; j < carte->largeur; j++) {
			if(carte->murs[i][j] && SDL_HasIntersection(hitBox,&carte->matriceRect[i][j])) { return true; }
		}
	}
	return false;	
}

arraylist_t * carte_verifierLesCollisionsEvents(carte_t * carte, SDL_Rect * hitBox) {
	for(int i = 0; i < carte->hauteur; i++) {
		for(int j = 0; j < carte->largeur; j++) {
			if(!arraylist_isEmpty(carte->ensembleEvents[i][j].lesEvents[0]) && SDL_HasIntersection(hitBox,&carte->matriceRect[i][j])) {
				return carte->ensembleEvents[i][j].lesEvents[0];
			}
		}
	}
	return NULL;
}

static void carte_verificationsArgs_ajouterEvent(carte_t * carte, int numPage, int xCase, int yCase) {
	if(carte == NULL) { Exception("La carte de l'event a ajouter est NULL"); }
	if(xCase < 0 || xCase > carte->largeur - 1) { Exception("La xCase de l'event est < 0 ou > largeur - 1 de la carte"); }
	if(yCase < 0 || yCase > carte->hauteur - 1) { Exception("La yCase de l'event est < 0 ou > hauteur - 1 de la carte"); }
	if(numPage < 0 || numPage >= NB_PAGES_EVENT) { Exception("Le numPage de l'event est < 0 ou >= NB_PAGES"); }
}

void carte_ajouterEvent(carte_t * carte, int numPage, int xCase, int yCase, e_type type, void * evtPtr) {
	carte_verificationsArgs_ajouterEvent(carte, numPage, xCase, yCase);
	event_t * e = event_creer(type, evtPtr);
	arraylist_add(carte->ensembleEvents[yCase][xCase].lesEvents[numPage],e);
}

void carte_ajouterMonstre(carte_t * carte, monstre_t * monstre) {
	if(monstre == NULL) { Exception("La monstre a ajouter dans la carte est NULL"); }
	if(monstre_getXCase(monstre) > carte->largeur - 1) { Exception("La xCase du monstre > largeur - 1 de la carte"); }
	if(monstre_getYCase(monstre) > carte->hauteur - 1) { Exception("La yCase du monstre > hauteur - 1 de la carte"); }
	arraylist_add(carte->lesMonstres,monstre);
}

void carte_detruire(carte_t * carte) { // Pas besoin de free le chipset ou la musique utilisé(e) car il/elle est détruit(e) dans l'arraylist lesChipsets/lesMusiques
	for(int i = 0; i < carte->hauteur; i++) {
		for(int j = 0; j < carte->largeur; j++) {
			for(int p = 0; p < NB_PAGES_EVENT; p++) {
				arraylist_detruire(carte->ensembleEvents[i][j].lesEvents[p],true);
			}
		}
		free(carte->ensembleEvents[i]);
		free(carte->matriceRect[i]);
	}
	free(carte->ensembleEvents);
	arraylist_detruire(carte->lesMonstres,true);
	free(carte->matriceRect);
	freeMatriceBOOL(carte->murs,carte->hauteur);
	for(int c = 0; c < 3; c++) {
		freeMatriceINT(carte->couches[c],carte->hauteur);
	}
	free(carte->nom);
	free(carte);
}