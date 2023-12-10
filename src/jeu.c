// @author Metroidzeta

#include "jeu.h"

jeu_t * jeu_creer(SDL_Renderer * renderer) {
	jeu_t * jeu = malloc(sizeof(jeu_t));
	verifAlloc(jeu,"Erreur d'allocation du jeu");

	jeu->programme_actif = false;
	jeu->frames = 0;
	jeu->FPS_result = 0;
	jeu->numCouleur_cadres = 0;

	jeu->couleurs_cadres[0] = BLEU_FONCE_TRANSPARENT;
	jeu->couleurs_cadres[1] = VERT_FONCE_TRANSPARENT;
	jeu->couleurs_cadres[2] = BORDEAUX_TRANSPARENT;
	jeu->couleurs_cadres[3] = OR_FONCE_TRANSPARENT;
	jeu->couleurs_cadres[4] = GRIS_FONCE_TRANSPARENT;
	jeu->deuxTiersSeconde = 1000.0f * 2.0f / 3.0f;

	jeu->lesAffichages = arraylist_creer(AL_SDL_TEXTURE);
	jeu->lesPolices = arraylist_creer(AL_TTF_FONT);
	jeu->lesMusiques = arraylist_creer(AL_MUSIQUE);
	jeu->lesBruitages = arraylist_creer(AL_BRUITAGE);
	jeu->lesChipsets = arraylist_creer(AL_CHIPSET);
	jeu->lesCartes = arraylist_creer(AL_CARTE);

	creation_donnees(renderer,jeu);
	creation_events(jeu);

	jeu->xPosEcranJoueur = (WINDOW_WIDTH / 2) % TAILLE_CASES == 0 ? WINDOW_WIDTH / 2 : (WINDOW_WIDTH / 2) - ((WINDOW_WIDTH / 2) % TAILLE_CASES);
	jeu->yPosEcranJoueur = (WINDOW_HEIGHT / 2) % TAILLE_CASES == 0 ? WINDOW_HEIGHT / 2 : (WINDOW_HEIGHT / 2) - ((WINDOW_HEIGHT / 2) % TAILLE_CASES);
	// HIT BOX JOUEUR (A L'ECRAN)        ________.x__________  ________.y__________  _____.w_____  _____.h_____
	jeu->hitBoxEcranJoueur = (SDL_Rect) {jeu->xPosEcranJoueur, jeu->yPosEcranJoueur, TAILLE_CASES, TAILLE_CASES};

	creation_notreJoueur(renderer,jeu);

	// HIT BOX EPEE (A L'ECRAN)
	// BAS                                   _________.x_________  ____________________.y________________________  _____.w_____  ______.h________
	jeu->hit_box_epee_ecran[0] = (SDL_Rect) {jeu->xPosEcranJoueur, jeu->yPosEcranJoueur + (TAILLE_CASES / 2) + 10, TAILLE_CASES, TAILLE_CASES / 2};
	// GAUCHE                                ___________.x____________  _________.y_________  ______.w________  _____.h_____
	jeu->hit_box_epee_ecran[1] = (SDL_Rect) {jeu->xPosEcranJoueur - 10, jeu->yPosEcranJoueur, TAILLE_CASES / 2, TAILLE_CASES};
	// DROITE                                _____________________.x_______________________  _________.y_________  ______.w________  _____.h_____
	jeu->hit_box_epee_ecran[2] = (SDL_Rect) {jeu->xPosEcranJoueur + (TAILLE_CASES / 2) + 10, jeu->yPosEcranJoueur, TAILLE_CASES / 2, TAILLE_CASES};
	// HAUT                                  _________.x_________  ____________________.y________________________  _____.w_____  ______.h________
	jeu->hit_box_epee_ecran[3] = (SDL_Rect) {jeu->xPosEcranJoueur, jeu->yPosEcranJoueur - (TAILLE_CASES / 2) + 10, TAILLE_CASES, TAILLE_CASES / 2};

	for(int i = 0; i < 3; i++) { //         .x  __.y__  .w  .h
		jeu->srcRectFiolePV[i] = (SDL_Rect) {0, i * 72, 16, 72};
		jeu->srcRectFiolePM[i] = (SDL_Rect) {32, i * 72, 16, 72};
	}
	//                               .x  _________.y_________  _______.w________  _________.h_________
	jeu->dstRectFiolePV = (SDL_Rect) {1, WINDOW_HEIGHT * 0.69, WINDOW_WIDTH / 25, WINDOW_HEIGHT * 0.25};
	jeu->dstRectFiolePM = (SDL_Rect) {WINDOW_WIDTH * 0.96 - 1, WINDOW_HEIGHT * 0.69, WINDOW_WIDTH / 25, WINDOW_HEIGHT * 0.25};
	jeu->fiolesTiming = 0;
	jeu->delaiMessage = 0;

	memset(jeu->message,0,TAILLE_MAX_MSG_REELLE);
	memset(jeu->saveMessage,0,TAILLE_MAX_MSG_REELLE);
	memset(jeu->messageChar2octets,0,TAILLE_MAX_MSG_REELLE - 1);
	memset(jeu->saveMessageChar2octets,0,TAILLE_MAX_MSG_REELLE - 1);
	jeu->compteurLettres = jeu->compteurLettresReelles = 0;

	for(int i = 0; i < 3; i++) {
		memset(jeu->recapMessages[i],0,TAILLE_MAX_MSG_REELLE);
	}
	jeu->compteurRecap = 0;
	jeu->afficherRecap = 0;
	jeu->resultat_aleatoire = 0;
	jeu->degatsAffiches = 0;

	jeu->mursVisibles = jeu->menuVisible = false;
	jeu->alEventsActuels = NULL;
	jeu->nbEventPass = 0;

	return jeu;
}

