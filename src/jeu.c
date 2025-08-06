// @author Alain Barbier alias "Metroidzeta"

#include "headers/jeu.h"

static void jeu_validerArguments(SDL_Renderer *renderer) {
	if (FPS < 1 || FPS > 999) Exception("FPS trop bas ou trop eleve");
	if (!renderer) Exception("Renderer NULL");
}

static void creation_heros(SDL_Renderer *renderer, jeu_t *jeu) {
	FILE *fichierPseudo = fopen("PSEUDO.txt", "r");
	if (!fichierPseudo) { fclose(fichierPseudo); Exception("Echec d'ouverture du fichier pseudo"); }

	char nomHeros[26];
	fscanf(fichierPseudo, "%25s", nomHeros);
	fclose(fichierPseudo);

	if (!*nomHeros) strcpy(nomHeros, "Test");
	heros_result_t res = heros_creer(&jeu->heros, renderer, nomHeros, getSkin(jeu, 0), VOLEUR, 1, 1000, 12, 12, getPolice(jeu, 1), getCarte2(jeu, "Chateau_Roland_Cour_Interieure"), 10);
	if (res != HEROS_OK) { LOG_ERROR("Heros : %s", heros_strerror(res)); Exception(""); }

	SDL_QueryTexture(jeu->heros->textureNom, NULL, NULL, &jeu->rectPseudo.w, &jeu->rectPseudo.h);
	jeu->rectPseudo.x = jeu->hitBoxHerosEcran.x - (jeu->rectPseudo.w / 2) + (TAILLE_CASES / 2) - 2;
	jeu->rectPseudo.y = jeu->hitBoxHerosEcran.y + TAILLE_CASES - 2;

	jeu_updateOffSetHeros(jeu);
	jeu->musiqueActuelle = jeu->heros->carteActuelle->musique;
}

static void creation_affichages(SDL_Renderer *renderer, jeu_t *jeu) {
	// Création de l'affichage (jeu_ajouterAffichage(ptr renderer, chemin, ptr jeu))
	arraylist_creer(&jeu->affichages, AL_TEXTURE);
	jeu_ajouterAffichage(renderer, "fioles.png", jeu); // 0 (fioles)
	jeu_ajouterAffichage(renderer, "xp.png", jeu); // 1 (barre XP)
}

static void creation_monstresData(SDL_Renderer *renderer, jeu_t *jeu) {
	// Création des monstresData (jeu_ajouterMonstreData(ptr renderer, nomFichier, nom, PVMax, xp, piecesOr, ptr jeu))
	arraylist_creer(&jeu->monstresData, AL_MONSTRE_DATA);
	jeu_ajouterMonstreData(renderer, "blob_bleu.png", "Blob Bleu", 432, 33, 9, jeu); // 0 (Blob bleu)
	jeu_ajouterMonstreData(renderer, "lapin.png", "Lapin", 609, 163, 14, jeu); // 1 (Lapin)
}

static void creation_polices(jeu_t *jeu) {
	// Création des polices (jeu_ajouterPolice(nomFichier, taille, ptr jeu))
	arraylist_creer(&jeu->polices, AL_FONT);
	jeu_ajouterPolice("arial.ttf", WINDOW_WIDTH / 80, jeu); // 0 (FPS)
	jeu_ajouterPolice("arial.ttf", TAILLE_CASES * 0.68, jeu); // 1 (Arial, texte normal)
	jeu_ajouterPolice("Zelda Breath of the Wild.otf", WINDOW_WIDTH / 28, jeu); // 2 (Coups d'attaques)
}

static void creation_cartes(jeu_t *jeu) {
	// Création des cartes (jeu_ajouterCarte(nom, largeur, hauteur, nomChipset, nomMusique, ptr jeu))
	arraylist_creer(&jeu->cartes, AL_CARTE);
	jeu_ajouterCarte("Sarosa_Milice_Accueil", 20, 10, "BZ.png", "Castle_1.mp3", jeu); // 0
	jeu_ajouterCarte("Sarosa", 50, 50, "VillageTangaFinal.png", "Sarosa.mp3", jeu); // 1
	jeu_ajouterCarte("Chateau_Roland_Exterieur", 50, 35, "grey_cas42.png", "bahamut_lagoon.mp3", jeu); // 2
	jeu_ajouterCarte("Chateau_Roland_Cour_Interieure", 15, 20, "PalaisRoland2.png" , "Castle_3.mp3", jeu); // 3
	jeu_ajouterCarte("Chateau_Roland_Etage_01", 15, 12, "PalaisRolandInt.png", "Castle_3.mp3", jeu); // 4
	jeu_ajouterCarte("Chateau_Roland_Salle_Trone", 19, 22, "PalaisRoland2.png", "2000_ordeal.mp3", jeu); // 5
	jeu_ajouterCarte("Chateau_Roland_Salle_Trone_Nouveau", 19, 23, "PalaisRolandNouveau.png", "2000_ordeal.mp3", jeu); // 6
	jeu_ajouterCarte("Sarosa_Foret_Est", 50 ,30, "VillageTangaFinal.png", "bahamut_lagoon.mp3", jeu); // 7
	jeu_ajouterCarte("Marais_Coacville", 70, 55, "MaraisTanga.png", "cc_viper_manor.mp3", jeu); // 8
	jeu_ajouterCarte("Coacville_Marais_Sud", 20, 18, "marais2.png", "cc_viper_manor.mp3", jeu); // 9
	jeu_ajouterCarte("Coacville_Marecage_Sud", 30, 40, "marais2.png", "cc_viper_manor.mp3", jeu); // 10
	jeu_ajouterCarte("Coacville", 43, 40, "Coacville_exterieur.png", "suikoden-ii-two-rivers.mp3", jeu); // 11
	jeu_ajouterCarte("Coacville_Marecage_Nord", 40, 50, "marais2.png", "cc_viper_manor.mp3", jeu); // 12
	jeu_ajouterCarte("Coacville_Donjon_Exterieur", 25, 40, "chipset173.png", "cc_viper_manor.mp3", jeu); // 13
	jeu_ajouterCarte("Donjon1_Entree", 20, 15, "chipset175.png", "mystery3.ogg", jeu); // 14
	jeu_ajouterCarte("Donjon1_salle4", 30, 15, "chipset175.png", "mystery3.ogg", jeu); // 15
	jeu_ajouterCarte("Donjon1_salle5", 15, 15, "chipset175.png", "mystery3.ogg", jeu); // 16
	jeu_ajouterCarte("Arene_Hunter", 29, 32, "HunterArene.png", "hunter.ogg", jeu); // 17
	jeu_ajouterCarte("carte18", 20, 20, "grass.png", "illusionary_world.mp3", jeu); // 18
	jeu_ajouterCarte("Foret_Sud_Sarosa", 50, 31, "chipset5c.png", "chapt1medfill.mp3", jeu); // 19
}

