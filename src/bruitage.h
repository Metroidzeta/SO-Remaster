// @author Metroidzeta

#ifndef BRUITAGE_H
#define BRUITAGE_H

#include "base.h"

typedef struct bruitage_s {
	char * nom;         // Nom du bruitage
	Mix_Chunk * son;    // Le son
} bruitage_t;

bruitage_t * bruitage_creer(char * nom, const char * chemin);
void bruitage_verificationsArgs(char * nom);
void bruitage_play(bruitage_t * bruitage);
void bruitage_detruire(bruitage_t * bruitage);

#endif