// @author Metroidzeta

#include "arraylist.h"
#include "joueur.h"
#include "bruitage.h"

arraylist_t * arraylist_creer(al_type altype) {
	arraylist_verificationsArgs();
	arraylist_t * a = malloc(sizeof(arraylist_t)); verifAlloc(a,"Echec d'allocation de l'arraylist");
	a->tab = malloc(sizeof(void*) * ARRAYLIST_INITIAL_CAPACITY);
	a->altype = altype;
	if(!a->tab) {
		perror("Echec d'allocation de memoire du tableau de l'arraylist");
		free(a); // on vide l'allocation précédente
		exit(EXIT_FAILURE);
	}
	a->capacite = ARRAYLIST_INITIAL_CAPACITY;
	a->taille = 0;
	return a;
}

void arraylist_verificationsArgs() {
	if(ARRAYLIST_INITIAL_CAPACITY < 1) { Exception("ARRAYLIST_INITIAL_CAPACITY < 1"); }
}

bool arraylist_isEmpty(arraylist_t * a) { return a->taille == 0; }

bool arraylist_needToEnlargeCapacity(arraylist_t * a) {
	if(a->taille == a->capacite) {
		a->capacite *= 2;
		a->tab = (void**) realloc(a->tab,sizeof(void*) * a->capacite);
		if(!a->tab) {
			perror("Echec de reallocation de memoire du tableau de l'arraylist");
			return false; // erreur
		}
	}
	return true;
}

bool arraylist_add(arraylist_t * a, void * ptr) {
	return arraylist_needToEnlargeCapacity(a) && (a->tab[a->taille++] = ptr);
}

void * arraylist_get(arraylist_t * a, int pos) {
	return (pos >= 0 && pos < a->taille) ? a->tab[pos] : NULL;
}

void arraylist_detruireElements(arraylist_t * a, bool detruireElements) {
	if(detruireElements) {
		switch(a->altype) { // Ce sont des tableaux de pointeurs
			case AL_SDL_RECT: for(int i = 0; i < a->taille; i++) { free(a->tab[i]); } break; // Si c'est un tableau de SDL_Rect
			case AL_TEXTURE: for(int i = 0; i < a->taille; i++) { SDL_DestroyTexture(a->tab[i]); } break; // Si c'est un tableau de SDL_Texture (Textures)
			case AL_SKIN: for(int i = 0; i < a->taille; i++) { skin_detruire(a->tab[i]); } break; // Si c'est un tableau de skins
			case AL_MONSTRE_DATA: for(int i = 0; i < a->taille; i++) { monstre_data_detruire(a->tab[i]); } break; // Si c'est un tableau de monstresData
			case AL_MONSTRE: for(int i = 0; i < a->taille; i++) { monstre_detruire(a->tab[i]); } break; // Si c'est un tableau de monstres
			case AL_FONT: for(int i = 0; i < a->taille; i++) { TTF_CloseFont(a->tab[i]); } break; // Si c'est un tableau de TTF_Font (Polices)
			case AL_MUSIQUE: for(int i = 0; i < a->taille; i++) { musique_detruire(a->tab[i]); } break; // Si c'est un tableau de musiques
			case AL_BRUITAGE: for(int i = 0; i < a->taille; i++) { bruitage_detruire(a->tab[i]); } break; // Si c'est un tableau de bruitages
			case AL_CHIPSET: for(int i = 0; i < a->taille; i++) { chipset_detruire(a->tab[i]); } break; // Si c'est un tableau de chipsets
			case AL_CARTE: for(int i = 0; i < a->taille; i++) { carte_detruire(a->tab[i]); } break; // Si c'est un tableau de cartes
			case AL_JOUEUR: for(int i = 0;i < a->taille; i++) { joueur_detruire(a->tab[i]); } break; // Si c'est un tableau de joueurs
			case AL_EVENT: for(int i = 0; i < a->taille; i++) { event_detruire(a->tab[i]); } break; // Si c'est un tableau d'events
			default: break; // Gérer le cas par défaut
		}
	} else {
		for(int i = 0; i < a->taille; i++) { a->tab[i] = NULL; }
	}
}

void arraylist_clear(arraylist_t * a, bool detruireElements) {
	arraylist_detruireElements(a,detruireElements);
	if(a->capacite != ARRAYLIST_INITIAL_CAPACITY) {
		a->capacite = ARRAYLIST_INITIAL_CAPACITY;
		a->tab = (void**) realloc(a->tab,sizeof(void*) * a->capacite);
	}
	a->taille = 0;
}

void arraylist_detruire(arraylist_t * a, bool detruireElements) {
	arraylist_detruireElements(a,detruireElements);
	free(a->tab);
	free(a);
}