#include "backend.c"


void rafraichir(enum zones zone)
//Redessine la fenêtre affichant la grille en cours de création ou d'utilisation.
{
	SDL_Rect rect_symbole_editer_titre = {(xmax - longueur_txt(titre, xmax, police)) / 2 - 34, 6, 24, 24};
	SDL_Rect rect_symbole_taille_police = {marge_gauche + 335, marge_bas + 2, 40, 40};
	SDL_Rect rect_symbole_orientation = {marge_gauche + 135, marge_bas, 40, 40};
	int h = 0, v = 1;
	char buffer[320];
	
	//Calculs:
	
	marge_gauche = (xmax - 620) / 10;
	marge_droite = 250 + (xmax - 620) / 10;
	marge_droite = xmax - marge_droite + 20;
	
	largeur_grille = marge_droite - marge_gauche - 20;
	if (largeur_grille > ymax - 120)
	{
		largeur_grille -= largeur_grille + 120 - ymax;
		marge_gauche += (marge_droite - marge_gauche - 20 - largeur_grille) / 4;
		marge_droite = marge_gauche + largeur_grille + 20;
	}
	
	marge_bas = 40 + largeur_grille + 20;
	if (ymax - marge_bas < 20)
	{marge_bas = 0;}
	
	largeur_case = largeur_grille / nbre_cases;
	largeur_grille -= largeur_grille - largeur_case * nbre_cases; //rapetisse légèrement la grille pour que toutes les cases aient la même taille
	
	//Affichage:
	
	//Arrière-plan:
	SDL_SetColor(fond, rend);
	SDL_RenderClear(rend);
	
	//Titre et son bouton d'édition:
	TTF_SetFontStyle(police, TTF_STYLE_UNDERLINE);
	afficher_txt_centre(titre, 0, xmax, 5, police, couleur_texte, rend);
	TTF_SetFontStyle(police, TTF_STYLE_NORMAL);
	if (zone >= nouv_grille)
	{
		if (zone == nouv_modifier_titre)
		{rect_arrondi(rect_symbole_editer_titre.x - 1, 5, 25, 25, couleur_selection, fond, rend);}
		else
		{rect_arrondi(rect_symbole_editer_titre.x - 1, 5, 25, 25, couleur_boutons, fond, rend);}
		SDL_RenderCopy(rend, symbole_modifier, NULL, &rect_symbole_editer_titre);
	}
	
	//Ligne ou colonne de la case:
	if (orientation == HORIZONTAL && selection_y >= 0)
	{
		if (zone >= nouv_grille) //création
		{
			for (int compteur = 0; compteur < nbre_cases; compteur++)
			{rectangle(marge_gauche + largeur_case * compteur, 40 + largeur_case * selection_y, largeur_case, largeur_case, 0, couleur_ombre, fond, rend);}
		}
		else //jeu / utilisation
		{
			for (int compteur = selection_x + 1; compteur < nbre_cases && !grille[compteur][selection_y].vide; compteur++)
			{rectangle(marge_gauche + largeur_case * compteur, 40 + largeur_case * selection_y, largeur_case, largeur_case, 0, couleur_ombre, fond, rend);}
			for (int compteur = selection_x - 1; compteur >= 0 && !grille[compteur][selection_y].vide; compteur--)
			{rectangle(marge_gauche + largeur_case * compteur, 40 + largeur_case * selection_y, largeur_case, largeur_case, 0, couleur_ombre, fond, rend);}
		}
	}
	else if (selection_x >= 0)
	{
		if (zone >= nouv_grille) //création
		{
			for (int compteur = 0; compteur < nbre_cases; compteur++)
			{rectangle(marge_gauche + largeur_case * selection_x, 40 + largeur_case * compteur, largeur_case, largeur_case, 0, couleur_ombre, fond, rend);}
		}
		else //jeu / utilisation
		{
			for (int compteur = selection_y + 1; compteur < nbre_cases && !grille[selection_x][compteur].vide; compteur++)
			{rectangle(marge_gauche + largeur_case * selection_x, 40 + largeur_case * compteur, largeur_case, largeur_case, 0, couleur_ombre, fond, rend);}
			for (int compteur = selection_y - 1; compteur >= 0 && !grille[selection_x][compteur].vide; compteur--)
			{rectangle(marge_gauche + largeur_case * selection_x, 40 + largeur_case * compteur, largeur_case, largeur_case, 0, couleur_ombre, fond, rend);}
		}
	}
	
	//Case sélectionnée:
	if (zone == sur_grille || zone == nouv_sur_grille)
	{rectangle(marge_gauche + largeur_case * focus_x, 40 + largeur_case * focus_y, largeur_case, largeur_case, 0, couleur_focus, fond, rend);}
	if (selection_x >= 0 && selection_y >= 0)
	{rectangle(marge_gauche + largeur_case * selection_x, 40 + largeur_case * selection_y, largeur_case, largeur_case, 0, couleur_selection, fond, rend);}
	
	//Grille:
	rectangle(marge_gauche, 40, largeur_grille, largeur_grille, 5, couleur_grille, fond, rend);
	SDL_SetColor(couleur_grille, rend);
	for (int compteur = 1; compteur < nbre_cases; compteur++)
	{
		SDL_RenderDrawLine(rend, marge_gauche + largeur_case * compteur - 1, 40, marge_gauche + largeur_case * compteur - 1, 40 + largeur_grille);
		SDL_RenderDrawLine(rend, marge_gauche + largeur_case * compteur, 40, marge_gauche + largeur_case * compteur, 40 + largeur_grille);
		SDL_RenderDrawLine(rend, marge_gauche + largeur_case * compteur + 1, 40, marge_gauche + largeur_case * compteur + 1, 40 + largeur_grille);
		
		SDL_RenderDrawLine(rend, marge_gauche, 40 + largeur_case * compteur - 1, marge_gauche + largeur_grille, 40 + largeur_case * compteur - 1);
		SDL_RenderDrawLine(rend, marge_gauche, 40 + largeur_case * compteur, marge_gauche + largeur_grille, 40 + largeur_case * compteur);
		SDL_RenderDrawLine(rend, marge_gauche, 40 + largeur_case * compteur + 1, marge_gauche + largeur_grille, 40 + largeur_case * compteur + 1);
	}
	
	//Contenu de la grille:
	for (int a = 0; a < nbre_cases; a++)
	{
		for (int b = 0; b < nbre_cases; b++)
		{
			if (grille[a][b].vide == false)
			{
				if (zone >= nouv_grille)
				{afficher_txt_centre(grille[a][b].solution, marge_gauche + largeur_case * a, marge_gauche + largeur_case * (a + 1), 40 + largeur_case * b + 10, police_grille, couleur_texte, rend);}
				else
				{
					//Réponses valides ou invalides:
					if (zone >= verification && zone <= verification_finale && grille[a][b].validation == 'v')
					{rect_arrondi(marge_gauche + largeur_case * a + 2, 40 + largeur_case * b + 2, largeur_case - 4, largeur_case - 4, couleur_valide, couleur_grille, rend);}
					else if (zone >= verification && zone <= verification_finale && grille[a][b].validation == 'i')
					{rect_arrondi(marge_gauche + largeur_case * a + 2, 40 + largeur_case * b + 2, largeur_case - 4, largeur_case - 4, couleur_invalide, couleur_grille, rend);}
					
					if (grille[a][b].essai[0] != 0)
					{afficher_txt_centre(grille[a][b].essai, marge_gauche + largeur_case * a, marge_gauche + largeur_case * (a + 1), 40 + largeur_case * b + 10, police_grille, couleur_texte, rend);}
				}
			}
			else if (zone < nouv_grille)
			{rectangle(marge_gauche + largeur_case * a, 40 + largeur_case * b, largeur_case, largeur_case, 0, couleur_grille, fond, rend);}
		}
	}
	
	//Légende:
	TTF_SetFontStyle(police, TTF_STYLE_UNDERLINE);
	afficher_txt("Horizontalement:", marge_droite, 40, xmax - marge_droite - 20, police, couleur_texte, rend);
	afficher_txt("Verticalement:", marge_droite, 300, xmax - marge_droite - 20, police, couleur_texte, rend);
	TTF_SetFontStyle(police, TTF_STYLE_NORMAL);
	
	if (mots != NULL)
	{
		while (mots->prec != NULL)
		{mots = mots->prec;}
		
		while (mots->suiv != NULL)
		{
			if (mots->orientation == HORIZONTAL)
			{
				sprintf(buffer, "%c. %s", 'A' + h, mots->legende);
				afficher_txt(buffer, marge_droite, 70 + h * 25, xmax - marge_droite - 20, police, couleur_texte, rend);
				sprintf(buffer, "%c", 'A' + h);
				afficher_txt(buffer, marge_gauche + largeur_case * mots->x + 3, 40 + largeur_case * mots->y + largeur_case / 2 - 10, largeur_case, police_nbre_grille, couleur_texte, rend);
				h++;
			}
			else //mots->orientation == VERTICAL
			{
				sprintf(buffer, "%d. %s", v, mots->legende);
				afficher_txt(buffer, marge_droite, 305 + v * 25, xmax - marge_droite - 20, police, couleur_texte, rend);
				sprintf(buffer, "%d", v);
				afficher_txt(buffer, marge_gauche + largeur_case * mots->x + 3, 40 + largeur_case * mots->y, largeur_case, police_nbre_grille, couleur_texte, rend);
				v++;
			}
			mots = mots->suiv;
		}
		
		if (mots->orientation == HORIZONTAL)
		{
			sprintf(buffer, "%c. %s", 'A' + h, mots->legende);
			afficher_txt(buffer, marge_droite, 70 + h * 25, xmax - marge_droite - 20, police, couleur_texte, rend);
			sprintf(buffer, "%c", 'A' + h);
			afficher_txt(buffer, marge_gauche + largeur_case * mots->x + 3, 40 + largeur_case * mots->y + largeur_case / 2 - 10, largeur_case, police_nbre_grille, couleur_texte, rend);
			h++;
		}
		else //mots->orientation == VERTICAL
		{
			sprintf(buffer, "%d. %s", v, mots->legende);
			afficher_txt(buffer, marge_droite, 305 + v * 25, xmax - marge_droite - 20, police, couleur_texte, rend);
			sprintf(buffer, "%d", v);
			afficher_txt(buffer, marge_gauche + largeur_case * mots->x + 3, 40 + largeur_case * mots->y, largeur_case, police_nbre_grille, couleur_texte, rend);
			v++;
		}
	}
	
	//Boutons:
	rect_arrondi(marge_gauche, marge_bas, 110, 40, couleur_boutons, fond, rend);
	if (zone == nouv_annuler || zone == annuler)
	{rect_arrondi(marge_gauche, marge_bas, 110, 40, couleur_selection, fond, rend);}
	afficher_txt_centre("Annuler", marge_gauche, marge_gauche + 110, marge_bas + 10, police, couleur_texte, rend);
	
	rect_arrondi(marge_gauche + 130, marge_bas, 50, 40, couleur_boutons, fond, rend);
	if (zone == nouv_mod_orientation || zone == mod_orientation)
	{rect_arrondi(marge_gauche + 130, marge_bas, 50, 40, couleur_selection, fond, rend);}
	SDL_RenderCopy(rend, symbole_orientation, NULL, &rect_symbole_orientation);
	
	rect_arrondi(marge_gauche + 200, marge_bas, 110, 40, couleur_boutons, fond, rend);
	if (zone == nouv_identifier || zone == verifier)
	{rect_arrondi(marge_gauche + 200, marge_bas, 110, 40, couleur_selection, fond, rend);}
	if (zone >= nouv_grille)
	{
		afficher_txt_centre("Identifier", marge_gauche + 200, marge_gauche + 310, marge_bas, petite_police, couleur_texte, rend);
		afficher_txt_centre("le mot", marge_gauche + 200, marge_gauche + 310, marge_bas + 20, petite_police, couleur_texte, rend);
	}
	else
	{afficher_txt_centre("Vérifier", marge_gauche + 200, marge_gauche + 310, marge_bas + 10, police, couleur_texte, rend);}
	
	rect_arrondi(marge_gauche + 330, marge_bas, 50, 40, couleur_boutons, fond, rend);
	if (zone == nouv_mod_taille_police || zone == mod_taille_police)
	{rect_arrondi(marge_gauche + 330, marge_bas, 50, 40, couleur_selection, fond, rend);}
	SDL_RenderCopy(rend, symbole_taille_police, NULL, &rect_symbole_taille_police);
	
	rect_arrondi(marge_gauche + 400, marge_bas, 110, 40, couleur_boutons, fond, rend);
	if (zone == nouv_terminer || zone == terminer)
	{rect_arrondi(marge_gauche + 400, marge_bas, 110, 40, couleur_selection, fond, rend);}
	afficher_txt_centre("Terminer", marge_gauche + 400, marge_gauche + 510, marge_bas + 10, police, couleur_texte, rend);
	
	//Vérification finale:
	if (zone == verification_finale)
	{
		rectangle(marge_gauche + 540, marge_bas, xmax - marge_gauche - 570, 40, 0, gris, fond, rend);
		afficher_txt_centre("\"Enter\" pour continuer", marge_gauche + 540, xmax - 30, marge_bas + 10, petite_police, couleur_texte, rend);
	}
	
	if (!ne_pas_afficher)
	{SDL_RenderPresent(rend);}
}


