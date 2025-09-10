#include "creation.c"


void erreur (int code, char msg[])
//Log et/ou affiche un message d'erreur.
//Les codes d'erreurs inférieurs à 100 sont considéré très importants, tandis que les codes supérieurs à 100 seront souvent ignorés.
//Les codes de 1 à 10 sont considérés fatal.
//Les messages d'erreurs doivent contenir moins de 300 caractères et les codes d'erreur doivent être supérieurs à 0.
{
	char message[320] = "";
	FILE* ferreur;
	
	//Affiche un pop-up:
	if (code < 100)
	{
		if (code <= 10)
		{sprintf(message, "Erreur fatale %d.\n%s", code, msg);}
		else
		{sprintf(message, "Erreur %d.\n%s", code, msg);}
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "CMC - Erreur", message, fenetre);
	}
	
	//Log dans le fichier d'erreurs:
	if (errlog)
	{
		ferreur = fopen(nom_ferreur, "a+");
		
		if (code >= 100)
		{sprintf(message, "Erreur %d.\n%s", code, msg);}
		fprintf(ferreur, "\n%s\n", message);
		if (code <= 10)
		{fprintf(ferreur, "== ERREUR FATALE ==\nDémarrez le programme depuis un terminal avec l'option \"-d\" (\"-?\" pour plus d'informations) pour déboguer le programme.\nIl est conseillé de d'abord supprimer ce fichier.\n\n");}
		
		fclose(ferreur);
	}
}


void gestion_arguments (char arg[])
//Gère les arguments reçus par l'application à son ouverture.
{
	FILE* ferreur; //pour l'option de débogage
	
	
	if (!strcmp(arg, "-?") || !strcmp(arg, "-a") || !strcmp(arg, "-h") || !strcmp(arg, "--aide") || !strcmp(arg, "--help"))
	{
		printf("CMC\nCréateur de Mots Croisés\n\n");
		printf("Voici la liste des arguments acceptés par ce programme:\n");
		printf("--aide (-a ou -?)  affiche ce texte, puis quitte\n");
		printf("--deboguer (-d)    démarre le programme en mode débogage\n");
		printf("--version (-v)     affiche la version du programme, puis quitte\n");
		printf("\nCe programme est normalement démarré sans arguments et il n'est pas nécessaire de le démarrer depuis un terminal.\n");
		exit(0);
	}
	
	else if (!strcmp(arg, "-v") || !strcmp(arg, "--version"))
	{printf("CMC - Créateur de Mots Croisés\n---\nVersion %s \"%s\"\n", VERSION, NOM_VERSION); exit(0);}
	
	else if (!strcmp(arg, "-d") || !strcmp(arg, "--deboguer") || !strcmp(arg, "--debug"))
	{
		ferreur = fopen(nom_ferreur, "a+");
		
		if (debogage != true)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Mode débogage", "Mode débogage activé.\nN'oubliez pas de consulter votre terminal et le fichier d'erreurs (erreurs.txt).", NULL);
			fprintf(ferreur, "\n== Activation du mode débogage ==\nDémarrage de l'application...\n");
			fprintf(ferreur, "version=CMC %s \"%s\"\n\n", VERSION, NOM_VERSION);
			fprintf(ferreur, "Versions SDL:\n");
			afficher_versions_SDL(ferreur);
			debogage = true;
		}
		
		fprintf(ferreur, "\npolice=%s\nicones (modif. titre, taille police, changer or.): [1]=%s   [2]=%s   [3]=%s\n", nom_police_principale, fichier_symbole_modifier, fichier_symbole_taille_police, fichier_symbole_orientation);
		fprintf(ferreur, "tailles_polices (principale, petite, grille, nombres): [1]=%d   [2]=%d   [3]=%d   [4]=%d\n", taille_police_principale, taille_petite_police, taille_police_grille, taille_police_nbre_grille);
		fprintf(ferreur, "taille_fenêtre (XxY): minimale=%dx%d   défaut=%dx%d   actuelle=%dx%d\n", LARGEUR_MIN, HAUTEUR_MIN, largeur_fenetre, hauteur_fenetre, xmax, ymax);
		fprintf(ferreur, "marges=débrouille-toi... (calculées au redessinage)\n");
		fprintf(ferreur, "sélection (X, Y)=(%d, %d)   focus (X, Y)=(%d, %d)   => -1 = aucune sélection\norientation=%d   => 0 = Hor. / 1 = Ver.\n", selection_x, selection_y, focus_x, focus_y, orientation);
		fprintf(ferreur, "\nErreur(s) SDL: %s\n", SDL_GetError());
		
		fclose(ferreur);
	}
	
	else
	{erreur(11, "Argument non reconnu.\nEntrez \"./cmc --aide\" pour en savoir plus."); printf("\"%s\" n'est pas un argument accepté par ce programme.\n", arg);}
}


