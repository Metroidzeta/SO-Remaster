// @author Alain Barbier alias "Metroidzeta"

#include "arraylist.h"
#include "joueur.h"
#include "bruitage.h"

static void arraylist_validerArguments() {
	if (ARRAYLIST_INITIAL_CAPACITY < 1) Exception("ARRAYLIST_INITIAL_CAPACITY < 1");
}

arraylist_t * arraylist_creer(al_type altype) {
	arraylist_validerArguments();

	arraylist_t *a = malloc(sizeof(arraylist_t));
	if (!a) Exception("Echec creation arraylist");

	a->tab = malloc(ARRAYLIST_INITIAL_CAPACITY * sizeof(void *));
	if (!a->tab) { free(a); Exception("Echec creation tableau arraylist"); }

	a->altype = altype;
	a->capacite = ARRAYLIST_INITIAL_CAPACITY;
	a->taille = 0;
	return a;
}

bool arraylist_isEmpty(arraylist_t *a) { return !a || a->taille == 0; }

static void arraylist_enlargeCapacity(arraylist_t *a) {
	const int newCapacite = a->capacite * 2;
	void **newTab = (void**) realloc(a->tab, newCapacite * sizeof(void *));
	if (!newTab) Exception("Echec réallocation tableau arraylist");
	a->tab = newTab;
	a->capacite = newCapacite;
}

void arraylist_add(arraylist_t *a, void *ptr) {
	if (!a || !ptr) return;
	if (a->taille == a->capacite) arraylist_enlargeCapacity(a);
	a->tab[a->taille++] = ptr;
}

void * arraylist_get(arraylist_t *a, int pos) {
	return (a && pos >= 0 && pos < a->taille) ? a->tab[pos] : NULL;
}

static void arraylist_detruireElement(void *elem, al_type type) {
	switch (type) { // un pointeur vers..
		case AL_SDL_RECT:       free(elem); break; // SDL_Rect
		case AL_TEXTURE:        SDL_DestroyTexture(elem); break; // SDL_Texture
		case AL_SKIN:           skin_detruire(elem); break; // Skin
		case AL_MONSTRE_DATA:   monstreData_detruire(elem); break; // MonstreData
		case AL_MONSTRE:        monstre_detruire(elem); break; // Monstre
		case AL_FONT:           TTF_CloseFont(elem); break; // TTF_Font (Police)
		case AL_MUSIQUE:        musique_detruire(elem); break; // Musique
		case AL_BRUITAGE:       bruitage_detruire(elem); break; // Bruitage
		case AL_CHIPSET:        chipset_detruire(elem); break; // Chipset
		case AL_CARTE:          carte_detruire(elem); break; // Carte
		case AL_JOUEUR:         joueur_detruire(elem); break; // Joueur
		case AL_EVENT:          event_detruire(elem); break; // Event
		default: break;
	}
}

static void arraylist_detruireElements(arraylist_t *a, bool libererMemoireElements) {
	for (int i = 0; i < a->taille; ++i) {
		if (libererMemoireElements) arraylist_detruireElement(a->tab[i], a->altype);
		a->tab[i] = NULL;
	}
}

void arraylist_clear(arraylist_t *a, bool libererMemoireElements) {
	if (!a) return;
	arraylist_detruireElements(a, libererMemoireElements);

	if (a->capacite != ARRAYLIST_INITIAL_CAPACITY) {
		void **newTab = (void **) realloc(a->tab, ARRAYLIST_INITIAL_CAPACITY * sizeof(void *));
		if (!newTab) Exception("Echec réallocation tableau arraylist");
		a->tab = newTab;
		a->capacite = ARRAYLIST_INITIAL_CAPACITY;
	}
	a->taille = 0;
}

void arraylist_detruire(arraylist_t *a, bool libererMemoireElements) {
	if (!a) return;
	arraylist_detruireElements(a, libererMemoireElements);
	free(a->tab);
	free(a);
}