void creation_donnees(SDL_Renderer * renderer, jeu_t * jeu) {
	// Création de l'affichage (ptr renderer, chemin, ptr jeu)
	ajouterAffichage(renderer,"img/fioles.png",jeu); // affichage 0 (fioles)

	// Création des polices (chemin, taillePolice, ptr jeu)
	ajouterPolice("polices/arial.ttf",WINDOW_WIDTH / 80,jeu); // font 0 (FPS)
	ajouterPolice("polices/arial.ttf",TAILLE_CASES * 0.68,jeu); // font 1 (Arial, texte normal)
	ajouterPolice("polices/Zelda Breath of the Wild.otf",WINDOW_WIDTH / 28,jeu); // font 2 (Coups d'attaque)

	// Création des musiques (nom, chemin, ptr jeu)
	ajouterMusique("Castle_1","musiques/Castle_1.mp3",jeu); // musique 0
	ajouterMusique("Sarosa","musiques/Sarosa.mp3",jeu); // musique 1
	ajouterMusique("bahamut_lagoon","musiques/bahamut_lagoon.mp3",jeu); // musique 2
	ajouterMusique("Castle_3","musiques/Castle_3.mp3",jeu); // musique 3
	ajouterMusique("2000_ordeal","musiques/2000_ordeal.mp3",jeu); // musique 4
	ajouterMusique("cc_viper_manor","musiques/cc_viper_manor.mp3",jeu); // musique 5
	ajouterMusique("suikoden-ii-two-rivers","musiques/suikoden-ii-two-rivers.mp3",jeu); // musique 6
	ajouterMusique("mystery3","musiques/mystery3.ogg",jeu); // musique 7
	ajouterMusique("hunter","musiques/hunter.ogg",jeu); // musique 8
	ajouterMusique("illusionary_world","musiques/illusionary_world.mp3",jeu); // musique 9
	ajouterMusique("chapt1medfill","musiques/chapt1medfill.mp3",jeu); // musique 10

	// Création des bruitages (chemin, ptr jeu)
	ajouterBruitage("Blow1","bruitages/Blow1.wav",jeu); // bruitage 0
	ajouterBruitage("Kill1","bruitages/Kill1.wav",jeu); // bruitage 1
	ajouterBruitage("Damage3","bruitages/Damage3.ogg",jeu); // bruitage 2

	// Création des chipsets (ptr renderer, nom, tailleBloc, chemin, ptr jeu)
	ajouterChipset(renderer,"BZ",16,"img/BZ.png",jeu); // chipset 0
	ajouterChipset(renderer,"VillageTangaFinal",16,"img/VillageTangaFinal.png",jeu); // chipset 1
	ajouterChipset(renderer,"grey_cas42",16,"img/grey_cas42.png",jeu); // chipset 2
	ajouterChipset(renderer,"PalaisRoland2",16,"img/PalaisRoland2.png",jeu); // chipset 3
	ajouterChipset(renderer,"PalaisRolandInt",16,"img/PalaisRolandInt.png",jeu); // chipset 4
	ajouterChipset(renderer,"PalaisRolandNouveau",48,"img/PalaisRolandNouveau.png",jeu); // chipset 5
	ajouterChipset(renderer,"MaraisTanga",16,"img/MaraisTanga.png",jeu); // chipset 6
	ajouterChipset(renderer,"marais2",16,"img/marais2.png",jeu); // chipset 7
	ajouterChipset(renderer,"Coacville_exterieur",16,"img/Coacville_exterieur.png",jeu); // chipset 8
	ajouterChipset(renderer,"chipset173",16,"img/chipset173.png",jeu); // chipset 9
	ajouterChipset(renderer,"chipset175",16,"img/chipset175.png",jeu); // chipset 10
	ajouterChipset(renderer,"HunterArene",16,"img/HunterArene.png",jeu); // chipset 11
	ajouterChipset(renderer,"grass",32,"img/grass.png",jeu); // chipset 12
	ajouterChipset(renderer,"chipset5c",16,"img/chipset5c.png",jeu); // chipset 13

	// Création des cartes (nom, hauteur, largeur, ptr chipset, ptr musique, ptr jeu)
	ajouterCarte("Sarosa_Milice_Accueil",10,20,getChipset2(jeu,"BZ"),getMusique2(jeu,"Castle_1"),jeu); // carte 0
	ajouterCarte("Sarosa",50,50,getChipset2(jeu,"VillageTangaFinal"),getMusique2(jeu,"Sarosa"),jeu); // carte 1
	ajouterCarte("Chateau_Roland_Exterieur",35,50,getChipset2(jeu,"grey_cas42"),getMusique2(jeu,"bahamut_lagoon"),jeu); // carte 2
	ajouterCarte("Chateau_Roland_Cour_Interieure",20,15,getChipset2(jeu,"PalaisRoland2"),getMusique2(jeu,"Castle_3"),jeu); // carte 3
	ajouterCarte("Chateau_Roland_Etage_01",12,15,getChipset2(jeu,"PalaisRolandInt"),getMusique2(jeu,"Castle_3"),jeu); // carte 4
	ajouterCarte("Chateau_Roland_Salle_Trone",22,19,getChipset2(jeu,"PalaisRoland2"),getMusique2(jeu,"2000_ordeal"),jeu); // carte 5
	ajouterCarte("Chateau_Roland_Salle_Trone_Nouveau",23,19,getChipset2(jeu,"PalaisRolandNouveau"),getMusique2(jeu,"2000_ordeal"),jeu); // carte 6
	ajouterCarte("Sarosa_Foret_Est",30,50,getChipset2(jeu,"VillageTangaFinal"),getMusique2(jeu,"bahamut_lagoon"),jeu); // carte 7
	ajouterCarte("Marais_Coacville",55,70,getChipset2(jeu,"MaraisTanga"),getMusique2(jeu,"cc_viper_manor"),jeu); // carte 8
	ajouterCarte("Coacville_Marais_Sud",18,20,getChipset2(jeu,"marais2"),getMusique2(jeu,"cc_viper_manor"),jeu); // carte 9
	ajouterCarte("Coacville_Marecage_Sud",40,30,getChipset2(jeu,"marais2"),getMusique2(jeu,"cc_viper_manor"),jeu); // carte 10
	ajouterCarte("Coacville",40,43,getChipset2(jeu,"Coacville_exterieur"),getMusique2(jeu,"suikoden-ii-two-rivers"),jeu); // carte 11
	ajouterCarte("Coacville_Marecage_Nord",50,40,getChipset2(jeu,"marais2"),getMusique2(jeu,"cc_viper_manor"),jeu); // carte 12
	ajouterCarte("Coacville_Donjon_Exterieur",40,25,getChipset2(jeu,"chipset173"),getMusique2(jeu,"cc_viper_manor"),jeu); // carte 13
	ajouterCarte("Donjon1_Entree",15,20,getChipset2(jeu,"chipset175"),getMusique2(jeu,"mystery3"),jeu); // carte 14
	ajouterCarte("Donjon1_salle5",15,15,getChipset2(jeu,"chipset175"),getMusique2(jeu,"mystery3"),jeu); // carte 15
	ajouterCarte("Arene_Hunter",32,29,getChipset2(jeu,"HunterArene"),getMusique2(jeu,"hunter"),jeu); // carte 16
	ajouterCarte("carte17",20,20,getChipset2(jeu,"grass"),getMusique2(jeu,"illusionary_world"),jeu); // carte 17
	ajouterCarte("Foret_Sud_Sarosa",31,50,getChipset2(jeu,"chipset5c"),getMusique2(jeu,"chapt1medfill"),jeu); // carte 18
}

