#include "cmc.h"

//Les fonctions de ce fichier sont extraites du projet "Minesweeper".


void tronquer (char txt[])
//Enlève le dernier caractère d'une string (reçue en paramètre et modifiée directement à la source vu qu'une string est un array qui est en fait un pointeur...).
{
	int compteur;
	
	for (compteur = 0; txt[compteur] != '\000'; compteur++) {}
	txt[compteur - 1] = '\000';
}


char* enleve_majuscule (char string[])
//Remplace chaque majuscule de la string reçue en paramètre par la minuscule correspondante.
//Renvoie la string sans majuscules.
//Équivalent de strlwr().
{
	for (int compteur = 0; string[compteur] != '\0'; compteur++)
	{string[compteur] = tolower(string[compteur]);}
	return string;
}

int afficher_txt (char txt[], int x, int y, int longueur_max, TTF_Font* police, SDL_Color couleur, SDL_Renderer* renderer)
//Affiche du texte dans une fenêtre.
//Renvoie la longueur du texte affiché.
/* Paramètres:	- txt = texte à afficher
				- x, y = coordonnées du coin supérieur gauche du texte
				- longueur_max = longueur maximale du texte (changera de ligne si plus long)
				- police = la police à utiliser
				- couleur = la couleur du texte
				- renderer = le renderer où s'affichera le texte ou NULL si on ne veut pas l'afficher */
{
	if (police == NULL || txt[0] == '\000')
	{return 0;}
	
	SDL_Surface* surface = TTF_RenderUTF8_Blended_Wrapped(police, txt, couleur, longueur_max); //Utiliser "blended" plutôt que "Solid" rend le txt bcp plus beau!!! (probablement à cause que je render avec du alpha blending...)
	SDL_Rect rect = {x, y, surface->w, surface->h};
	
	if (renderer != NULL)
	{
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_RenderCopy(renderer, texture, NULL, &rect);
		
		SDL_DestroyTexture(texture);
	}
	
	SDL_FreeSurface(surface);
	return rect.w;
}


int afficher_txt_centre (char txt[], int x_gauche, int x_droite, int y, TTF_Font* police, SDL_Color couleur, SDL_Renderer* renderer)
//Affiche du texte à l'écran en le centrant entre deux valeurs de x.
//Renvoie la longueur du texte affiché.
//ATTENTION: Le centrage ne fonctionne pas si le texte est sur plus d'une ligne!
/* Paramètres: 	- txt = texte à afficher
				- x_gauche, x_droite = limites gauche et droite de l'endroit où sera affiché le texte
				- y = hauteur du texte (le haut du texte)
				- police = la police à utiliser
				- couleur = la couleur du texte
				- renderer = le renderer à utiliser ou NULL si on ne veut pas afficher le texte */
{
	if (police == NULL || txt[0] == '\000')
	{return 0;}
	
	SDL_Surface* surface = TTF_RenderUTF8_Blended_Wrapped(police, txt, transparent, x_droite - x_gauche);
	int longueur = surface->w;
	
	afficher_txt(txt, x_gauche + (x_droite - x_gauche - longueur) / 2, y, x_droite - x_gauche, police, couleur, renderer);
	
	SDL_FreeSurface(surface);
	return longueur;
}


