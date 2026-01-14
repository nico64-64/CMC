# CMC - Créateur de Mots Croisés

Programme graphique servant à créer des grilles de mots croisées, avec lesquelles on peut ensuite jouer.

Ce programme n'est pas destiné à rivaliser avec les créateurs de mots croisés modernes (notamment ceux en ligne), qui sont beaucoup plus complets.

Programmé en C avec SDL2 par Nicolas Audette.

## Installation

### Sur Linux
Utilisez la release 0.4 si vous avez un système x86_64 standard et que vous ne voulez pas compiler vous-même. Assurez-vous d'installer la librairie graphique SDL2 si elle n'est pas déjà installée sur votre système. Vous aurez besoin des packages `libsdl2`, `libsdl2_image` et `libsdl2_ttf` (les noms exacts varient selon la distribution).

Sinon, vous pouvez compiler le programme avec `gcc cmc.c -o cmc -lSDL2 -lSDL2_image -lSDL2_ttf`. Vous devez d'abord avoir installé les packages `libsdl2-dev`, `libsdl2_image-dev` et `libsdl2_ttf-dev` (les noms exacts varient selon la distribution).

Si vous voulez créer un launcher (fichier .desktop) pour pouvoir partir ce programme depuis votre desktop (bureau), assurez-vous de partir le script `run.sh`: celui-ci s'assurera à son tour que le programme sera en mesure de trouver ses fichiers (qui sont cherchés avec des chemins relatifs).<br>
Pour cela, vous devrez toutefois d'abord inscrire le chemin d'accès absolu du répertoire où vous avez installé ce programme dans `run.sh`.

### Sur Windows
Utilisez la release 0.4 cross-compilée pour Windows.

## Utilisation

Avec CMC, vous pouvez créer des grilles de mots croisés (enregistrées dans des fichiers .txt) et y jouer. Une grille déjà construite nommée "démo" est fournie avec le programme.

Vous pouvez en apprendre plus en consultant l'aide intégrée du programme.

## Notes

### Police
La police fournie avec ce programme (dejavu_sans.ttf) est licenciée par DejaVu Fonts. Consultez le fichier dejavu_licence.txt pour plus de détails.

Vous pouvez remplacer cette police par n'importe quelle autre police TTF dans les réglages du programme.

### Usage du terminal
#### Sur Linux
Vous pouvez démarrer le programme depuis votre terminal, ce qui vous permettra d'accéder à quelques options de démarrage, mais ce n'est vraiment pas nécessaire.

Vous pouvez entrer `./cmc -?` pour obtenir la liste de ces options.<br>
Avec une configuration normale, le terminal est inutile sauf pour le débogage.

#### Sur Windows
La version Windows de la release 0.4 a été compilée sans support pour le terminal, donc il ne sert à rien de démarrer le programme depuis le terminal.

### Réglage des couleurs
Le réglage des couleurs utilise le color picker de zenity (`zenity --color-picker`).<br>
Sur Linux, vous pouvez l'installer facilement (s'il ne l'est pas déjà), mais ce logiciel n'existe pas sur Windows.

Si vous ne pouvez/voulez pas utiliser le color picker de zenity, vous pouvez le remplacer par n'importe quel autre en modifiant le réglage approprié. Votre color picker devra toutefois fournir un output de style `rgb(R,G,B)` ou `rgba(R,G,B,A)` pour être utilisé par ce programme.

Si vous préférez modifier les valeurs numériques rgba manuellement ou si vous n'avez pas de color picker répondant à ces conditions, vous pouvez le faire directement dans le fichier ./source/reglages.txt qui sera créé lorsque vous ouvrirez les réglages du programme pour la première fois.<br>
Fermez d'abord le programme, puis faites vos modifications au fichier en respectant la syntaxe qui y est indiquée et enfin, démarrez le programme à nouveau pour apprécier vos changements.