carte_t * getCarte2(jeu_t * jeu, char * nom) {
	for(int i = 0; i < jeu->lesCartes->taille; i++) {
		carte_t * carte = arraylist_get(jeu->lesCartes,i);
		if(strcmp(carte->nom,nom) == 0) {
			return carte;
		}
	}
	return NULL;
}

chipset_t * getChipset2(jeu_t * jeu, char * nom) {
	for(int i = 0; i < jeu->lesChipsets->taille; i++) {
		chipset_t * chipset = arraylist_get(jeu->lesChipsets,i);
		if(strcmp(chipset->nom,nom) == 0) {
			return chipset;
		}
	}
	return NULL;
}

musique_t * getMusique2(jeu_t * jeu, char * nom) {
	for(int i = 0; i < jeu->lesMusiques->taille; i++) {
		musique_t * musique = arraylist_get(jeu->lesMusiques,i);
		if(strcmp(musique->nom,nom) == 0) {
			return musique;
		}
	}
	return NULL;
}

void creation_events(jeu_t * jeu) {
	// Création des Events   

	// Création des messages (carte, numPage, {xCase, yCase}, e_type = E_MESSAGE, event_creerMsg(message))
	carte_ajouterEvent(getCarte2(jeu,"Sarosa"),0,24,10,E_MESSAGE,event_creerMsg("Les coups critiques sont affichés en rouge et infligent 200% de ton attaque de base"));
	carte_ajouterEvent(getCarte2(jeu,"Donjon1_salle5"),0,7,3,E_MESSAGE,event_creerMsg("Metroidzeta : Malheureusement l'aventure s'arrête ici."));
	carte_ajouterEvent(getCarte2(jeu,"Donjon1_salle5"),0,7,3,E_MESSAGE,event_creerMsg("Metroidzeta : Le jeu est entièrement codé en C avec la bibliothèque SDL2, rien à voir avec RPG Maker."));
	carte_ajouterEvent(getCarte2(jeu,"Donjon1_salle5"),0,7,3,E_MESSAGE,event_creerMsg("Metroidzeta : Est-ce que ça te plaît ? Tu peux participer au projet si tu le souhaites ;)"));

	// Création des téléportations (carte, numPage, {xCaseSrc, yCaseSrc}, e_type = E_TELEPORTATION, event_creerTP({xCaseDst, yCaseDst}, numCarteDst))
	carte_ajouterEvent(getCarte2(jeu,"Sarosa_Milice_Accueil"),0,8,3,E_TELEPORTATION,event_creerTP(11,15,getCarte2(jeu,"Sarosa")));
	carte_ajouterEvent(getCarte2(jeu,"Sarosa"),0,10,15,E_TELEPORTATION,event_creerTP(8,4,getCarte2(jeu,"Sarosa_Milice_Accueil")));
	carte_ajouterEvent(getCarte2(jeu,"Sarosa"),0,23,0,E_TELEPORTATION,event_creerTP(25,33,getCarte2(jeu,"Chateau_Roland_Exterieur")));
	carte_ajouterEvent(getCarte2(jeu,"Sarosa"),0,49,27,E_TELEPORTATION,event_creerTP(1,13,getCarte2(jeu,"Sarosa_Foret_Est")));
	carte_ajouterEvent(getCarte2(jeu,"Sarosa"),0,26,49,E_TELEPORTATION,event_creerTP(28,1,getCarte2(jeu,"Foret_Sud_Sarosa")));
	carte_ajouterEvent(getCarte2(jeu,"Chateau_Roland_Exterieur"),0,25,34,E_TELEPORTATION,event_creerTP(23,1,getCarte2(jeu,"Sarosa")));
	carte_ajouterEvent(getCarte2(jeu,"Chateau_Roland_Exterieur"),0,25,31,E_TELEPORTATION,event_creerTP(7,18,getCarte2(jeu,"Chateau_Roland_Cour_Interieure")));
	carte_ajouterEvent(getCarte2(jeu,"Chateau_Roland_Exterieur"),0,25,6,E_TELEPORTATION,event_creerTP(7,5,getCarte2(jeu,"Chateau_Roland_Cour_Interieure")));
	carte_ajouterEvent(getCarte2(jeu,"Chateau_Roland_Exterieur"),0,25,21,E_TELEPORTATION,event_creerTP(7,9,getCarte2(jeu,"Chateau_Roland_Etage_01")));
	carte_ajouterEvent(getCarte2(jeu,"Chateau_Roland_Cour_Interieure"),0,7,19,E_TELEPORTATION,event_creerTP(25,32,getCarte2(jeu,"Chateau_Roland_Exterieur")));
	carte_ajouterEvent(getCarte2(jeu,"Chateau_Roland_Cour_Interieure"),0,0,7,E_TELEPORTATION,event_creerTP(1,9,getCarte2(jeu,"Chateau_Roland_Etage_01")));
	carte_ajouterEvent(getCarte2(jeu,"Chateau_Roland_Cour_Interieure"),0,0,8,E_TELEPORTATION,event_creerTP(1,9,getCarte2(jeu,"Chateau_Roland_Etage_01")));
	carte_ajouterEvent(getCarte2(jeu,"Chateau_Roland_Cour_Interieure"),0,14,7,E_TELEPORTATION,event_creerTP(13,9,getCarte2(jeu,"Chateau_Roland_Etage_01")));
	carte_ajouterEvent(getCarte2(jeu,"Chateau_Roland_Cour_Interieure"),0,14,8,E_TELEPORTATION,event_creerTP(13,9,getCarte2(jeu,"Chateau_Roland_Etage_01")));
	carte_ajouterEvent(getCarte2(jeu,"Chateau_Roland_Cour_Interieure"),0,14,13,E_TELEPORTATION,event_creerTP(1,17,getCarte2(jeu,"Arene_Hunter")));
	carte_ajouterEvent(getCarte2(jeu,"Chateau_Roland_Cour_Interieure"),0,14,14,E_TELEPORTATION,event_creerTP(1,17,getCarte2(jeu,"Arene_Hunter")));
	carte_ajouterEvent(getCarte2(jeu,"Chateau_Roland_Cour_Interieure"),0,7,4,E_TELEPORTATION,event_creerTP(25,5,getCarte2(jeu,"Chateau_Roland_Exterieur")));
	carte_ajouterEvent(getCarte2(jeu,"Chateau_Roland_Etage_01"),0,0,10,E_TELEPORTATION,event_creerTP(1,9,getCarte2(jeu,"Chateau_Roland_Cour_Interieure")));
	carte_ajouterEvent(getCarte2(jeu,"Chateau_Roland_Etage_01"),0,14,10,E_TELEPORTATION,event_creerTP(13,9,getCarte2(jeu,"Chateau_Roland_Cour_Interieure")));
	carte_ajouterEvent(getCarte2(jeu,"Chateau_Roland_Etage_01"),0,7,10,E_TELEPORTATION,event_creerTP(25,22,getCarte2(jeu,"Chateau_Roland_Exterieur")));
	carte_ajouterEvent(getCarte2(jeu,"Chateau_Roland_Etage_01"),0,6,3,E_TELEPORTATION,event_creerTP(9,20,getCarte2(jeu,"Chateau_Roland_Salle_Trone_Nouveau")));
	carte_ajouterEvent(getCarte2(jeu,"Chateau_Roland_Etage_01"),0,7,3,E_TELEPORTATION,event_creerTP(9,20,getCarte2(jeu,"Chateau_Roland_Salle_Trone_Nouveau")));
	carte_ajouterEvent(getCarte2(jeu,"Chateau_Roland_Etage_01"),0,8,3,E_TELEPORTATION,event_creerTP(9,20,getCarte2(jeu,"Chateau_Roland_Salle_Trone_Nouveau")));
	carte_ajouterEvent(getCarte2(jeu,"Chateau_Roland_Salle_Trone"),0,8,20,E_TELEPORTATION,event_creerTP(1,7,getCarte2(jeu,"Chateau_Roland_Cour_Interieure")));
	carte_ajouterEvent(getCarte2(jeu,"Chateau_Roland_Salle_Trone"),0,9,20,E_TELEPORTATION,event_creerTP(1,7,getCarte2(jeu,"Chateau_Roland_Cour_Interieure")));
	carte_ajouterEvent(getCarte2(jeu,"Chateau_Roland_Salle_Trone"),0,10,20,E_TELEPORTATION,event_creerTP(1,7,getCarte2(jeu,"Chateau_Roland_Cour_Interieure")));
	carte_ajouterEvent(getCarte2(jeu,"Chateau_Roland_Salle_Trone_Nouveau"),0,8,21,E_TELEPORTATION,event_creerTP(7,4,getCarte2(jeu,"Chateau_Roland_Etage_01")));
	carte_ajouterEvent(getCarte2(jeu,"Chateau_Roland_Salle_Trone_Nouveau"),0,9,21,E_TELEPORTATION,event_creerTP(7,4,getCarte2(jeu,"Chateau_Roland_Etage_01")));
	carte_ajouterEvent(getCarte2(jeu,"Chateau_Roland_Salle_Trone_Nouveau"),0,10,21,E_TELEPORTATION,event_creerTP(7,4,getCarte2(jeu,"Chateau_Roland_Etage_01")));
	carte_ajouterEvent(getCarte2(jeu,"Sarosa_Foret_Est"),0,0,13,E_TELEPORTATION,event_creerTP(48,27,getCarte2(jeu,"Sarosa")));
	carte_ajouterEvent(getCarte2(jeu,"Sarosa_Foret_Est"),0,25,29,E_TELEPORTATION,event_creerTP(19,1,getCarte2(jeu,"Marais_Coacville")));
	carte_ajouterEvent(getCarte2(jeu,"Marais_Coacville"),0,19,0,E_TELEPORTATION,event_creerTP(25,28,getCarte2(jeu,"Sarosa_Foret_Est")));
	carte_ajouterEvent(getCarte2(jeu,"Marais_Coacville"),0,69,42,E_TELEPORTATION,event_creerTP(1,15,getCarte2(jeu,"Coacville_Marais_Sud")));
	carte_ajouterEvent(getCarte2(jeu,"Coacville_Marais_Sud"),0,0,15,E_TELEPORTATION,event_creerTP(68,42,getCarte2(jeu,"Marais_Coacville")));
	carte_ajouterEvent(getCarte2(jeu,"Coacville_Marais_Sud"),0,8,5,E_TELEPORTATION,event_creerTP(17,38,getCarte2(jeu,"Coacville_Marecage_Sud")));
	carte_ajouterEvent(getCarte2(jeu,"Coacville_Marecage_Sud"),0,17,39,E_TELEPORTATION,event_creerTP(8,6,getCarte2(jeu,"Coacville_Marais_Sud")));
	carte_ajouterEvent(getCarte2(jeu,"Coacville_Marecage_Sud"),0,15,0,E_TELEPORTATION,event_creerTP(16,38,getCarte2(jeu,"Coacville")));
	carte_ajouterEvent(getCarte2(jeu,"Coacville"),0,16,39,E_TELEPORTATION,event_creerTP(15,1,getCarte2(jeu,"Coacville_Marecage_Sud")));
	carte_ajouterEvent(getCarte2(jeu,"Coacville"),0,18,0,E_TELEPORTATION,event_creerTP(22,48,getCarte2(jeu,"Coacville_Marecage_Nord")));
	carte_ajouterEvent(getCarte2(jeu,"Coacville_Marecage_Nord"),0,22,49,E_TELEPORTATION,event_creerTP(18,1,getCarte2(jeu,"Coacville")));
	carte_ajouterEvent(getCarte2(jeu,"Coacville_Marecage_Nord"),0,20,0,E_TELEPORTATION,event_creerTP(11,38,getCarte2(jeu,"Coacville_Donjon_Exterieur")));
	carte_ajouterEvent(getCarte2(jeu,"Coacville_Donjon_Exterieur"),0,11,39,E_TELEPORTATION,event_creerTP(20,1,getCarte2(jeu,"Coacville_Marecage_Nord")));
	carte_ajouterEvent(getCarte2(jeu,"Coacville_Donjon_Exterieur"),0,11,5,E_TELEPORTATION,event_creerTP(9,13,getCarte2(jeu,"Donjon1_Entree")));
	carte_ajouterEvent(getCarte2(jeu,"Coacville_Donjon_Exterieur"),0,0,9,E_TELEPORTATION,event_creerTP(18,9,getCarte2(jeu,"carte17")));
	carte_ajouterEvent(getCarte2(jeu,"Donjon1_Entree"),0,8,14,E_TELEPORTATION,event_creerTP(11,6,getCarte2(jeu,"Coacville_Donjon_Exterieur")));
	carte_ajouterEvent(getCarte2(jeu,"Donjon1_Entree"),0,9,14,E_TELEPORTATION,event_creerTP(11,6,getCarte2(jeu,"Coacville_Donjon_Exterieur")));
	carte_ajouterEvent(getCarte2(jeu,"Donjon1_Entree"),0,10,14,E_TELEPORTATION,event_creerTP(11,6,getCarte2(jeu,"Coacville_Donjon_Exterieur")));
	carte_ajouterEvent(getCarte2(jeu,"Donjon1_Entree"),0,8,3,E_TELEPORTATION,event_creerTP(7,13,getCarte2(jeu,"Donjon1_salle5")));
	carte_ajouterEvent(getCarte2(jeu,"Donjon1_salle5"),0,6,14,E_TELEPORTATION,event_creerTP(8,4,getCarte2(jeu,"Donjon1_Entree")));
	carte_ajouterEvent(getCarte2(jeu,"Donjon1_salle5"),0,7,14,E_TELEPORTATION,event_creerTP(8,4,getCarte2(jeu,"Donjon1_Entree")));
	carte_ajouterEvent(getCarte2(jeu,"Donjon1_salle5"),0,8,14,E_TELEPORTATION,event_creerTP(8,4,getCarte2(jeu,"Donjon1_Entree")));
	carte_ajouterEvent(getCarte2(jeu,"Arene_Hunter"),0,0,17,E_TELEPORTATION,event_creerTP(13,14,getCarte2(jeu,"Chateau_Roland_Cour_Interieure")));
	carte_ajouterEvent(getCarte2(jeu,"Arene_Hunter"),0,28,17,E_TELEPORTATION,event_creerTP(1,9,getCarte2(jeu,"carte17")));
	carte_ajouterEvent(getCarte2(jeu,"carte17"),0,19,9,E_TELEPORTATION,event_creerTP(1,9,getCarte2(jeu,"Coacville_Donjon_Exterieur")));
	carte_ajouterEvent(getCarte2(jeu,"carte17"),0,0,9,E_TELEPORTATION,event_creerTP(27,17,getCarte2(jeu,"Arene_Hunter")));
	carte_ajouterEvent(getCarte2(jeu,"Foret_Sud_Sarosa"),0,28,0,E_TELEPORTATION,event_creerTP(26,48,getCarte2(jeu,"Sarosa")));

	// Création des jouer musiques (carte, numPage, {xCaseSrc, yCaseSrc}, e_type = E_JOUER_MUSIQUE, event_creerJM(numMusique))
	carte_ajouterEvent(getCarte2(jeu,"Sarosa"),0,6,17,E_MESSAGE,event_creerMsg("La musique \"Hunter\" sera joué après ce message"));
	carte_ajouterEvent(getCarte2(jeu,"Sarosa"),0,6,17,E_JOUER_MUSIQUE,event_creerJM(getMusique2(jeu,"hunter")));
	carte_ajouterEvent(getCarte2(jeu,"Sarosa"),0,6,17,E_MESSAGE,event_creerMsg("Arrêt de la musique \"Hunter\" après ce message"));
	carte_ajouterEvent(getCarte2(jeu,"Sarosa"),0,6,17,E_ARRETER_MUSIQUE,NULL);
	carte_ajouterEvent(getCarte2(jeu,"Sarosa"),0,6,17,E_MESSAGE,event_creerMsg("On remet la musique de \"Sarosa\" après ce message"));
	carte_ajouterEvent(getCarte2(jeu,"Sarosa"),0,6,17,E_JOUER_MUSIQUE,event_creerJM(getMusique2(jeu,"Sarosa")));
}

