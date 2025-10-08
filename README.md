# SO-Remaster v1.9
Projet de recréation du jeu vidéo Slayers Online(MMORPG) en C (en offline pour le moment, il n'est pas ligne)  
Le moteur de jeu RPG en C/SDL2 est entièrement conçu et codé par Metroidzeta.  

## POUR DEMARRER LE PROGRAMME

Tout d’abord compiler avec Windows (dans le dossier principal) :  
```gcc -Wall src/*.c -o prog -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image```  
Puis exécuter :  
```./prog```  

![Slayers-Online-Remaster-screen-1](https://raw.githubusercontent.com/Metroidzeta/SO-Remaster/refs/heads/main/captures/Slayers-Online-remaster-v14-screen-1.png)
![Slayers-Online-Remaster-screen-2](https://raw.githubusercontent.com/Metroidzeta/SO-Remaster/refs/heads/main/captures/Slayers-Online-remaster-v14-screen-2.png)

## CONTROLES

**Flèches directionnelles ← ↑ ↓ → :** bouger  
**Touches A:** voir les murs  
**Touche S:** attaquer (avec une hitBox visible)  
**Touche ECHAP:** voir le menu (par défaut les statistiques)  
**Touche Q:** quitter le jeu (!attention, cette touche quitte le jeu instantanément)  

## TELECHARGER LE JEU

[Télécharger le jeu en entier ici (v1.9)](https://mega.nz/file/ksU3UYqA#EPViM6-E5P84TvPkfbFO-jK5mYxzm0ZUXRbXgYxhkpg)
