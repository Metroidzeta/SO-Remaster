// @author Metroidzeta

#include "carte.h"

carte_t * carte_creer(char * nom, int hauteur, int largeur, chipset_t * chipset, musique_t * musique, bool depuisMatrices) {
	carte_verificationsArgs(nom,hauteur,largeur,chipset,musique);
	carte_t * carte = malloc(sizeof(carte_t));
	verifAlloc(carte,"Erreur d'allocation de la carte");

	carte->nom = strdup(nom); // il ne faut pas écrire : "carte->nom = nom;" car on ne copie alors que des adresses
	verifAllocStrCopy(carte->nom,nom);

	carte->hauteur = hauteur;
	carte->largeur = largeur;

	for(int n = 0; n < 3; n++) {
		carte->couches[n] = creerMatriceINT(hauteur,largeur,-1,"Erreur d'allocation de la matrice INT d'une des couches de la carte!");
	}
	carte->murs = creerMatriceBOOL(hauteur,largeur,false,"Erreur d'allocation de la matrice BOOL des murs de la carte!");

	carte->matriceRect = malloc(sizeof(SDL_Rect**) * hauteur);
	verifAlloc(carte->matriceRect,"Erreur d'allocation de la matrice des rectangles correspondant aux cases de la carte");

	carte->ensembleEvents = malloc(sizeof(ensemble_events_t*) * hauteur);
	verifAlloc(carte->ensembleEvents,"Erreur d'allocation de la matrice de l'ensemble des events da la carte");

	for(int i = 0; i < hauteur; i++) {
		carte->matriceRect[i] = malloc(sizeof(SDL_Rect*) * largeur);
		verifAllocLigne(carte->matriceRect[i],i,"Erreur d'allocation de la matrice des rectangles des cases de la carte");

		carte->ensembleEvents[i] = malloc(sizeof(ensemble_events_t) * largeur);
		verifAllocLigne(carte->ensembleEvents[i],i,"Erreur d'allocation de la matrice de l'ensemble des events de la carte");

		for(int j = 0; j < largeur; j++) { //      
			carte->matriceRect[i][j] = malloc(sizeof(SDL_Rect));
			verifAlloc(carte->matriceRect[i][j],"Erreur d'allocation d'un rectangle de la matrice des rectangles des cases de la carte");
			//                                      _______.x_______  _______.y_______  _____.w_____  _____.h_____
			*carte->matriceRect[i][j] = (SDL_Rect) {j * TAILLE_CASES, i * TAILLE_CASES, TAILLE_CASES, TAILLE_CASES};
			for(int p = 0; p < NB_PAGES; p++) {
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

void carte_verificationsArgs(char * nom, int hauteur, int largeur, chipset_t * chipset, musique_t * musique) {
	if(nom == NULL) { Exception("Le nom de la carte est NULL"); }
	if(nom[0] == '\0') { Exception("Le nom de la carte est vide"); }
	if(hauteur < 1) { Exception("Le hauteur de la carte est < 1"); }
	if(largeur < 1) { Exception("Le largeur de la carte est < 1"); }
	if(chipset == NULL) { Exception("Le chipset de la carte est NULL"); }
}

FILE ** ouvrirFichiersMatrices(char * nom, const char * typeOuverture) {
	FILE ** tab_fichiers = malloc(sizeof(FILE*) * 4);
	verifAlloc(tab_fichiers,"Erreur d'allocation du tableau des fichiers contenant les matrices");

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

carte_t * carte_creerDepuisMatrices(char * nom, int hauteur, int largeur, chipset_t * chipset, musique_t * musique) {
	carte_t * carte = carte_creer(nom,hauteur,largeur,chipset,musique,true);
	FILE ** tab_fichiers = ouvrirFichiersMatrices(nom,"r");
	int temp;

	for(int i = 0; i < carte->hauteur; i++) {
		for(int j = 0; j < carte->largeur; j++) {
			for(int k = 0; k < 3; k++) {
				fscanf(tab_fichiers[k],"%d",&carte->couches[k][i][j]);
			}
			fscanf(tab_fichiers[3],"%d",&temp);
			carte->murs[i][j] = (bool) temp;
		}
	}

	for(int i = 0; i < 4; i++) { fclose(tab_fichiers[i]); } // Fermeture des fichiers
	free(tab_fichiers); // Libération du tableau de pointeurs de fichiers

	return carte;
}

carte_t * carte_creerDepuisMatricesTiled(char * nom, int hauteur, int largeur, chipset_t * chipset, musique_t * musique) {
	carte_t * carte = carte_creerDepuisMatrices(nom,hauteur,largeur,chipset,musique);
	for(int i = 0; i < carte->hauteur; i++) {
		for(int j = 0; j < carte->largeur; j++) {
			for(int k = 0; k < 3; k++) {
				carte->couches[k][i][j]--;
			}
		}
	}

	return carte;
}

void carte_afficherMatriceCouche(carte_t * carte, int numCouche) {
	if(numCouche < 0 || numCouche > 2) { Exception("Le numCouche est < 0 ou > 2"); }
	printf("Couche %d :\n",numCouche);
	for(int i = 0; i < carte->hauteur; i++) {
		for(int j = 0; j < carte->largeur; j++) {
			printf("%4d ",carte->couches[numCouche][i][j]); // Affiche avec largeur fixe de 4 caratères
		}
		printf("\n");
	}
}

void carte_afficherLesMatricesCouches(carte_t * carte) {
	carte_afficherMatriceCouche(carte,0);
	carte_afficherMatriceCouche(carte,1);
	carte_afficherMatriceCouche(carte,2);
}

void carte_afficherMatriceMurs(carte_t * carte) {
	printf("Murs :\n");
	for(int i = 0; i < carte->hauteur; i++) {
		for(int j = 0; j < carte->largeur; j++) {
			printf("%d ",carte->murs[i][j]);
		}
		printf("\n");
	}
}

void carte_ecrireMatrices(carte_t * carte) {
	FILE ** tab_fichiers = ouvrirFichiersMatrices(carte->nom,"w");

	for(int i = 0; i < carte->hauteur; i++) {
		for(int j = 0; j < carte->largeur; j++) {
			for(int k = 0; k < 3; k++) {
				fprintf(tab_fichiers[k],"%4d ",carte->couches[k][i][j]); // Ecrit avec largeur fixe de 4 caratères
			}
			fprintf(tab_fichiers[3],"%d ",carte->murs[i][j]);
		}
		if(i < carte->hauteur - 1) {
			for(int a = 0; a < 4; a++) {
				fprintf(tab_fichiers[a],"\n");
			}
		}
	}

	for(int i = 0; i < 4; i++) { fclose(tab_fichiers[i]); } // Fermeture des fichiers
	free(tab_fichiers); // Libération du tableau de pointeurs de fichiers
}

bool carte_verifierLesCollisionsMurs(SDL_Rect * RectJoueur, carte_t * carte) {
	for(int i = 0; i < carte->hauteur; i++) {
		for(int j = 0; j < carte->largeur; j++) {
			if(carte->murs[i][j] && SDL_HasIntersection(RectJoueur,carte->matriceRect[i][j])) { return true; }
		}
	}
	return false;	
}

arraylist_t * carte_verifierLesCollisionsEvents(SDL_Rect * RectJoueur, carte_t * carte) {
	for(int i = 0; i < carte->hauteur; i++) {
		for(int j = 0; j < carte->largeur; j++) {
			if(!arraylist_isEmpty(carte->ensembleEvents[i][j].lesEvents[0])) {
				if(SDL_HasIntersection(RectJoueur,carte->matriceRect[i][j])) {
					return carte->ensembleEvents[i][j].lesEvents[0];
				}
			}
		}
	}
	return NULL;
}

void carte_ajouterEvent(carte_t * carte, int numPage, int xCaseSrc, int yCaseSrc, e_type type, void * evtPtr) {
	if(carte == NULL) { Exception("La carte de l'event a ajouter est NULL"); }
	if(xCaseSrc < 0 || xCaseSrc > carte->largeur - 1) { Exception("La xCaseSrc de l'event est < 0 ou > largeur - 1 de la carte"); }
	if(yCaseSrc < 0 || yCaseSrc > carte->hauteur - 1) { Exception("La yCaseSrc de l'event est < 0 ou > hauteur - 1 de la carte"); }
	if(numPage < 0 || numPage >= NB_PAGES) { Exception("Le numPage de l'event est < 0 ou >= NB_PAGES"); }
	event_t * e = event_creer(type,evtPtr);
	arraylist_add(carte->ensembleEvents[yCaseSrc][xCaseSrc].lesEvents[numPage],e);
}

void carte_detruire(carte_t * carte) { // Pas besoin de free le chipset ou la musique utilisé(e) car il est détruit dans l'arraylist lesChipsets ou lesMusiques
	for(int i = 0; i < carte->hauteur; i++) {
		for(int j = 0; j < carte->largeur; j++) {
			for(int p = 0; p < NB_PAGES; p++) {
				arraylist_detruire(carte->ensembleEvents[i][j].lesEvents[p]);
			}
			free(carte->matriceRect[i][j]);
		}
		free(carte->ensembleEvents[i]);
		free(carte->matriceRect[i]);
	}
	free(carte->ensembleEvents);
	free(carte->matriceRect);
	freeMatriceBOOL(carte->murs,carte->hauteur);
	for(int n = 0; n < 3; n++) {
		freeMatriceINT(carte->couches[n],carte->hauteur);
	}
	free(carte->nom);
	free(carte);
}