static void creation_events(jeu_t *jeu) {
	// Création des messages (carte_ajouterEvent(ptr carte, numPage, xCase, yCase, e_type = E_MSG, event_creerMsg(msg)))
	carte_ajouterEvent(getCarte2(jeu, "Sarosa"), 0, 24, 10, E_MSG, event_creerMsg("Les coups critiques sont affichés en rouge et infligent 200% de ton attaque de base."));
	carte_ajouterEvent(getCarte2(jeu, "Coacville"), 0, 11, 6, E_MSG, event_creerMsg("Une odeur trop affreuse sort de cette maison... Je préfère rester dehors."));
	carte_ajouterEvent(getCarte2(jeu, "Donjon1_salle4"), 0, 29, 8, E_MSG, event_creerMsg("Metroidzeta : Malheureusement l'aventure s'arrête ici."));
	carte_ajouterEvent(getCarte2(jeu, "Donjon1_salle4"), 0, 29, 8, E_MSG, event_creerMsg("Metroidzeta : Le jeu est entièrement codé en C avec la bibliothèque SDL2, rien à voir avec RPG Maker."));
	carte_ajouterEvent(getCarte2(jeu, "Donjon1_salle4"), 0, 29, 8, E_MSG, event_creerMsg("Metroidzeta : Est-ce que ça te plaît ? Tu peux participer au projet si tu le souhaites ;)"));

	// Création des téléportations (carte_ajouterEvent(ptr carte, numPage, xCaseSrc, yCaseSrc, e_type = E_TP, event_creerTP(xCaseDst, yCaseDst, ptr carteDst)))
	carte_ajouterEvent(getCarte2(jeu, "Sarosa_Milice_Accueil"), 0, 8, 3, E_TP, event_creerTP(11, 15, getCarte2(jeu, "Sarosa")));
	carte_ajouterEvent(getCarte2(jeu, "Sarosa"), 0, 10, 15, E_TP, event_creerTP(8, 4, getCarte2(jeu, "Sarosa_Milice_Accueil")));
	carte_ajouterEvent(getCarte2(jeu, "Sarosa"), 0, 23, 0, E_TP, event_creerTP(25, 33, getCarte2(jeu, "Chateau_Roland_Exterieur")));
	carte_ajouterEvent(getCarte2(jeu, "Sarosa"), 0, 49, 27, E_TP, event_creerTP(1, 13, getCarte2(jeu, "Sarosa_Foret_Est")));
	carte_ajouterEvent(getCarte2(jeu, "Sarosa"), 0, 26, 49, E_TP, event_creerTP(28, 1, getCarte2(jeu, "Foret_Sud_Sarosa")));
	carte_ajouterEvent(getCarte2(jeu, "Chateau_Roland_Exterieur"), 0, 25, 34, E_TP, event_creerTP(23, 1, getCarte2(jeu, "Sarosa")));
	carte_ajouterEvent(getCarte2(jeu, "Chateau_Roland_Exterieur"), 0, 25, 31, E_TP, event_creerTP(7, 18, getCarte2(jeu, "Chateau_Roland_Cour_Interieure")));
	carte_ajouterEvent(getCarte2(jeu, "Chateau_Roland_Exterieur"), 0, 25, 6, E_TP, event_creerTP(7, 5, getCarte2(jeu, "Chateau_Roland_Cour_Interieure")));
	carte_ajouterEvent(getCarte2(jeu, "Chateau_Roland_Exterieur"), 0, 25, 21, E_TP, event_creerTP(7, 9, getCarte2(jeu, "Chateau_Roland_Etage_01")));
	carte_ajouterEvent(getCarte2(jeu, "Chateau_Roland_Cour_Interieure"), 0, 7, 19, E_TP, event_creerTP(25, 32, getCarte2(jeu, "Chateau_Roland_Exterieur")));
	carte_ajouterEvent(getCarte2(jeu, "Chateau_Roland_Cour_Interieure"), 0, 0, 7, E_TP, event_creerTP(1, 9, getCarte2(jeu, "Chateau_Roland_Etage_01")));
	carte_ajouterEvent(getCarte2(jeu, "Chateau_Roland_Cour_Interieure"), 0, 0, 8, E_TP, event_creerTP(1, 9, getCarte2(jeu, "Chateau_Roland_Etage_01")));
	carte_ajouterEvent(getCarte2(jeu, "Chateau_Roland_Cour_Interieure"), 0, 14, 7, E_TP, event_creerTP(13, 9, getCarte2(jeu, "Chateau_Roland_Etage_01")));
	carte_ajouterEvent(getCarte2(jeu, "Chateau_Roland_Cour_Interieure"), 0, 14, 8, E_TP, event_creerTP(13, 9, getCarte2(jeu, "Chateau_Roland_Etage_01")));
	carte_ajouterEvent(getCarte2(jeu, "Chateau_Roland_Cour_Interieure"), 0, 14, 13, E_TP, event_creerTP(1, 17, getCarte2(jeu, "Arene_Hunter")));
	carte_ajouterEvent(getCarte2(jeu, "Chateau_Roland_Cour_Interieure"), 0, 14, 14, E_TP, event_creerTP(1, 17, getCarte2(jeu, "Arene_Hunter")));
	carte_ajouterEvent(getCarte2(jeu, "Chateau_Roland_Cour_Interieure"), 0, 7, 4, E_TP, event_creerTP(25, 5, getCarte2(jeu, "Chateau_Roland_Exterieur")));
	carte_ajouterEvent(getCarte2(jeu, "Chateau_Roland_Etage_01"), 0, 0, 10, E_TP, event_creerTP(1, 9, getCarte2(jeu, "Chateau_Roland_Cour_Interieure")));
	carte_ajouterEvent(getCarte2(jeu, "Chateau_Roland_Etage_01"), 0, 14, 10, E_TP, event_creerTP(13, 9, getCarte2(jeu, "Chateau_Roland_Cour_Interieure")));
	carte_ajouterEvent(getCarte2(jeu, "Chateau_Roland_Etage_01"), 0, 7, 10, E_TP, event_creerTP(25, 22, getCarte2(jeu, "Chateau_Roland_Exterieur")));
	carte_ajouterEvent(getCarte2(jeu, "Chateau_Roland_Etage_01"), 0, 6, 3, E_TP, event_creerTP(9, 20, getCarte2(jeu, "Chateau_Roland_Salle_Trone_Nouveau")));
	carte_ajouterEvent(getCarte2(jeu, "Chateau_Roland_Etage_01"), 0, 7, 3, E_TP, event_creerTP(9, 20, getCarte2(jeu, "Chateau_Roland_Salle_Trone_Nouveau")));
	carte_ajouterEvent(getCarte2(jeu, "Chateau_Roland_Etage_01"), 0, 8, 3, E_TP, event_creerTP(9, 20, getCarte2(jeu, "Chateau_Roland_Salle_Trone_Nouveau")));
	carte_ajouterEvent(getCarte2(jeu, "Chateau_Roland_Salle_Trone"), 0, 8, 20, E_TP, event_creerTP(1, 7, getCarte2(jeu, "Chateau_Roland_Cour_Interieure")));
	carte_ajouterEvent(getCarte2(jeu, "Chateau_Roland_Salle_Trone"), 0, 9, 20, E_TP, event_creerTP(1, 7, getCarte2(jeu, "Chateau_Roland_Cour_Interieure")));
	carte_ajouterEvent(getCarte2(jeu, "Chateau_Roland_Salle_Trone"), 0, 10, 20, E_TP, event_creerTP(1, 7, getCarte2(jeu, "Chateau_Roland_Cour_Interieure")));
	carte_ajouterEvent(getCarte2(jeu, "Chateau_Roland_Salle_Trone_Nouveau"), 0, 8, 21, E_TP, event_creerTP(7, 4, getCarte2(jeu, "Chateau_Roland_Etage_01")));
	carte_ajouterEvent(getCarte2(jeu, "Chateau_Roland_Salle_Trone_Nouveau"), 0, 9, 21, E_TP, event_creerTP(7, 4, getCarte2(jeu, "Chateau_Roland_Etage_01")));
	carte_ajouterEvent(getCarte2(jeu, "Chateau_Roland_Salle_Trone_Nouveau"), 0, 10, 21, E_TP, event_creerTP(7, 4, getCarte2(jeu, "Chateau_Roland_Etage_01")));
	carte_ajouterEvent(getCarte2(jeu, "Sarosa_Foret_Est"), 0, 0, 13, E_TP, event_creerTP(48, 27, getCarte2(jeu, "Sarosa")));
	carte_ajouterEvent(getCarte2(jeu, "Sarosa_Foret_Est"), 0, 25, 29, E_TP, event_creerTP(19, 1, getCarte2(jeu, "Marais_Coacville")));
	carte_ajouterEvent(getCarte2(jeu, "Marais_Coacville"), 0, 19, 0, E_TP, event_creerTP(25, 28, getCarte2(jeu, "Sarosa_Foret_Est")));
	carte_ajouterEvent(getCarte2(jeu, "Marais_Coacville"), 0, 69, 42, E_TP, event_creerTP(1, 15, getCarte2(jeu, "Coacville_Marais_Sud")));
	carte_ajouterEvent(getCarte2(jeu, "Coacville_Marais_Sud"), 0, 0, 15, E_TP, event_creerTP(68, 42, getCarte2(jeu, "Marais_Coacville")));
	carte_ajouterEvent(getCarte2(jeu, "Coacville_Marais_Sud"), 0, 8, 5, E_TP, event_creerTP(17, 38, getCarte2(jeu, "Coacville_Marecage_Sud")));
	carte_ajouterEvent(getCarte2(jeu, "Coacville_Marecage_Sud"), 0, 17, 39, E_TP, event_creerTP(8, 6, getCarte2(jeu, "Coacville_Marais_Sud")));
	carte_ajouterEvent(getCarte2(jeu, "Coacville_Marecage_Sud"), 0, 15, 0, E_TP, event_creerTP(16, 38, getCarte2(jeu, "Coacville")));
	carte_ajouterEvent(getCarte2(jeu, "Coacville"), 0, 16, 39, E_TP, event_creerTP(15, 1, getCarte2(jeu, "Coacville_Marecage_Sud")));
	carte_ajouterEvent(getCarte2(jeu, "Coacville"), 0, 18, 0, E_TP, event_creerTP(22, 48, getCarte2(jeu, "Coacville_Marecage_Nord")));
	carte_ajouterEvent(getCarte2(jeu, "Coacville_Marecage_Nord"), 0, 22, 49, E_TP, event_creerTP(18, 1, getCarte2(jeu,"Coacville")));
	carte_ajouterEvent(getCarte2(jeu, "Coacville_Marecage_Nord"), 0, 20, 0, E_TP, event_creerTP(11, 38, getCarte2(jeu, "Coacville_Donjon_Exterieur")));
	carte_ajouterEvent(getCarte2(jeu, "Coacville_Donjon_Exterieur"), 0, 11, 39, E_TP, event_creerTP(20, 1, getCarte2(jeu, "Coacville_Marecage_Nord")));
	carte_ajouterEvent(getCarte2(jeu, "Coacville_Donjon_Exterieur"), 0, 11, 5, E_TP, event_creerTP(9, 13, getCarte2(jeu, "Donjon1_Entree")));
	carte_ajouterEvent(getCarte2(jeu, "Coacville_Donjon_Exterieur"), 0, 0, 9, E_TP, event_creerTP(18, 9, getCarte2(jeu, "carte18")));
	carte_ajouterEvent(getCarte2(jeu, "Donjon1_Entree"), 0, 8, 14, E_TP, event_creerTP(11, 6, getCarte2(jeu, "Coacville_Donjon_Exterieur")));
	carte_ajouterEvent(getCarte2(jeu, "Donjon1_Entree"), 0, 9, 14, E_TP, event_creerTP(11, 6, getCarte2(jeu, "Coacville_Donjon_Exterieur")));
	carte_ajouterEvent(getCarte2(jeu, "Donjon1_Entree"), 0, 10, 14, E_TP, event_creerTP(11, 6, getCarte2(jeu, "Coacville_Donjon_Exterieur")));
	carte_ajouterEvent(getCarte2(jeu, "Donjon1_Entree"), 0, 8, 3, E_TP, event_creerTP(7, 13, getCarte2(jeu, "Donjon1_salle5")));
	carte_ajouterEvent(getCarte2(jeu, "Donjon1_salle4"), 0, 13, 14, E_TP, event_creerTP(7, 4, getCarte2(jeu, "Donjon1_salle5")));
	carte_ajouterEvent(getCarte2(jeu, "Donjon1_salle4"), 0, 14, 14, E_TP, event_creerTP(7, 4, getCarte2(jeu, "Donjon1_salle5")));
	carte_ajouterEvent(getCarte2(jeu, "Donjon1_salle4"), 0, 15, 14, E_TP, event_creerTP(7, 4, getCarte2(jeu, "Donjon1_salle5")));
	carte_ajouterEvent(getCarte2(jeu, "Donjon1_salle5"), 0, 7, 3, E_TP, event_creerTP(14, 13, getCarte2(jeu, "Donjon1_salle4")));
	carte_ajouterEvent(getCarte2(jeu, "Donjon1_salle5"), 0, 6, 14, E_TP, event_creerTP(8, 4, getCarte2(jeu, "Donjon1_Entree")));
	carte_ajouterEvent(getCarte2(jeu, "Donjon1_salle5"), 0, 7, 14, E_TP, event_creerTP(8, 4, getCarte2(jeu, "Donjon1_Entree")));
	carte_ajouterEvent(getCarte2(jeu, "Donjon1_salle5"), 0, 8, 14, E_TP, event_creerTP(8, 4, getCarte2(jeu, "Donjon1_Entree")));
	carte_ajouterEvent(getCarte2(jeu, "Arene_Hunter"), 0, 0, 17, E_TP, event_creerTP(13, 14, getCarte2(jeu, "Chateau_Roland_Cour_Interieure")));
	carte_ajouterEvent(getCarte2(jeu, "Arene_Hunter"), 0, 28, 17, E_TP, event_creerTP(1, 9, getCarte2(jeu, "carte18")));
	carte_ajouterEvent(getCarte2(jeu, "carte18"), 0, 19, 9, E_TP, event_creerTP(1, 9, getCarte2(jeu, "Coacville_Donjon_Exterieur")));
	carte_ajouterEvent(getCarte2(jeu, "carte18"), 0, 0, 9, E_TP, event_creerTP(27, 17, getCarte2(jeu, "Arene_Hunter")));
	carte_ajouterEvent(getCarte2(jeu, "Foret_Sud_Sarosa"), 0, 28, 0, E_TP, event_creerTP(26, 48, getCarte2(jeu, "Sarosa")));

	// Création des jouer musiques (+ message) (ptr carte, numPage, {xCase, yCase}, e_type = E_JM, event_creerJM(ptr musique))
	carte_ajouterEvent(getCarte2(jeu, "Sarosa"), 0, 6, 17, E_MSG, event_creerMsg("La musique \"Hunter\" sera joué après ce message"));
	carte_ajouterEvent(getCarte2(jeu, "Sarosa"), 0, 6, 17, E_JM, event_creerJM(getMusique2(jeu, "hunter.ogg")));
	carte_ajouterEvent(getCarte2(jeu, "Sarosa"), 0, 6, 17, E_MSG, event_creerMsg("Arrêt de la musique \"Hunter\" après ce message"));
	carte_ajouterEvent(getCarte2(jeu, "Sarosa"), 0, 6, 17, E_AM, NULL);
	carte_ajouterEvent(getCarte2(jeu, "Sarosa"), 0, 6, 17, E_MSG, event_creerMsg("On remet la musique de \"Sarosa\" après ce message"));
	carte_ajouterEvent(getCarte2(jeu, "Sarosa"), 0, 6, 17, E_JM, event_creerJM(getMusique2(jeu, "Sarosa.mp3")));

	// Création des modifications de PV du héros (ajouterCarte(ptr carte, numPage, {xCase, yCase}, e_type = E_CPV, event_creerChangePV(valeur)))
	carte_ajouterEvent(getCarte2(jeu, "Arene_Hunter"),0, 6, 8, E_CPV, event_creerChangePV(-15));
	carte_ajouterEvent(getCarte2(jeu, "Arene_Hunter"),0, 22, 8, E_CPV, event_creerChangePV(15));

	// Création des modifications de PM du héros (ajouterCarte(ptr carte, numPage, {xCase, yCase}, e_type = E_CPM, event_creerChangePM(valeur)))
	carte_ajouterEvent(getCarte2(jeu, "Arene_Hunter"), 0, 6, 26, E_CPM, event_creerChangePM(-6));
	carte_ajouterEvent(getCarte2(jeu, "Arene_Hunter"), 0, 22, 26, E_CPM, event_creerChangePM(6));
}