int main (int argc, char* argv[])
{
	//Gestion des arguments reçus par le programme:
	for (int num_arg = 1; num_arg < argc; num_arg++)
	{gestion_arguments(argv[num_arg]);}
	
	//Création de l'interface graphique:
	init();
	
	//Ouverture du menu principal:
	menu();
	
	//Ne devrait jamais être exécuté:
	quitter();
}


void init ()
//Initialise l'interface graphique SDL au démarrage du programme.
{
	char buffer[300];
	
	//Initialisation de SDL:
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		sprintf(buffer, "Erreur lors de l'initialisation de SDL.\n(%s)\n", SDL_GetError());
		erreur(1, buffer);
		exit(1);
	}
	
	//Initialisation de SDL_ttf:
	if (TTF_Init() < 0)
	{
		sprintf(buffer, "Erreur lors de l'initialisation de SDL_ttf.\n(%s)\n", TTF_GetError());
		erreur(2, buffer);
		SDL_Quit();
		exit(2);
	}
	
	//Création de la fenêtre:
	fenetre = SDL_CreateWindow("CMC - Créateur de Mots Croisés", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, largeur_fenetre, hauteur_fenetre, SDL_WINDOW_RESIZABLE);
	if (fenetre == NULL)
	{
		sprintf(buffer, "Erreur lors de la création de la fenêtre SDL:\n%s\n", SDL_GetError());
		erreur(3, buffer);
		TTF_Quit();
		SDL_Quit();
		exit(3);
	}
	
	//Création du renderer:
	rend = SDL_CreateRenderer(fenetre, -1, 0);
	if (rend == NULL)
	{
		sprintf(buffer, "Erreur lors de la création du renderer via SDL:\n%s\n", SDL_GetError());
		erreur(4, buffer);
		TTF_Quit();
		SDL_DestroyWindow(fenetre);
		SDL_Quit();
		exit(4);
	}
	SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND); //permet l'utilisation de couleurs transparentes semi-transparentes
	
	//Taille minimale de la fenêtre:
	SDL_SetWindowMinimumSize(fenetre, LARGEUR_MIN, HAUTEUR_MIN); //doit être placé après la création du renderer pour que ça marche (bug)
	SDL_GetWindowSize(fenetre, &xmax, &ymax);
	
	//Chargement des polices ttf:
	police = TTF_OpenFont(nom_police_principale, taille_police_principale);
	if (police == NULL)
	{
		sprintf(buffer, "Erreur lors du chargement de la police principale:\n%s\n", TTF_GetError());
		erreur(5, buffer);
		TTF_Quit();
		SDL_DestroyWindow(fenetre);
		SDL_Quit();
		exit(5);
	}
	police_grille = TTF_OpenFont(nom_police_principale, taille_police_grille);
	petite_police = TTF_OpenFont(nom_police_principale, taille_petite_police);
	police_nbre_grille = TTF_OpenFont(nom_police_principale, taille_police_nbre_grille);
	
	//Chargement des curseurs:
	curseur_normal = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	curseur_txt = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
	if (curseur_normal == NULL || curseur_txt == NULL)
	{sprintf(buffer, "Erreur lors de la création des curseurs de l'application: %s.\n\n", SDL_GetError()); erreur(101, buffer);}
	
	//Chargement des symboles:
	symbole_modifier = IMG_LoadTexture(rend, fichier_symbole_modifier);
	if (symbole_modifier == NULL)
	{sprintf(buffer, "Erreur lors du chargement du symbole de modification: %s\nLe dossier \"source\" a-t-il été altéré?", SDL_GetError()); erreur(12, buffer);}
	
	symbole_taille_police = IMG_LoadTexture(rend, fichier_symbole_taille_police);
	if (symbole_taille_police == NULL)
	{sprintf(buffer, "Erreur lors du chargement du symbole de modification de la taille de la police: %s\nLe dossier \"source\" a-t-il été altéré?", SDL_GetError()); erreur(12, buffer);}
	
	symbole_orientation = IMG_LoadTexture(rend, fichier_symbole_orientation);
	if (symbole_orientation == NULL)
	{sprintf(buffer, "Erreur lors du chargement du symbole de changement d'orientation: %s\nLe dossier \"source\" a-t-il été altéré?", SDL_GetError()); erreur(12, buffer);}
	
	//Remplissage en fond:
	SDL_SetColor(fond, rend);
	SDL_RenderClear(rend);
	SDL_RenderPresent(rend);
}


