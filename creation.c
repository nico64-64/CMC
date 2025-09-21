#include "jeu.c"


void nouvelle_grille ()
//Gère la création d'une nouvelle grille de mots croisés.
{
	SDL_Event ev;
	int buffint = 0;
	int choix = 0;
	
	
	SDL_MessageBoxData popup_modifier =
	{
		SDL_MESSAGEBOX_INFORMATION,
		fenetre,
		"CMC - Modifier la grille",
		"Le programme a détecté une grille déjà chargée. Voulez-vous la modifier?\nCliquez \"non\" pour créer une nouvelle grille.",
		2,
		boutons_oui_non,
		NULL
	};
	
	SDL_MessageBoxData popup_tester =
	{
		SDL_MESSAGEBOX_INFORMATION,
		fenetre,
		"CMC - Tester la grille?",
		"Voulez-vous tester cette grille?",
		2,
		boutons_oui_non,
		NULL
	};
	
	SDL_MessageBoxData popup_modifier_case =
	{
		SDL_MESSAGEBOX_INFORMATION,
		fenetre,
		"CMC - Case déjà occupée",
		"Le programme détecte que cette case fait partie d'un mot déjà enregistré.\nVous pouvez quand même modifier cette case, mais cela ne devrait pas être utilisé pour modifier un mot en profondeur, puisque cette modification n'affectera que la grille (et non la liste de mots interne ni la légende).\nIl est recommandé d'effacer le mot au complet en appuyant à nouveau sur le bouton \"Identifier le mot\" avant de le modifier.\nVoulez-vous vraiment modifier cette case?",
		2,
		boutons_oui_non,
		NULL
	};
	
	SDL_MessageBoxData popup_quitter =
	{
		SDL_MESSAGEBOX_INFORMATION,
		fenetre,
		"CMC - Quitter?",
		"Voulez-vous vraiment quitter cette page?\nVotre grille en cours de création ne sera pas enregistrée.",
		2,
		boutons_oui_non,
		NULL
	};
	
	//Demande si on veut modifier la grille (s'il y a lieu):
	if (grille != NULL && mots != NULL)
	{SDL_ShowMessageBox(&popup_modifier, &choix);}
	
	if (grille == NULL || mots == NULL || !choix)
	{
		//Reset de la grille actuelle (devrait déjà être fait, mais au cas où...):
		liberer_memoire();
		
		//Demande la taille de la nouvelle grille:
		nbre_cases = demander_nbre("Taille de la grille", "Entrez la taille (nombre de cases par ligne/colonne) de la grille à créer.\nLes grilles doivent présentement toujours être carrées.\nCe nombre doit être supérieur à 0 et en choisir un trop élevé pourrait rendre la grille inutilisable.", nbre_cases, fenetre);
		if (nbre_cases <= 0)
		{erreur(21, "Nombre de cases invalide.\nLe nombre de cases reçu est inférieur ou égal à 0!\nLe programme utilisera la valeur par défaut (10) pour continuer."); nbre_cases = 10;}
		
		//Initialisation de la nouvelle grille:
		grille = calloc(nbre_cases, sizeof(struct _case*));
		if (grille == NULL)
		{erreur(13, "Impossible d'allouer assez de mémoire pour créer une nouvelle grille de mots croisés.\nDétails techniques: calloc failed (1 de 2)"); return;}
		
		for (int compteur = 0; compteur < nbre_cases; compteur++)
		{
			grille[compteur] = calloc(nbre_cases, sizeof(struct _case));
			if (grille[compteur] == NULL)
			{erreur(13, "Impossible d'allouer assez de mémoire pour créer une nouvelle grille de mots croisés.\nDétails techniques: calloc failed (2 de 2, vérifier l'itération...)"); return;}
			
			//Initialise à 0 les 3 variables de chaque case:
			for (int c2 = 0; c2 < nbre_cases; c2++)
			{
				grille[compteur][c2].vide = true;
				grille[compteur][c2].solution[0] = 0;
				grille[compteur][c2].essai[0] = 0;
			}
		}
	}
	
	rafraichir(nouv_grille);
	simuler_mvm_souris(); //permet de trouver les coordonnées du curseur en simulant un SDL_MOUSEMOTION event
	rafraichir(nouv_grille); //la 2e fois permet aux calculs pour les icones de se faire avec les bonnes données (sinon, les icones sont toutes en haut au début...)
	
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
			rafraichir(nouv_grille);
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
				if (selection_y > 0)
				{selection_y--;}
				break;
			
			case SDLK_DOWN:
				if (selection_y < nbre_cases - 1 && selection_y >= 0)
				{selection_y++;}
				break;
			
			case SDLK_LEFT:
				if (selection_x > 0)
				{selection_x--;}
				break;
			
			case SDLK_RIGHT:
				if (selection_x < nbre_cases - 1 && selection_x >= 0)
				{selection_x++;}
				break;
			
			case SDLK_TAB:
				if (orientation == HORIZONTAL)
				{orientation = VERTICAL;}
				else
				{orientation = HORIZONTAL;}
				rafraichir(nouv_grille);
				break;
			
			case SDLK_SPACE:
				if (selection_x >= 0 && selection_y >= 0)
				{
					if (orientation == HORIZONTAL && selection_x < nbre_cases - 1)
					{selection_x++;}
					else if (orientation == VERTICAL && selection_y < nbre_cases - 1)
					{selection_y++;}
					rafraichir(nouv_grille);
				}
				break;
			
			case SDLK_RETURN:
			case SDLK_KP_ENTER:
				if (selection_x >= 0 && selection_y >= 0)
				{identifier_mot();}
				break;
			
			case SDLK_BACKSPACE:
				if (grille[selection_x][selection_y].essai[0] == 1)
				{
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Case barrée", "Le programme détecte que cette case fait partie d'un mot déjà enregistré.\nPour effacer cette case, vous devez d'abord effacer le mot complet en cliquant à nouveau sur le bouton \"Identifier le mot\".\nSinon, vous pouvez simplement modifier cette case en y entrant une nouvelle lettre.", fenetre);
				}
				else if (selection_x >= 0 && selection_y >= 0)
				{
					grille[selection_x][selection_y].vide = true;
					grille[selection_x][selection_y].solution[0] = 0;
										
					if (orientation == HORIZONTAL && selection_x > 0)
					{selection_x--;}
					else if (orientation == VERTICAL && selection_y > 0)
					{selection_y--;}
					rafraichir(nouv_sur_grille);
				}
				break;
			
			case SDLK_DELETE:
				if (grille[selection_x][selection_y].essai[0] == 1)
				{
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Case barrée", "Le programme détecte que cette case fait partie d'un mot déjà enregistré.\nPour effacer cette case, vous devez d'abord effacer le mot complet en cliquant à nouveau sur le bouton \"Identifier le mot\".\nSinon, vous pouvez simplement modifier cette case en y entrant une nouvelle lettre.", fenetre);
				}
				else if (selection_x >= 0 && selection_y >= 0)
				{
					grille[selection_x][selection_y].vide = true;
					grille[selection_x][selection_y].solution[0] = 0;
					rafraichir(nouv_sur_grille);
				}
				break;
			}
			rafraichir(nouv_grille);
			break;
		
		case SDL_TEXTINPUT:
			if (selection_x >= 0 && selection_y >= 0)
			{
				if (grille[selection_x][selection_y].essai[0] == 1 && strcmp(ev.text.text, " ") != 0)
				{
					SDL_ShowMessageBox(&popup_modifier_case, &choix);
					if (!choix) //annuler
					{break;}
					//sinon, on continue (sans avoir modifié le mot lui-même dans la listes des mots!)... <- TODO
				}
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
					grille[selection_x][selection_y].vide = false;
					strcpy(grille[selection_x][selection_y].solution, ev.text.text);
					enlever_majuscule(grille[selection_x][selection_y].solution); //transformation des majuscules en minuscules
					
					if (orientation == HORIZONTAL && selection_x < nbre_cases - 1)
					{selection_x++;}
					else if (orientation == VERTICAL && selection_y < nbre_cases - 1)
					{selection_y++;}
					
					rafraichir(nouv_grille);
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
						{focus_x = l; focus_y = h; rafraichir(nouv_sur_grille);}
					}
				}
			}
			else if (ev.motion.x >= marge_gauche && ev.motion.x <= marge_gauche + 110 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40)
			{rafraichir(nouv_annuler);}
			else if (ev.motion.x >= marge_gauche + 130 && ev.motion.x <= marge_gauche + 180 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40)
			{rafraichir(nouv_mod_orientation);}
			else if (ev.motion.x >= marge_gauche + 200 && ev.motion.x <= marge_gauche + 310 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40)
			{rafraichir(nouv_identifier);}
			else if (ev.motion.x >= marge_gauche + 330 && ev.motion.x <= marge_gauche + 380 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40)
			{rafraichir(nouv_mod_taille_police);}
			else if (ev.motion.x >= marge_gauche + 400 && ev.motion.x <= marge_gauche + 510 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40)
			{rafraichir(nouv_terminer);}
			else if (ev.motion.x >= (xmax - longueur_txt(titre, xmax, police)) / 2 - 34 && ev.motion.x <= (xmax - longueur_txt(titre, xmax, police)) / 2 - 9 && ev.motion.y >= 5 && ev.motion.y <= 30)
			{rafraichir(nouv_modifier_titre);}
			else
			{rafraichir(nouv_grille);}
			break;
		
		case SDL_MOUSEBUTTONDOWN:
			if (ev.motion.x >= marge_gauche && ev.motion.x <= marge_gauche + largeur_grille && ev.motion.y >= 40 && ev.motion.y <= 40 + largeur_grille) //clic dans la grille
			{
				selection_x = focus_x;
				selection_y = focus_y;
				rafraichir(nouv_sur_grille);
			}
			else if (ev.motion.x >= marge_gauche && ev.motion.x <= marge_gauche + 110 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40) //annuler
			{
				selection_x = -1;
				selection_y = -1;
				SDL_ShowMessageBox(&popup_quitter, &choix);
				if (choix)
				{liberer_memoire(); return;}
			}
			else if (ev.motion.x >= marge_gauche + 130 && ev.motion.x <= marge_gauche + 180 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40) //changer d'orientation
			{
				if (orientation == HORIZONTAL)
				{orientation = VERTICAL;}
				else
				{orientation = HORIZONTAL;}
				rafraichir(nouv_mod_orientation);
			}
			else if (ev.motion.x >= marge_gauche + 200 && ev.motion.x <= marge_gauche + 310 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40) //identifier
			{identifier_mot();}
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
					rafraichir(nouv_grille);
				}
				else if (buffint != -6699) //signifie qu'aucun input n'a été pris, finalement, donc ce n'est (probablement) pas une erreur...
				{SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Valeur non acceptée", "La taille de police que vous avez spécifiée est invalide.\nVeuillez choisir un nombre positif plus petit que 70.", fenetre);}
			}
			else if (ev.motion.x >= marge_gauche + 400 && ev.motion.x <= marge_gauche + 510 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40) //terminer
			{
				if (sauvegarder_grille())
				{
					SDL_ShowMessageBox(&popup_tester, &choix);
					
					if (choix) //tester
					{partie();}
					else //ne pas tester
					{liberer_memoire();}
					return;
				}
			}
			else if (ev.motion.x >= (xmax - longueur_txt(titre, xmax, police)) / 2 - 34 && ev.motion.x <= (xmax - longueur_txt(titre, xmax, police)) / 2 - 9 && ev.motion.y >= 5 && ev.motion.y <= 30) //modifier le titre
			{
				demander_txt("Modifier le titre de la grille", "Entrer ici le titre de cette grille de mots croisés.\nCe titre devrait être significatif et pas trop long. Il sera utilisé pour nommer le fichier de sauvegarde de la grille.\nLes accents et les caractères spéciaux ne sont pas recommandés.", titre, sizeof(titre), fenetre); rafraichir(nouv_grille);
			}
			else //clic dans le vide
			{
				selection_x = -1;
				selection_y = -1;
				rafraichir(nouv_grille);
			}
			break;
		}
	}
}


