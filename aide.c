#include "creation.c"


void aide ()
//Gère l'accueil du module d'aide et les crédits (tous sous forme de pop-ups SDL).
{
	SDL_MessageBoxButtonData boutons_popup_aide[4] =
	{
		{SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Aucune (annuler)"},
		{0, 1, "Création"},
		{0, 2, "Jeu"},
		{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 3, "À propos de CMC"}
	};
	SDL_MessageBoxData popup_aide =
	{
		SDL_MESSAGEBOX_INFORMATION,
		fenetre,
		"CMC - Rubriques d'aide",
		"Bienvenue dans le module d'aide de CMC!\nQuelle rubrique voulez-vous consulter?",
		4,
		boutons_popup_aide,
		NULL
	};
	int choix;
	char buffer[700];
	
	
	//Demande quelle rubrique l'utilisateur veut consulter:
	SDL_ShowMessageBox(&popup_aide, &choix);
	
	//Annuler:
	if (!choix)
	{return;}
	
	//Création:
	else if (choix == 1)
	{aide_creation();}
	
	//Jeu:
	else if (choix == 2)
	{aide_jeu();}
	
	//Crédits:
	else
	{
		sprintf(buffer, "CMC - Créateur de Mots Croisés\nVersion %s (%s)\nProgrammé en C par Nicolas Audette.\n\nCMC a pour but de permettre la création, la sauvegarde et l'utilisation de grilles informatiques de mots croisés.\nL'interface de ce programme reste assez rudimentaire (certains \"corner cases\" ont été volontairement laissés dans un état plutôt imparfait\net le programme fait beaucoup usage des pop-ups systèmes de SDL, par exemple) et loin de ce que plusieurs applications web peuvent offrir,\nce qui est voulu.\n\nCe programme accepte des arguments lorsque démarré depuis un terminal.\nEntrez \"./cmc --aide\" pour en savoir plus.", VERSION, NOM_VERSION);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "À propos de CMC", buffer, fenetre);
		return;
	}
}


