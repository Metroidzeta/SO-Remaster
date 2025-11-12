# SO-Remaster v2.0
Projet de recréation d'un moteur de jeu 2D action-RPG inspiré du MMORPG Slayers Online (mais en offline pour le moment, il n'est pas en ligne)  
Le moteur est entièrement conçu et codé en C/SDL2 from scratch (de 0) par Metroidzeta (Alain Barbier).  

## POUR COMPILER LE PROGRAMME

Tout d’abord compiler avec Windows (à partir du dossier racine) :  
```gcc -Wall src/*.c -o prog -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image```  
Puis exécuter :  
```./prog```  

![Slayers-Online-Remaster-screen-1](https://raw.githubusercontent.com/Metroidzeta/SO-Remaster/refs/heads/main/captures/Slayers-Online-remaster-v14-screen-1.png)
![Slayers-Online-Remaster-screen-2](https://raw.githubusercontent.com/Metroidzeta/SO-Remaster/refs/heads/main/captures/Slayers-Online-remaster-v14-screen-2.png)

## 🎮 CONTRÔLES

**Flèches directionnelles ← ↑ ↓ → :** bouger (diagonale possible)  
**Touche A :** voir les murs  
**Touche B :** mettre le jeu en plein écran  
**Touche S :** attaquer (avec une hitBox visible)  
**Touche ENTREE :** écrire un message (puis le poster)  
**Touche ECHAP :** voir le menu (par défaut les statistiques)  
**Touche F1 :** voir l'historique des messages  
**Touche F3 :** réécrire le message précédent  
**Touche F5 :** changer de couleur de fenêtre  
**Touche Q :** quitter le jeu (/!\ attention, cette touche quitte le jeu instantanément)  

## TÉLÉCHARGER LE JEU

[Télécharger le jeu en entier ici (v2.0)](https://mega.nz/file/VpsUQDzT#rn6ould68k04i_2qtD2fqTvpgurVc1J4W6_AkItjuXM)

## POUR LANCER LE JEU

Depuis la racine du projet, exécuter le fichier ```prog.exe```

## 📜 LICENCE

Ce projet est distribué sous la Licence d’Utilisation Personnelle et Éducative (Educational and Personal Use License).

Vous pouvez lire, étudier, forker et modifier le code à titre personnel ou éducatif.

Vous ne pouvez pas publier, redistribuer ni utiliser ce code dans un cadre commercial ou public, sauf autorisation écrite de l’auteur.

Les ressources graphiques, musicales et sonores appartiennent à leurs auteurs respectifs et ne sont pas couvertes par cette licence.

👉 Voir le fichier complet [LICENSE.md](https://github.com/Metroidzeta/SO-Remaster/blob/main/LICENSE.md) pour les termes détaillés (versions française et anglaise).

📧 Contact : [Alain1293@outlook.fr](mailto:Alain1293@outlook.fr)

© 2025 Alain Barbier alias Metroidzeta – Tous droits réservés.