static void ajouterMonstreCarte(carte_t *carte, monstreData_t *data, const int x, const int y) {
	monstre_t *monstre = NULL;
	monstre_result_t res = monstre_creer(&monstre, data, x, y);
	if (res != MONSTRE_OK) { LOG_ERROR("Monstre : %s", monstre_strerror(res)); Exception(""); }
	carte_ajouterMonstre(carte, monstre);
}

static void creation_monstres(jeu_t *jeu) {
	// Création des monstres (carte_ajouterMonstre(ptr carte, ptr monstre, ptr jeu))
	ajouterMonstreCarte(getCarte2(jeu, "Sarosa_Foret_Est"), getMonstreData2(jeu, "Blob Bleu"), 9, 23);
	ajouterMonstreCarte(getCarte2(jeu, "Sarosa_Foret_Est"), getMonstreData2(jeu, "Blob Bleu"), 34, 13);
	ajouterMonstreCarte(getCarte2(jeu, "Sarosa_Foret_Est"), getMonstreData2(jeu, "Lapin"), 33, 24);
	ajouterMonstreCarte(getCarte2(jeu, "Arene_Hunter"), getMonstreData2(jeu, "Blob Bleu"), 9, 9);
	ajouterMonstreCarte(getCarte2(jeu, "Arene_Hunter"), getMonstreData2(jeu, "Lapin"), 10, 9);
	ajouterMonstreCarte(getCarte2(jeu, "Arene_Hunter"), getMonstreData2(jeu, "Blob Bleu"), 9, 10);
	ajouterMonstreCarte(getCarte2(jeu, "Arene_Hunter"), getMonstreData2(jeu, "Lapin"), 10, 10);
}

