#include "creation.c"

#define NBRE_PARAM 16 //ATTENTION: J'ai des "-12" hardcodés un peu partout...


struct parametre
//Structure d'un paramètre modifiable des réglages.
{
	char nom[60];
	char descr[650];
	char type; //i = int, b = bool, c = char (non-implanté), s = str, C = couleur (non-implanté), t = titre de section (pas un paramètre)
	int* ptr_int; //pour les int et les bool
	char* ptr_str; //pour les str et les char
};


struct parametre param[NBRE_PARAM] =
//Liste des paramètres (incluant les sous-titres).
{
		{"Usage fréquent:", "-", 't', NULL, NULL},
	{"durée du délai de validation d'un mot", "Lorsque vous validez un mot (en cliquant sur le bouton \"vérifier\" en bas de la grille, lors d'une partie), les cases correctes s'affichent en vert et les cases incorrectes en rouge-orangé pour une certaine durée, avant de revenir à la normale.\nVous pouvez ici modifier cette durée (en millisecondes).", 'i', &delai_validation_mot, NULL},
		{"Symboles et icones:", "-", 't', NULL, NULL},
	{"symbole de modification", "Vous pouvez ici remplacer le symbole de modification du titre d'une grille, utilisé lors de la création de celle-ci.\nVous devez fournir un chemin d'accès valide à une fichier PNG contenant le nouveau symbole.\nLe chemin d'accès doit être écrit selon la convention Linux (même si le programme est exécuté sous Windows).\nN'oubliez pas l'extension \".png\".\nIl est recommandé d'utiliser les mêmes dimensions que le symbole original. Un fichier n'ayant pas les mêmes proportions ou dimensions que le fichier original sera étiré ou compressé.", 's', NULL, fichier_symbole_modifier},
	{"symbole de modification de la taille de la police", "Vous pouvez ici remplacer le symbole de modification de la taille de la police du texte et des nombres dans les grilles.\nVous devez fournir un chemin d'accès valide à une fichier PNG contenant le nouveau symbole.\nLe chemin d'accès doit être écrit selon la convention Linux (même si le programme est exécuté sous Windows).\nN'oubliez pas l'extension \".png\".\nIl est recommandé d'utiliser les mêmes dimensions que le symbole original. Un fichier n'ayant pas les mêmes proportions ou dimensions que le fichier original sera étiré ou compressé.", 's', NULL, fichier_symbole_taille_police},
	{"symbole d'inversion de l'orientation", "Vous pouvez ici remplacer le symbole d'inversion de l'orientation de l'entrée de texte dans la grille.\nVous devez fournir un chemin d'accès valide à une fichier PNG contenant le nouveau symbole.\nLe chemin d'accès doit être écrit selon la convention Linux (même si le programme est exécuté sous Windows).\nN'oubliez pas l'extension \".png\".\nIl est recommandé d'utiliser les mêmes dimensions que le symbole original. Un fichier n'ayant pas les mêmes proportions ou dimensions que le fichier original sera étiré ou compressé.", 's', NULL, fichier_symbole_orientation},
		{"Autres paramètres:", "-", 't', NULL, NULL},
	{"largeur initiale de la fenêtre", "Vous pouvez modifier ici la largeur que devrait avoir la fenêtre de l'application à son ouverture.\nNotez que cela n'affectera pas la taille atuelle de la fenêtre et qu'il est présentement impossible de faire démarrer le programme avec une fenêtre maximisée.\nEn tous temps, la largeur de la fenêtre ne peut pas être inférieure à 800.\nToutes les dimensions sont en pixels (tels que vus par le programme).", 'i', &largeur_fenetre, NULL},
	{"hauteur initiale de la fenêtre", "Vous pouvez modifier ici la hauteur que devrait avoir la fenêtre de l'application à son ouverture.\nNotez que cela n'affectera pas la taille atuelle de la fenêtre et qu'il est présentement impossible de faire démarrer le programme avec une fenêtre maximisée.\nEn tous temps, la hauteur de la fenêtre ne peut pas être inférieure à 600.\nToutes les dimensions sont en pixels (tels que vus par le programme).", 'i', &hauteur_fenetre, NULL},
	{"logguer les erreurs dans un fichier", "Par défaut, le programme laisse une trace de toutes ses erreurs dans un fichier nommé \"erreurs.txt\" situé au même endroit que le programme.\nCe réglage permet de désactiver cette fonctionnalité.\nVeuillez noter que cela n'empêchera pas les pop-ups d'erreurs d'apparaître à l'écran.\nSi vous utilisez cette application sous Linux et que vous voulez désactiver cette fonctionnalité parce qu'elle crée des fichiers d'erreurs au mauvais endroit,\nassurez-vous plutôt de toujours ouvrir l'application via le script \"run.sh\", situé au même endroit que l'application.\nDésactiver cette fonctionnalité n'est pas recommandé.", 'b', (int*) &errlog, NULL},
		{"Affichage du texte:", "-", 't', NULL, NULL},
	{"police", "Vous pouvez changer ici la police utilisée par l'application.\nPour ce faire, vous devez fournir un fichier \".ttf\" contenant les informations de cette police.\nPlusieurs fichiers de différentes polices sont déjà fournis avec ce programme. Vous pouvez les consulter dans le dossier \"source\" du programme.\nVous pouvez donc remplacer le fichier \".ttf\" actuel par un de ceux-là ou un autre.\nAssurez-vous de fournir un chemin d'accès valide selon la convention Linux (même si vous utilisez la version Windows).", 's', NULL, nom_police_principale},
	{"taille de la police principale", "Vous pouvez modifier ici la taille de la police utilisée pour les boutons, les titres, la légende de la grille, etc.\nLa taille par défaut est 20 pts.\nVeuillez noter que la taille varira selon la police utilisée.", 'i', &taille_police_principale, NULL},
	{"taille de la petite police", "Vous pouvez modifier ici la taille de la police utilisée pour ce texte et quelques autres explications.\nLa taille par défaut est 18 pts.\nVeuillez noter que la taille varira selon la police utilisée.", 'i', &taille_petite_police, NULL},
	{"taille de la police du texte dans la grille", "Vous pouvez modifier ici la taille du texte affiché dans une grille de mots croisés.\nLa taille par défaut est 30 pts.\nVeuillez noter que la taille varira selon la police utilisée.\nCe réglage permet aussi de régler la taille de ce texte sans modifier celle des nombres et des lettres indiquant le début des mots dans la grille, contrairement au bouton présent en dessous de la grille (qui ne sauvegarde pas non-plus vos modifications). (Utiliser ce bouton brisera cette indépendance.)", 'i', &taille_police_grille, NULL},
	{"taille de la police des nombres dans la grille", "Vous pouvez modifier ici la taille des lettres et des nombres servant à indiquer le début des mots dans une grille de mots croisés.\nLa taille par défaut est 20 pts.\nVeuillez noter que la taille varira selon la police utilisée.\nCe réglage permet aussi de régler cette taille sans modifier celle du texte dans la grille, contrairement au bouton présent en dessous de la grille (qui ne sauvegarde pas non-plus vos modifications). (Utiliser ce bouton brisera cette indépendance et effacera votre modification jusqu'à la fermeture du programme.)", 'i', &taille_police_nbre_grille, NULL},
	//Ne pas oublier de mettre le NBRE_PARAM à jour si un paramètre (ou un titre de section) est ajouté!
};

