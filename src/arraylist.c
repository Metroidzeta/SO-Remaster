// @author Alain Barbier alias "Metroidzeta"

#include "arraylist.h"
#include "joueur.h"
#include "bruitage.h"

static void arraylist_verificationsArgs() {
	if(ARRAYLIST_INITIAL_CAPACITY < 1) { Exception("ARRAYLIST_INITIAL_CAPACITY < 1"); }
}

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

bool arraylist_isEmpty(arraylist_t * a) { return a->taille == 0; }

static void arraylist_enlargeCapacity(arraylist_t * a) {
	a->capacite *= 2;
	void ** new_tab = (void**) realloc(a->tab,sizeof(void*) * a->capacite);
	if(!new_tab) {
		perror("Echec de reallocation de memoire du tableau de l'arraylist");
		free(a->tab);
		free(a);
		exit(EXIT_FAILURE);
	}
	a->tab = new_tab;
}

void arraylist_add(arraylist_t * a, void * ptr) {
	if(a->taille == a->capacite) { arraylist_enlargeCapacity(a); }
	a->tab[a->taille++] = ptr;
}

void * arraylist_get(arraylist_t * a, int pos) {
	return (pos >= 0 && pos < a->taille) ? a->tab[pos] : NULL;
}

static void arraylist_detruireElement(void * elem, al_type type) {
	switch(type) { // C'est un pointeur vers..
		case AL_SDL_RECT: free(elem); break; // un SDL_Rect (Rectangle)
		case AL_TEXTURE: SDL_DestroyTexture(elem); break; // une SDL_Texture (Texture)
		case AL_SKIN: skin_detruire(elem); break; // un Skin
		case AL_MONSTRE_DATA: monstre_data_detruire(elem); break; // un MonstreData
		case AL_MONSTRE: monstre_detruire(elem); break; // un Monstre
		case AL_FONT: TTF_CloseFont(elem); break; // une TTF_Font (Police)
		case AL_MUSIQUE: musique_detruire(elem); break; // une Musique
		case AL_BRUITAGE: bruitage_detruire(elem); break; // un Bruitage
		case AL_CHIPSET: chipset_detruire(elem); break; // un Chipset
		case AL_CARTE: carte_detruire(elem); break; // une Carte
		case AL_JOUEUR: joueur_detruire(elem); break; // un Joueur
		case AL_EVENT: event_detruire(elem); break; // un Event
		default: break; // Gérer le cas par défaut
	}
}

void arraylist_detruireElements(arraylist_t * a, bool libererMemoireElements) {
	if(libererMemoireElements) {
		for(int i = 0; i < a->taille; i++) {
			arraylist_detruireElement(a->tab[i], a->altype);
		}
	} else {
		for(int i = 0; i < a->taille; i++) {
			a->tab[i] = NULL;
		}
	}
}

void arraylist_clear(arraylist_t * a, bool libererMemoireElements) {
	arraylist_detruireElements(a, libererMemoireElements);
	if(a->capacite != ARRAYLIST_INITIAL_CAPACITY) {
		a->capacite = ARRAYLIST_INITIAL_CAPACITY;
		a->tab = (void**) realloc(a->tab,sizeof(void*) * a->capacite);
	}
	a->taille = 0;
}

void arraylist_detruire(arraylist_t * a, bool libererMemoireElements) {
	arraylist_detruireElements(a, libererMemoireElements);
	free(a->tab);
	free(a);
}