void afficher_menu (enum zones_menu zone)
//Affiche le menu principal dans la fenêtre de l'application.
//Reçoit la position du curseur en paramètre.
{
	SDL_SetColor(fond, rend);
	SDL_RenderClear(rend);
	
	TTF_SetFontStyle(police, TTF_STYLE_BOLD);
	afficher_txt_centre("Menu Principal", 0, xmax, 10, police, couleur_texte, rend);
	TTF_SetFontStyle(police, TTF_STYLE_NORMAL);
	
	if (zone == creer)
	{rect_arrondi(xmax / 2 - 150, 100, 300, 100, couleur_selection, fond, rend);}
	else
	{rect_arrondi(xmax / 2 - 150, 100, 300, 100, couleur_boutons, fond, rend);}
	afficher_txt_centre("Créer une nouvelle grille", xmax / 2 - 150, xmax / 2 + 150, 140, police, couleur_texte, rend);
	
	if (zone == jouer)
	{rect_arrondi(xmax / 2 - 150, 230, 300, 100, couleur_selection, fond, rend);}
	else
	{rect_arrondi(xmax / 2 - 150, 230, 300, 100, couleur_boutons, fond, rend);}
	afficher_txt_centre("Jouer aux mots croisés", xmax / 2 - 150, xmax / 2 + 150, 255, police, couleur_texte, rend);
	afficher_txt_centre("(charger une grille)", xmax / 2 - 150, xmax / 2 + 150, 285, police, couleur_texte, rend);
	
	SDL_SetColor(couleur_texte, rend);
	SDL_RenderDrawLine(rend, xmax / 2 - 150, 360, xmax / 2 + 150, 360);
	
	if (zone == aide)
	{rect_arrondi(xmax / 2 - 150, 390, 300, 50, couleur_selection, fond, rend);}
	else
	{rect_arrondi(xmax / 2 - 150, 390, 300, 50, couleur_boutons, fond, rend);}
	afficher_txt_centre("Ouvrir le module d'aide", xmax / 2 - 150, xmax / 2 + 150, 405, police, couleur_texte, rend);
	
	if (zone == reglages)
	{rect_arrondi(xmax / 2 - 150, 470, 300, 50, couleur_selection, fond, rend);}
	else
	{rect_arrondi(xmax / 2 - 150, 470, 300, 50, couleur_boutons, fond, rend);}
	afficher_txt_centre("Accéder aux réglages", xmax / 2 - 150, xmax / 2 + 150, 485, police, couleur_texte, rend);
	
	afficher_txt("CMC Version", xmax - 175, ymax - 25, 200, police, couleur_texte, rend);
	afficher_txt(VERSION, xmax - 40, ymax - 25, 40, police, couleur_texte, rend);
	
	SDL_RenderPresent(rend);
}