jeu_t * jeu_creer(SDL_Renderer *renderer) {
	jeu_validerArguments(renderer);

	jeu_t *jeu = calloc(1, sizeof(jeu_t));
	if (!jeu) Exception("Echec allocation jeu");

	jeu->enCours = false;
	jeu->frames = jeu->FPS_result = jeu->numCouleur_cadres = 0;

	jeu->couleurs_cadres[0] = BLEU_FONCE_TRANSPARENT;
	jeu->couleurs_cadres[1] = VERT_FONCE_TRANSPARENT;
	jeu->couleurs_cadres[2] = BORDEAUX_TRANSPARENT;
	jeu->couleurs_cadres[3] = OR_FONCE_TRANSPARENT;
	jeu->couleurs_cadres[4] = GRIS_FONCE_TRANSPARENT;
	jeu->deuxTiersSeconde = 1000.0f * 2.0f / 3.0f;

	creation_affichages(renderer, jeu);
	chargerSkins_get(renderer, &jeu->skins);
	creation_monstresData(renderer, jeu);
	creation_polices(jeu);
	chargerBruitages_get(&jeu->bruitages);
	chargerMusiques_get(&jeu->musiques);
	chargerChipsets_get(renderer, &jeu->chipsets);
	creation_cartes(jeu);
	creation_events(jeu);
	creation_monstres(jeu);

	const int xHerosEcran = (WINDOW_WIDTH / 2) % TAILLE_CASES == 0 ? WINDOW_WIDTH / 2 : (WINDOW_WIDTH / 2) - ((WINDOW_WIDTH / 2) % TAILLE_CASES);
	const int yHerosEcran = (WINDOW_HEIGHT / 2) % TAILLE_CASES == 0 ? WINDOW_HEIGHT / 2 : (WINDOW_HEIGHT / 2) - ((WINDOW_HEIGHT / 2) % TAILLE_CASES);
	// HIT BOX HEROS (A L'ECRAN)        ____.x_____  ____.y_____  _____.w_____  _____.h_____
	jeu->hitBoxHerosEcran = (SDL_Rect) {xHerosEcran, yHerosEcran, TAILLE_CASES, TAILLE_CASES};

	creation_heros(renderer, jeu);

	// HIT BOX EPEE (A L'ECRAN)                  ____.x_____  ________________.y___________________  _____.w_____  ______.h________
	jeu->hitBoxEpeeHerosEcran[BAS] = (SDL_Rect){ xHerosEcran, yHerosEcran + (TAILLE_CASES / 2) + 10, TAILLE_CASES, TAILLE_CASES / 2 };
	jeu->hitBoxEpeeHerosEcran[GAUCHE] = (SDL_Rect){ xHerosEcran - 10, yHerosEcran, TAILLE_CASES / 2, TAILLE_CASES };
	jeu->hitBoxEpeeHerosEcran[DROITE] = (SDL_Rect){ xHerosEcran + (TAILLE_CASES / 2) + 10, yHerosEcran, TAILLE_CASES / 2, TAILLE_CASES };
	jeu->hitBoxEpeeHerosEcran[HAUT] = (SDL_Rect){ xHerosEcran, yHerosEcran - (TAILLE_CASES / 2) + 10, TAILLE_CASES, TAILLE_CASES / 2 };

	memset(jeu->message, 0, sizeof(jeu->message));
	memset(jeu->messageCharNbOctets, 0, sizeof(jeu->messageCharNbOctets));
	memset(jeu->recapMessages, 0, sizeof(jeu->recapMessages));

	for (int i = 0; i < 2; ++i) { //        .x  _______________.y________________   _______.w________  _______________.h________________
		jeu->dstRectFiolePV[i] = (SDL_Rect){ 1, WINDOW_HEIGHT * (0.69 + i * 0.073), WINDOW_WIDTH / 25, WINDOW_HEIGHT * (0.25 - i * 0.072) };
		jeu->dstRectFiolePM[i] = (SDL_Rect){ WINDOW_WIDTH * 0.96 - 1, WINDOW_HEIGHT * (0.69 + i * 0.073), WINDOW_WIDTH / 25, WINDOW_HEIGHT * (0.25 - i * 0.072) };
		for (int j = 0; j < 3; ++j) { //            __.x__  _______.y______  .w  ____.h_____
			jeu->srcRectFiolePV[i][j] = (SDL_Rect){ i * 16, i * 21 + j * 72, 16, 72 - i * 21 };
			jeu->srcRectFiolePM[i][j] = (SDL_Rect){ 32 + i * 16, i * 21 + j * 72, 16, 72 - i * 21 };
		}
	}
	jeu->fiolesTiming = 0;
	jeu->delaiMessage = 0;
	jeu->compteurLettres = jeu->compteurLettresReelles = 0;
	for (int i = 0; i < 3; ++i) {
		jeu->textureFiolePVMorte[i] = creerTextureVide(renderer, 16, 51);
		jeu->textureFiolePMMorte[i] = creerTextureVide(renderer, 16, 51);
	}
	jeu->dstRectBarreXP = (SDL_Rect) { WINDOW_WIDTH * 0.02, WINDOW_HEIGHT * 0.95, WINDOW_WIDTH * 0.96, WINDOW_HEIGHT * 0.08 };
	jeu->compteurRecap = 0;
	jeu->afficherRecap = 0;
	jeu->degatsAffiches = 0;

	jeu->estCoupCritique = jeu->mursVisibles = jeu->menuVisible = false;
	jeu->alEventsActuels = NULL;
	jeu->lesHitBoxDesMonstresTouches = NULL;
	arraylist_creer(&jeu->lesHitBoxDesMonstresTouches, AL_SDL_RECT);
	jeu->nbEventPass = 0;

	return jeu;
}