void rectangle (int x, int y, int largeur, int hauteur, int epaisseur, SDL_Color couleur, SDL_Color fond, SDL_Renderer* renderer)
//Affiche un rectangle à l'écran, selon les paramètres spécifiés:
/* Paramètres:	- x, y = coordonnées du coin supérieur gauche du rectangle à dessiner
				- largeur = largeur du rectangle
				- hauteur = hauteur du rectangle
				- epaisseur = épaisseur du contour du rectangle (de 1 à 5 pixels) (0 = rectangle plein)
				- couleur = couleur du rectangle
				- fond = couleur du fond ("background")
				- renderer = renderer à utiliser */
{
	SDL_Rect rect = {x, y, largeur, hauteur};
	
	SDL_SetColor(couleur, renderer);
	
	if (!epaisseur) //plein
	{SDL_RenderFillRect(renderer, &rect);}
		
	else //vide
	{
		SDL_RenderDrawRect(renderer, &rect); //rectangle aux coordonnées spécifiées
		if (epaisseur >= 2) //vers l'intérieur
		{
			rect.x++; rect.y++; rect.w -= 2; rect.h -= 2;
			SDL_RenderDrawRect(renderer, &rect);
		}
		if (epaisseur >= 3) //vers l'extérieur
		{
			rect.x -= 2; rect.y -= 2; rect.w += 4; rect.h += 4;
			SDL_RenderDrawRect(renderer, &rect);
			if (epaisseur <= 4)
			{
				SDL_SetColor(fond, renderer);
				SDL_RenderDrawPoint(renderer, x - 1, y - 1);
				SDL_RenderDrawPoint(renderer, x - 1, y + hauteur);
				SDL_RenderDrawPoint(renderer, x + largeur, y - 1);
				SDL_RenderDrawPoint(renderer, x + largeur, y + hauteur);
				SDL_SetColor(couleur, renderer);
			}
		}
		if (epaisseur >= 4) //vers l'intérieur x2
		{
			rect.x = x + 2; rect.y = y + 2; rect.w = largeur - 4; rect.h = hauteur - 4;
			SDL_RenderDrawRect(renderer, &rect);
		}
		if (epaisseur >= 5) //vers l'extérieur x2
		{
			rect.x -= 4; rect.y -= 4; rect.w += 8; rect.h += 8;
			SDL_RenderDrawRect(renderer, &rect);
			
			SDL_RenderDrawPoint(renderer, x + 3, y + 3);
			SDL_RenderDrawPoint(renderer, x + largeur - 4, y + 3);
			SDL_RenderDrawPoint(renderer, x + 3, y + hauteur - 4);
			SDL_RenderDrawPoint(renderer, x + largeur - 4, y + hauteur - 4);
			
			SDL_SetColor(fond, renderer);
			
			SDL_RenderDrawPoint(renderer, x - 2, y - 2);
			SDL_RenderDrawPoint(renderer, x - 2, y - 1);
			SDL_RenderDrawPoint(renderer, x - 1, y - 2);
			
			SDL_RenderDrawPoint(renderer, x - 2, y + hauteur);
			SDL_RenderDrawPoint(renderer, x - 2, y + hauteur + 1);
			SDL_RenderDrawPoint(renderer, x - 1, y + hauteur + 1);
			
			SDL_RenderDrawPoint(renderer, x + largeur, y - 2);
			SDL_RenderDrawPoint(renderer, x + largeur + 1, y - 2);
			SDL_RenderDrawPoint(renderer, x + largeur + 1, y - 1);
			
			SDL_RenderDrawPoint(renderer, x + largeur + 1, y + hauteur + 1);
			SDL_RenderDrawPoint(renderer, x + largeur, y + hauteur + 1);
			SDL_RenderDrawPoint(renderer, x + largeur + 1, y + hauteur);
		}
	}
}