void aide_creation ()
//Gère la rubrique d'aide pour la création d'une grille.
//Ne devrait être appelée que par la fonction aide.
{
	SDL_Event ev;
	char buffer[20];
	
	//Reset de la grille actuelle:
	liberer_memoire();
	
	//Création d'une nouvelle grille vierge de 10x10:
	grille = calloc(nbre_cases, sizeof(struct _case*));
	if (grille == NULL)
	{erreur(13, "Impossible d'allouer assez de mémoire pour créer une nouvelle grille de mots croisés.\nDétails techniques: calloc failed (1 de 2)"); return;}
	nbre_cases = 10;
	
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
	
	//Ajout de quelques entrées dans la grille et la légende:
	strcpy(buffer, "exemple");
	for (unsigned compteur = 0; compteur < strlen(buffer); compteur++)
	{
		grille[compteur][0].vide = false;
		grille[compteur][0].solution[0] = buffer[compteur];
		grille[compteur][0].solution[1] = '\000';
	}
	strcpy(buffer, "mots");
	for (unsigned compteur = 0; compteur < strlen(buffer); compteur++)
	{
		grille[3][compteur].vide = false;
		grille[3][compteur].solution[0] = buffer[compteur];
		grille[3][compteur].solution[1] = '\000';
	}
	strcpy(buffer, "expliqué");
	for (unsigned compteur = 0; compteur < 8; compteur++)
	{
		grille[6][compteur].vide = false;
		grille[6][compteur].solution[0] = buffer[compteur];
		grille[6][compteur].solution[1] = '\000';
	}
	grille[6][7].solution[1] = buffer[8];
	grille[6][7].solution[2] = '\000';
	
	rafraichir(nouv_grille);
	ne_pas_afficher = true;
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Aide", "La fenêtre suivante est une copie exacte (avec des données fictives) de la fenêtre de création d'une grille.\nToutefois, elle n'agit pas comme telle.\nCliquez sur n'importe quoi à l'écran pour en apprendre davantage à son sujet.\n\nAppuyez sur \"enter\" ou \"escape\" pour quitter cette fenêtre.", fenetre);
	
	while (1)
	{
		rafraichir(nouv_grille);
		
		SDL_WaitEvent(&ev);
		switch (ev.type)
		{
		case SDL_QUIT:
			quitter();
			break;
		
		case SDL_WINDOWEVENT:
			SDL_GetWindowSize(fenetre, &xmax, &ymax);
			break;
		
		case SDL_KEYDOWN:
			if (ev.key.keysym.sym == SDLK_ESCAPE || ev.key.keysym.sym == SDLK_RETURN || ev.key.keysym.sym == SDLK_KP_ENTER || ev.key.keysym.sym == SDLK_SPACE)
			{
				liberer_memoire();
				ne_pas_afficher = false;
				return;
			}
			break;
		
		case SDL_MOUSEMOTION:
			if (ev.motion.x >= marge_gauche && ev.motion.x <= marge_gauche + largeur_grille && ev.motion.y >= 40 && ev.motion.y <= 40 + largeur_grille) //clic dans la grille
			{rectangle(marge_gauche, 40, largeur_grille, largeur_grille, 0, couleur_focus, fond, rend);}
			else if (ev.motion.x >= marge_gauche && ev.motion.x <= marge_gauche + 110 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40) //annuler
			{rectangle(marge_gauche, marge_bas, 110, 40, 0, couleur_focus, fond, rend);}
			else if (ev.motion.x >= marge_gauche + 130 && ev.motion.x <= marge_gauche + 180 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40) //changer d'orientation
			{rectangle(marge_gauche + 130, marge_bas, 50, 40, 0, couleur_focus, fond, rend);}
			else if (ev.motion.x >= marge_gauche + 200 && ev.motion.x <= marge_gauche + 310 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40) //identifier
			{rectangle(marge_gauche + 200, marge_bas, 110, 40, 0, couleur_focus, fond, rend);}
			else if (ev.motion.x >= marge_gauche + 330 && ev.motion.x <= marge_gauche + 380 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40) //agrandir/rapetisser la police
			{rectangle(marge_gauche + 330, marge_bas, 50, 40, 0, couleur_focus, fond, rend);}
			else if (ev.motion.x >= marge_gauche + 400 && ev.motion.x <= marge_gauche + 510 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40) //terminer
			{rectangle(marge_gauche + 400, marge_bas, 110, 40, 0, couleur_focus, fond, rend);}
			else if (ev.motion.x >= (xmax - longueur_txt(titre, xmax, police)) / 2 - 34 && ev.motion.x <= (xmax + longueur_txt(titre, xmax, police)) / 2 && ev.motion.y >= 5 && ev.motion.y <= 30) //modifier le titre
			{rectangle((xmax - longueur_txt(titre, xmax, police)) / 2 - 34, 5, 35 + longueur_txt(titre, xmax, police), 25, 0, couleur_focus, fond, rend);}
			else if (ev.motion.x >= marge_droite && ev.motion.x <= xmax - 10 && ev.motion.y >= 40 && ev.motion.y <= marge_bas - 10) //légende
			{rectangle(marge_droite, 40, xmax - marge_droite - 10, marge_bas - 50, 0, couleur_focus, fond, rend);}
			break;
		
		case SDL_MOUSEBUTTONDOWN:
			if (ev.button.x >= marge_gauche && ev.button.x <= marge_gauche + largeur_grille && ev.button.y >= 40 && ev.button.y <= 40 + largeur_grille) //clic dans la grille
			{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Aide CMC - Création: Grille", \
				"Ceci est la future grille de mots croisés (son corrigé, si vous voulez...).\nChaque case peut contenir un caractère (soit une lettre (avec ou sans accent), un chiffre, un tiret ou un apostrophe)\nou être laissée vide (dans ce cas, la case sera noircie lorsque la grille sera jouée).\nToutes les majuscules sont automatiquement transformées en minuscules.\n\nLes espaces ne sont pas acceptés en tant que caractère, mais appuyer sur la barre d'espacement avancera votre curseur\nd'une case dans l'orientation actuelle.\nVous pouvez aussi vous déplacer dans la grille en utilisant les flèches du clavier.\nAppuyer sur \"Backspace\" supprimera le contenu de la case sélectionnée et vous fera reculer d'une case,\ntandis que \"delete\" videra la case sans déplacer votre curseur.\n\nLorsque vous avez écrit votre mot au complet, appuyer sur \"identifier\" pour y associer une description.\nUne fois qu'un mot a été identifié, il est possible de le modifier (non-recommandé!),\nde le supprimer (recommandé si vous voulez vraiment le modifier) ou d'en modifier la description\n(recommandé si vous ne voulez pas modifier le mot lui-même).", fenetre);
			}
			else if (ev.button.x >= marge_gauche && ev.button.x <= marge_gauche + 110 && ev.button.y >= marge_bas && ev.button.y <= marge_bas + 40) //annuler
			{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Aide CMC - Création: Annuler", \
				"Cliquez ici pour abandonner votre grille en cours de création.\nVotre grille sera supprimée et vous serez ramené(e) au menu principal.", fenetre);
			}
			else if (ev.button.x >= marge_gauche + 130 && ev.button.x <= marge_gauche + 180 && ev.button.y >= marge_bas && ev.button.y <= marge_bas + 40) //changer d'orientation
			{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Aide CMC - Création: Changer d'orientation", \
				"Cliquez sur ce bouton pour changer d'orientation dans la grille.\nPar défaut, vous commencez en écrivant horizontalement, mais vous pouvez aussi écrire des mots verticaux!\nVotre orientation est indiquée par une ligne (horizontale) ou une colonne (verticale) ombragée au niveau\nde la case sélectionnée dans la grille.\nIl est important d'avoir la bonne orientation lorsque vous identifiez un mot (via le bouton).\nVous pouvez aussi appuyer sur \"tab\" pour changer d'orientation.", fenetre);
			}
			else if (ev.button.x >= marge_gauche + 200 && ev.button.x <= marge_gauche + 310 && ev.button.y >= marge_bas && ev.button.y <= marge_bas + 40) //identifier
			{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Aide CMC - Création: Identifier un mot", \
				"Cliquez ici pour identifier un mot dans la grille et y ajouter une description.\nLorsque vous avez écrit un mot dans la grille, il est très important de l'identifier (idéalement tout de suite après l'avoir écrit),\nafin que le programme puisse le faire lui aussi.\n\nIl est *impératif* que vous ayiez sélectionné une case faisant partie du mot (ou la case vide immédiatement après)\net la bonne orientation (voir le bouton juste à gauche de celui-ci) pour identifier le bon mot!\n\nUne fois le mot identifié avec succès, le programme vous demandera de lui lier une description:\nC'est cette description qui sera affichée à droite de la grille pour permettre au joueur d'identifier le mot à écrire à cet endroit.\nC'est aussi à ce moment que le programme associera un nombre (mot horizontal) ou une lettre (mot vertical) à votre mot.\n(Les nombres et les lettres sont assignées dans l'ordre où vous identifiez les mots.)\n\nUne fois le mot identifié, il est possible de le modifier dans la grille (non-recommandé!),\nde le supprimer complètement (recommandé si vous voulez le modifier dans la grille)\nou de modifier sa description (recommandé si vous ne voulez pas modifier le mot dans la grille).", fenetre);
			}
			else if (ev.button.x >= marge_gauche + 330 && ev.button.x <= marge_gauche + 380 && ev.button.y >= marge_bas && ev.button.y <= marge_bas + 40) //agrandir/rapetisser la police
			{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Aide CMC - Création: Modifier la taille de la police", \
				"Cliquez sur ce bouton pour modifier la taille du texte dans la grille.\nCela ouvrira une nouvelle fenêtre où vous pourrez effectuer cete modification.\nLes nombres et les lettres identifiant les mots seront aussi affectés par vos changements\n(leur taille est normalement relative à celle du texte dans la grille).\n\nVous pouvez aussi modifier cela dans les réglages du programme (recommandé), qui sont bien plus complets.\nNotez toutefois que les modifications faites via ce boutons remplaceront vos réglages jusqu'à la fermeture du programme,\nmais ne seront pas enregistrées pour les prochaines fois où vous utiliserez ce programme.", fenetre);
			}
			else if (ev.button.x >= marge_gauche + 400 && ev.button.x <= marge_gauche + 510 && ev.button.y >= marge_bas && ev.button.y <= marge_bas + 40) //terminer
			{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Aide CMC - Création: Terminer", \
				"Cliquez ici lorsque vous avez terminé votre grille.\n\nElle sera d'abord vérifiée très sommairement (juste ce qu'il faut pour ne pas faire planter le programme\nou causer quelque chose de vraiment bizarre), mais vous devriez quand même vous être d'abord assuré\nd'avoir bel et bien identifié et associé une description à chaque mot dans la grille.\nLa grille sera ensuite sauvegardée dans un fichier \".txt\" nommé selon le titre de la grille\net placé dans le dossier \"grilles\" du programme.", fenetre);
			}
			else if (ev.button.x >= (xmax - longueur_txt(titre, xmax, police)) / 2 - 34 && ev.button.x <= (xmax + longueur_txt(titre, xmax, police)) / 2 && ev.button.y >= 5 && ev.button.y <= 30) //modifier le titre
			{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Aide CMC - Création: Titre de la grille", \
				"Cliquez sur ce bouton pour modifier le titre de la grille.\n\nChaque grille doit avoir un titre unique. Celui-ci peut contenir presque n'importe quels caractères,\nmais il est très fortement recommandé d'éviter les caractères suivants: .,;:\\/~$.\nIl est aussi recommandé d'éviter les accents (si possible...) et de garder une longueur (nombre de caractères) raisonnable.\n\nCe titre sera utilisé comme nom de fichier pour la sauvegarde de la grille (d'où les caractères non recommandés).", fenetre);
			}
			else if (ev.motion.x >= marge_droite && ev.motion.x <= xmax - 10 && ev.motion.y >= 40 && ev.motion.y <= marge_bas - 10) //légende
			{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Aide CMC - Création: Légende", \
				"Les descriptions des mots identifiés dans la grille s'afficheront ici.\nPour les modifier, vous devez sélectionner une case (dans la grille) faisant partie du mot à modifier et cliquer sur \"identifier\".", fenetre);
			}
			break;
		}
		SDL_RenderPresent(rend);
	}
}


