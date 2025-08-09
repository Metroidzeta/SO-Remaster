// @author Alain Barbier alias "Metroidzeta"

#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include "base.h"

#define ARRAYLIST_INITIAL_CAPACITY 4

typedef enum {
	ARRAYLIST_OK = 0,
	ARRAYLIST_ERR_NULL_POINTER,
	ARRAYLIST_ERR_INVALID_INITIAL_CAPACITY,
	ARRAYLIST_ERR_MEMORY_BASE,
	ARRAYLIST_ERR_MEMORY_TAB
} arraylist_result_t;

typedef enum {
	AL_SDL_RECT,
	AL_TEXTURE, AL_SKIN,
	AL_MONSTRE_DATA,
	AL_MONSTRE,
	AL_POLICE,
	AL_MUSIQUE,
	AL_BRUITAGE,
	AL_CHIPSET,
	AL_CARTE,
	AL_HEROS,
	AL_EVENT
} al_type;

typedef struct {
	al_type altype;   // Le type des élements du tableau
	void **tab;       // Le tableau qui contient les éléments
	int capacite;     // Taille réelle (capacité de stockage) du tableau
	int taille;       // Nombre d'éléments actuellement stockés dans le tableau
} arraylist_t;

arraylist_result_t arraylist_creer(arraylist_t **out_arraylist, al_type type);
bool arraylist_isEmpty(arraylist_t * a);
void arraylist_add(arraylist_t *a, void *ptr);
void * arraylist_get(arraylist_t *a, int pos);
void arraylist_clear(arraylist_t *a, bool libererMemoireElements);
void arraylist_detruire(arraylist_t *a, bool libererMemoireElements);
const char * arraylist_strerror(arraylist_result_t res);

#endif