bool demander_txt (char titre_recu[], char explications[], char input[], int max, SDL_Window* fenetre_source)
//Créé une nouvelle fenêtre de style "pop-up" pour demander un input de texte à l'utilisateur.
//Renvoie 1 en cas de succès et 0 en cas d'erreur.
/* Paramètres:	- titre_recu = titre de la fenêtre (maximum 200 caractères)
				- explications = texte expliquant à l'utilisateur ce qu'il doit écrire
				- input = string où sera enregistré l'input de l'utilisateur
				- max = taille maximale de la string input (devrait donc toujours être "sizeof(input)")
				- fenetre_source = ptr vers la structure SDL_Window de la fenêtre à partir de laquelle est appelée cette fonction */
{
	SDL_Window* fenetre_d;
	SDL_Renderer* rend_d;
	Uint32 ID_fenetre_d;
	SDL_Event ev;
	char titre_fenetre[230] = "CMC - ";
	char ancien_input[max];
	char buffer[300];
	char focus = 0; //0 = nulle part, 'i' = boîte d'input, 'a' = bouton "annuler", 't' = bouton "terminé". S'applique à la sélection clavier "seulement".
	unsigned termine = 2; //0 ou 1 = valeur à retourner, 2 = pas terminé
	
	//Recopie de strings:
	strcat(titre_fenetre, titre_recu);
	if (input[0] == '\000')
	{ancien_input[0] = '\000';}
	else
	{strcpy(ancien_input, input);}
	
	//Création de la fenêtre:
	fenetre_d = SDL_CreateWindow(titre_fenetre, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 650, 400, 0); //fenêtre centrée et non resizeable
	if (fenetre_d == NULL)
	{sprintf(buffer, "Erreur lors de la création de la fenêtre SDL pour demander du texte:\n%s\n", SDL_GetError()); erreur(50, buffer); return 0;}
	
	//Création du renderer:
	rend_d = SDL_CreateRenderer(fenetre_d, -1, 0);
	if (rend_d == NULL)
	{sprintf(buffer, "Erreur lors de la création du renderer SDL de la fenêtre pour demander du texte:\n%s\n", SDL_GetError()); erreur(51, buffer); SDL_DestroyWindow(fenetre_d); return 0;}
	SDL_SetRenderDrawBlendMode(rend_d, SDL_BLENDMODE_BLEND); //permet l'utilisation de couleurs semi-transparentes (et transparentes)
	
	//Taille minimale et ID de la fenêtre:
	ID_fenetre_d = SDL_GetWindowID(fenetre_d);
	SDL_SetWindowResizable(fenetre_source, SDL_FALSE); //la fenêtre source n'a plus d'affaire à se faire resizer...
	SDL_RaiseWindow(fenetre_d);
	
	//Dessin de la fenêtre et gestion de l'input utilisateur:
	while (termine >= 2)
	{
		//Remplissage avec la couleur du fond:
		SDL_SetColor(fond, rend_d);
		SDL_RenderClear(rend_d);
		
		//Affichage du titre:
		TTF_SetFontStyle(police, TTF_STYLE_UNDERLINE);
		afficher_txt_centre(titre_recu, 0, 650, 5, police, couleur_texte, rend_d);
		TTF_SetFontStyle(police, TTF_STYLE_NORMAL);
		
		//Affichage des instructions:
		afficher_txt(explications, 30, 50, 590, petite_police, couleur_texte, rend_d);
		
		//Affichage de la boîte d'input:
		rectangle(30, 260, 590, 40, 0, couleur_boutons, fond, rend_d);
		if (focus == 'i')
		{rectangle(30, 260, 590, 40, 0, couleur_focus, fond, rend_d);}
		rectangle(30, 260, 590, 40, 4, couleur_texte, fond, rend_d);
		if (input[0] != '\000')
		{afficher_txt(input, 40, 270, 570, police, couleur_texte, rend_d);}
		
		//Affichage des boutons:
		rect_arrondi(370, 340, 120, 40, couleur_boutons, fond, rend_d);
		if (focus == 'a')
		{rect_arrondi(370, 340, 120, 40, couleur_selection, fond, rend_d);}
		if (ev.motion.x >= 370 && ev.motion.x <= 490 && ev.motion.y >= 340 && ev.motion.y <= 380)
		{rect_arrondi(370, 340, 120, 40, couleur_selection, fond, rend_d);}
		afficher_txt_centre("Annuler", 370, 490, 350, police, couleur_texte, rend_d);
		
		rect_arrondi(510, 340, 120, 40, couleur_boutons, fond, rend_d);
		if (focus == 't')
		{rect_arrondi(510, 340, 120, 40, couleur_selection, fond, rend_d);}
		if (ev.motion.x >= 510 && ev.motion.x <= 630 && ev.motion.y >= 340 && ev.motion.y <= 380)
		{rect_arrondi(510, 340, 120, 40, couleur_selection, fond, rend_d);}
		afficher_txt_centre("Terminé", 510, 630, 350, police, couleur_texte, rend_d);
		
		//Rendering et gestion de l'input utilisateur:
		SDL_RenderPresent(rend_d);
		SDL_WaitEvent(&ev);
		
		switch (ev.type)
		{
		case SDL_WINDOWEVENT:
			if (ev.window.windowID != ID_fenetre_d) //si l'utilisateur joue avec l'autre fenêtre (lui donnant ainsi le focus, déclenchant cet event), on veut le ramener à la bonne place
			{SDL_RaiseWindow(fenetre_d); SDL_FlashWindow(fenetre_d, SDL_FLASH_UNTIL_FOCUSED);}
			else if (ev.window.event == SDL_WINDOWEVENT_CLOSE) //SDL_QUIT ne fonctionne pas avec plusieurs fenêtres ouvertes...
			{termine = 0;}
			break;
		
		case SDL_MOUSEMOTION:
			if (ev.motion.x >= 30 && ev.motion.x <= 620 && ev.motion.y >= 260 && ev.motion.y <= 300)
			{SDL_SetCursor(curseur_txt);}
			else
			{SDL_SetCursor(curseur_normal);}
			//Le hovering des 2 autres boutons est géré avec l'affichage.
			break;
		
		case SDL_KEYDOWN:
			switch (ev.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				if (!focus)
				{termine = 0;}
				else
				{focus = 0;}
				break;
			
			case SDLK_TAB:
				if (focus == 'i')
				{focus = 'a';}
				else
				{focus = 'i';}
				break;
			
			case SDLK_RIGHT:
			case SDLK_LEFT:
				if (focus == 'a')
				{focus = 't';}
				else if (focus == 't')
				{focus = 'a';}
				break;
			
			case SDLK_RETURN:
			case SDLK_KP_ENTER:
				if (focus == 'i')
				{termine = 1;}
				else if (focus == 'a')
				{termine = 0;}
				else if (focus == 't')
				{termine = 1;}
				break;
			
			case SDLK_BACKSPACE:
				if (!focus)
				{focus = 'i';}
				if (focus == 'i' && strlen(input) > 0)
				{tronquer(input);}
				break;
			}
			break;
		
		case SDL_MOUSEBUTTONDOWN:
			if (ev.button.x >= 30 && ev.button.x <= 620 && ev.button.y >= 250 && ev.button.y <= 290)
			{focus = 'i';} //boîte d'input
			else if (ev.button.x >= 370 && ev.button.x <= 490 && ev.button.y >= 340 && ev.button.y <= 380)
			{termine = 0;}
			else if (ev.button.x >= 510 && ev.button.x <= 630 && ev.button.y >= 340 && ev.button.y <= 380)
			{termine = 1;}
			else
			{focus = 0;}
			break;
		
		case SDL_TEXTINPUT:
			focus = 'i';
			if (strlen(input) < max - 1)
			{strcat(input, ev.text.text);}
			break;
		}
	}
	
	//Destruction de la fenêtre et du renderer et retour aux réglages:
	SDL_DestroyRenderer(rend_d);
	SDL_DestroyWindow(fenetre_d);
	SDL_SetWindowResizable(fenetre_source, SDL_TRUE);
	
	//Remise de l'ancien input si nécessaire:
	if (!termine)
	{
		if (ancien_input[0] == '\000')
		{input[0] = '\000';}
		else
		{strcpy(input, ancien_input);}
	}
	
	return (bool) termine;
}

