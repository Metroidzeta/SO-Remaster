// @author Alain Barbier alias "Metroidzeta"

#ifndef BRUITAGE_H
#define BRUITAGE_H

#include "base.h"

typedef struct bruitage_s {
	char * nom;
	Mix_Chunk * son;
} bruitage_t;

bruitage_t * bruitage_creer(char * nomFichier);
void bruitage_play(bruitage_t * bruitage);
void bruitage_detruire(bruitage_t * bruitage);

#endif