SDL_Texture * getAffichage(jeu_t *jeu, int pos) { return arraylist_get(jeu->affichages, pos); }
skin_t * getSkin(jeu_t *jeu, int pos) { return arraylist_get(jeu->skins, pos); }
monstreData_t * getMonstreData(jeu_t *jeu, int pos) { return arraylist_get(jeu->monstresData, pos); }
TTF_Font * getPolice(jeu_t *jeu, int pos) { return arraylist_get(jeu->polices, pos); }
musique_t * getMusique(jeu_t *jeu, int pos) { return arraylist_get(jeu->musiques, pos); }
bruitage_t * getBruitage(jeu_t *jeu, int pos) { return arraylist_get(jeu->bruitages, pos); }
chipset_t * getChipset(jeu_t *jeu, int pos) { return arraylist_get(jeu->chipsets, pos); }
carte_t * getCarte(jeu_t *jeu, int pos) { return arraylist_get(jeu->cartes, pos); }
event_t * getEventActuel(jeu_t *jeu, int pos) { return arraylist_get(jeu->alEventsActuels, pos); }
SDL_Rect * getHitBoxMonstreTouche(jeu_t *jeu, int pos) { return arraylist_get(jeu->lesHitBoxDesMonstresTouches, pos); }

monstreData_t * getMonstreData2(jeu_t *jeu, const char *nom) {
	for (int i = 0; i < jeu->monstresData->taille; ++i) {
		monstreData_t *monstreData = arraylist_get(jeu->monstresData, i);
		if (strcmp(monstreData->nom, nom) == 0) return monstreData;
	}
	return NULL;
}