bool colonne_affichee = 0; //indique quelle colonne (#0 ou #1) est présentement affichée (ne s'applique pas si la fenêtre est assez grande pour afficher les 2)


void reglages ()
//Gère les réglages de l'application.
{
	SDL_Event ev;
	bool redessiner = 0;
	int buffint;
	
	
	rafraichir_reglages(0);
	
	while (1)
	{
		SDL_WaitEvent(&ev);
		
		switch (ev.type)
		{
		case SDL_QUIT:
			quitter();
			break;
		
		case SDL_WINDOWEVENT:
			SDL_GetWindowSize(fenetre, &xmax, &ymax);
			rafraichir_reglages(0);
			break;
		
		case SDL_KEYDOWN:
			switch (ev.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				return;
			}
			break;
		
		case SDL_MOUSEMOTION:
			if (ev.motion.x >= xmax - 210 && ev.motion.x <= xmax - 20 && ev.motion.y >= ymax - 55 && ev.motion.y <= ymax - 15)
			{rafraichir_reglages('T');}
			else if (ev.motion.x >= xmax - 210 && ev.motion.x <= xmax - 20 && ev.motion.y >= ymax - 105 && ev.motion.y <= ymax - 65)
			{rafraichir_reglages('R');}
			else if (ev.motion.x >= xmax - 210 && ev.motion.x <= xmax - 20 && ev.motion.y >= ymax - 155 && ev.motion.y <= ymax - 115 && xmax <= 1150)
			{rafraichir_reglages('+');}
			else if (ev.motion.x >= 50 && ev.motion.x <= 550)
			{
				redessiner = 1;
				for (int compteur = 0; compteur < 10 - colonne_affichee * (NBRE_PARAM - 12); compteur++) //ATTENTION! Le -12 est hardcodé...
				{
					if (ev.motion.y >= 50 + compteur * 50 && ev.motion.y <= 90 + compteur * 50)
					{rafraichir_reglages(10 + compteur + 10 * colonne_affichee); redessiner = 0;}
				}
			}
			else if (ev.motion.x >= 600 && ev.motion.x <= 1100 && xmax >= 1150)
			{
				redessiner = 1;
				for (int compteur = 0; compteur < NBRE_PARAM - 10; compteur++)
				{
					if (ev.motion.y >= 50 + compteur * 50 && ev.motion.y <= 90 + compteur * 50)
					{rafraichir_reglages(20 + compteur); redessiner = 0;}
				}
			}
			else
			{rafraichir_reglages(0);}
			if (redessiner)
			{rafraichir_reglages(0); redessiner = 0;}
			break;
		
		case SDL_MOUSEBUTTONDOWN:
			if (ev.button.x >= xmax - 210 && ev.button.x <= xmax - 20 && ev.button.y >= ymax - 55 && ev.button.y <= ymax - 15) //Terminé
			{enregistrer_reglages(); appliquer_reglages();} //le programme va redémarrer ici
			else if (ev.button.x >= xmax - 210 && ev.button.x <= xmax - 20 && ev.button.y >= ymax - 105 && ev.button.y <= ymax - 65) //Réinitialiser
			{reinitialiser_reglages();}
			else if (ev.button.x >= xmax - 210 && ev.button.x <= xmax - 20 && ev.button.y >= ymax - 155 && ev.button.y <= ymax - 115 && xmax <= 1150) //Plus d'options
			{
				if (!colonne_affichee)
				{colonne_affichee = 1;}
				else
				{colonne_affichee = 0;}
				rafraichir_reglages('+');
			}
			else if (ev.button.x >= 50 && ev.button.x <= 550) //1ère colonne
			{
				for (int compteur = 0; compteur < 10 - colonne_affichee * (NBRE_PARAM - 12); compteur++) //ATTENTION! Le -12 est hardcodé...
				{
					if (ev.button.y >= 50 + compteur * 50 && ev.button.y <= 90 + compteur * 50)
					{
						if (param[compteur + 10 * colonne_affichee].type == 'i') //int
						{
							buffint = demander_nbre(param[compteur + 10 * colonne_affichee].nom, param[compteur + 10 * colonne_affichee].descr, *param[compteur + 10 * colonne_affichee].ptr_int, fenetre);
							if (buffint == -6699) {} //annulé
							else if (buffint <= 0)
							{SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Valeur négative ou nulle", "Les valeurs négatives et nulles ne sont pas acceptées.\nVeuillez réessayer.", fenetre);}
							else
							{*param[compteur + 10 * colonne_affichee].ptr_int = buffint;}
						}
						else if (param[compteur + 10 * colonne_affichee].type == 'b') //bool
						{
							SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Informations sur le paramètre", param[compteur + 10 * colonne_affichee].descr, fenetre);
							if (*((bool*) param[compteur + 10 * colonne_affichee].ptr_int))
							{*((bool*) param[compteur + 10 * colonne_affichee].ptr_int) = false;}
							else
							{*((bool*) param[compteur + 10 * colonne_affichee].ptr_int) = true;}
							rafraichir_reglages(compteur + 10 + colonne_affichee * 10);
						}
						else if (param[compteur + 10 * colonne_affichee].type == 's')
						{demander_txt(param[compteur + 10 * colonne_affichee].nom, param[compteur + 10 * colonne_affichee].descr, param[compteur + 10 * colonne_affichee].ptr_str, 200, fenetre);}
					}
				}
			}
			else if (ev.button.x >= 600 && ev.button.x <= 1100 && xmax >= 1150) //2e colonne
			{
				for (int compteur = 0; compteur < NBRE_PARAM - 10; compteur++)
				{
					if (ev.button.y >= 50 + compteur * 50 && ev.button.y <= 90 + compteur * 50)
					{
						if (param[compteur + 10].type == 'i') //int
						{
							buffint = demander_nbre(param[compteur + 10].nom, param[compteur + 10].descr, *param[compteur + 10].ptr_int, fenetre);
							if (buffint == -6699) {} //annulé
							else if (buffint <= 0)
							{SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Valeur négative ou nulle", "Les valeurs négatives et nulles ne sont pas acceptées.\nVeuillez réessayer.", fenetre);}
							else
							{*param[compteur + 10].ptr_int = buffint;}
						}
						else if (param[compteur + 10].type == 'b') //bool
						{
							SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Informations sur le paramètre", param[compteur + 10].descr, fenetre);
							if (*((bool*) param[compteur + 10].ptr_int))
							{*((bool*) param[compteur + 10].ptr_int) = false;}
							else
							{*((bool*) param[compteur + 10].ptr_int) = true;}
							rafraichir_reglages(compteur + 20);
						}
						else if (param[compteur + 10].type == 's')
						{demander_txt(param[compteur + 10].nom, param[compteur + 10].descr, param[compteur + 10].ptr_str, 200, fenetre);}
					}
				}
			}
			break;
		}
	}
}


void rafraichir_reglages (char zone)
//Redessine l'écran des réglages.
{
	SDL_SetColor(fond, rend);
	SDL_RenderClear(rend);
	
	TTF_SetFontStyle(police, TTF_STYLE_BOLD);
	afficher_txt_centre("Réglages", 0, xmax, 10, police, couleur_texte, rend);
	TTF_SetFontStyle(police, TTF_STYLE_NORMAL);
	
	if (xmax >= 1150 && colonne_affichee != 0)
	{colonne_affichee = 0;}
	
	for (int compteur = 0; compteur < 10 - colonne_affichee * (NBRE_PARAM - 12); compteur++) //ATTENTION! Le -12 est hardcodé...
	{
		if (param[compteur + colonne_affichee * 10].type != 't')
		{
			if (zone == compteur + 10 + colonne_affichee * 10)
			{rect_arrondi(50, 50 + compteur * 50, 500, 40, couleur_selection, fond, rend);}
			else
			{rect_arrondi(50, 50 + compteur * 50, 500, 40, couleur_boutons, fond, rend);}
		}
		afficher_txt(param[compteur + colonne_affichee * 10].nom, 60, 60 + compteur * 50, 500, police, couleur_texte, rend);
		if (param[compteur + colonne_affichee * 10].type == 'b')
		{
			if (*((bool*)param[compteur + colonne_affichee * 10].ptr_int))
			{afficher_txt("ON", 500, 60 + compteur * 50, 50, police, couleur_texte, rend);}
			else
			{afficher_txt("OFF", 500, 60 + compteur * 50, 50, police, couleur_texte, rend);}
		}
	}
	
	if (xmax >= 1150)
	{
		for (int compteur = 0; compteur + 10 < NBRE_PARAM; compteur++)
		{
			if (param[compteur + 10].type != 't')
			{
				if (zone == compteur + 20)
				{rect_arrondi(600, 50 + compteur * 50, 500, 40, couleur_selection, fond, rend);}
				else
				{rect_arrondi(600, 50 + compteur * 50, 500, 40, couleur_boutons, fond, rend);}
			}
			afficher_txt(param[compteur + 10].nom, 610, 60 + compteur * 50, 500, police, couleur_texte, rend);
			if (param[compteur + 10].type == 'b')
			{
				if (*((bool*)param[compteur + 10].ptr_int))
				{afficher_txt("ON", 850, 60 + compteur * 50, 50, police, couleur_texte, rend);}
				else
				{afficher_txt("OFF", 850, 60 + compteur * 50, 50, police, couleur_texte, rend);}
			}
		}
	}
	else
	{
		if (zone == '+')
		{rect_arrondi(xmax - 210, ymax - 155, 190, 40, couleur_selection, fond, rend);}
		else
		{rect_arrondi(xmax - 210, ymax - 155, 190, 40, couleur_boutons, fond, rend);}
		afficher_txt_centre("Plus d'options", xmax - 210, xmax - 20, ymax - 145, police, couleur_texte, rend);
		
		if (!colonne_affichee)
		{afficher_txt_centre("Page 1 / 2", xmax - 210, xmax - 20, ymax - 185, police, couleur_texte, rend);}
		else
		{afficher_txt_centre("Page 2 / 2", xmax - 210, xmax - 20, ymax - 185, police, couleur_texte, rend);}
	}
	
	if (zone == 'R')
	{rect_arrondi(xmax - 210, ymax - 105, 190, 40, couleur_selection, fond, rend);}
	else
	{rect_arrondi(xmax - 210, ymax - 105, 190, 40, couleur_boutons, fond, rend);}
	afficher_txt_centre("Réinitialiser", xmax - 210, xmax - 20, ymax - 95, police, couleur_texte, rend);
	
	if (zone == 'T')
	{rect_arrondi(xmax - 210, ymax - 55, 190, 40, couleur_selection, fond, rend);}
	else
	{rect_arrondi(xmax - 210, ymax - 55, 190, 40, couleur_boutons, fond, rend);}
	afficher_txt_centre("Terminé", xmax - 210, xmax - 20, ymax - 45, police, couleur_texte, rend);
	
	afficher_txt("CMC Version", 5, ymax - 25, 200, police, couleur_texte, rend);
	afficher_txt(VERSION, afficher_txt("CMC Version", 0, 0, 1000, police, blanc, NULL) + 10, ymax - 25, 100, police, couleur_texte, rend);
	
	SDL_RenderPresent(rend);
}


void reinitialiser_reglages ()
//Remet tous les réglages à leur valeur par défaut.
{
	largeur_fenetre = 900;
	hauteur_fenetre = 700;
	strcpy(nom_police_principale, "./source/dejavu_sans.ttf");
	taille_police_principale = 20;
	taille_petite_police = 18;
	taille_police_grille = 30;
	taille_police_nbre_grille = 20;
	strcpy(fichier_symbole_modifier, "./source/edit.png");
	strcpy(fichier_symbole_taille_police, "./source/taille_police.png");
	strcpy(fichier_symbole_orientation, "./source/orientation.png");
	delai_validation_mot = 1500;
	errlog = true;
	
	enregistrer_reglages();
	appliquer_reglages();
}


void appliquer_reglages ()
//Redémarre l'application pour appliquer les réglages.
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Appliquer les paramètres", "L'application va redémarrer pour appliquer vos nouveaux réglages.", fenetre);
	execl("./cmc", "cmc", (char*) NULL); //le 2e paramètre ne sert à rien (?)
	
	//Ne devrait jamais être exécuté:
	erreur(24, "Impossible de redémarrer l'application.\nVeuillez rouvrir CMC par vous-même.\nDétails techniques: execl n'a pas arrêté le programme.");
	quitter();
}


void enregistrer_reglages ()
//Enregistre les réglages actuels dans fconfig.
{
	FILE* fconfig = fopen(nom_fconfig, "w+");
	
	if (fconfig == NULL)
	{
		erreur(25, \
			"Impossible de créer un nouveau fichier de réglages à l'emplacement spécifié.Vos réglages ne seront donc pas enregistrés.\nDétails techniques: fopen en w+ n'a pas ouvert de fichier!\nA-t-on les permissions requises?");
		return;
	}
	
	fprintf(fconfig, "* Fichier de configuration (réglages) de l'éditeur de mots croisés CMC.\n* \"cmc --aide\" pour plus de détails.\n*\n");
	fprintf(fconfig, "* Ce fichier est généré automatiquement et son contenu est effacé et remplacé à chaque ouverture des réglages.\n* Modifiez à vos risques et périls.\n");
	fprintf(fconfig, "* Toute ligne précédée d'un \"*\" séparé du texte par un espace sera ignorée par le programme.\n* Ne mettez qu'un seul paramètre par ligne.\n\n\n");
	
	fprintf(fconfig, "CMC_Version = %s %s\n\n", VERSION, NOM_VERSION);
	
	fprintf(fconfig, "Dimensions_Fenetre = %d x %d\n\n", largeur_fenetre, hauteur_fenetre);
	fprintf(fconfig, "Police = %s\n", nom_police_principale);
	fprintf(fconfig, "Police_Principale = %d\nPetite_Police = %d\nPolice_Grille = %d\nPolice_Nombres = %d\n\n", taille_police_principale, taille_petite_police, taille_police_grille, taille_police_nbre_grille);
	fprintf(fconfig, "Symbole_Modifier = %s\nSymbole_Taille_Police = %s\nSymbole_Orientation = %s\n\n", fichier_symbole_modifier, fichier_symbole_taille_police, fichier_symbole_orientation);
	fprintf(fconfig, "Délai_Validation = %d\n\nLog_Erreurs = %d\n", delai_validation_mot, errlog);
	
	fclose(fconfig);
}


bool lire_reglages ()
//Lit les réglages depuis fconfig.
//Utilisé uniquement au démarrage de l'application.
//Renvoie 0 en cas d'échec et 1 en cas de succès (ou si le fichier n'existe pas).
{
	char ligne[500];
	char* buffer;
	FILE* fconfig = fopen(nom_fconfig, "r");
	
	if (fconfig == NULL)
	{
		if (debogage)
		{printf("Aucun fichier de réglages trouvé en %s.\nUtilisation des paramètres par défaut.\n", nom_fconfig);}
		return 1;
	}
	
	while (fgets(ligne, sizeof(ligne), fconfig) != NULL)
	{
		enlever_majuscule(ligne);
		buffer = strtok(ligne, " =:;\n");
		
		if (buffer == NULL || !strcmp(buffer, "*") || !strcmp(buffer, "cmc_version")) {}
		
		else if (!strcmp(buffer, "dimensions_fenetre"))
		{
			buffer = strtok(NULL, " =:;\n");
			if (buffer == NULL || !sscanf(buffer, "%d", &largeur_fenetre))
			{
				erreur(26, "Fichier de réglages invalide.\nLes réglages par défaut seront utilisés.\nOuvrez le programme en mode débogage pour en savoir plus.");
				if (debogage)
				{printf("Fichier de config invalide:\nDans la ligne débutant par \"%s\", le mot \"%s\" devrait être un nombre.\n\n", ligne, buffer);}
				return 0;
			}
			buffer = strtok(NULL, " =:;\n");
			if (!strcmp(buffer, "x") || !strcmp(buffer, "par") || !strcmp(buffer, "sur"))
			{buffer = strtok(NULL, " =:;\n");}
			if (buffer == NULL || !sscanf(buffer, "%d", &hauteur_fenetre))
			{
				erreur(26, "Fichier de réglages invalide.\nLes réglages par défaut seront utilisés.\nOuvrez le programme en mode débogage pour en savoir plus.");
				if (debogage)
				{printf("Fichier de config invalide:\nDans la ligne débutant par \"%s\", le mot \"%s\" devrait être un nombre.\n\n", ligne, buffer);}
				return 0;
			}
		}
		
		else if (!strcmp(buffer, "police"))
		{
			buffer = strtok(NULL, " =:;\n");
			if (buffer == NULL)
			{
				erreur(26, "Fichier de réglages invalide.\nLes réglages par défaut seront utilisés.\nOuvrez le programme en mode débogage pour en savoir plus.");
				if (debogage)
				{printf("Fichier de config invalide:\nIl manque une valeur à l'identifiant \"%s\" (seul l'identifiant est détecté).\n\n", ligne);}
				return 0;
			}
			strcpy(nom_police_principale, buffer);
		}
		
		else if (!strcmp(buffer, "police_principale"))
		{
			buffer = strtok(NULL, " =:;\n");
			if (buffer == NULL || !sscanf(buffer, "%d", &taille_police_principale))
			{
				erreur(26, "Fichier de réglages invalide.\nLes réglages par défaut seront utilisés.\nOuvrez le programme en mode débogage pour en savoir plus.");
				if (debogage)
				{printf("Fichier de config invalide:\nDans la ligne débutant par \"%s\", le mot \"%s\" devrait être un nombre.\n\n", ligne, buffer);}
				return 0;
			}
		}
		
		else if (!strcmp(buffer, "petite_police"))
		{
			buffer = strtok(NULL, " =:;\n");
			if (buffer == NULL || !sscanf(buffer, "%d", &taille_petite_police))
			{
				erreur(26, "Fichier de réglages invalide.\nLes réglages par défaut seront utilisés.\nOuvrez le programme en mode débogage pour en savoir plus.");
				if (debogage)
				{printf("Fichier de config invalide:\nDans la ligne débutant par \"%s\", le mot \"%s\" devrait être un nombre.\n\n", ligne, buffer);}
				return 0;
			}
		}
		
		else if (!strcmp(buffer, "police_grille"))
		{
			buffer = strtok(NULL, " =:;\n");
			if (buffer == NULL || !sscanf(buffer, "%d", &taille_police_grille))
			{
				erreur(26, "Fichier de réglages invalide.\nLes réglages par défaut seront utilisés.\nOuvrez le programme en mode débogage pour en savoir plus.");
				if (debogage)
				{printf("Fichier de config invalide:\nDans la ligne débutant par \"%s\", le mot \"%s\" devrait être un nombre.\n\n", ligne, buffer);}
				return 0;
			}
		}
		
		else if (!strcmp(buffer, "police_nombres"))
		{
			buffer = strtok(NULL, " =:;\n");
			if (buffer == NULL || !sscanf(buffer, "%d", &taille_police_nbre_grille))
			{
				erreur(26, "Fichier de réglages invalide.\nLes réglages par défaut seront utilisés.\nOuvrez le programme en mode débogage pour en savoir plus.");
				if (debogage)
				{printf("Fichier de config invalide:\nDans la ligne débutant par \"%s\", le mot \"%s\" devrait être un nombre.\n\n", ligne, buffer);}
				return 0;
			}
		}
		
		else if (!strcmp(buffer, "symbole_modifier"))
		{
			buffer = strtok(NULL, " =:;\n");
			if (buffer == NULL)
			{
				erreur(26, "Fichier de réglages invalide.\nLes réglages par défaut seront utilisés.\nOuvrez le programme en mode débogage pour en savoir plus.");
				if (debogage)
				{printf("Fichier de config invalide:\nIl manque une valeur à l'identifiant \"%s\" (seul l'identifiant est détecté).\n\n", ligne);}
				return 0;
			}
			strcpy(fichier_symbole_modifier, buffer);
		}
		
		else if (!strcmp(buffer, "symbole_taille_police"))
		{
			buffer = strtok(NULL, " =:;\n");
			if (buffer == NULL)
			{
				erreur(26, "Fichier de réglages invalide.\nLes réglages par défaut seront utilisés.\nOuvrez le programme en mode débogage pour en savoir plus.");
				if (debogage)
				{printf("Fichier de config invalide:\nIl manque une valeur à l'identifiant \"%s\" (seul l'identifiant est détecté).\n\n", ligne);}
				return 0;
			}
			strcpy(fichier_symbole_taille_police, buffer);
		}
		
		else if (!strcmp(buffer, "symbole_orientation"))
		{
			buffer = strtok(NULL, " =:;\n");
			if (buffer == NULL)
			{
				erreur(26, "Fichier de réglages invalide.\nLes réglages par défaut seront utilisés.\nOuvrez le programme en mode débogage pour en savoir plus.");
				if (debogage)
				{printf("Fichier de config invalide:\nIl manque une valeur à l'identifiant \"%s\" (seul l'identifiant est détecté).\n\n", ligne);}
				return 0;
			}
			strcpy(fichier_symbole_orientation, buffer);
		}
		
		else if (!strcmp(buffer, "delai_validation"))
		{
			buffer = strtok(NULL, " =:;\n");
			if (buffer == NULL || !sscanf(buffer, "%d", &delai_validation_mot))
			{
				erreur(26, "Fichier de réglages invalide.\nLes réglages par défaut seront utilisés.\nOuvrez le programme en mode débogage pour en savoir plus.");
				if (debogage)
				{printf("Fichier de config invalide:\nDans la ligne débutant par \"%s\", le mot \"%s\" devrait être un nombre.\n\n", ligne, buffer);}
				return 0;
			}
		}
		
		else if (!strcmp(buffer, "log_erreurs"))
		{
			buffer = strtok(NULL, " =:;\n");
			if (buffer == NULL || (strcmp(buffer, "0") != 0 && strcmp(buffer, "1") != 0))
			{
				erreur(26, "Fichier de réglages invalide.\nLes réglages par défaut seront utilisés.\nOuvrez le programme en mode débogage pour en savoir plus.");
				if (debogage)
				{printf("Fichier de config invalide:\nDans la ligne débutant par \"%s\", le mot \"%s\" devrait être un nombre.\n\n", ligne, buffer);}
				return 0;
			}
			if (!strcmp(buffer, "0"))
			{errlog = 0;}
			else
			{errlog = 1;}
		}
	}
	
	fclose(fconfig);
	return 1;
}