void jeu_updateOffSetJoueur(jeu_t * jeu) {
	jeu->xOffSetJoueur = jeu->xPosEcranJoueur - jeu->joueur->x;
	jeu->yOffSetJoueur = jeu->yPosEcranJoueur - jeu->joueur->y;
}

void creation_notreJoueur(SDL_Renderer * renderer, jeu_t * jeu) { // Création de notre joueur
	FILE * fichier_pseudo = ouvrirFichier("PSEUDO.txt","r","Impossible d'ouvrir le fichier PSEUDO.txt (en lecture)");
	char nomJoueur[26];
	fscanf(fichier_pseudo,"%25s",nomJoueur);
	fclose(fichier_pseudo);

	if(nomJoueur[0] == 0) { strcpy(nomJoueur,"Test"); }
	jeu->joueur = joueur_creer(renderer,nomJoueur,VOLEUR,1,1000,"img/Evil.png",12,12,getPolice(jeu,1),getCarte2(jeu,"Chateau_Roland_Cour_Interieure"),10);

	SDL_QueryTexture(jeu->joueur->nomTexture,NULL,NULL,&jeu->rectPseudo.w,&jeu->rectPseudo.h);
	jeu->rectPseudo.x = jeu->xPosEcranJoueur - (jeu->rectPseudo.w / 2) + (TAILLE_CASES / 2) - 2;
	jeu->rectPseudo.y = jeu->yPosEcranJoueur + TAILLE_CASES - 2;

	jeu_updateOffSetJoueur(jeu);
	jeu->musiqueActuelle = jeu->joueur->carteActuelle->musique;
}

