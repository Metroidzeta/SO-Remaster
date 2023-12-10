// @author Metroidzeta

#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include "base.h"

#define ARRAYLIST_INITIAL_CAPACITY 4

//                 0          1          2             3             4            5           6           7
typedef enum { AL_CHIPSET, AL_CARTE, AL_JOUEUR, AL_SDL_TEXTURE, AL_TTF_FONT, AL_MUSIQUE, AL_BRUITAGE, AL_EVENT} al_type;

typedef struct arraylist_s {
	void ** tab;      // Le tableau qui contient les éléments
	int capacite;     // Taille réelle (capacité de stockage) du tableau
	int taille;       // Nombre d'éléments actuellement stockés dans le tableau
	al_type altype;   // Le type des élements du tableau
} arraylist_t;

arraylist_t * arraylist_creer(al_type altype);
void arraylist_verificationsArgs();
bool arraylist_isEmpty(arraylist_t * a);
bool arraylist_needToEnlargeCapacity(arraylist_t * a);
bool arraylist_add(arraylist_t * a, void * ptr);
void * arraylist_get(arraylist_t * a, int pos);
void arraylist_detruire(arraylist_t * a);

#endif