// @author Alain Barbier alias "Metroidzeta"

#include "jeu.h"

jeu_t * jeu_creer(SDL_Renderer * renderer) {
	jeu_t * jeu = malloc(sizeof(jeu_t)); verifAlloc(jeu,"Erreur d'allocation du jeu");

	jeu->programme_actif = false;
	jeu->frames = jeu->FPS_result = jeu->numCouleur_cadres = 0;

	jeu->couleurs_cadres[0] = BLEU_FONCE_TRANSPARENT;
	jeu->couleurs_cadres[1] = VERT_FONCE_TRANSPARENT;
	jeu->couleurs_cadres[2] = BORDEAUX_TRANSPARENT;
	jeu->couleurs_cadres[3] = OR_FONCE_TRANSPARENT;
	jeu->couleurs_cadres[4] = GRIS_FONCE_TRANSPARENT;
	jeu->deuxTiersSeconde = 1000.0f * 2.0f / 3.0f;

	creation_donnees(renderer,jeu);
	creation_events(jeu);
	creation_monstres(jeu);

	int xJoueurEcran = (WINDOW_WIDTH / 2) % TAILLE_CASES == 0 ? WINDOW_WIDTH / 2 : (WINDOW_WIDTH / 2) - ((WINDOW_WIDTH / 2) % TAILLE_CASES);
	int yJoueurEcran = (WINDOW_HEIGHT / 2) % TAILLE_CASES == 0 ? WINDOW_HEIGHT / 2 : (WINDOW_HEIGHT / 2) - ((WINDOW_HEIGHT / 2) % TAILLE_CASES);
	// HIT BOX JOUEUR (A L'ECRAN)        _____.x_____  _____.y_____  _____.w_____  _____.h_____
	jeu->hitBoxJoueurEcran = (SDL_Rect) {xJoueurEcran, yJoueurEcran, TAILLE_CASES, TAILLE_CASES};

	creation_notreJoueur(renderer,jeu);

	// HIT BOX EPEE (A L'ECRAN)                   _____.x_____  ________________.y____________________  _____.w_____  ______.h________
	jeu->hitBoxEpeeJoueurEcran[BAS] = (SDL_Rect) {xJoueurEcran, yJoueurEcran + (TAILLE_CASES / 2) + 10, TAILLE_CASES, TAILLE_CASES / 2};
	jeu->hitBoxEpeeJoueurEcran[GAUCHE] = (SDL_Rect) {xJoueurEcran - 10, yJoueurEcran, TAILLE_CASES / 2, TAILLE_CASES};
	jeu->hitBoxEpeeJoueurEcran[DROITE] = (SDL_Rect) {xJoueurEcran + (TAILLE_CASES / 2) + 10, yJoueurEcran, TAILLE_CASES / 2, TAILLE_CASES};
	jeu->hitBoxEpeeJoueurEcran[HAUT] = (SDL_Rect) {xJoueurEcran, yJoueurEcran - (TAILLE_CASES / 2) + 10, TAILLE_CASES, TAILLE_CASES / 2};

	for(int i = 0; i < 2; i++) { //         .x  _______________.y________________   _______.w________  _______________.h________________
		jeu->dstRectFiolePV[i] = (SDL_Rect) {1, WINDOW_HEIGHT * (0.69 + i * 0.073), WINDOW_WIDTH / 25, WINDOW_HEIGHT * (0.25 - i * 0.072)};
		jeu->dstRectFiolePM[i] = (SDL_Rect) {WINDOW_WIDTH * 0.96 - 1, WINDOW_HEIGHT * (0.69 + i * 0.073), WINDOW_WIDTH / 25, WINDOW_HEIGHT * (0.25 - i * 0.072)};
		memset(jeu->message[i],0,TAILLE_MAX_MSG_REELLE);
		memset(jeu->messageChar2octets[i],0,TAILLE_MAX_MSG_REELLE - 1);
		for(int j = 0; j < 3; j++) { //             __.x__  _______.y______  .w  ____.h_____
			jeu->srcRectFiolePV[i][j] = (SDL_Rect) {i * 16, i * 21 + j * 72, 16, 72 - i * 21};
			jeu->srcRectFiolePM[i][j] = (SDL_Rect) {32 + i * 16, i * 21 + j * 72, 16, 72 - i * 21};
		}
	}
	jeu->fiolesTiming = 0;
	jeu->delaiMessage = 0;
	jeu->compteurLettres = jeu->compteurLettresReelles = 0;
	for(int i = 0; i < 3; i++) {
		jeu->textureFiolePVMorte[i] = creerTextureVide(renderer,16,51);
		jeu->textureFiolePMMorte[i] = creerTextureVide(renderer,16,51);
		memset(jeu->recapMessages[i],0,TAILLE_MAX_MSG_REELLE);
	}
	jeu->dstRectBarreXP = (SDL_Rect) {WINDOW_WIDTH * 0.02, WINDOW_HEIGHT * 0.95, WINDOW_WIDTH * 0.96, WINDOW_HEIGHT * 0.08};
	jeu->compteurRecap = 0;
	jeu->afficherRecap = 0;
	jeu->degatsAffiches = 0;

	jeu->estCoupCritique = jeu->mursVisibles = jeu->menuVisible = false;
	jeu->alEventsActuels = NULL;
	jeu->lesHitBoxDesMonstresTouches = arraylist_creer(AL_SDL_RECT);
	jeu->nbEventPass = 0;

	return jeu;
}