int demander_nbre (char titre_recu[], char explications[], int valeur_initiale, SDL_Window* fenetre_source)
//Créé une nouvelle fenêtre de style "pop-up" pour demander un nombre à l'utilisateur
//Noter que les valeurs maximales
//Renvoie la valeur reçue en input (maximum 99 999, minimum -99 999).
//Renvoie -6699 en cas d'erreur.
/* Paramètres:	- titre_recu = titre de la fenêtre (maximum 200 caractères)
				- explications = texte expliquant à l'utilisateur ce qu'il doit écrire
				- valeur_initiale = valeur de ce paramètre au moment de l'ouverture de cette fenêtre
				- fenetre_source = ptr vers la structure SDL_Window de la fenêtre à partir de laquelle est appelée cette fonction */
{
	SDL_Window* fenetre_d;
	SDL_Renderer* rend_d;
	Uint32 ID_fenetre_d;
	SDL_Event ev;
	char titre_fenetre[230] = "CMC - ";
	char buffer[300];
	char focus = 0; //0 = nulle part, 'i' = boîte d'input, 'a' = bouton "annuler", 't' = bouton "terminé". S'applique à la sélection clavier "seulement".
	int input = valeur_initiale;
	unsigned termine = 2; //0 ou 1 = valeur à retourner, 2 = pas terminé
	
	//Recopie de strings:
	strcat(titre_fenetre, titre_recu);
	//Création de la fenêtre:
	fenetre_d = SDL_CreateWindow(titre_fenetre, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 650, 400, 0); //fenêtre centrée et non resizeable
	if (fenetre_d == NULL)
	{sprintf(buffer, "Erreur lors de la création de la fenêtre SDL pour demander un nombre:\n%s\n", SDL_GetError()); erreur(50, buffer); return -6699;}
	
	//Création du renderer:
	rend_d = SDL_CreateRenderer(fenetre_d, -1, 0);
	if (rend_d == NULL)
	{sprintf(buffer, "Erreur lors de la création du renderer SDL de la fenêtre pour demander un nombre:\n%s\n", SDL_GetError()); erreur(51, buffer); SDL_DestroyWindow(fenetre_d); return -6699;}
	SDL_SetRenderDrawBlendMode(rend_d, SDL_BLENDMODE_BLEND); //permet l'utilisation de couleurs semi-transparentes (et transparentes)
	
	//Taille minimale et ID de la fenêtre:
	ID_fenetre_d = SDL_GetWindowID(fenetre_d);
	SDL_SetWindowResizable(fenetre_source, SDL_FALSE); //la fenêtre source n'a plus d'affaire à se faire resizer...
	SDL_RaiseWindow(fenetre_d);
	
	//Dessin de la fenêtre et gestion de l'input utilisateur:
	while (termine >= 2)
	{
		//Remplissage avec la couleur du fond:
		SDL_SetColor(fond, rend_d);
		SDL_RenderClear(rend_d);
		
		//Affichage du titre:
		TTF_SetFontStyle(police, TTF_STYLE_UNDERLINE);
		afficher_txt_centre(titre_recu, 0, 650, 5, police, couleur_texte, rend_d);
		TTF_SetFontStyle(police, TTF_STYLE_NORMAL);
		
		//Affichage des instructions:
		afficher_txt(explications, 30, 50, 590, petite_police, couleur_texte, rend_d);
		
		//Affichage de la boîte d'input (incluant le + et le -):
		rectangle(30, 260, 180, 40, 0, couleur_boutons, fond, rend_d);
		if (focus == 'i')
		{rectangle(70, 260, 100, 40, 0, couleur_focus, fond, rend_d);}
		if (focus == '-')
		{rectangle(30, 260, 40, 40, 0, couleur_selection, fond, rend_d);}
		else if (ev.motion.x >= 30 && ev.motion.x <= 70 && ev.motion.y >= 260 && ev.motion.y <= 300)
		{rectangle(30, 260, 40, 40, 0, couleur_focus, fond, rend_d);}
		if (focus == '+')
		{rectangle(168, 260, 40, 40, 0, couleur_selection, fond, rend_d);}
		else if (ev.motion.x >= 168 && ev.motion.x <= 208 && ev.motion.y >= 260 && ev.motion.y <= 300)
		{rectangle(168, 260, 40, 40, 0, couleur_focus, fond, rend_d);}
		rectangle(30, 260, 180, 40, 4, couleur_texte, fond, rend_d);
		SDL_SetColor(noir, rend_d);
		SDL_RenderDrawLine(rend_d, 69, 260, 69, 300);
		SDL_RenderDrawLine(rend_d, 70, 260, 70, 300);
		SDL_RenderDrawLine(rend_d, 71, 260, 71, 300);
		afficher_txt_centre("-", 30, 70, 268, police, couleur_texte, rend_d);
		SDL_RenderDrawLine(rend_d, 167, 260, 167, 300);
		SDL_RenderDrawLine(rend_d, 168, 260, 168, 300);
		SDL_RenderDrawLine(rend_d, 169, 260, 169, 300);
		afficher_txt_centre("+", 168, 208, 268, police, couleur_texte, rend_d);
		sprintf(buffer, "%d", input);
		afficher_txt(buffer, 80, 270, 80, police, couleur_texte, rend_d);
		
		//Affichage des boutons:
		rect_arrondi(370, 340, 120, 40, couleur_boutons, fond, rend_d);
		if (focus == 'a')
		{rect_arrondi(370, 340, 120, 40, couleur_selection, fond, rend_d);}
		if (ev.motion.x >= 370 && ev.motion.x <= 490 && ev.motion.y >= 340 && ev.motion.y <= 380)
		{rect_arrondi(370, 340, 120, 40, couleur_selection, fond, rend_d);}
		afficher_txt_centre("Annuler", 370, 490, 350, police, couleur_texte, rend_d);
		
		rect_arrondi(510, 340, 120, 40, couleur_boutons, fond, rend_d);
		if (focus == 't')
		{rect_arrondi(510, 340, 120, 40, couleur_selection, fond, rend_d);}
		if (ev.motion.x >= 510 && ev.motion.x <= 630 && ev.motion.y >= 340 && ev.motion.y <= 380)
		{rect_arrondi(510, 340, 120, 40, couleur_selection, fond, rend_d);}
		afficher_txt_centre("Terminé", 510, 630, 350, police, couleur_texte, rend_d);
		
		//Rendering et gestion de l'input utilisateur:
		SDL_RenderPresent(rend_d);
		SDL_WaitEvent(&ev);
		
		switch (ev.type)
		{
		case SDL_WINDOWEVENT:
			if (ev.window.windowID != ID_fenetre_d) //si l'utilisateur joue avec l'autre fenêtre (lui donnant ainsi le focus, déclenchant cet event), on veut le ramener à la bonne place
			{SDL_RaiseWindow(fenetre_d); SDL_FlashWindow(fenetre_d, SDL_FLASH_UNTIL_FOCUSED);}
			else if (ev.window.event == SDL_WINDOWEVENT_CLOSE) //SDL_QUIT ne fonctionne pas avec plusieurs fenêtres ouvertes...
			{termine = 0;}
			break;
		
		case SDL_MOUSEMOTION:
			if (ev.motion.x >= 70 && ev.motion.x <= 170 && ev.motion.y >= 260 && ev.motion.y <= 300)
			{SDL_SetCursor(curseur_txt);}
			else
			{SDL_SetCursor(curseur_normal);}
			//Le hovering des 2 boutons est géré avec l'affichage.
			break;
		
		case SDL_KEYDOWN:
			switch (ev.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				if (!focus)
				{termine = 0;}
				else
				{focus = 0;}
				break;
			
			case SDLK_TAB:
				if (focus == 'i')
				{focus = 'a';}
				else
				{focus = 'i';}
				break;
			
			case SDLK_RIGHT:
				if (focus == 'a')
				{focus = 't';}
				else if (focus == 't')
				{focus = 'a';}
				else if (focus == 'i')
				{focus = '+';}
				else if (focus == '-')
				{focus = 'i';}
				else if (focus == '+')
				{focus = '-';}
				break;
			
			case SDLK_LEFT:
				if (focus == 'a')
				{focus = 't';}
				else if (focus == 't')
				{focus = 'a';}
				else if (focus == '-')
				{focus = '+';}
				else if (focus == 'i')
				{focus = '-';}
				else if (focus == '+')
				{focus = 'i';}
				break;
			
			case SDLK_RETURN:
			case SDLK_KP_ENTER:
				if (focus == 'i')
				{termine = 1;}
				else if (focus == 'a')
				{termine = 0;}
				else if (focus == 't')
				{termine = 1;}
				else if (focus == '-')
				{input--;}
				else if (focus == '+')
				{input++;}
				break;
			
			case SDLK_BACKSPACE:
				if (!focus)
				{focus = 'i';}
				if (focus == 'i')
				{
					if (input >= 10)
					{input = input / 10;}
					else
					{input = 0;}
				}
				break;
			
			case SDLK_0:
			case SDLK_1:
			case SDLK_2:
			case SDLK_3:
			case SDLK_4:
			case SDLK_5:
			case SDLK_6:
			case SDLK_7:
			case SDLK_8:
			case SDLK_9:
				if (!focus)
				{focus = 'i';}
				if (input > - 10000 && input < 10000)
				{
					input = input * 10;
					input += ev.key.keysym.sym - SDLK_0;
				}
				break;
			
			case SDLK_KP_0:
				if (!focus)
				{focus = 'i';}
				if (input > - 10000 && input < 10000)
				{input = input * 10;}
				break;
			
			case SDLK_KP_1:
			case SDLK_KP_2:
			case SDLK_KP_3:
			case SDLK_KP_4:
			case SDLK_KP_5:
			case SDLK_KP_6:
			case SDLK_KP_7:
			case SDLK_KP_8:
			case SDLK_KP_9:
				if (!focus)
				{focus = 'i';}
				if (input > - 10000 && input < 10000)
				{
					input = input * 10;
					input += ev.key.keysym.sym - SDLK_KP_1 + 1;
				}
				break;
			
			case SDLK_MINUS:
			case SDLK_KP_MINUS:
				if (!focus)
				{focus = 'i';}
				input = -input;
				break;
			}
			break;
		
		case SDL_MOUSEBUTTONDOWN:
			if (ev.button.x >= 70 && ev.button.x <= 167 && ev.button.y >= 250 && ev.button.y <= 290)
			{focus = 'i';} //boîte d'input
			else if (ev.button.x >= 370 && ev.button.x <= 490 && ev.button.y >= 340 && ev.button.y <= 380)
			{termine = 0;}
			else if (ev.button.x >= 510 && ev.button.x <= 630 && ev.button.y >= 340 && ev.button.y <= 380)
			{termine = 1;}
			else if (ev.button.x >= 30 && ev.button.x <= 70 && ev.button.y >= 260 && ev.button.y <= 300)
			{input--;}
			else if (ev.button.x >= 168 && ev.button.x <= 208 && ev.button.y >= 260 && ev.button.y <= 300)
			{input++;}
			else
			{focus = 0;}
			break;
		}
	}
	
	//Destruction de la fenêtre et du renderer et retour aux réglages:
	SDL_DestroyRenderer(rend_d);
	SDL_DestroyWindow(fenetre_d);
	SDL_SetWindowResizable(fenetre_source, SDL_TRUE);
	
	if (!termine)
	{return -6699;}
	return input;
}