void partie()
//Gère le déroulement d'une partie du début à la fin.
//Utilise la grille déjà chargée s'il y en a une et demande/permet d'en ouvrir/charger ou d'en créer une sinon.
//Efface toute entrée potentielle dans "grille[x][y].essai".
//Ne libère pas la mémoire derrière lui.
{
	SDL_Event ev;
	int choix;
	int buffint;
	
	SDL_MessageBoxData popup_rejouer =
	{
		SDL_MESSAGEBOX_INFORMATION,
		fenetre,
		"CMC - Rejouer?",
		"Voulez-vous rejouer cette grille?",
		2,
		boutons_oui_non,
		NULL
	};
	
	SDL_MessageBoxData popup_quitter =
	{
		SDL_MESSAGEBOX_INFORMATION,
		fenetre,
		"CMC - Quitter?",
		"Voulez-vous vraiment quitter cette page?\nVotre progression ne sera pas enregistrée.",
		2,
		boutons_oui_non,
		NULL
	};
	
	
	if (debogage)
	{
		printf("Nouvelle partie... ");
		if (grille == NULL)
		{printf("Aucune grille préchargée.\n");}
		else
		{printf("Grille déjà chargée (titrée \"%s\").\n", titre);}
	}
	
	//Ouverture/Chargement d'une nouvelle grille:
	if (grille == NULL)
	{
		strcpy(titre, "./grilles/");
		if (!demander_txt("Choisir une grille", "Entrez ici le nom de la grille à charger.\nLes grilles enregistrées par ce programme le sont dans ./grilles/.\nSi vous n'essayez pas de charger un document altéré ou créé manuellement, laissez le ./grilles/ en place et écrivez le nom de la grille ensuite.\nSinon, si votre fichier est enregistré ailleurs, vous devriez le déplacer (recommandé) ou entrer ici son chemin d'accès.\nDans tous les cas, vous pouvez écrire l'extension .txt ou pas. Notez toutefois que si vous ne le faites pas, le programme s'en chargera par lui-même.", titre, sizeof(titre) - 4, fenetre))
		{return;}
		
		if (!ouvrir_grille(titre))
		{return;}
	}
	
	//Préparation de la grille:
	selection_x = -1;
	selection_y = -1;
	focus_x = 0;
	focus_y = 0;
	orientation = HORIZONTAL;
	for (int a = 0; a < nbre_cases; a++)
	{
		for (int b = 0; b < nbre_cases; b++)
		{grille[a][b].essai[0] = 0;}
	}
	
	//Affichage:
	rafraichir(0);
	rafraichir(0); //la 2e fois permet aux calculs pour les icones de se faire avec les bonnes données (sinon, les icones sont toutes en haut au début...)
	simuler_mvm_souris(); //trouve les coordonnées du curseur en simulant un SDL_MOUSEMOTION event
	
	//Déroulement de la partie:
	while (1)
	{
		SDL_WaitEvent(&ev);
		
		switch (ev.type)
		{
		case SDL_QUIT:
			SDL_ShowMessageBox(&popup_quitter, &choix);
			if (choix)
			{quitter();}
			break;
		
		case SDL_WINDOWEVENT:
			SDL_GetWindowSize(fenetre, &xmax, &ymax);
			rafraichir(0);
			break;
		
		case SDL_KEYDOWN:
			switch (ev.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				SDL_ShowMessageBox(&popup_quitter, &choix);
				if (choix)
				{return;}
				break;
			
			case SDLK_UP:
				if (selection_y > 0 && !grille[selection_x][selection_y - 1].vide)
				{selection_y--;}
				break;
			
			case SDLK_DOWN:
				if (selection_y < nbre_cases - 1 && selection_y >= 0 && !grille[selection_x][selection_y + 1].vide)
				{selection_y++;}
				break;
			
			case SDLK_LEFT:
				if (selection_x > 0 && !grille[selection_x - 1][selection_y].vide)
				{selection_x--;}
				break;
			
			case SDLK_RIGHT:
				if (selection_x < nbre_cases - 1 && selection_x >= 0 && !grille[selection_x + 1][selection_y].vide)
				{selection_x++;}
				break;
			
			case SDLK_TAB:
				if (orientation == HORIZONTAL)
				{orientation = VERTICAL;}
				else
				{orientation = HORIZONTAL;}
				break;
			
			case SDLK_BACKSPACE:
				if (selection_x >= 0 && selection_y >= 0)
				{
					grille[selection_x][selection_y].essai[0] = 0;
										
					if (orientation == HORIZONTAL && selection_x > 0 && !grille[selection_x - 1][selection_y].vide)
					{selection_x--;}
					else if (orientation == VERTICAL && selection_y > 0 && !grille[selection_x][selection_y - 1].vide)
					{selection_y--;}
				}
				break;
			
			case SDLK_DELETE:
				if (selection_x >= 0 && selection_y >= 0)
				{grille[selection_x][selection_y].essai[0] = 0;}
				break;
			}
			rafraichir(0);
			break;
		
		case SDL_TEXTINPUT:
			if (selection_x >= 0 && selection_y >= 0 && !grille[selection_x][selection_y].vide)
			{
				if (!strcmp(ev.text.text, "¶") || !strcmp(ev.text.text, ",") || !strcmp(ev.text.text, ".") || !strcmp(ev.text.text, ";") || !strcmp(ev.text.text, ":") || !strcmp(ev.text.text, "!") || !strcmp(ev.text.text, "@") \
					|| !strcmp(ev.text.text, "#") || !strcmp(ev.text.text, "$") || !strcmp(ev.text.text, "?") || !strcmp(ev.text.text, "%") || !strcmp(ev.text.text, "&") || !strcmp(ev.text.text, "*") || !strcmp(ev.text.text, "(") \
					|| !strcmp(ev.text.text, ")") || !strcmp(ev.text.text, "_") || !strcmp(ev.text.text, "=") || !strcmp(ev.text.text, "+") || !strcmp(ev.text.text, "/") || !strcmp(ev.text.text, "\\") || !strcmp(ev.text.text, "|") \
					|| !strcmp(ev.text.text, "{") || !strcmp(ev.text.text, "}") || !strcmp(ev.text.text, "[") || !strcmp(ev.text.text, "]") || !strcmp(ev.text.text, "±") || !strcmp(ev.text.text, "£") || !strcmp(ev.text.text, "¤") \
					|| !strcmp(ev.text.text, "½") || !strcmp(ev.text.text, "¬") || !strcmp(ev.text.text, "\"") || !strcmp(ev.text.text, "<") || !strcmp(ev.text.text, ">") || !strcmp(ev.text.text, "°") || !strcmp(ev.text.text, "~") \
					|| !strcmp(ev.text.text, "µ") || !strcmp(ev.text.text, "§") || !strcmp(ev.text.text, "€") || !strcmp(ev.text.text, "«") || !strcmp(ev.text.text, "»"))
					//caractères acceptés (pas dans cette liste): apostrophe ('), tiret (-), chiffres, lettres (minuscules et majuscules) et accents
				{
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Caractère interdit", \
						"Vous ne pouvez pas insérer ce caractère ici.\nSeules les lettres (incluant les accents), les chiffres, l'apostrophe et le tiret sont permis.", fenetre);
				}
				else if (strcmp(ev.text.text, " ") != 0)
				{
					strcpy(grille[selection_x][selection_y].essai, ev.text.text);
					enlever_majuscule(grille[selection_x][selection_y].essai); //transformation des majuscules en minuscules
					
					if (orientation == HORIZONTAL && selection_x < nbre_cases - 1 && !grille[selection_x + 1][selection_y].vide)
					{selection_x++;}
					else if (orientation == VERTICAL && selection_y < nbre_cases - 1 && !grille[selection_x][selection_y + 1].vide)
					{selection_y++;}
					
					rafraichir(0);
				}
			}
			break;
		
		case SDL_MOUSEMOTION:
			if (ev.motion.x >= marge_gauche && ev.motion.x <= marge_gauche + largeur_grille && ev.motion.y >= 40 && ev.motion.y <= 40 + largeur_grille)
			{
				for (int l = 0; l < nbre_cases; l++)
				{
					for (int h = 0; h < nbre_cases; h++)
					{
						if (ev.motion.x >= marge_gauche + largeur_case * l && ev.motion.x <= marge_gauche + largeur_case * (l + 1) && ev.motion.y >= 40 + largeur_case * h && ev.motion.y <= 40 + largeur_case * (h + 1))
						{
							if (grille[l][h].vide)
							{rafraichir(0);}
							else
							{focus_x = l; focus_y = h; rafraichir(sur_grille);}
						}
					}
				}
			}
			else if (ev.motion.x >= marge_gauche && ev.motion.x <= marge_gauche + 110 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40)
			{rafraichir(annuler);}
			else if (ev.motion.x >= marge_gauche + 130 && ev.motion.x <= marge_gauche + 180 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40)
			{rafraichir(mod_orientation);}
			else if (ev.motion.x >= marge_gauche + 200 && ev.motion.x <= marge_gauche + 310 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40)
			{rafraichir(verifier);}
			else if (ev.motion.x >= marge_gauche + 330 && ev.motion.x <= marge_gauche + 380 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40)
			{rafraichir(mod_taille_police);}
			else if (ev.motion.x >= marge_gauche + 400 && ev.motion.x <= marge_gauche + 510 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40)
			{rafraichir(terminer);}
			else
			{rafraichir(0);}
			break;
		
		case SDL_MOUSEBUTTONDOWN:
			if (ev.motion.x >= marge_gauche && ev.motion.x <= marge_gauche + largeur_grille && ev.motion.y >= 40 && ev.motion.y <= 40 + largeur_grille) //clic dans la grille
			{
				selection_x = focus_x;
				selection_y = focus_y;
				rafraichir(sur_grille);
			}
			else if (ev.motion.x >= marge_gauche && ev.motion.x <= marge_gauche + 110 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40) //annuler
			{
				selection_x = -1;
				selection_y = -1;
				SDL_ShowMessageBox(&popup_quitter, &choix);
				if (choix)
				{return;}
			}
			else if (ev.motion.x >= marge_gauche + 130 && ev.motion.x <= marge_gauche + 180 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40) //changer d'orientation
			{
				if (orientation == HORIZONTAL)
				{orientation = VERTICAL;}
				else
				{orientation = HORIZONTAL;}
				rafraichir(mod_orientation);
			}
			else if (ev.motion.x >= marge_gauche + 200 && ev.motion.x <= marge_gauche + 310 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40) //vérifier
			{verifier_mot();}
			else if (ev.motion.x >= marge_gauche + 330 && ev.motion.x <= marge_gauche + 380 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40) //agrandir/rapetisser la police
			{
				buffint = demander_nbre("Modifier la taille de la police", "Vous pouvez ajuster ici la taille de la police utilisée dans la grille.\nNotez que la taille des numéros et lettres indiquant le début des mots sera aussi modifiée en conséquence.\nCela est très utile lorsqu'une \"petite\" grille est visualisée dans une grande fenêtre (ou l'inverse!).\nLa taille par défaut est de 30 pts.", taille_police_grille, fenetre);
				if (buffint > 0 && buffint < 70)
				{
					taille_police_grille = buffint;
					TTF_CloseFont(police_grille);
					police_grille = TTF_OpenFont(nom_police_principale, taille_police_grille);
					taille_police_nbre_grille = buffint - 10;
					TTF_CloseFont(police_nbre_grille);
					police_nbre_grille = TTF_OpenFont(nom_police_principale, taille_police_nbre_grille);
					rafraichir(0);
				}
				else if (buffint != -6699) //signifie qu'aucun input n'a été pris, finalement, donc ce n'est (probablement) pas une erreur...
				{SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Valeur non acceptée", "La taille de police que vous avez spécifiée est invalide.\nVeuillez choisir un nombre positif plus petit que 70.", fenetre);}
			}
			else if (ev.motion.x >= marge_gauche + 400 && ev.motion.x <= marge_gauche + 510 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40) //terminer
			{
				verifier_grille();
				SDL_ShowMessageBox(&popup_rejouer, &choix);
				if (!choix)
				{liberer_memoire(); return;}
				else
				{partie(); return;}
			}
			else //clic dans le vide
			{
				selection_x = -1;
				selection_y = -1;
				rafraichir(0);
			}
			break;
		}
	}
}


void verifier_mot ()
//Vérifie si un mot entré dans la grille contient les bonnes lettres ou pas.
//Modifier "delai_verification_mot" pour voir plus ou moins longtemps la validation.
{	
	int buff = 0; //corrige un bug très étrange du nombre de caratères qui change en cours de route... (voir plus bas)
	
	if (mots == NULL || grille == NULL)
	{erreur(17, "Aucune grille ou légende n'a été chargée avant cette vérification.\nComment ça a pu arriver?!"); return;}
	
	if (selection_x >= 0 && selection_y >= 0)
	{
		if (orientation == HORIZONTAL)
		{
			//Identification du 1er caractère du mot:
			while (selection_x > 0 && grille[selection_x - 1][selection_y].vide != true)
			{selection_x--;}
			rafraichir(0);
			
			//Identification du mot:
			while (mots->prec != NULL)
			{mots = mots->prec;}
			while (mots->suiv != NULL && (mots->x != selection_x || mots->y != selection_y))
			{mots = mots->suiv;}
			if (mots->x != selection_x || mots->y != selection_y)
			{erreur(16, "Aucun mot horizontal n'a été associé à cette zone non-vide de la grille.\nAvez-vous sélectionnée la bonne orientation?"); return;}
			
			buff = mots->nbre_car; //place le nombre de caractères dans un buffer, parce que sinon, ce nombre change... (POURQUOI?!! COMMENT?!!)
			//Vérification du mot
			for (int n = selection_x; n < selection_x + mots->nbre_car; n++)
			{
				if (!strcmp(grille[n][selection_y].essai, grille[n][selection_y].solution))
				{grille[n][selection_y].validation = 'v';}
				else
				{grille[n][selection_y].validation = 'i';}
			}
			
			//Affichage:
			rafraichir(verification);
			SDL_Delay(delai_validation_mot);
			
			mots->nbre_car = buff; //reprend le nombre de caractères dans un buffer, parce que sinon, ce nombre change... (POURQUOI?!! COMMENT?!!)
			//Retour à la normale:
			for (int n = selection_x; n < selection_x + mots->nbre_car; n++)
			{grille[n][selection_y].validation = 0;}
			rafraichir(0);
		}
		
		else //orientation == VERTICAL
		{
			//Identification du 1er caractère du mot:
			while (selection_y > 0 && grille[selection_x][selection_y - 1].vide != true)
			{selection_y--;}
			rafraichir(0);
			
			//Identification du mot:
			while (mots->prec != NULL)
			{mots = mots->prec;}
			while (mots->suiv != NULL && (mots->x != selection_x || mots->y != selection_y))
			{mots = mots->suiv;}
			if (mots->x != selection_x || mots->y != selection_y)
			{erreur(16, "Aucun mot vertical n'a été associé à cette zone non-vide de la grille.\nAvez-vous sélectionnée la bonne orientation?"); return;}
			
			buff = mots->nbre_car; //place le nombre de caractères dans un buffer, parce que sinon, ce nombre change... (POURQUOI?!! COMMENT?!!)
			//Vérification du mot
			for (int n = selection_y; n < selection_y + mots->nbre_car; n++)
			{
				if (!strcmp(grille[selection_x][n].essai, grille[selection_x][n].solution))
				{grille[selection_x][n].validation = 'v';}
				else
				{grille[selection_x][n].validation = 'i';}
			}
			
			//Affichage:
			rafraichir(verification);
			SDL_Delay(delai_validation_mot);
			
			mots->nbre_car = buff; //reprend le nombre de caractères dans un buffer, parce que sinon, ce nombre change... (POURQUOI?!! COMMENT?!!)
			//Retour à la normale:
			for (int n = selection_y; n < selection_y + mots->nbre_car; n++)
			{grille[selection_x][n].validation = 0;}
			rafraichir(0);
		}
	}
	else
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Aucune case sélectionnée", \
			"Veuillez sélectionner (dans la grille) la case de la première lettre du mot à identifier ou cliquer sur \"terminer\" pour vérifier la grille au complet.", fenetre);
	}
}