chipset_t * getChipset2(jeu_t *jeu, const char *nom) {
	for (int i = 0; i < jeu->chipsets->taille; ++i) {
		chipset_t *chipset = arraylist_get(jeu->chipsets, i);
		if (strcmp(chipset->nom, nom) == 0) return chipset;
	}
	return NULL;
}

carte_t * getCarte2(jeu_t *jeu, const char *nom) {
	for (int i = 0; i < jeu->cartes->taille; ++i) {
		carte_t *carte = arraylist_get(jeu->cartes, i);
		if (strcmp(carte->nom, nom) == 0) return carte;
	}
	return NULL;
}

musique_t * getMusique2(jeu_t *jeu, const char *nom) {
	for (int i = 0; i < jeu->musiques->taille; ++i) {
		musique_t * musique = arraylist_get(jeu->musiques, i);
		if (strcmp(musique->nom, nom) == 0) return musique;
	}
	return NULL;
}

void jeu_updateOffSetHeros(jeu_t *jeu) {
	jeu->xOffSetHeros = jeu->hitBoxHerosEcran.x - jeu->heros->position.x;
	jeu->yOffSetHeros = jeu->hitBoxHerosEcran.y - jeu->heros->position.y;
}

void jeu_ajouterAffichage(SDL_Renderer *renderer, const char *nomFichier, jeu_t *jeu) { arraylist_add(jeu->affichages, creerImage(renderer, nomFichier)); }