void aide_jeu ()
//Gère la rubrique d'aide pour l'utilisation d'une grille.
//Ne devrait être appelée que par la fonction aide.
{
	SDL_Event ev;
	char buffer[100];
	
	//Reset de la grille actuelle:
	liberer_memoire();
	
	//Création d'une nouvelle grille vierge de 10x10:
	grille = calloc(nbre_cases, sizeof(struct _case*));
	if (grille == NULL)
	{erreur(13, "Impossible d'allouer assez de mémoire pour créer une nouvelle grille de mots croisés.\nDétails techniques: calloc failed (1 de 2)"); return;}
	nbre_cases = 10;
	
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
	
	//Ajout de quelques entrées dans la grille et la légende:
	strcpy(buffer, "exemple");
	for (unsigned compteur = 0; compteur < strlen(buffer); compteur++)
	{
		grille[compteur][0].vide = false;
		grille[compteur][0].solution[0] = buffer[compteur];
		grille[compteur][0].solution[1] = '\000';
	}
	strcpy(buffer, "mots");
	for (unsigned compteur = 0; compteur < strlen(buffer); compteur++)
	{
		grille[3][compteur].vide = false;
		grille[3][compteur].solution[0] = buffer[compteur];
		grille[3][compteur].solution[1] = '\000';
	}
	strcpy(buffer, "expliqué");
	for (unsigned compteur = 0; compteur < 8; compteur++)
	{
		grille[6][compteur].vide = false;
		grille[6][compteur].solution[0] = buffer[compteur];
		grille[6][compteur].solution[1] = '\000';
	}
	grille[6][7].solution[1] = buffer[8];
	grille[6][7].solution[2] = '\000';
	
	rafraichir(0);
	ne_pas_afficher = true;
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Aide", "La fenêtre suivante est une copie exacte (avec des données fictives) de la fenêtre de jeu.\nToutefois, elle n'agit pas comme telle.\nCliquez sur n'importe quoi à l'écran pour en apprendre davantage à son sujet.\n\nAppuyez sur \"enter\" ou \"escape\" pour quitter cette fenêtre.", fenetre);
	
	while (1)
	{
		rafraichir(0);
		
		//Ajoute des choses à la légende:
		sprintf(buffer, "A. Illustre concrètement un concept théorique.");
		afficher_txt(buffer, marge_droite, 70, xmax - marge_droite - 20, police, couleur_texte, rend);
		sprintf(buffer, "A");
		afficher_txt(buffer, marge_gauche + 3, largeur_case / 2 - 10 + 40, largeur_case, police_nbre_grille, couleur_texte, rend);
		
		sprintf(buffer, "1. Groupes de lettres ayant un certain sens.");
		afficher_txt(buffer, marge_droite, 305 + 1 * 25, xmax - marge_droite - 20, police, couleur_texte, rend);
		sprintf(buffer, "1");
		afficher_txt(buffer, marge_gauche + largeur_case * 3 + 3, 40 + largeur_case * 0, largeur_case, police_nbre_grille, couleur_texte, rend);
		
		sprintf(buffer, "2. Lorsqu'on comprend, c'est que c'est bien ________.");
		afficher_txt(buffer, marge_droite, 305 + 3 * 25, xmax - marge_droite - 20, police, couleur_texte, rend);
		sprintf(buffer, "2");
		afficher_txt(buffer, marge_gauche + largeur_case * 6 + 3, 40 + largeur_case * 0, largeur_case, police_nbre_grille, couleur_texte, rend);
		
		//Gère l'input utilisateur:
		SDL_WaitEvent(&ev);
		switch (ev.type)
		{
		case SDL_QUIT:
			quitter();
			break;
		
		case SDL_WINDOWEVENT:
			SDL_GetWindowSize(fenetre, &xmax, &ymax);
			break;
		
		case SDL_KEYDOWN:
			if (ev.key.keysym.sym == SDLK_ESCAPE || ev.key.keysym.sym == SDLK_RETURN || ev.key.keysym.sym == SDLK_KP_ENTER || ev.key.keysym.sym == SDLK_SPACE)
			{
				liberer_memoire();
				ne_pas_afficher = false;
				return;
			}
			break;
		
		case SDL_MOUSEMOTION:
			if (ev.motion.x >= marge_gauche && ev.motion.x <= marge_gauche + largeur_grille && ev.motion.y >= 40 && ev.motion.y <= 40 + largeur_grille) //clic dans la grille
			{rectangle(marge_gauche, 40, largeur_grille, largeur_grille, 0, couleur_focus, fond, rend);}
			else if (ev.motion.x >= marge_gauche && ev.motion.x <= marge_gauche + 110 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40) //annuler
			{rectangle(marge_gauche, marge_bas, 110, 40, 0, couleur_focus, fond, rend);}
			else if (ev.motion.x >= marge_gauche + 130 && ev.motion.x <= marge_gauche + 180 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40) //changer d'orientation
			{rectangle(marge_gauche + 130, marge_bas, 50, 40, 0, couleur_focus, fond, rend);}
			else if (ev.motion.x >= marge_gauche + 200 && ev.motion.x <= marge_gauche + 310 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40) //identifier
			{rectangle(marge_gauche + 200, marge_bas, 110, 40, 0, couleur_focus, fond, rend);}
			else if (ev.motion.x >= marge_gauche + 330 && ev.motion.x <= marge_gauche + 380 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40) //agrandir/rapetisser la police
			{rectangle(marge_gauche + 330, marge_bas, 50, 40, 0, couleur_focus, fond, rend);}
			else if (ev.motion.x >= marge_gauche + 400 && ev.motion.x <= marge_gauche + 510 && ev.motion.y >= marge_bas && ev.motion.y <= marge_bas + 40) //terminer
			{rectangle(marge_gauche + 400, marge_bas, 110, 40, 0, couleur_focus, fond, rend);}
			else if (ev.motion.x >= marge_droite && ev.motion.x <= xmax - 10 && ev.motion.y >= 40 && ev.motion.y <= marge_bas - 10) //légende
			{rectangle(marge_droite, 40, xmax - marge_droite - 10, marge_bas - 50, 0, couleur_focus, fond, rend);}
			break;
		
		case SDL_MOUSEBUTTONDOWN:
			if (ev.button.x >= marge_gauche && ev.button.x <= marge_gauche + largeur_grille && ev.button.y >= 40 && ev.button.y <= 40 + largeur_grille) //clic dans la grille
			{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Aide CMC - Jeu: Grille", \
				"Ceci est la grille de mots croisés.\nChaque case peut contenir un caractère (soit une lettre (avec ou sans accent), un chiffre, un tiret ou un apostrophe).\nLes cases pleines (de la même couleur que la grille) ne peuvent pas être remplies.\nToutes les majuscules sont automatiquement transformées en minuscules.\n\nVous pouvez vous déplacer dans la grille en utilisant les flèches du clavier.\nAppuyer sur \"Backspace\" supprimera le contenu de la case sélectionnée et vous fera reculer d'une case,\ntandis que \"delete\" videra la case sans déplacer votre curseur.\n\nLorsque vous avez écrit votre mot au complet, vous pouvez appuyer sur \"valider\" pour le vérifier.\nVous pouvez aussi attendre d'avoir tout terminé et vérifier tous les mots en même temps en appuyant sur \"terminé\".", fenetre);
			}
			else if (ev.button.x >= marge_gauche && ev.button.x <= marge_gauche + 110 && ev.button.y >= marge_bas && ev.button.y <= marge_bas + 40) //annuler
			{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Aide CMC - Jeu: Annuler", \
				"Cliquez ici pour abandonner votre partie.\nVotre grille restera en mémoire (vous permettant d'y rejouer ou de la modifier facilement)\net vous serez ramené(e) au menu principal.", fenetre);
			}
			else if (ev.button.x >= marge_gauche + 130 && ev.button.x <= marge_gauche + 180 && ev.button.y >= marge_bas && ev.button.y <= marge_bas + 40) //changer d'orientation
			{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Aide CMC - Jeu: Changer d'orientation", \
				"Cliquez sur ce bouton pour changer d'orientation dans la grille.\nPar défaut, vous commencez en écrivant horizontalement, mais vous pouvez aussi écrire des mots verticaux!\nVotre orientation est indiquée par une ligne (horizontale) ou une colonne (verticale) ombragée au niveau\nde la case sélectionnée dans la grille.\nIl est important d'avoir la bonne orientation lorsque vous validez un mot (via le bouton).\nVous pouvez aussi appuyer sur \"tab\" pour changer d'orientation.", fenetre);
			}
			else if (ev.button.x >= marge_gauche + 200 && ev.button.x <= marge_gauche + 310 && ev.button.y >= marge_bas && ev.button.y <= marge_bas + 40) //identifier
			{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Aide CMC - Jeu: Valider un mot", \
				"Cliquez ici pour valider un mot dans la grille.\nToutes les cases de ce mot qui contiennent le bon caractère seront surlignées en vert (ou la couleur choisie dans les réglages)\net les autres en orange (aussi modifiable dans les réglages).\nIl est important que vous ayiez sélectionné une case (dans la grille) faisant partie du mot\net que vous ayiez la bonne orientation pour que le bon mot soit validé.", fenetre);
			}
			else if (ev.button.x >= marge_gauche + 330 && ev.button.x <= marge_gauche + 380 && ev.button.y >= marge_bas && ev.button.y <= marge_bas + 40) //agrandir/rapetisser la police
			{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Aide CMC - Jeu: Modifier la taille de la police", \
				"Cliquez sur ce bouton pour modifier la taille du texte dans la grille.\nCela ouvrira une nouvelle fenêtre où vous pourrez effectuer cete modification.\nLes nombres et les lettres identifiant les mots seront aussi affectés par vos changements\n(leur taille est normalement relative à celle du texte dans la grille).\n\nVous pouvez aussi modifier cela dans les réglages du programme (recommandé), qui sont bien plus complets.\nNotez toutefois que les modifications faites via ce boutons remplaceront vos réglages jusqu'à la fermeture du programme,\nmais ne seront pas enregistrées pour les prochaines fois où vous utiliserez ce programme.", fenetre);
			}
			else if (ev.button.x >= marge_gauche + 400 && ev.button.x <= marge_gauche + 510 && ev.button.y >= marge_bas && ev.button.y <= marge_bas + 40) //terminer
			{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Aide CMC - Jeu: Terminer", \
				"Cliquez ici lorsque vous avez remplie la grille au complet.\nVos réponses seront validées (cases correctes en vert et cases incorrectes en orange\n(ces couleurs peuvent être modifiées dans les réglages)), puis vous pourrez\nrecommencer de zéro ou revenir au menu principal.", fenetre);
			}
			else if (ev.motion.x >= marge_droite && ev.motion.x <= xmax - 10 && ev.motion.y >= 40 && ev.motion.y <= marge_bas - 10) //légende
			{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Aide CMC - Jeu: Légende", \
				"Les descriptions des mots identifiés dans la grille s'afficheront ici.\nLa lettre ou le nombre précédant la description vous indique l'endroit où se trouve la première lettre du mot dans la grille.", fenetre);
			}
			break;
		}
		SDL_RenderPresent(rend);
	}
}