void ajouterChipset(SDL_Renderer * renderer, char * nom, int tailleBloc, const char * chemin, jeu_t * jeu) { arraylist_add(jeu->lesChipsets,chipset_creer(renderer,nom,tailleBloc,chemin)); }
void ajouterCarte(char * nom, int hauteur, int largeur, chipset_t * chipset, musique_t * musique, jeu_t * jeu) { arraylist_add(jeu->lesCartes,carte_creerDepuisMatrices(nom,hauteur,largeur,chipset,musique)); }
void ajouterCarteVide(char * nom, int hauteur, int largeur, chipset_t * chipset, musique_t * musique, jeu_t * jeu) { arraylist_add(jeu->lesCartes,carte_creer(nom,hauteur,largeur,chipset,musique,false)); }
void ajouterAffichage(SDL_Renderer * renderer, const char * chemin, jeu_t * jeu) { arraylist_add(jeu->lesAffichages,creerTextureImage(renderer,chemin)); }
void ajouterPolice(const char * chemin, int taille, jeu_t * jeu) { arraylist_add(jeu->lesPolices,creerPolice(chemin,taille)); }
void ajouterMusique(char * nom, const char * chemin, jeu_t * jeu) { arraylist_add(jeu->lesMusiques,musique_creer(nom,chemin)); }
void ajouterBruitage(char * nom, const char * chemin, jeu_t * jeu) { arraylist_add(jeu->lesBruitages,bruitage_creer(nom,chemin)); }