void menu ()
//Gère le menu principal de l'application.
{
	SDL_Event ev;
	int choix;
		
	SDL_MessageBoxData popup_quitter =
	{
		SDL_MESSAGEBOX_INFORMATION,
		fenetre,
		"CMC - Quitter?",
		"Voulez-vous vraiment quitter l'application?",
		2,
		boutons_oui_non,
		NULL
	};
	
	
	afficher_menu(0);
	
	while (1)
	{
		SDL_WaitEvent(&ev);
		
		switch (ev.type)
		{
		case SDL_QUIT:
			quitter();
			return;
		
		case SDL_WINDOWEVENT:
			SDL_GetWindowSize(fenetre, &xmax, &ymax);
			afficher_menu(0);
			break;
		
		case SDL_KEYDOWN:
			switch (ev.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				SDL_ShowMessageBox(&popup_quitter, &choix);
				if (choix)
				{quitter();}
				afficher_menu(0);
				break;
			
			case SDLK_RETURN:
			case SDLK_KP_ENTER:
			case SDLK_TAB:
			case SDLK_UP:
			case SDLK_DOWN:
			case SDLK_LEFT:
			case SDLK_RIGHT:
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Clavier non-supporté", "Vous ne pouvez pas utiliser le clavier pour sélectionner un bouton dans ce menu.", fenetre);
				break;
			}
			break;
		
		case SDL_MOUSEMOTION:
			if (ev.motion.x >= xmax / 2 - 150 && ev.motion.x <= xmax / 2 + 150)
			{
				if (ev.motion.y >= 100 && ev.motion.y <= 200)
				{afficher_menu(creer);}
				
				else if (ev.motion.y >= 230 && ev.motion.y <= 330)
				{afficher_menu(jouer);}
				
				else if (ev.motion.y >= 390 && ev.motion.y <= 440)
				{afficher_menu(aide);}
				
				else if (ev.motion.y >= 470 && ev.motion.y <= 520)
				{afficher_menu(reglages);}
				
				else
				{afficher_menu(0);}
			}
			else
			{afficher_menu(0);}
			break;
		
		case SDL_MOUSEBUTTONDOWN:
			if (ev.button.x >= xmax / 2 - 150 && ev.button.x <= xmax / 2 + 150)
			{
				if (ev.button.y >= 100 && ev.button.y <= 200) //nouvelle grille
				{nouvelle_grille(); afficher_menu(0);}
				
				else if (ev.button.y >= 230 && ev.button.y <= 330) //jouer
				{partie(); afficher_menu(0);}
				
				else if (ev.button.y >= 390 && ev.button.y <= 440) //aide
				{/*...*/ afficher_menu(0);}
				
				else if (ev.button.y >= 470 && ev.button.y <= 520) //réglages
				{/*...*/ afficher_menu(0);}
			}
			break;
		}
	}
}


void quitter ()
//Ferme l'interface graphique SDL et libère la mémoire nécessaire.
{
	FILE* ferreur; //pour l'option de débogage...
	
	
	liberer_memoire();
	
	if (symbole_modifier != NULL)
	{SDL_DestroyTexture(symbole_modifier);}
	if (symbole_taille_police != NULL)
	{SDL_DestroyTexture(symbole_taille_police);}
	if (symbole_orientation != NULL)
	{SDL_DestroyTexture(symbole_orientation);}
			
	SDL_FreeCursor(curseur_txt);
	SDL_FreeCursor(curseur_normal);
	TTF_CloseFont(petite_police);
	TTF_CloseFont(police_nbre_grille);
	TTF_CloseFont(police_grille);
	TTF_CloseFont(police);
	
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(fenetre);
	
	TTF_Quit();
	SDL_Quit();
	
	if (debogage)
	{
		ferreur = fopen(nom_ferreur, "a+");
		fprintf(ferreur, "\nFermeture du programme...\nBon débogage!\n\n");
		fclose(ferreur);
	}
	
	exit(0);
}