void identifier_mot ()
//Associe une description à un mot dans la grille, créant un nouvel élément de la liste "mots".
//Ne doit être appelé que par la fonction "nouvelle_grille" lorsque le bouton "identifier un mot" ou la touche "enter" sont cliqué/entré.
{
	char buffer[300];
	mot* buffer_mot = NULL;
	int choix = 0;
	bool flag_2e_mot = 0;
	
	SDL_MessageBoxButtonData boutons_effacer_mot[3] =
	{
		{SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Annuler"},
		{0, 1, "Modifier"},
		{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 2, "Effacer"}
	};
	
	SDL_MessageBoxData popup_effacer_mot =
	{
		SDL_MESSAGEBOX_INFORMATION,
		fenetre,
		"CMC - Effacer le mot déjà enregistré?",
		"Le programme détecte qu'un mot a déjà été enregistré à cet endroit.\nVoulez-vous modifier la description du mot, effacer complétement ce mot de la grille et de la légende ou annuler cette action?",
		3,
		boutons_effacer_mot,
		NULL
	};
	
	
	//Est-ce qu'une case est sélectionnnée?
	if (selection_x == -1 || selection_y == -1)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Aucune case sélectionnée", "Vous devez sélectionner une case contenant une des lettres du mot à identifier.\nAssurez-vous d'avoir la bonne orientation!", fenetre);
		return;
	}
	
	//Est-ce que la case est vide? Et si on revient en arrière d'une case?
	if (grille[selection_x][selection_y].vide)
	{
		if (orientation == HORIZONTAL && selection_x > 0 && !grille[selection_x - 1][selection_y].vide)
		{selection_x--;}
		else if (orientation == VERTICAL && selection_y > 0 && !grille[selection_x][selection_y - 1].vide)
		{selection_y--;}
	}
	
	//On revient au début de la liste des mots:
	while (mots != NULL && mots->suiv != NULL)
	{mots = mots->suiv;}
	
	//Une case est sélectionnée:
	if (selection_x >= 0 && selection_y >= 0)
	{
		//Case vide:
		if (grille[selection_x][selection_y].vide)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Case vide sélectionnée", "La case présentement sélectionnée est vide.\nVeuillez sélectionner la case de la première lettre du mot à identifier.", \
				fenetre);
			return;
		}
		
		//Case non-vide sans caractère (erreur):
		else if (grille[selection_x][selection_y].solution[0] == '\000')
		{erreur(15, "Les données du programme semblent s'être corrompues: Vous avez sélectionné une case non-vide contenant une solution vide."); return;}
		
		//Case déjà occupée par un mot:
		else if (grille[selection_x][selection_y].essai[0] == 1)
		{
			while (mots->prec != NULL && !flag_2e_mot && !(mots->orientation == orientation && mots->x == selection_x && mots->y == selection_y))
			{
				if (mots->orientation != orientation)
				{
					if (mots->orientation == HORIZONTAL && selection_x >= mots->x && selection_x <= mots->x + mots->nbre_car && selection_y == mots->y)
					{flag_2e_mot = true;}
					else if (mots->orientation == VERTICAL && selection_y >= mots->y && selection_y <= mots->y + mots->nbre_car && selection_x == mots->x)
					{flag_2e_mot = true;}
				}
				mots = mots->prec;
			}
			if (mots->orientation != orientation)
			{
				if (mots->orientation == HORIZONTAL && selection_x >= mots->x && selection_x <= mots->x + mots->nbre_car && selection_y == mots->y)
				{flag_2e_mot = true;}
				else if (mots->orientation == VERTICAL && selection_y >= mots->y && selection_y <= mots->y + mots->nbre_car && selection_x == mots->x)
				{flag_2e_mot = true;}
			}
			else if (flag_2e_mot)
			{flag_2e_mot = 0;}
			while (mots->suiv != NULL)
			{mots = mots->suiv;}
			
			if (!flag_2e_mot)
			{SDL_ShowMessageBox(&popup_effacer_mot, &choix);}
			if (flag_2e_mot)
			{/*On fait juste laisser le code couler...*/}
			else if (!choix)
			{return;}
			else
			{
				if (choix == 1)
				{sprintf(buffer, "Veuillez entrer ici une brève description du mot \"%s\".", mots->mot);}
				
				if (choix == 2 || !demander_txt("Légende du mot", buffer, mots->legende, sizeof(mots->legende), fenetre) || mots->legende[0] == '\000')
				{
					if (mots->legende[0] == '\000')
					{SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Description invalide", "Veuillez fournir une description pour que ce mot soit enregistré dans la légende de la grille.", fenetre);}
					
					if (mots->orientation == HORIZONTAL)
					{
						for (int compteur = 0; compteur < mots->nbre_car; compteur++)
						{grille[mots->x + compteur][mots->y].essai[0] = 0;}
					}
					else //orientation == VERTICAL
					{
						for (int compteur = 0; compteur < mots->nbre_car; compteur++)
						{grille[mots->x][mots->y + compteur].essai[0] = 0;}
					}
					
					if (mots->prec == NULL)
					{
						buffer_mot = mots->suiv;
						free(mots);
						mots = buffer_mot;
						if (mots != NULL && mots->suiv != NULL)
						{mots->suiv->prec = NULL;}
					}
					else
					{
						buffer_mot = mots->suiv;
						mots = mots->prec;
						free(mots->suiv);
						mots->suiv = buffer_mot;
						if (mots->suiv != NULL)
						{mots->suiv->prec = mots;}
					}
				}
				rafraichir(nouv_grille);
				nbre_mots--;
				return;
			}
		}
		
		if (orientation == HORIZONTAL)
		{
			//Identification du 1er caractère du mot:
			while (selection_x > 0 && grille[selection_x - 1][selection_y].vide != true)
			{selection_x--;}
			rafraichir(nouv_grille);
			
			//Ajout du mot à la liste:
			buffer_mot = mots;
			mots = malloc(sizeof(mot));
			mots->prec = buffer_mot;
			if (mots->prec != NULL)
			{mots->prec->suiv = mots;}
			mots->suiv = NULL;
			mots->orientation = HORIZONTAL;
			mots->x = selection_x;
			mots->y = selection_y;
			
			//Identification du nombre de caractères:
			while (selection_x < nbre_cases && grille[selection_x][selection_y].vide != true)
			{selection_x++;}
			mots->nbre_car = selection_x - mots->x;
			selection_x = mots->x;
			
			//Lecture du mot lui-même:
			strcpy(mots->mot, "");
			for (int compteur = 0; compteur < mots->nbre_car; compteur++)
			{strcat(mots->mot, grille[mots->x + compteur][mots->y].solution);}
			
			//Demande de la légende:
			strcpy(mots->legende, "");
			sprintf(buffer, "Veuillez entrer ici une brève description du mot \"%s\".\nCe mot est écrit horizontalement dans la grille.", mots->mot);
			if (!demander_txt("Légende du mot", buffer, mots->legende, sizeof(mots->legende), fenetre) || mots->legende[0] == '\000')
			{
				if (mots->legende[0] == '\000')
				{SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Description invalide", "Veuillez fournir une description pour que ce mot soit enregistré dans la légende de la grille.", fenetre);}
				if (mots->prec == NULL)
				{free(mots); mots = NULL;}
				else
				{
					mots = mots->prec;
					free(mots->suiv);
					mots->suiv = NULL;
				}
			}
			else
			{
				//Marquage des cases:
				for (int compteur = 0; compteur < mots->nbre_car; compteur++)
				{grille[mots->x + compteur][mots->y].essai[0] = 1;} //Je met essai = 1 pour marquer la case comme faisant mtn partie d'un mot
			}
		}
		
		else //orientation == VERTICAL
		{
			//Identification du 1er caractère du mot:
			while (selection_y > 0 && grille[selection_x][selection_y - 1].vide != true)
			{selection_y--;}
			rafraichir(nouv_grille);
			
			//Ajout du mot à la liste:
			buffer_mot = mots;
			mots = malloc(sizeof(mot));
			mots->prec = buffer_mot;
			if (mots->prec != NULL)
			{mots->prec->suiv = mots;}
			mots->suiv = NULL;
			mots->orientation = VERTICAL;
			mots->x = selection_x;
			mots->y = selection_y;
			
			//Identification du nombre de caractères:
			while (selection_y < nbre_cases && grille[selection_x][selection_y].vide != true)
			{selection_y++;}
			mots->nbre_car = selection_y - mots->y;
			selection_y = mots->y;
			
			//Lecture du mot lui-même:
			strcpy(mots->mot, "");
			for (int compteur = 0; compteur < mots->nbre_car; compteur++)
			{strcat(mots->mot, grille[mots->x][mots->y + compteur].solution);}
			
			//Demande de la légende:
			strcpy(mots->legende, "");
			sprintf(buffer, "Veuillez entrer ici une brève description du mot \"%s\".\nCe mot est écrit verticalement dans la grille.", mots->mot);
			if (!demander_txt("Légende du mot", buffer, mots->legende, sizeof(mots->legende), fenetre) || mots->legende[0] == '\000')
			{
				if (mots->legende[0] == '\000')
				{SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Description invalide", "Veuillez fournir une description pour que ce mot soit enregistré dans la légende de la grille.", fenetre);}
				if (mots->prec == NULL)
				{free(mots); mots = NULL;}
				else
				{
					mots = mots->prec;
					free(mots->suiv);
					mots->suiv = NULL;
				}
			}
			else
			{
				//Marquage des cases:
				for (int compteur = 0; compteur < mots->nbre_car; compteur++)
				{grille[mots->x][mots->y + compteur].essai[0] = 1;} //Je met essai = 1 pour marquer la case comme faisant mtn partie d'un mot
			}
		}
		
		//Affichage:
		rafraichir(nouv_grille);
		nbre_mots++;
	}
	else
	{SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Aucune case sélectionnée", "Veuillez sélectionner (dans la grille) la case de la première lettre du mot à identifier.", fenetre);}
}


bool sauvegarder_grille ()
//Enregistre une grille nouvellement créée.
//Renvoie 1 en cas de réussite ou 0 en cas d'erreur ou d'annulation.
{
	char nom_fsauv[250] = "./grilles/";
	FILE* fsauv = NULL;
	int choix = -1; //ID de bouton reçu de la fonction "SDL_ShowMessageBox"
	
	SDL_MessageBoxButtonData boutons_ecraser[2] =
	{
		{SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Annuler"},
		{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Écraser"}
	};
	SDL_MessageBoxData msgbox_ecraser =
	{
		SDL_MESSAGEBOX_INFORMATION,
		fenetre,
		"CMC - Fichier déjà existant",
		"Il semblerait que vous ayez déjà enregistré une grille ayant le même titre que celle-ci.\nVoulez-vous écraser l'ancienne grille ou annuler cette sauvegarde?",
		2,
		boutons_ecraser,
		NULL
	};
	
	
	//Vérifions si on a quelque chose à enregistrer (ça devrait déjà être le cas, mais bon...):
	if (grille == NULL || mots == NULL)
	{erreur(14, "Impossible d'enregistrer la grille:\nLe programme ne détecte aucune grille ou légende présentement active."); return 0;}
	
	//Vérifions qu'un mot a bel et bien été assigné à chaque case non-vide de la grille:
	for (int a = 0; a < nbre_cases; a++)
	{
		for (int b = 0; b< nbre_cases; b++)
		{
			if (!grille[a][b].vide && grille[a][b].essai[0] != 1)
			{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Grille incomplète", "Veuillez assigner une description (bouton \"identifier un mot\") à chaque mot de la grille avant d'essayer de l'enregistrer.", fenetre);
				return 0;
			}
		}
	}
	
	//Vérifions si on a un titre:
	if (titre[0] == '\000')
	{SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Titre invalide", "Veuillez entrer un titre unique permettant d'identifier cette grille.", fenetre); return 0;}
	
	//Complétons le nom du fichier et vérifions s'il existe déjà:
	strcat(nom_fsauv, titre);
	strcat(nom_fsauv, ".txt");
	fsauv = fopen(nom_fsauv, "r");
	if (fsauv != NULL)
	{
		fclose(fsauv);
		SDL_ShowMessageBox(&msgbox_ecraser, &choix);
		if (!choix)
		{return 0;}
	}
	
	//Créons le nouveau fichier:
	fsauv = fopen(nom_fsauv, "w+");
	fprintf(fsauv, "# %s.txt\n\n# Sauvegarde d'une grille de mots croisés créée par CMC (programme Créateur de Mots Croisés).\n# Toutes les lignes commençant par un \"#\" seront ignorées par le programme.\n", titre);
	fprintf(fsauv, "# Modifiez ce fichier à vos risques et périls.\n\n\n");
	
	fprintf(fsauv, "Dimensions: %d  # nombre de cases (les grilles sont toujours carrées)\n", nbre_cases);
	fprintf(fsauv, "Nombre de mots: %d\n\n", nbre_mots);
	
	fprintf(fsauv, "Légende:\n");
	fprintf(fsauv, "# Syntaxe pour un mot: [ mot, x (position horizontale), y (position verticale), orientation (0/1 = Horizontal/Vertical), nombre de caractères (nombre de cases occupées), légende (description du mot) ]\n");
	fprintf(fsauv, "# La position du mot est celle de son premier caractère. Les valeurs de x et y vont de 0 au nombre de cases - 1.\n");
	while (mots->prec != NULL)
	{mots = mots->prec;}
	while (mots->suiv != NULL)
	{
		fprintf(fsauv, "[ %s, %d, %d, %d, %d, %s ],\n", mots->mot, mots->x, mots->y, mots->orientation, mots->nbre_car, mots->legende);
		mots = mots->suiv;
	}
	fprintf(fsauv, "[ %s, %d, %d, %d, %d, %s ]\n\n", mots->mot, mots->x, mots->y, mots->orientation, mots->nbre_car, mots->legende);
	
	fprintf(fsauv, "Grille:\n# Syntaxe pour une case: [ vide (0/1), solution (caractère) ]\n# Les cases doivent être disposées comme doit l'être la grille\n");
	for (int a = 0; a < nbre_cases; a++)
	{
		for (int b = 0; b < nbre_cases; b++)
		{fprintf(fsauv, "[ %d, %s ] ", grille[b][a].vide, grille[b][a].solution);}
		fprintf(fsauv, "\n");
	}
	
	if (debogage)
	{printf("Grille sauvegardée (fichier %s).\n", nom_fsauv);}
	
	//Fermons le fichier, et c'est terminé!
	fclose(fsauv);
	return 1;
}