void creation_donnees(SDL_Renderer * renderer, jeu_t * jeu) {
	// Création de l'affichage (ptr renderer, chemin, ptr jeu)
	jeu->lesAffichages = arraylist_creer(AL_TEXTURE);
	ajouterAffichage(renderer,"fioles.png",jeu); // affichage 0 (fioles)
	ajouterAffichage(renderer,"xp.png",jeu); // affichage 1 (barre XP)

	// Création des skins (ajouterSkin(ptr renderer, nomFichier, ptr jeu))
	jeu->lesSkins = arraylist_creer(AL_SKIN);
	ajouterSkin(renderer,"Evil.png",jeu); // skin 0

	// Création des monstresData (ajouterMonstreData(ptr renderer, nomFichier, nom, PVMax, xp, piecesOr, ptr jeu))
	jeu->lesMonstresData = arraylist_creer(AL_MONSTRE_DATA);
	ajouterMonstreData(renderer,"blob_bleu.png","Blob Bleu",432,33,9,jeu); // monstreData 0
	ajouterMonstreData(renderer,"lapin.png","Lapin",609,163,14,jeu); //monstreData 1

	// Création des polices (ajouterPolice(nomFichier, taille, ptr jeu))
	jeu->lesPolices = arraylist_creer(AL_FONT);
	ajouterPolice("arial.ttf",WINDOW_WIDTH / 80,jeu); // font 0 (FPS)
	ajouterPolice("arial.ttf",TAILLE_CASES * 0.68,jeu); // font 1 (Arial, texte normal)
	ajouterPolice("Zelda Breath of the Wild.otf",WINDOW_WIDTH / 28,jeu); // font 2 (Coups d'attaque)

	// Création des musiques (ajouterMusique(nomFichier, ptr jeu))
	jeu->lesMusiques = arraylist_creer(AL_MUSIQUE);
	ajouterMusique("Castle_1.mp3",jeu); // musique 0
	ajouterMusique("Sarosa.mp3",jeu); // musique 1
	ajouterMusique("bahamut_lagoon.mp3",jeu); // musique 2
	ajouterMusique("Castle_3.mp3",jeu); // musique 3
	ajouterMusique("2000_ordeal.mp3",jeu); // musique 4
	ajouterMusique("cc_viper_manor.mp3",jeu); // musique 5
	ajouterMusique("suikoden-ii-two-rivers.mp3",jeu); // musique 6
	ajouterMusique("mystery3.ogg",jeu); // musique 7
	ajouterMusique("hunter.ogg",jeu); // musique 8
	ajouterMusique("illusionary_world.mp3",jeu); // musique 9
	ajouterMusique("chapt1medfill.mp3",jeu); // musique 10

	// Création des bruitages (ajouterBruitage(nomFichier, ptr jeu))
	jeu->lesBruitages = arraylist_creer(AL_BRUITAGE);
	ajouterBruitage("Blow1.wav",jeu); // bruitage 0
	ajouterBruitage("Kill1.wav",jeu); // bruitage 1
	ajouterBruitage("Damage3.ogg",jeu); // bruitage 2

	// Création des chipsets (ajouterChipset(ptr renderer, nomFichier, tailleTuile, ptr jeu))
	jeu->lesChipsets = arraylist_creer(AL_CHIPSET);
	ajouterChipset(renderer,"BZ.png",16,jeu); // chipset 0
	ajouterChipset(renderer,"VillageTangaFinal.png",16,jeu); // chipset 1
	ajouterChipset(renderer,"grey_cas42.png",16,jeu); // chipset 2
	ajouterChipset(renderer,"PalaisRoland2.png",16,jeu); // chipset 3
	ajouterChipset(renderer,"PalaisRolandInt.png",16,jeu); // chipset 4
	ajouterChipset(renderer,"PalaisRolandNouveau.png",48,jeu); // chipset 5
	ajouterChipset(renderer,"MaraisTanga.png",16,jeu); // chipset 6
	ajouterChipset(renderer,"marais2.png",16,jeu); // chipset 7
	ajouterChipset(renderer,"Coacville_exterieur.png",16,jeu); // chipset 8
	ajouterChipset(renderer,"chipset173.png",16,jeu); // chipset 9
	ajouterChipset(renderer,"chipset175.png",16,jeu); // chipset 10
	ajouterChipset(renderer,"HunterArene.png",16,jeu); // chipset 11
	ajouterChipset(renderer,"grass.png",32,jeu); // chipset 12
	ajouterChipset(renderer,"chipset5c.png",16,jeu); // chipset 13

	// Création des cartes (ajouterCarte(nom, largeur, hauteur, ptr chipset, ptr musique, ptr jeu))
	jeu->lesCartes = arraylist_creer(AL_CARTE);
	ajouterCarte("Sarosa_Milice_Accueil",20,10,getChipset2(jeu,"BZ.png"),getMusique2(jeu,"Castle_1.mp3"),jeu); // carte 0
	ajouterCarte("Sarosa",50,50,getChipset2(jeu,"VillageTangaFinal.png"),getMusique2(jeu,"Sarosa.mp3"),jeu); // carte 1
	ajouterCarte("Chateau_Roland_Exterieur",50,35,getChipset2(jeu,"grey_cas42.png"),getMusique2(jeu,"bahamut_lagoon.mp3"),jeu); // carte 2
	ajouterCarte("Chateau_Roland_Cour_Interieure",15,20,getChipset2(jeu,"PalaisRoland2.png"),getMusique2(jeu,"Castle_3.mp3"),jeu); // carte 3
	ajouterCarte("Chateau_Roland_Etage_01",15,12,getChipset2(jeu,"PalaisRolandInt.png"),getMusique2(jeu,"Castle_3.mp3"),jeu); // carte 4
	ajouterCarte("Chateau_Roland_Salle_Trone",19,22,getChipset2(jeu,"PalaisRoland2.png"),getMusique2(jeu,"2000_ordeal.mp3"),jeu); // carte 5
	ajouterCarte("Chateau_Roland_Salle_Trone_Nouveau",19,23,getChipset2(jeu,"PalaisRolandNouveau.png"),getMusique2(jeu,"2000_ordeal.mp3"),jeu); // carte 6
	ajouterCarte("Sarosa_Foret_Est",50,30,getChipset2(jeu,"VillageTangaFinal.png"),getMusique2(jeu,"bahamut_lagoon.mp3"),jeu); // carte 7
	ajouterCarte("Marais_Coacville",70,55,getChipset2(jeu,"MaraisTanga.png"),getMusique2(jeu,"cc_viper_manor.mp3"),jeu); // carte 8
	ajouterCarte("Coacville_Marais_Sud",20,18,getChipset2(jeu,"marais2.png"),getMusique2(jeu,"cc_viper_manor.mp3"),jeu); // carte 9
	ajouterCarte("Coacville_Marecage_Sud",30,40,getChipset2(jeu,"marais2.png"),getMusique2(jeu,"cc_viper_manor.mp3"),jeu); // carte 10
	ajouterCarte("Coacville",43,40,getChipset2(jeu,"Coacville_exterieur.png"),getMusique2(jeu,"suikoden-ii-two-rivers.mp3"),jeu); // carte 11
	ajouterCarte("Coacville_Marecage_Nord",40,50,getChipset2(jeu,"marais2.png"),getMusique2(jeu,"cc_viper_manor.mp3"),jeu); // carte 12
	ajouterCarte("Coacville_Donjon_Exterieur",25,40,getChipset2(jeu,"chipset173.png"),getMusique2(jeu,"cc_viper_manor.mp3"),jeu); // carte 13
	ajouterCarte("Donjon1_Entree",20,15,getChipset2(jeu,"chipset175.png"),getMusique2(jeu,"mystery3.ogg"),jeu); // carte 14
	ajouterCarte("Donjon1_salle4",30,15,getChipset2(jeu,"chipset175.png"),getMusique2(jeu,"mystery3.ogg"),jeu); // carte 15
	ajouterCarte("Donjon1_salle5",15,15,getChipset2(jeu,"chipset175.png"),getMusique2(jeu,"mystery3.ogg"),jeu); // carte 16
	ajouterCarte("Arene_Hunter",29,32,getChipset2(jeu,"HunterArene.png"),getMusique2(jeu,"hunter.ogg"),jeu); // carte 17
	ajouterCarte("carte18",20,20,getChipset2(jeu,"grass.png"),getMusique2(jeu,"illusionary_world.mp3"),jeu); // carte 18
	ajouterCarte("Foret_Sud_Sarosa",50,31,getChipset2(jeu,"chipset5c.png"),getMusique2(jeu,"chapt1medfill.mp3"),jeu); // carte 19
}