chipset_t * getChipset(jeu_t * jeu, int pos) { return arraylist_get(jeu->lesChipsets,pos); }
carte_t * getCarte(jeu_t * jeu, int pos) { return arraylist_get(jeu->lesCartes,pos); }
SDL_Texture * getAffichage(jeu_t * jeu, int pos) { return arraylist_get(jeu->lesAffichages,pos); }
TTF_Font * getPolice(jeu_t * jeu, int pos) { return arraylist_get(jeu->lesPolices,pos); }
musique_t * getMusique(jeu_t * jeu, int pos) { return arraylist_get(jeu->lesMusiques,pos); }
bruitage_t * getBruitage(jeu_t * jeu, int pos) { return arraylist_get(jeu->lesBruitages,pos); }
event_t * getEventActuel(jeu_t * jeu, int pos) { return arraylist_get(jeu->alEventsActuels,pos); }

void musique_stopAndPlay(musique_t * musiqueActuelle, musique_t * musiqueSuivante) {
	if(musiqueActuelle != NULL) { musique_stop(musiqueActuelle); }
	if(musiqueSuivante != NULL) { musique_play(musiqueSuivante); }
}

void viderMessage(jeu_t * jeu) {
	memset(jeu->message,0,TAILLE_MAX_MSG_REELLE);
	jeu->compteurLettres = jeu->compteurLettresReelles = 0;
}