void jeu_ajouterMonstreData(SDL_Renderer *renderer, const char *nomFichier, const char *nom, int PVMax, int xp, int piecesOr, jeu_t *jeu) {
	monstreData_t *monstreData = NULL;
	monstreData_result_t res = monstreData_creer(&monstreData, renderer, nomFichier, nom, PVMax, xp, piecesOr);
	if (res != MONSTREDATA_OK) { LOG_ERROR("MonstreData : %s", monstreData_strerror(res)); Exception(""); }
	arraylist_add(jeu->monstresData, monstreData);
}

void jeu_ajouterPolice(const char *nomFichier, int taille, jeu_t *jeu) { arraylist_add(jeu->polices, creerPolice(nomFichier, taille)); }

void jeu_ajouterCarte(const char *nom, int hauteur, int largeur, const char *nomChipset, const char *nomMusique, jeu_t *jeu) {
	arraylist_add(jeu->cartes, carte_creerDepuisFichiers(nom, hauteur, largeur, getChipset2(jeu, nomChipset), getMusique2(jeu, nomMusique)));
}

void jeu_ajouterCarteVide(const char *nom, int hauteur, int largeur, chipset_t *chipset, musique_t *musique, jeu_t *jeu) { arraylist_add(jeu->cartes, carte_creer(nom, hauteur, largeur,chipset, musique, false)); }

void musique_stopAndPlay(musique_t *musiqueActuelle, musique_t *musiqueSuivante) {
	if (musiqueActuelle) musique_stop(musiqueActuelle);
	if (musiqueSuivante) musique_play(musiqueSuivante);
}

void viderMessage(jeu_t *jeu) {
	memset(jeu->message[0], 0, TAILLE_MAX_MSG_REELLE);
	memset(jeu->messageCharNbOctets[0], 0, TAILLE_MAX_MSG);
	jeu->compteurLettres = jeu->compteurLettresReelles = 0;
}

void sauvegarderMessage(jeu_t *jeu) {
	strcpy(jeu->message[1], jeu->message[0]);
	copyIntArray(jeu->messageCharNbOctets[1], jeu->messageCharNbOctets[0], TAILLE_MAX_MSG);
}

void remettreDernierMessage(jeu_t *jeu) {
	strcpy(jeu->message[0], jeu->message[1]);
	copyIntArray(jeu->messageCharNbOctets[0], jeu->messageCharNbOctets[1], TAILLE_MAX_MSG);
	jeu->compteurLettresReelles = strlen(jeu->message[0]);
	int compteur = 0;
	while (compteur < TAILLE_MAX_MSG && jeu->messageCharNbOctets[0][compteur] != 0) compteur++;
	jeu->compteurLettres = compteur;
}

void ajouterMessageHistorique(jeu_t *jeu) {
	if (jeu->compteurRecap == 3) {
		for (int i = 0; i < 2; ++i) {
			strcpy(jeu->recapMessages[i], jeu->recapMessages[i + 1]);
		}
		jeu->compteurRecap = 2;
	}
	strcpy(jeu->recapMessages[jeu->compteurRecap], jeu->message[0]);
	jeu->compteurRecap++;
}

void afficherHitboxAttaqueEpee(SDL_Renderer *renderer, jeu_t *jeu) {
	dessinerRectangle(renderer ,&jeu->hitBoxEpeeHerosEcran[jeu->heros->direction], BLANC_TRANSPARENT);
}

static void dessinerFiolePVMorte(SDL_Renderer *renderer, jeu_t *jeu, int index, int hauteur) {
    SDL_Rect src = { 16, 21 + index * 72, 16, hauteur };
    SDL_Rect dst = { 0, 0, 16, hauteur };

    SDL_DestroyTexture(jeu->textureFiolePVMorte[index]);
    jeu->textureFiolePVMorte[index] = creerTextureVide(renderer, 16, 51);
    SDL_SetRenderTarget(renderer, jeu->textureFiolePVMorte[index]); // dessiner sur cette texture
    dessinerTexture(renderer, getAffichage(jeu, 0), &src, &dst, "Echec dessin fiole PV morte SDL_RenderCopy");
}

void updateFiolePV(SDL_Renderer *renderer, jeu_t *jeu) {
	const double ratioPV = heros_getRatioPV(jeu->heros);
	const int hauteur = maxInt(1, (int)(51 * (1 - ratioPV)));
	for (int i = 0; i < 3; ++i) dessinerFiolePVMorte(renderer, jeu, i, hauteur);
	SDL_SetRenderTarget(renderer, NULL); // redessine sur le renderer principal
}

static void dessinerFiolePMMorte(SDL_Renderer *renderer, jeu_t *jeu, int index, int hauteur) {
	SDL_Rect src = { 48, 21 + index * 72, 16, hauteur };
	SDL_Rect dst = { 0, 0, 16, hauteur };

	SDL_DestroyTexture(jeu->textureFiolePMMorte[index]);
	jeu->textureFiolePMMorte[index] = creerTextureVide(renderer, 16, 51);
	SDL_SetRenderTarget(renderer, jeu->textureFiolePMMorte[index]); // dessiner sur cette texture
	dessinerTexture(renderer, getAffichage(jeu, 0), &src, &dst, "Echec dessin fiole PM morte SDL_RenderCopy");
}

