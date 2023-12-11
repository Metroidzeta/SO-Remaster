// @author Metroidzeta

#include "arraylist.h"
#include "joueur.h"
#include "bruitage.h"

arraylist_t * arraylist_creer(al_type altype) {
	arraylist_verificationsArgs();
	arraylist_t * a = malloc(sizeof(arraylist_t));
	verifAlloc(a,"Echec d'allocation de l'arraylist");

	a->tab = malloc(sizeof(void*) * ARRAYLIST_INITIAL_CAPACITY);
	if(!a->tab) {
		perror("Echec d'allocation de memoire du tableau de l'arraylist");
		free(a); // on vide l'allocation précédente
		exit(EXIT_FAILURE);
	}
	a->altype = altype;
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

void arraylist_detruire(arraylist_t * a) {
	switch(a->altype) {
		case AL_CHIPSET: // Si c'est un tableau de chipsets
			for(int i = 0;i < a->taille;i++) { chipset_detruire(a->tab[i]); } break;

		case AL_CARTE: // Si c'est un tableau de cartes
			for(int i = 0;i < a->taille;i++) { carte_detruire(a->tab[i]); } break;

		case AL_JOUEUR: // Si c'est un tableau de joueurs
			for(int i = 0;i < a->taille;i++) { joueur_detruire(a->tab[i]); } break;

		case AL_SDL_TEXTURE: // Si c'est un tableau de SDL_Texture (Textures)
			for(int i = 0;i < a->taille;i++) { SDL_DestroyTexture(a->tab[i]); } break;

		case AL_TTF_FONT: // Si c'est un tableau de TTF_Font (Polices)
			for(int i = 0;i < a->taille;i++) { TTF_CloseFont(a->tab[i]); } break;

		case AL_MUSIQUE: // Si c'est un tableau de musiques
			for(int i = 0;i < a->taille;i++) { musique_detruire(a->tab[i]); } break;

		case AL_BRUITAGE: // Si c'est un tableau de bruitages
			for(int i = 0;i < a->taille;i++) { bruitage_detruire(a->tab[i]); } break;

		case AL_EVENT: // Si c'est un tableau d'events
			for(int i = 0;i < a->taille;i++) { event_detruire(a->tab[i]); } break;

		default: // Gérer le cas par défaut
			break;
	}
	free(a->tab);
	free(a);
}