void sauvegarderMessage(jeu_t * jeu) {
	strcpy(jeu->saveMessage,jeu->message);
	bool_array_copy(jeu->saveMessageChar2octets,jeu->messageChar2octets,TAILLE_MAX_MSG_REELLE - 1);
}

void remettreDernierMessage(jeu_t * jeu) {
	strcpy(jeu->message,jeu->saveMessage);
	bool_array_copy(jeu->messageChar2octets,jeu->saveMessageChar2octets,TAILLE_MAX_MSG_REELLE - 1);
	jeu->compteurLettresReelles = strlen(jeu->message);
	int compteur = 0;
	for(int i = 0; i < jeu->compteurLettresReelles; i++) {
		if(!jeu->messageChar2octets[i]) {
			compteur++;
		}
	}
	jeu->compteurLettres = compteur;
}

void ajouterMessageHistorique(jeu_t * jeu) {
	if(jeu->compteurRecap == 3) {
		for(int i = 0; i < 2; i++) {
			strcpy(jeu->recapMessages[i],jeu->recapMessages[i + 1]);
		}
		jeu->compteurRecap = 2;
	}
	strcpy(jeu->recapMessages[jeu->compteurRecap],jeu->message);
	jeu->compteurRecap++;
}

void afficherHitboxAttaqueEpee(SDL_Renderer * renderer, jeu_t * jeu) {
	dessinerRectangle(renderer,&jeu->hit_box_epee_ecran[jeu->joueur->direction],BLANC_TRANSPARENT);
}

