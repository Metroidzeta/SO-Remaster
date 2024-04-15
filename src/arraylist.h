// @author Alain Barbier alias "Metroidzeta"

#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include "base.h"

#define ARRAYLIST_INITIAL_CAPACITY 4

typedef enum {AL_SDL_RECT, AL_TEXTURE, AL_SKIN, AL_MONSTRE_DATA, AL_MONSTRE, AL_FONT,
AL_MUSIQUE, AL_BRUITAGE, AL_CHIPSET, AL_CARTE, AL_JOUEUR, AL_EVENT} al_type;

typedef struct arraylist_s {
	al_type altype;   // Le type des élements du tableau
	void ** tab;      // Le tableau qui contient les éléments
	int capacite;     // Taille réelle (capacité de stockage) du tableau
	int taille;       // Nombre d'éléments actuellement stockés dans le tableau
} arraylist_t;

arraylist_t * arraylist_creer(al_type altype);
void arraylist_verificationsArgs();
bool arraylist_isEmpty(arraylist_t * a);
bool arraylist_needToEnlargeCapacity(arraylist_t * a);
bool arraylist_add(arraylist_t * a, void * ptr);
void * arraylist_get(arraylist_t * a, int pos);
void arraylist_detruireElements(arraylist_t * a, bool detruireElements);
void arraylist_clear(arraylist_t * a, bool detruireElements);
void arraylist_detruire(arraylist_t * a, bool detruireElements);

#endif