void verifier_grille()
//Valide chaque case non-vide de la grille et affiche ça à l'écran.
//Attend ensuite un enter du joueur pour retourner.
{
	SDL_Event ev;
	
	for (int a = 0; a < nbre_cases; a++)
	{
		for (int b = 0; b < nbre_cases; b++)
		{
			if (!grille[a][b].vide)
			{
				if (!strcmp(grille[a][b].essai, grille[a][b].solution))
				{grille[a][b].validation = 'v';}
				else
				{grille[a][b].validation = 'i';}
			}
		}
	}
	
	while (1)
	{
		rafraichir(verification_finale);
		SDL_WaitEvent(&ev);
		
		if (ev.type == SDL_KEYDOWN && (ev.key.keysym.sym == SDLK_RETURN || ev.key.keysym.sym == SDLK_KP_ENTER || ev.key.keysym.sym == SDLK_ESCAPE))
		{return;}
		else if (ev.type == SDL_QUIT)
		{quitter();}
		//else if (ev.type == SDL_KEYDOWN || ev.type == SDL_MOUSEBUTTONDOWN)
		//{SDL_ShowSimpleMessageBox();}
	}
}


bool ouvrir_grille(char nom[])
//Ouvre et lit un fichier de sauvegarde de grille (dont le nom (avec ou sans .txt) doit être fourni en paramètre).
//Initialise cette grille pour qu'on puisse y jouer.
//Renvoie 1 en cas de succès ou 0 en cas d'échec.
//En cas d'échec, un message d'erreur assez détaillé s'affichera et encore plus d'information peut être obtenue en activant le mode débogage.
{
	FILE* fsauv = NULL; //fichier où est sauvegardée la grille (fichier lu)
	int buffint = strlen(nom); //utilisé à quelques reprises...
	char car = ' '; //dernier caractère lu dans fsauv
	char ligne[600] = ""; //dernière ligne lue dans fsauv
	char buffer[300] = " "; //"dernier" mot lu dans fsauv (via ligne)
	bool commentaire = false; //indique la présence d'un commentaire en fin de ligne
	bool lecture_legende = false; //indique qu'on est présentement en train de lire la légende
	bool lecture_grille = false; //même chose, mais pour la grille
	int etape = 0; //indique l'étape (#) où on est rendu dans la lecture de la légende ou de la grille
	int compte_tours = 0; //compte le nombre d'entrées dans la légende ou la grille ont été faits jusqu'à maintenant (débogage seulement)
	struct mot* buffer_mot = NULL; //buffer servant à ajouter un nouveau mot à la légende
	
	if (buffint < 4)
	{
		erreur(22, "Le nom du fichier est trop court.\nIl est présentement impossible d'ouvrir un fichier de moins de 4 caractères.\nDe toute façon, vous devriez enregistrer vos grilles dans le dossier ./grilles/ (et n'oubliez pas de d'écrire ce chemin d'accès ici).");
		return 0;
	}
	
	if (nom[buffint - 4] != '.' || nom[buffint - 3] != 't' || nom[buffint - 2] != 'x' || nom[buffint - 1] != 't')
	{strcat(nom, ".txt");}
	
	fsauv = fopen(nom, "r");
	if (fsauv == NULL)
	{
		erreur(18, "Fichier inexistant ou impossible à ouvrir.");
		if (debogage)
		{printf("Le fichier \"%s\" n'existe pas ou est impossible à ouvrir.\n", nom);}
		return 0;
	}
	
	//Retrouvage du titre:
	sprintf(buffer, "%s", strtok(nom, "/."));
	sprintf(ligne, "%s", strtok(NULL, "/."));
	if (strcmp(ligne, "") != 0)
	{sprintf(buffer, "%s", strtok(nom, "/."));}
	strcpy(titre, ligne);
	ligne[0] = '\000';
	buffer[0] = ' ';
	buffer[1] = '\000';
	
	if (debogage)
	{printf("Ouverture du fichier \"%s\"...\n", nom);}
	
	nbre_cases = 0;
	
	do
	{
		//Réinitialisation des données:
		strcpy(ligne, "");
		strcpy(buffer, "");
		
		//Lecture du 1er caractère et du reste de la ligne si c'est un commentaire:
		car = fgetc(fsauv);
		if (car == '#')
		{
			while (car != '\n' && car != EOF)
			{car = fgetc(fsauv);}
		}
		
		//Lecture et analyse de la légende:
		else if (lecture_legende)
		{
			if (debogage)
			{printf("Lecture de la légende...\n"); compte_tours = 0;}
			
			while (car != EOF && lecture_legende)
			{
				if (car == ' ') {}
				
				else if (car == '#' && !etape)
				{
					while (car != '\n' && car != EOF)
					{car = fgetc(fsauv);}
				}
				
				else if (car == '[' && !etape)
				{
					etape = 1;
					if (debogage)
					{compte_tours++;}
				}
				
				else if (etape == 1)
				{
					for (int compteur = 0; car != ',' && car != ';' && car != ':' && car != '\n' && car != EOF; compteur++)
					{buffer[compteur] = car; buffer[compteur + 1] = '\000'; car = fgetc(fsauv);}
					
					if (car == '\n' && car == EOF)
					{erreur(19, "Fichier source non-conforme.\nDétails techniques: Légende incomplète."); return 0;}
					
					buffer_mot = mots;
					mots = malloc(sizeof(mot));
					mots->prec = buffer_mot;
					if (mots->prec != NULL)
					{mots->prec->suiv = mots;}
					mots->suiv = NULL;
					strcpy(mots->mot, buffer);
					etape = 2;
				}
				
				else if (etape == 2)
				{
					if (car < '0' || car > '9')
					{erreur(19, "Fichier source non-conforme.\nDétails techniques: Légende syntaxiquement incorrecte (x)."); return 0;}
					mots->x = car - '0';
					car = fgetc(fsauv); // = virgule
					etape = 3;
				}
				
				else if (etape == 3)
				{
					if (car < '0' || car > '9')
					{erreur(19, "Fichier source non-conforme.\nDétails techniques: Légende syntaxiquement incorrecte (y)."); return 0;}
					mots->y = car - '0';
					car = fgetc(fsauv); // = virgule
					etape = 4;
				}
				
				else if (etape == 4)
				{
					if (car != '0' && car != '1')
					{erreur(19, "Fichier source non-conforme.\nDétails techniques: Légende syntaxiquement incorrecte (orientation)."); return 0;}
					mots->orientation = car - '0';
					car = fgetc(fsauv); // = virgule
					etape = 5;
				}
				
				else if (etape == 5)
				{
					if (car < '0' || car > '9')
					{erreur(19, "Fichier source non-conforme.\nDétails techniques: Légende syntaxiquement incorrecte (nbre de caractères)."); return 0;}
					mots->nbre_car = car - '0';
					car = fgetc(fsauv); // = virgule
					etape = 6;
				}
				
				else if (etape == 6)
				{
					for (int compteur = 0; car != ',' && car != ';' && car != ':' && car != ']' && car != '\n' && car != EOF; compteur++)
					{buffer[compteur] = car; buffer[compteur + 1] = '\000'; car = fgetc(fsauv);}
					
					if (car == '\n' && car == EOF)
					{erreur(19, "Fichier source non-conforme.\nDétails techniques: Légende incomplète."); return 0;}
					if (car == ']')
					{ungetc(']', fsauv);}
					
					strcpy(mots->legende, buffer);
					etape = 7;
				}
				
				else if (etape == 7)
				{
					if (car != ']')
					{
						erreur(19, "Fichier source non-conforme.\nDétails techniques: Légende syntaxiquement incorrecte. (fin d'entrée)");
						if (debogage)
						{printf("Attendu: \"]\" VS Lu: \"%c\"\n", car);}
						return 0;
					}
					
					car = fgetc(fsauv);
					if (car == ',') //la légende continue
					{car = fgetc(fsauv);} //prend le newline ('\n')
					else //fin de la légende
					{lecture_legende = false;}
					etape = 0;
				}
								
				else
				{
					erreur(19, "Fichier source non-conforme.\nDétails techniques: Légende syntaxiquement incorrecte. (général)");
					if (debogage)
					{printf("Légende syntaxiquement incorrecte: %de ligne, étape %d, au caractère \"%c\".\n", compte_tours, etape, car);}
					return 0;
				}
				
				car = fgetc(fsauv);
			}
			
			if (debogage)
			{printf("%d entrées de légende lues.\n", compte_tours);}
		}
		
		//Lecture et analyse de la grille:
		else if (lecture_grille)
		{
			if (debogage)
			{printf("Lecture de la grille...\n"); compte_tours = 0;}
			etape = 0;
			
			if (nbre_cases <= 0)
			{erreur(20, "Fichier source non-conforme: Nombre de cases non-initialisé.\nLe nombre de cases de cette grille n'a pas pu être lu avant la grille elle-même."); return 0;}
			
			//Initialisation de la nouvelle grille:
			grille = calloc(nbre_cases, sizeof(struct _case*));
			if (grille == NULL)
			{erreur(13, "Impossible d'allouer assez de mémoire pour créer une nouvelle grille de mots croisés.\nDétails techniques: calloc failed (1 de 2)"); return 0;}
			
			for (int compteur = 0; compteur < nbre_cases; compteur++)
			{
				grille[compteur] = calloc(nbre_cases, sizeof(struct _case));
				if (grille[compteur] == NULL)
				{erreur(13, "Impossible d'allouer assez de mémoire pour créer une nouvelle grille de mots croisés.\nDétails techniques: calloc failed (2 de 2, vérifier l'itération...)"); return 0;}
				
				//Initialise à 0 les 4 variables de chaque case:
				for (int c2 = 0; c2 < nbre_cases; c2++)
				{
					grille[compteur][c2].solution[0] = '\000';
					grille[compteur][c2].solution[1] = '\000';
					grille[compteur][c2].essai[0] = '\000';
					grille[compteur][c2].validation = 0;
				}
			}
			
			for (int b = 0; b < nbre_cases && car != EOF; b++)
			{
				for (int a = 0; a < nbre_cases && car != EOF; )
				{
					if (car == ' ') {}
					
					else if (car == '#' && !etape)
					{
						while (car != '\n' && car != EOF)
						{car = fgetc(fsauv);}
					}
					
					else if (car == '[' && !etape)
					{
						if (debogage)
						{compte_tours++;}
						etape = 1;
					}
					
					else if (etape == 1)
					{
						if (car != '0' && car != '1')
						{erreur(19, "Fichier source non-conforme.\nDétails techniques: Grille syntaxiquement incorrecte (case vide (0/1))."); return 0;}
						grille[a][b].vide = car - '0';
						car = fgetc(fsauv); // = virgule
						etape = 2;
					}
					
					else if (etape == 2)
					{
						if (car != ']')
						{
							grille[a][b].solution[0] = car;
							car = fgetc(fsauv);
							if (car != ' ' && car != ',' && car != ';' && car != ':' && car != ']' && car != ' ' && car != '\n' && car != EOF)
							{grille[a][b].solution[1] = car; grille[a][b].solution[2] = '\000';}
						}
						
						if (car == ']')
						{ungetc(']', fsauv);}
						etape = 3;
					}
					
					else if (etape == 3)
					{
						if (car != ']')
						{
							erreur(19, "Fichier source non-conforme.\nDétails techniques: Grille syntaxiquement incorrecte. (fin d'entrée)");
							if (debogage)
							{printf("Attendu: \"]\" VS Lu: \"%c\"\n", car);}
							return 0;
						}
						
						a++;
						car = fgetc(fsauv); //prend le newline ('\n')...
						if (car == ' ') //...qui pourrait être précédé par un espace
						{
							car = fgetc(fsauv);
							if (car != '\n')
							{ungetc('[', fsauv);}
						}
						etape = 0;
					}
					
					else
					{
						erreur(19, "Fichier source non-conforme.\nDétails techniques: Grille syntaxiquement incorrecte. (général)");
						if (debogage)
						{
							printf("Légende syntaxiquement incorrecte: %de ligne (comptée en débogage), ligne %d/%d (comptée régulièrement), colonne %d/%d, ", compte_tours, a, nbre_cases, b, nbre_cases);
							printf("étape %d, au caractère \"%c\".\n", etape, car);
						}
						return 0;
					}
					
					car = fgetc(fsauv);
				}
			}
			
			lecture_grille = false;
		}
		
		//Lecture et analyse de la ligne si ce n'est pas un commentaire:
		else if (car != '\n' && car != EOF)
		{
			//Lecture:
			buffer[1] = '\000';
			while (car != '\n' && car != EOF && !commentaire)
			{
				if (car == '#')
				{commentaire = true;}
				else
				{
					buffer[0] = car;
					strcat(ligne, buffer);
					car = fgetc(fsauv);
				}
			}
			
			//Lecture de l'éventuel commentaire en fin de ligne:
			while (car != '\n' && car != EOF)
			{car = fgetc(fsauv);}
			commentaire = false;
			
			//Analyse:
			sprintf(buffer, "%s", strtok(ligne, " \n:;,.[]"));
			enlever_majuscule(buffer);
			if (!strcmp(buffer, "dimensions") || !strcmp(buffer, "dimension")) //dimensions
			{
				if (debogage)
				{printf("Lecture des dimensions de la grille...\n");}
				
				sprintf(buffer, "%s", strtok(NULL, " \n:;,.[]"));
				if (!est_un_nbre(buffer))
				{erreur(19, "Fichier source non-conforme.\nDétails techniques: Les dimensions ne sont pas un nombre!"); return 0;}
				else
				{sscanf(buffer, "%d", &nbre_cases);}
			}
			else if (!strcmp(buffer, "nombre") || !strcmp(buffer, "nbre")) //nbre de mots
			{
				if (debogage)
				{printf("Lecture du nombre de mots...\n");}
				
				sprintf(buffer, "%s", strtok(NULL, " \n:;,.[]"));
				if (!strcmp(buffer, "de"))
				{sprintf(buffer, "%s", strtok(NULL, " \n:;,.[]"));}
				if (!strcmp(buffer, "mots"))
				{sprintf(buffer, "%s", strtok(NULL, " \n:;,.[]"));}
				if (!est_un_nbre(buffer))
				{erreur(19, "Fichier source non-conforme.\nDétails techniques: Le nombre de mots n'est pas un nombre!"); return 0;}
				else
				{sscanf(buffer, "%d", &nbre_mots);}
			}
			else if (!strcmp(buffer, "légende") || !strcmp(buffer, "legende")) //légende / descriptions
			{lecture_legende = true;}
			else if (!strcmp(buffer, "grille")) //grille
			{lecture_grille = true;}
			else //entrée non-reconnue
			{
				erreur(19, "Fichier source non-conforme.\nDétails techniques: Entrée non-reconnue (et ce n'est pas un commentaire)!");
				if (debogage)
				{printf("Entrée non-reconnue: \"%s\" (contexte: \"%s\")\n", buffer, ligne);}
				return 0;
			}
			
			if (strtok(NULL, " \n:;,.[]") != NULL) //trop de mots à cette ligne
			{erreur(19, "Fichier source non-conforme.\nDétails techniques: Trop de mots sur une ligne!");}
		}
		
	} while (car != EOF);
	
	fclose(fsauv);
	return 1;
}