void afficherFiolePV(SDL_Renderer * renderer, jeu_t * jeu) {
	dessinerTexture(renderer,getAffichage(jeu,0),&jeu->srcRectFiolePV[jeu->fiolesTiming],&jeu->dstRectFiolePV,"Impossible de dessiner la fiole PV avec SDL_RenderCopy");
}

void afficherFiolePM(SDL_Renderer * renderer, jeu_t * jeu) {
	dessinerTexture(renderer,getAffichage(jeu,0),&jeu->srcRectFiolePM[jeu->fiolesTiming],&jeu->dstRectFiolePM,"Impossible de dessiner la fiole PM avec SDL_RenderCopy");
}

void afficherCouche(SDL_Renderer * renderer, carte_t * carte, int couche, jeu_t * jeu) {
	int tailleBloc = carte->chipset->tailleBloc;
	int nbBlocsEnLargeur = carte->chipset->nbBlocsEnLargeur;
	SDL_Rect srcRect = {0, 0, tailleBloc, tailleBloc};
	SDL_Rect dstRect = {0, 0, TAILLE_CASES, TAILLE_CASES};
	int joueurXCase = joueur_getXCase(jeu->joueur);
	int joueurYCase = joueur_getYCase(jeu->joueur);

	int numero;
	int i_debut = (joueurYCase - WINDOW_HEIGHT_CASES_DIV2) < 0 ? 0 : (joueurYCase - WINDOW_HEIGHT_CASES_DIV2);
	int j_debut = (joueurXCase - WINDOW_WIDTH_CASES_DIV2) < 0 ? 0 : (joueurXCase - WINDOW_WIDTH_CASES_DIV2);
	int i_fin = (joueurYCase + WINDOW_HEIGHT_CASES_DIV2 + 3) > carte->hauteur ? carte->hauteur : (joueurYCase + WINDOW_HEIGHT_CASES_DIV2 + 3);
	int j_fin = (joueurXCase + WINDOW_WIDTH_CASES_DIV2 + 3) > carte->largeur ? carte->largeur : (joueurXCase + WINDOW_WIDTH_CASES_DIV2 + 3);

	//printf("i_debut = %d, i_fin = %d, j_debut = %d, j_fin = %d\n", i_debut, i_fin, j_debut, j_fin);
	//SDL_GetWindowSurface(window);

	for(int i = i_debut; i < i_fin; i++) {
		for(int j = j_debut; j < j_fin; j++) {
			numero = carte->couches[couche][i][j];
			if(numero != -1) {
				srcRect.x = (numero % nbBlocsEnLargeur) * tailleBloc;
				srcRect.y = (numero / nbBlocsEnLargeur) * tailleBloc;

				dstRect.x = j * TAILLE_CASES + jeu->xOffSetJoueur;
				dstRect.y = i * TAILLE_CASES + jeu->yOffSetJoueur;

				dessinerTexture(renderer,carte->chipset->imageChipset,&srcRect,&dstRect,"Erreur : Impossible de dessiner une case texture chipset sur une couche avec SDL_RenderCopy");
			}
		}
	}
}

void afficherMurs(SDL_Renderer * renderer, carte_t * carte, jeu_t * jeu) {
	SDL_Rect rect = {0, 0, TAILLE_CASES, TAILLE_CASES};
	for(int i = 0; i < carte->hauteur; i++) {
		for(int j = 0; j < carte->largeur; j++) {
			if(carte->murs[i][j]) {
				rect.x = j * TAILLE_CASES + jeu->xOffSetJoueur;
				rect.y = i * TAILLE_CASES + jeu->yOffSetJoueur;
				dessinerRectangle(renderer,&rect,VIOLET_TRANSPARENT);
			}
		}
	}
}

void jeu_detruire(jeu_t * jeu) {
	joueur_detruire(jeu->joueur);
	arraylist_detruire(jeu->lesCartes);
	arraylist_detruire(jeu->lesChipsets);
	arraylist_detruire(jeu->lesBruitages);
	arraylist_detruire(jeu->lesMusiques);
	arraylist_detruire(jeu->lesPolices);
	arraylist_detruire(jeu->lesAffichages);
	free(jeu);
}