void updateFiolePM(SDL_Renderer *renderer, jeu_t *jeu) {
	const double ratioPM = heros_getRatioPM(jeu->heros);
	const int hauteur = maxInt(1, (int) (51 * (1 - ratioPM)));
	for (int i = 0; i < 3; ++i) dessinerFiolePMMorte(renderer, jeu, i, hauteur);
	SDL_SetRenderTarget(renderer, NULL); // redessine sur le renderer principal
}

void afficherFiolePV(SDL_Renderer *renderer, jeu_t *jeu) {
	dessinerTexture(renderer, getAffichage(jeu, 0), &jeu->srcRectFiolePV[0][jeu->fiolesTiming], &jeu->dstRectFiolePV[0], "Echec dessin fiole PV vivante avec SDL_RenderCopy");
	dessinerTexture(renderer, jeu->textureFiolePVMorte[jeu->fiolesTiming], NULL, &jeu->dstRectFiolePV[1], "Echec dessin fiole PV morte avec SDL_RenderCopy");
}

void afficherFiolePM(SDL_Renderer *renderer, jeu_t *jeu) {
	dessinerTexture(renderer, getAffichage(jeu, 0), &jeu->srcRectFiolePM[0][jeu->fiolesTiming], &jeu->dstRectFiolePM[0], "Echec dessin fiole PM vivante avec SDL_RenderCopy");
	dessinerTexture(renderer, jeu->textureFiolePMMorte[jeu->fiolesTiming], NULL, &jeu->dstRectFiolePM[1], "Echec dessin fiole PM morte avec SDL_RenderCopy");
}

void afficherBarreXP(SDL_Renderer *renderer, jeu_t *jeu) {
	dessinerTexture(renderer, getAffichage(jeu, 1), NULL, &jeu->dstRectBarreXP, "Echec dessin barre XP avec SDL_RenderCopy");
}

static void calculerBorneFenetre(const carte_t *carte, const jeu_t *jeu, int *x0, int *x1, int *y0, int *y1) {
	const int herosXCase = heros_getXCase(jeu->heros);
	const int herosYCase = heros_getYCase(jeu->heros);

	*x0 = maxInt(herosXCase - WINDOW_WIDTH_CASES_DIV2 - 1, 0);
	*x1 = minInt(herosXCase + WINDOW_WIDTH_CASES_DIV2 + 2, carte->largeur);
	*y0 = maxInt(herosYCase - WINDOW_HEIGHT_CASES_DIV2 - 1, 0);
	*y1 = minInt(herosYCase + WINDOW_HEIGHT_CASES_DIV2 + 2, carte->hauteur);
	//printf("x0 = %d, x1 = %d, y0 = %d, y1 = %d\n", x0, x1, y0, y1);
}

void afficherCouche(SDL_Renderer *renderer, carte_t *carte, int couche, jeu_t *jeu) {
	SDL_Rect dstRect = { 0, 0, TAILLE_CASES, TAILLE_CASES };
	int x0, x1, y0, y1;
	calculerBorneFenetre(carte, jeu, &x0, &x1, &y0, &y1);
	//SDL_GetWindowSurface(window);

	for (int i = y0; i < y1; ++i) {
		for (int j = x0; j < x1; ++j) {
			const int numTuile = carte->couches[couche][i][j];
			if (numTuile != -1) {
				dstRect.x = j * TAILLE_CASES + jeu->xOffSetHeros;
				dstRect.y = i * TAILLE_CASES + jeu->yOffSetHeros;

				dessinerTexture(renderer, carte->chipset->texture, &carte->chipset->tuiles[numTuile], &dstRect, "Echec dessin texture d'une tuile chipset sur une couche avec SDL_RenderCopy");
			}
		}
	}
}

void afficherMurs(SDL_Renderer *renderer, carte_t *carte, jeu_t *jeu) {
	SDL_Rect murRect = { 0, 0, TAILLE_CASES, TAILLE_CASES };
	int x0, x1, y0, y1;
	calculerBorneFenetre(carte, jeu, &x0, &x1, &y0, &y1);

	for (int i = y0; i < y1; ++i) {
		for (int j = x0; j < x1; ++j) {
			if (carte->murs[i][j]) {
				murRect.x = j * TAILLE_CASES + jeu->xOffSetHeros;
				murRect.y = i * TAILLE_CASES + jeu->yOffSetHeros;

				dessinerRectangle(renderer, &murRect, VIOLET_TRANSPARENT);
			}
		}
	}
}

void jeu_detruire(jeu_t *jeu) {
	if (!jeu) return;
	for (int i = 0; i < 3; ++i) {
		SDL_DestroyTexture(jeu->textureFiolePVMorte[i]);
		SDL_DestroyTexture(jeu->textureFiolePMMorte[i]);
	}
	heros_detruire(jeu->heros);
	arraylist_detruire(jeu->lesHitBoxDesMonstresTouches, false);
	arraylist_detruire(jeu->cartes, true);
	arraylist_detruire(jeu->chipsets, true);
	arraylist_detruire(jeu->bruitages, true);
	arraylist_detruire(jeu->musiques, true);
	arraylist_detruire(jeu->polices, true);
	arraylist_detruire(jeu->monstresData, true);
	arraylist_detruire(jeu->skins, true);
	arraylist_detruire(jeu->affichages, true);
	free(jeu);
}