monstre_data_t * getMonstreData2(jeu_t * jeu, char * nom) {
	for(int i = 0; i < jeu->lesMonstresData->taille; i++) {
		monstre_data_t * monstreData = arraylist_get(jeu->lesMonstresData,i);
		if(strcmp(monstreData->nom,nom) == 0) {
			return monstreData;
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

carte_t * getCarte2(jeu_t * jeu, char * nom) {
	for(int i = 0; i < jeu->lesCartes->taille; i++) {
		carte_t * carte = arraylist_get(jeu->lesCartes,i);
		if(strcmp(carte->nom,nom) == 0) {
			return carte;
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

	// Création des messages (ptr carte, numPage, {xCase, yCase}, e_type = E_MESSAGE, event_creerMsg(message))
	carte_ajouterEvent(getCarte2(jeu,"Sarosa"),0,24,10,E_MESSAGE,event_creerMsg("Les coups critiques sont affichés en rouge et infligent 200% de ton attaque de base."));
	carte_ajouterEvent(getCarte2(jeu,"Coacville"),0,11,6,E_MESSAGE,event_creerMsg("Une odeur trop affreuse sort de cette maison... Je préfère rester dehors."));
	carte_ajouterEvent(getCarte2(jeu,"Donjon1_salle4"),0,29,8,E_MESSAGE,event_creerMsg("Metroidzeta : Malheureusement l'aventure s'arrête ici."));
	carte_ajouterEvent(getCarte2(jeu,"Donjon1_salle4"),0,29,8,E_MESSAGE,event_creerMsg("Metroidzeta : Le jeu est entièrement codé en C avec la bibliothèque SDL2, rien à voir avec RPG Maker."));
	carte_ajouterEvent(getCarte2(jeu,"Donjon1_salle4"),0,29,8,E_MESSAGE,event_creerMsg("Metroidzeta : Est-ce que ça te plaît ? Tu peux participer au projet si tu le souhaites ;)"));

	// Création des téléportations (ptr carte, numPage, {xCaseSrc, yCaseSrc}, e_type = E_TELEPORTATION, event_creerTP({xCaseDst, yCaseDst}, ptr carteDst))
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
	carte_ajouterEvent(getCarte2(jeu,"Coacville_Donjon_Exterieur"),0,0,9,E_TELEPORTATION,event_creerTP(18,9,getCarte2(jeu,"carte18")));
	carte_ajouterEvent(getCarte2(jeu,"Donjon1_Entree"),0,8,14,E_TELEPORTATION,event_creerTP(11,6,getCarte2(jeu,"Coacville_Donjon_Exterieur")));
	carte_ajouterEvent(getCarte2(jeu,"Donjon1_Entree"),0,9,14,E_TELEPORTATION,event_creerTP(11,6,getCarte2(jeu,"Coacville_Donjon_Exterieur")));
	carte_ajouterEvent(getCarte2(jeu,"Donjon1_Entree"),0,10,14,E_TELEPORTATION,event_creerTP(11,6,getCarte2(jeu,"Coacville_Donjon_Exterieur")));
	carte_ajouterEvent(getCarte2(jeu,"Donjon1_Entree"),0,8,3,E_TELEPORTATION,event_creerTP(7,13,getCarte2(jeu,"Donjon1_salle5")));
	carte_ajouterEvent(getCarte2(jeu,"Donjon1_salle4"),0,13,14,E_TELEPORTATION,event_creerTP(7,4,getCarte2(jeu,"Donjon1_salle5")));
	carte_ajouterEvent(getCarte2(jeu,"Donjon1_salle4"),0,14,14,E_TELEPORTATION,event_creerTP(7,4,getCarte2(jeu,"Donjon1_salle5")));
	carte_ajouterEvent(getCarte2(jeu,"Donjon1_salle4"),0,15,14,E_TELEPORTATION,event_creerTP(7,4,getCarte2(jeu,"Donjon1_salle5")));
	carte_ajouterEvent(getCarte2(jeu,"Donjon1_salle5"),0,7,3,E_TELEPORTATION,event_creerTP(14,13,getCarte2(jeu,"Donjon1_salle4")));
	carte_ajouterEvent(getCarte2(jeu,"Donjon1_salle5"),0,6,14,E_TELEPORTATION,event_creerTP(8,4,getCarte2(jeu,"Donjon1_Entree")));
	carte_ajouterEvent(getCarte2(jeu,"Donjon1_salle5"),0,7,14,E_TELEPORTATION,event_creerTP(8,4,getCarte2(jeu,"Donjon1_Entree")));
	carte_ajouterEvent(getCarte2(jeu,"Donjon1_salle5"),0,8,14,E_TELEPORTATION,event_creerTP(8,4,getCarte2(jeu,"Donjon1_Entree")));
	carte_ajouterEvent(getCarte2(jeu,"Arene_Hunter"),0,0,17,E_TELEPORTATION,event_creerTP(13,14,getCarte2(jeu,"Chateau_Roland_Cour_Interieure")));
	carte_ajouterEvent(getCarte2(jeu,"Arene_Hunter"),0,28,17,E_TELEPORTATION,event_creerTP(1,9,getCarte2(jeu,"carte18")));
	carte_ajouterEvent(getCarte2(jeu,"carte18"),0,19,9,E_TELEPORTATION,event_creerTP(1,9,getCarte2(jeu,"Coacville_Donjon_Exterieur")));
	carte_ajouterEvent(getCarte2(jeu,"carte18"),0,0,9,E_TELEPORTATION,event_creerTP(27,17,getCarte2(jeu,"Arene_Hunter")));
	carte_ajouterEvent(getCarte2(jeu,"Foret_Sud_Sarosa"),0,28,0,E_TELEPORTATION,event_creerTP(26,48,getCarte2(jeu,"Sarosa")));

	// Création des jouer musiques (ptr carte, numPage, {xCase, yCase}, e_type = E_JOUER_MUSIQUE, event_creerJM(ptr musique))
	carte_ajouterEvent(getCarte2(jeu,"Sarosa"),0,6,17,E_MESSAGE,event_creerMsg("La musique \"Hunter\" sera joué après ce message"));
	carte_ajouterEvent(getCarte2(jeu,"Sarosa"),0,6,17,E_JOUER_MUSIQUE,event_creerJM(getMusique2(jeu,"hunter.ogg")));
	carte_ajouterEvent(getCarte2(jeu,"Sarosa"),0,6,17,E_MESSAGE,event_creerMsg("Arrêt de la musique \"Hunter\" après ce message"));
	carte_ajouterEvent(getCarte2(jeu,"Sarosa"),0,6,17,E_ARRETER_MUSIQUE,NULL);
	carte_ajouterEvent(getCarte2(jeu,"Sarosa"),0,6,17,E_MESSAGE,event_creerMsg("On remet la musique de \"Sarosa\" après ce message"));
	carte_ajouterEvent(getCarte2(jeu,"Sarosa"),0,6,17,E_JOUER_MUSIQUE,event_creerJM(getMusique2(jeu,"Sarosa.mp3")));

	// Création des modification de PV du joueur (ptr carte, numPage, {xCase, yCase}, e_type = E_CHANGE_PV, event_creerChangePV(valeur))
	carte_ajouterEvent(getCarte2(jeu,"Arene_Hunter"),0,6,8,E_CHANGE_PV,event_creerChangePV(-15));
	carte_ajouterEvent(getCarte2(jeu,"Arene_Hunter"),0,22,8,E_CHANGE_PV,event_creerChangePV(15));

	// Création des modification de PM du joueur (ptr carte, numPage, {xCase, yCase}, e_type = E_CHANGE_PM, event_creerChangePM(valeur))
	carte_ajouterEvent(getCarte2(jeu,"Arene_Hunter"),0,6,26,E_CHANGE_PM,event_creerChangePM(-6));
	carte_ajouterEvent(getCarte2(jeu,"Arene_Hunter"),0,22,26,E_CHANGE_PM,event_creerChangePM(6));
}

void creation_monstres(jeu_t * jeu) {
	// Création des monstres (carte_ajouterMonstre(ptr carte, ptr monstre, ptr jeu))
	carte_ajouterMonstre(getCarte2(jeu,"Sarosa_Foret_Est"),monstre_creer(getMonstreData2(jeu,"Blob Bleu"),9,23));
	carte_ajouterMonstre(getCarte2(jeu,"Sarosa_Foret_Est"),monstre_creer(getMonstreData2(jeu,"Blob Bleu"),34,13));
	carte_ajouterMonstre(getCarte2(jeu,"Sarosa_Foret_Est"),monstre_creer(getMonstreData2(jeu,"Lapin"),33,24));
	carte_ajouterMonstre(getCarte2(jeu,"Arene_Hunter"),monstre_creer(getMonstreData2(jeu,"Blob Bleu"),9,9));
	carte_ajouterMonstre(getCarte2(jeu,"Arene_Hunter"),monstre_creer(getMonstreData2(jeu,"Lapin"),10,9));
	carte_ajouterMonstre(getCarte2(jeu,"Arene_Hunter"),monstre_creer(getMonstreData2(jeu,"Blob Bleu"),9,10));
	carte_ajouterMonstre(getCarte2(jeu,"Arene_Hunter"),monstre_creer(getMonstreData2(jeu,"Lapin"),10,10));
}

void jeu_updateOffSetJoueur(jeu_t * jeu) {
	jeu->xOffSetJoueur = jeu->hitBoxJoueurEcran.x - jeu->joueur->position.x;
	jeu->yOffSetJoueur = jeu->hitBoxJoueurEcran.y - jeu->joueur->position.y;
}

void creation_notreJoueur(SDL_Renderer * renderer, jeu_t * jeu) { // Création de notre joueur
	FILE * fichier_pseudo = ouvrirFichier("PSEUDO.txt","r","Impossible d'ouvrir le fichier PSEUDO.txt");
	char nomJoueur[26];
	fscanf(fichier_pseudo,"%25s",nomJoueur);
	fclose(fichier_pseudo);

	if(nomJoueur[0] == '\0') { strcpy(nomJoueur,"Test"); }
	jeu->joueur = joueur_creer(renderer,nomJoueur,getSkin(jeu,0),VOLEUR,1,1000,12,12,getPolice(jeu,1),getCarte2(jeu,"Chateau_Roland_Cour_Interieure"),10);

	SDL_QueryTexture(jeu->joueur->textureNom,NULL,NULL,&jeu->rectPseudo.w,&jeu->rectPseudo.h);
	jeu->rectPseudo.x = jeu->hitBoxJoueurEcran.x - (jeu->rectPseudo.w / 2) + (TAILLE_CASES / 2) - 2;
	jeu->rectPseudo.y = jeu->hitBoxJoueurEcran.y + TAILLE_CASES - 2;

	jeu_updateOffSetJoueur(jeu);
	jeu->musiqueActuelle = jeu->joueur->carteActuelle->musique;
}

void ajouterAffichage(SDL_Renderer * renderer, char * nomFichier, jeu_t * jeu) { arraylist_add(jeu->lesAffichages,creerAffichage(renderer,nomFichier)); }
void ajouterSkin(SDL_Renderer * renderer, char * nomFichier, jeu_t * jeu) { arraylist_add(jeu->lesSkins,skin_creer(renderer,nomFichier)); }
void ajouterMonstreData(SDL_Renderer * renderer, char * nomFichier, char * nom, int PVMax, int xp, int piecesOr, jeu_t * jeu) { arraylist_add(jeu->lesMonstresData,monstre_data_creer(renderer,nomFichier,nom,PVMax,xp,piecesOr)); }
void ajouterPolice(char * nomFichier, int taille, jeu_t * jeu) { arraylist_add(jeu->lesPolices,creerPolice(nomFichier,taille)); }
void ajouterMusique(char * nomFichier, jeu_t * jeu) { arraylist_add(jeu->lesMusiques,musique_creer(nomFichier)); }
void ajouterBruitage(char * nomFichier, jeu_t * jeu) { arraylist_add(jeu->lesBruitages,bruitage_creer(nomFichier)); }
void ajouterChipset(SDL_Renderer * renderer, char * nomFichier, int tailleTuile, jeu_t * jeu) { arraylist_add(jeu->lesChipsets,chipset_creer(renderer,nomFichier,tailleTuile)); }
void ajouterCarte(char * nom, int hauteur, int largeur, chipset_t * chipset, musique_t * musique, jeu_t * jeu) { arraylist_add(jeu->lesCartes,carte_creerDepuisMatrices(nom,hauteur,largeur,chipset,musique)); }
void ajouterCarteVide(char * nom, int hauteur, int largeur, chipset_t * chipset, musique_t * musique, jeu_t * jeu) { arraylist_add(jeu->lesCartes,carte_creer(nom,hauteur,largeur,chipset,musique,false)); }

SDL_Texture * getAffichage(jeu_t * jeu, int pos) { return arraylist_get(jeu->lesAffichages,pos); }
skin_t * getSkin(jeu_t * jeu, int pos) { return arraylist_get(jeu->lesSkins,pos); }
monstre_data_t * getMonstreData(jeu_t * jeu, int pos) { return arraylist_get(jeu->lesMonstresData,pos); }
TTF_Font * getPolice(jeu_t * jeu, int pos) { return arraylist_get(jeu->lesPolices,pos); }
musique_t * getMusique(jeu_t * jeu, int pos) { return arraylist_get(jeu->lesMusiques,pos); }
bruitage_t * getBruitage(jeu_t * jeu, int pos) { return arraylist_get(jeu->lesBruitages,pos); }
chipset_t * getChipset(jeu_t * jeu, int pos) { return arraylist_get(jeu->lesChipsets,pos); }
carte_t * getCarte(jeu_t * jeu, int pos) { return arraylist_get(jeu->lesCartes,pos); }
event_t * getEventActuel(jeu_t * jeu, int pos) { return arraylist_get(jeu->alEventsActuels,pos); }
SDL_Rect * getHitBoxMonstreTouche(jeu_t * jeu, int pos) { return arraylist_get(jeu->lesHitBoxDesMonstresTouches,pos); }

void musique_stopAndPlay(musique_t * musiqueActuelle, musique_t * musiqueSuivante) {
	if(musiqueActuelle != NULL) { musique_stop(musiqueActuelle); }
	if(musiqueSuivante != NULL) { musique_play(musiqueSuivante); }
}

void viderMessage(jeu_t * jeu) {
	memset(jeu->message[0],0,TAILLE_MAX_MSG_REELLE);
	jeu->compteurLettres = jeu->compteurLettresReelles = 0;
}

void sauvegarderMessage(jeu_t * jeu) {
	strcpy(jeu->message[1],jeu->message[0]);
	bool_array_copy(jeu->messageChar2octets[1],jeu->messageChar2octets[0],TAILLE_MAX_MSG_REELLE - 1);
}

void remettreDernierMessage(jeu_t * jeu) {
	strcpy(jeu->message[0],jeu->message[1]);
	bool_array_copy(jeu->messageChar2octets[0],jeu->messageChar2octets[1],TAILLE_MAX_MSG_REELLE - 1);
	jeu->compteurLettresReelles = strlen(jeu->message[0]);
	int compteur = 0;
	for(int i = 0; i < jeu->compteurLettresReelles; i++) {
		if(!jeu->messageChar2octets[0][i]) {
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
	strcpy(jeu->recapMessages[jeu->compteurRecap],jeu->message[0]);
	jeu->compteurRecap++;
}

void afficherHitboxAttaqueEpee(SDL_Renderer * renderer, jeu_t * jeu) {
	dessinerRectangle(renderer,&jeu->hitBoxEpeeJoueurEcran[jeu->joueur->direction],BLANC_TRANSPARENT);
}

void updateFiolePV(SDL_Renderer * renderer, jeu_t * jeu) {
	double ratioPV = joueur_getRatioPV(jeu->joueur);
	SDL_Rect dstRectfiolePVMorte = (SDL_Rect) {0, 0, 16, 51 * (1 - ratioPV)};
	for(int i = 0; i < 3; i++) {
		jeu->srcRectFiolePV[1][i] = (SDL_Rect) {16, 21 + i * 72, 16, 51 * (1 - ratioPV)};
		SDL_DestroyTexture(jeu->textureFiolePVMorte[i]);
		jeu->textureFiolePVMorte[i] = creerTextureVide(renderer,16,51);
		SDL_SetRenderTarget(renderer,jeu->textureFiolePVMorte[i]); // On va dessiner sur cette texture
		dessinerTexture(renderer,getAffichage(jeu,0),&jeu->srcRectFiolePV[1][i],&dstRectfiolePVMorte,"Impossible de dessiner la fiole PV morte sur texture vide avec SDL_RenderCopy");
	}
	SDL_SetRenderTarget(renderer,NULL); // On redessine sur le renderer principal
}

void updateFiolePM(SDL_Renderer * renderer, jeu_t * jeu) {
	double ratioPM = joueur_getRatioPM(jeu->joueur);
	SDL_Rect dstRectfiolePMMorte = (SDL_Rect) {0, 0, 16, 51 * (1 - ratioPM)};
	for(int i = 0; i < 3; i++) {
		jeu->srcRectFiolePM[1][i] = (SDL_Rect) {48, 21 + i * 72, 16, 51 * (1 - ratioPM)};
		SDL_DestroyTexture(jeu->textureFiolePMMorte[i]);
		jeu->textureFiolePMMorte[i] = creerTextureVide(renderer,16,51);
		SDL_SetRenderTarget(renderer,jeu->textureFiolePMMorte[i]); // On va dessiner sur cette texture
		dessinerTexture(renderer,getAffichage(jeu,0),&jeu->srcRectFiolePM[1][i],&dstRectfiolePMMorte,"Impossible de dessiner la fiole PM morte sur une texture vide avec SDL_RenderCopy");
	}
	SDL_SetRenderTarget(renderer,NULL); // On redessine sur le renderer principal
}

void afficherFiolePV(SDL_Renderer * renderer, jeu_t * jeu) {
	dessinerTexture(renderer,getAffichage(jeu,0),&jeu->srcRectFiolePV[0][jeu->fiolesTiming],&jeu->dstRectFiolePV[0],"Impossible de dessiner la fiole PV vivante avec SDL_RenderCopy");
	dessinerTexture(renderer,jeu->textureFiolePVMorte[jeu->fiolesTiming],NULL,&jeu->dstRectFiolePV[1],"Impossible de dessiner la fiole PV morte avec SDL_RenderCopy");
}

void afficherFiolePM(SDL_Renderer * renderer, jeu_t * jeu) {
	dessinerTexture(renderer,getAffichage(jeu,0),&jeu->srcRectFiolePM[0][jeu->fiolesTiming],&jeu->dstRectFiolePM[0],"Impossible de dessiner la fiole PM vivante avec SDL_RenderCopy");
	dessinerTexture(renderer,jeu->textureFiolePMMorte[jeu->fiolesTiming],NULL,&jeu->dstRectFiolePM[1],"Impossible de dessiner la fiole PM morte avec SDL_RenderCopy");
}

void afficherBarreXP(SDL_Renderer * renderer, jeu_t * jeu) {
	dessinerTexture(renderer,getAffichage(jeu,1),NULL,&jeu->dstRectBarreXP,"Impossible de dessiner la barre XP vivante avec SDL_RenderCopy");
}

void afficherCouche(SDL_Renderer * renderer, carte_t * carte, int couche, jeu_t * jeu) {
	SDL_Rect dstRect = {0, 0, TAILLE_CASES, TAILLE_CASES};
	int joueurXCase = joueur_getXCase(jeu->joueur);
	int joueurYCase = joueur_getYCase(jeu->joueur);

	int num;
	int i_debut = (joueurYCase - WINDOW_HEIGHT_CASES_DIV2 - 1) < 0 ? 0 : (joueurYCase - WINDOW_HEIGHT_CASES_DIV2 - 1);
	int j_debut = (joueurXCase - WINDOW_WIDTH_CASES_DIV2 - 1) < 0 ? 0 : (joueurXCase - WINDOW_WIDTH_CASES_DIV2 - 1);
	int i_fin = (joueurYCase + WINDOW_HEIGHT_CASES_DIV2 + 3) > carte->hauteur ? carte->hauteur : (joueurYCase + WINDOW_HEIGHT_CASES_DIV2 + 3);
	int j_fin = (joueurXCase + WINDOW_WIDTH_CASES_DIV2 + 3) > carte->largeur ? carte->largeur : (joueurXCase + WINDOW_WIDTH_CASES_DIV2 + 3);

	//printf("i_debut = %d, i_fin = %d, j_debut = %d, j_fin = %d\n", i_debut, i_fin, j_debut, j_fin);
	//SDL_GetWindowSurface(window);

	for(int i = i_debut; i < i_fin; i++) {
		for(int j = j_debut; j < j_fin; j++) {
			num = carte->couches[couche][i][j];
			if(num != -1) {
				dstRect.x = j * TAILLE_CASES + jeu->xOffSetJoueur;
				dstRect.y = i * TAILLE_CASES + jeu->yOffSetJoueur;

				dessinerTexture(renderer,carte->chipset->texture,&carte->chipset->tuilesRegions[num],&dstRect,"Impossible de dessiner la texture d'une tuile du chipset d'une couche avec SDL_RenderCopy");
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
	for(int i = 0; i < 3; i++) {
		SDL_DestroyTexture(jeu->textureFiolePVMorte[i]);
		SDL_DestroyTexture(jeu->textureFiolePMMorte[i]);
	}
	joueur_detruire(jeu->joueur);
	arraylist_detruire(jeu->lesHitBoxDesMonstresTouches,false);
	arraylist_detruire(jeu->lesCartes,true);
	arraylist_detruire(jeu->lesChipsets,true);
	arraylist_detruire(jeu->lesBruitages,true);
	arraylist_detruire(jeu->lesMusiques,true);
	arraylist_detruire(jeu->lesPolices,true);
	arraylist_detruire(jeu->lesMonstresData,true);
	arraylist_detruire(jeu->lesSkins,true);
	arraylist_detruire(jeu->lesAffichages,true);
	free(jeu);
}