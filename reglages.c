#include "aide.c"

#define NBRE_PARAM 30


struct parametre
//Structure d'un paramètre modifiable des réglages.
{
	char nom[60]; //nom du paramètre (affiché sur le bouton)
	char descr[650]; //"courte" description (affichée dans le pop-up de modification) (NOTE: Les couleurs ne PEUVENT PAS avoir de description)
	char type; //type de paramètre/variable: i = int, b = bool, c = char (non-implanté), s = str, C = couleur, t = titre de section (pas un paramètre), '0' = espace vide (gap) (pas un paramètre)
	int* ptr_int; //pour les int et les bool
	char* ptr_str; //pour les str et les char
	SDL_Color* ptr_couleur; //pour les couleurs
};


struct parametre param[NBRE_PARAM] =
//Liste des paramètres (incluant les sous-titres et les espaces vides).
//Les paramètres sont affichées en colonnes de 10 paramètres par colonne.
{
		{"Personnalisation de la grille:", "-", 't', NULL, NULL, NULL},
	{"couleur de la grille", "-", 'C', NULL, NULL, &couleur_grille},
	{"couleur des cases valides", "-", 'C', NULL, NULL, &couleur_valide},
	{"couleur des cases invalides", "-", 'C', NULL, NULL, &couleur_invalide},
	{"couleur de l'ombrage", "-", 'C', NULL, NULL, &couleur_ombre},
	{"couleur du survol de souris", "-", 'C', NULL, NULL, &couleur_focus},
	
		{"Symboles et icones:", "-", 't', NULL, NULL, NULL},
	{"symbole de modification", "Vous pouvez ici remplacer le symbole de modification du titre d'une grille, utilisé lors de la création de celle-ci.\nVous devez fournir un chemin d'accès valide à une fichier PNG contenant le nouveau symbole.\nLe chemin d'accès doit être écrit selon la convention Linux (même si le programme est exécuté sous Windows).\nN'oubliez pas l'extension \".png\".\nIl est recommandé d'utiliser les mêmes dimensions que le symbole original. Un fichier n'ayant pas les mêmes proportions ou dimensions que le fichier original sera étiré ou compressé.", 's', NULL, fichier_symbole_modifier, NULL},
	{"symbole de modification de la taille de la police", "Vous pouvez ici remplacer le symbole de modification de la taille de la police du texte et des nombres dans les grilles.\nVous devez fournir un chemin d'accès valide à une fichier PNG contenant le nouveau symbole.\nLe chemin d'accès doit être écrit selon la convention Linux (même si le programme est exécuté sous Windows).\nN'oubliez pas l'extension \".png\".\nIl est recommandé d'utiliser les mêmes dimensions que le symbole original. Un fichier n'ayant pas les mêmes proportions ou dimensions que le fichier original sera étiré ou compressé.", 's', NULL, fichier_symbole_taille_police, NULL},
	{"symbole d'inversion de l'orientation", "Vous pouvez ici remplacer le symbole d'inversion de l'orientation de l'entrée de texte dans la grille.\nVous devez fournir un chemin d'accès valide à une fichier PNG contenant le nouveau symbole.\nLe chemin d'accès doit être écrit selon la convention Linux (même si le programme est exécuté sous Windows).\nN'oubliez pas l'extension \".png\".\nIl est recommandé d'utiliser les mêmes dimensions que le symbole original. Un fichier n'ayant pas les mêmes proportions ou dimensions que le fichier original sera étiré ou compressé.", 's', NULL, fichier_symbole_orientation, NULL},
	
		{"Affichage du texte:", "-", 't', NULL, NULL, NULL},
	{"couleur du texte", "-", 'C', NULL, NULL, &couleur_texte},
	{"police", "Vous pouvez changer ici la police utilisée par l'application.\nPour ce faire, vous devez fournir un fichier \".ttf\" contenant les informations de cette police.\nPlusieurs fichiers de différentes polices sont déjà fournis avec ce programme. Vous pouvez les consulter dans le dossier \"source\" du programme.\nVous pouvez donc remplacer le fichier \".ttf\" actuel par un de ceux-là ou un autre.\nAssurez-vous de fournir un chemin d'accès valide selon la convention Linux (même si vous utilisez la version Windows).", 's', NULL, nom_police_principale, NULL},
	{"taille de la police principale", "Vous pouvez modifier ici la taille de la police utilisée pour les boutons, les titres, la légende de la grille, etc.\nLa taille par défaut est 20 pts.\nVeuillez noter que la taille varira selon la police utilisée.", 'i', &taille_police_principale, NULL, NULL},
	{"taille de la petite police", "Vous pouvez modifier ici la taille de la police utilisée pour ce texte et quelques autres explications.\nLa taille par défaut est 18 pts.\nVeuillez noter que la taille varira selon la police utilisée.", 'i', &taille_petite_police, NULL, NULL},
	{"taille de la police du texte dans la grille", "Vous pouvez modifier ici la taille du texte affiché dans une grille de mots croisés.\nLa taille par défaut est 30 pts.\nVeuillez noter que la taille varira selon la police utilisée.\nCe réglage permet aussi de régler la taille de ce texte sans modifier celle des nombres et des lettres indiquant le début des mots dans la grille, contrairement au bouton présent en dessous de la grille (qui ne sauvegarde pas non-plus vos modifications). (Utiliser ce bouton brisera cette indépendance.)", 'i', &taille_police_grille, NULL, NULL},
	{"taille de la police des nombres dans la grille", "Vous pouvez modifier ici la taille des lettres et des nombres servant à indiquer le début des mots dans une grille de mots croisés.\nLa taille par défaut est 20 pts.\nVeuillez noter que la taille varira selon la police utilisée.\nCe réglage permet aussi de régler cette taille sans modifier celle du texte dans la grille, contrairement au bouton présent en dessous de la grille (qui ne sauvegarde pas non-plus vos modifications). (Utiliser ce bouton brisera cette indépendance et effacera votre modification jusqu'à la fermeture du programme.)", 'i', &taille_police_nbre_grille, NULL, NULL},
		
		{"", "VIDE", '0', NULL, NULL, NULL},
		{"", "VIDE", '0', NULL, NULL, NULL},
		{"", "VIDE", '0', NULL, NULL, NULL},
	
		{"Autres paramètres:", "-", 't', NULL, NULL, NULL},
	{"durée du délai de validation d'un mot", "Lorsque vous validez un mot (en cliquant sur le bouton \"vérifier\" en bas de la grille, lors d'une partie), les cases correctes s'affichent en vert et les cases incorrectes en rouge-orangé pour une certaine durée, avant de revenir à la normale.\nVous pouvez ici modifier cette durée (en millisecondes).", 'i', &delai_validation_mot, NULL, NULL},
	{"couleur de l'arrière-plan", "-", 'C', NULL, NULL, &fond},
	{"couleur des boutons", "-", 'C', NULL, NULL, &couleur_boutons},
	{"couleur de la sélection", "-", 'C', NULL, NULL, &couleur_selection},
	{"largeur initiale de la fenêtre", "Vous pouvez modifier ici la largeur que devrait avoir la fenêtre de l'application à son ouverture.\nNotez que cela n'affectera pas la taille atuelle de la fenêtre et qu'il est présentement impossible de faire démarrer le programme avec une fenêtre maximisée.\nEn tous temps, la largeur de la fenêtre ne peut pas être inférieure à 800.\nToutes les dimensions sont en pixels (tels que vus par le programme).", 'i', &largeur_fenetre, NULL, NULL},
	{"hauteur initiale de la fenêtre", "Vous pouvez modifier ici la hauteur que devrait avoir la fenêtre de l'application à son ouverture.\nNotez que cela n'affectera pas la taille atuelle de la fenêtre et qu'il est présentement impossible de faire démarrer le programme avec une fenêtre maximisée.\nEn tous temps, la hauteur de la fenêtre ne peut pas être inférieure à 600.\nToutes les dimensions sont en pixels (tels que vus par le programme).", 'i', &hauteur_fenetre, NULL, NULL},
	
		{"Paramètres avancés:", "-", 't', NULL, NULL, NULL},
	{"choix du sélecteur de couleur", "Cette application utilise une commande externe pour modifier une couleur. Vous pouvez changer cette commande ici.\nSi celle-ci n'est pas accessible, vous ne pourrez pas modifier de couleur via les réglages (et l'application pourrait crasher).\nPar défaut, cette commande est \"zenity --color-selection\", disponible sur la plupart des distributions Linux (\"sudo apt install zenity\" pour l'installer sur un système de type Debian/Ubuntu).\nSinon, vous pouvez modifier vous-même les valeurs rgba des couleurs dans le fichier de réglage (normalement nommé \"reglages.txt\" et placé dans le dossier \"source\" du programme).", 's', NULL, color_picker, NULL},
	{"logguer les erreurs dans un fichier", "Par défaut, le programme laisse une trace de toutes ses erreurs dans un fichier nommé \"erreurs.txt\" situé au même endroit que le programme.\nCe réglage permet de désactiver cette fonctionnalité.\nVeuillez noter que cela n'empêchera pas les pop-ups d'erreurs d'apparaître à l'écran.\nSi vous utilisez cette application sous Linux et que vous voulez désactiver cette fonctionnalité parce qu'elle crée des fichiers d'erreurs au mauvais endroit,\nassurez-vous plutôt de toujours ouvrir l'application via le script \"run.sh\", situé au même endroit que l'application.\nDésactiver cette fonctionnalité n'est pas recommandé.", 'b', (int*) &errlog, NULL, NULL},
	//Ne pas oublier de mettre le NBRE_PARAM à jour si un paramètre (ou un titre de section) est ajouté!
};

int nbre_col = 1; //nombre de colonnes affichées à l'écran
int num_col = 0; //indique quelle colonne est présentement affichée (ne s'applique pas si la fenêtre est assez grande pour tout afficher)


void reglages ()
//Gère les réglages de l'application.
{
	SDL_Event ev;	
	bool redessiner = false;
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
			else if (ev.motion.x >= xmax - 210 && ev.motion.x <= xmax - 20 && ev.motion.y >= ymax - 155 && ev.motion.y <= ymax - 115 && nbre_col < NBRE_PARAM / 10)
			{rafraichir_reglages('+');}
			else if (ev.motion.x >= 50 && ev.motion.x <= 550 && ev.motion.y >= 50 && ev.motion.y <= 550) //1ère colonne
			{
				redessiner = true;
				for (int compteur = 0; compteur < 10; compteur++)
				{
					if (ev.motion.y >= 50 + compteur * 50 && ev.motion.y <= 90 + compteur * 50)
					{rafraichir_reglages(compteur + 10); redessiner = false;}
				}
			}
			else if (ev.motion.x >= 600 && ev.motion.x <= 1100 && ev.motion.y >= 50 && ev.motion.y <= 550) //2e colonne
			{
				redessiner = true;
				for (int compteur = 0; compteur < 10; compteur++)
				{
					if (ev.motion.y >= 50 + compteur * 50 && ev.motion.y <= 90 + compteur * 50)
					{rafraichir_reglages(compteur + 20); redessiner = false;}
				}
			}
			else if (ev.motion.x >= 1150 && ev.motion.x <= 1650 && ev.motion.y >= 50 && ev.motion.y <= 550) //3e colonne
			{
				redessiner = true;
				for (int compteur = 0; compteur < 10; compteur++)
				{
					if (ev.motion.y >= 50 + compteur * 50 && ev.motion.y <= 90 + compteur * 50)
					{rafraichir_reglages(compteur + 30); redessiner = false;}
				}
			}
			else
			{rafraichir_reglages(0);}
			if (redessiner)
			{rafraichir_reglages(0); redessiner = false;}
			break;
		
		case SDL_MOUSEBUTTONDOWN:
			if (ev.button.x >= xmax - 210 && ev.button.x <= xmax - 20 && ev.button.y >= ymax - 55 && ev.button.y <= ymax - 15) //Terminé
			{enregistrer_reglages(); appliquer_reglages();} //le programme va redémarrer ici
			else if (ev.button.x >= xmax - 210 && ev.button.x <= xmax - 20 && ev.button.y >= ymax - 105 && ev.button.y <= ymax - 65) //Réinitialiser
			{reinitialiser_reglages();}
			else if (ev.button.x >= xmax - 210 && ev.button.x <= xmax - 20 && ev.button.y >= ymax - 155 && ev.button.y <= ymax - 115 && nbre_col < NBRE_PARAM / 10) //Plus d'options
			{
				if (num_col == NBRE_PARAM / 10 - nbre_col)
				{num_col = 0;}
				else
				{num_col++;}
				rafraichir_reglages('+');
			}
			else if (ev.button.x >= 50 && ev.button.x <= 550 && ev.button.y >= 50 && ev.button.y <= 550) //1ère colonne
			{
				for (int compteur = 0; compteur < 10; compteur++)
				{
					if (ev.button.y >= 50 + compteur * 50 && ev.button.y <= 90 + compteur * 50)
					{
						if (param[compteur + num_col * 10].type == 'i') //int
						{
							buffint = demander_nbre(param[compteur + num_col * 10].nom, param[compteur + num_col * 10].descr, *param[compteur + num_col * 10].ptr_int, fenetre);
							if (buffint == -6699) {} //annulé
							else if (buffint <= 0)
							{SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Valeur négative ou nulle", "Les valeurs négatives et nulles ne sont pas acceptées.\nVeuillez réessayer.", fenetre);}
							else
							{*param[compteur + num_col * 10].ptr_int = buffint;}
						}
						else if (param[compteur + num_col * 10].type == 'b') //bool
						{
							SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Informations sur le paramètre", param[compteur + num_col * 10].descr, fenetre);
							if (*((bool*) param[compteur + num_col * 10].ptr_int))
							{*((bool*) param[compteur + num_col * 10].ptr_int) = false;}
							else
							{*((bool*) param[compteur + num_col * 10].ptr_int) = true;}
							rafraichir_reglages(compteur);
						}
						else if (param[compteur + num_col * 10].type == 's') //string
						{demander_txt(param[compteur + num_col * 10].nom, param[compteur + num_col * 10].descr, param[compteur + num_col * 10].ptr_str, 200, fenetre);}
						else if (param[compteur + num_col * 10].type == 'C') //couleur
						{demander_couleur(param[compteur + num_col * 10].nom, param[compteur + num_col * 10].ptr_couleur, fenetre);}
					}
				}
			}
			else if (ev.button.x >= 600 && ev.button.x <= 1100 && ev.button.y >= 50 && ev.button.y <= 550) //2e colonne
			{
				for (int compteur = 0; compteur < 10; compteur++)
				{
					if (ev.button.y >= 50 + compteur * 50 && ev.button.y <= 90 + compteur * 50)
					{
						if (param[compteur + (num_col + 1) * 10].type == 'i') //int
						{
							buffint = demander_nbre(param[compteur + (num_col + 1) * 10].nom, param[compteur + (num_col + 1) * 10].descr, *param[compteur + (num_col + 1) * 10].ptr_int, fenetre);
							if (buffint == -6699) {} //annulé
							else if (buffint <= 0)
							{SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Valeur négative ou nulle", "Les valeurs négatives et nulles ne sont pas acceptées.\nVeuillez réessayer.", fenetre);}
							else
							{*param[compteur + (num_col + 1) * 10].ptr_int = buffint;}
						}
						else if (param[compteur + (num_col + 1) * 10].type == 'b') //bool
						{
							SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Informations sur le paramètre", param[compteur + (num_col + 1) * 10].descr, fenetre);
							if (*((bool*) param[compteur + (num_col + 1) * 10].ptr_int))
							{*((bool*) param[compteur + (num_col + 1) * 10].ptr_int) = false;}
							else
							{*((bool*) param[compteur + (num_col + 1) * 10].ptr_int) = true;}
							rafraichir_reglages(compteur);
						}
						else if (param[compteur + (num_col + 1) * 10].type == 's') //string
						{demander_txt(param[compteur + (num_col + 1) * 10].nom, param[compteur + (num_col + 1) * 10].descr, param[compteur + (num_col + 1) * 10].ptr_str, 200, fenetre);}
						else if (param[compteur + (num_col + 1) * 10].type == 'C') //couleur
						{demander_couleur(param[compteur + num_col * 10].nom, param[compteur + (num_col + 1) * 10].ptr_couleur, fenetre);}
					}
				}
			}
			else if (ev.button.x >= 1150 && ev.button.x <= 1650 && ev.button.y >= 50 && ev.button.y <= 550) //3e colonne
			{
				for (int compteur = 0; compteur < 10; compteur++)
				{
					if (ev.button.y >= 50 + compteur * 50 && ev.button.y <= 90 + compteur * 50)
					{
						if (param[compteur + (num_col + 2) * 10].type == 'i') //int
						{
							buffint = demander_nbre(param[compteur + (num_col + 2) * 10].nom, param[compteur + (num_col + 2) * 10].descr, *param[compteur + (num_col + 2) * 10].ptr_int, fenetre);
							if (buffint == -6699) {} //annulé
							else if (buffint <= 0)
							{SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Valeur négative ou nulle", "Les valeurs négatives et nulles ne sont pas acceptées.\nVeuillez réessayer.", fenetre);}
							else
							{*param[compteur + (num_col + 2) * 10].ptr_int = buffint;}
						}
						else if (param[compteur + (num_col + 2) * 10].type == 'b') //bool
						{
							SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Informations sur le paramètre", param[compteur + (num_col + 2) * 10].descr, fenetre);
							if (*((bool*) param[compteur + (num_col + 2) * 10].ptr_int))
							{*((bool*) param[compteur + (num_col + 2) * 10].ptr_int) = false;}
							else
							{*((bool*) param[compteur + (num_col + 2) * 10].ptr_int) = true;}
							rafraichir_reglages(compteur);
						}
						else if (param[compteur + (num_col + 2) * 10].type == 's') //string
						{demander_txt(param[compteur + (num_col + 2) * 10].nom, param[compteur + (num_col + 2) * 10].descr, param[compteur + (num_col + 2) * 10].ptr_str, 200, fenetre);}
						else if (param[compteur + (num_col + 2) * 10].type == 'C') //couleur
						{demander_couleur(param[compteur + num_col * 10].nom, param[compteur + (num_col + 2) * 10].ptr_couleur, fenetre);}
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
	char buffer[100] = "ERREUR";
	int col = 0; //indique la colonne présentement dessinée (utilisé dans la loop pour afficher les boutons uniquement)
	nbre_col = xmax / 575; //nombre de colonnes affichées à l'écran
	
	
	//Ajustement du nombre de colonnes si les boutons du bas à droite embarqueraient dessus:
	if (xmax < nbre_col * 575 + 210 && ((ymax - 195 < 550 && nbre_col < NBRE_PARAM / 10) || ymax - 115 < 550))
	{nbre_col--;}
	
	//Ajustement du numéro de colonnes si la logique essaie de nous faire afficher une colonne de plus que le véritable nombre total de colonnes:
	while (num_col + nbre_col > NBRE_PARAM / 10) //while parce que parfois, il faut faire ça plus qu'une fois...
	{num_col--;}
	
	//Remplissage de l'arrière-plan:
	SDL_SetColor(fond, rend);
	SDL_RenderClear(rend);
	
	//Affichage du titre:
	TTF_SetFontStyle(police, TTF_STYLE_BOLD);
	afficher_txt_centre("Réglages", 0, xmax, 10, police, couleur_texte, rend);
	TTF_SetFontStyle(police, TTF_STYLE_NORMAL);
	
	//Affichage des boutons des paramètres:
	for (int compteur = 0; compteur < 10 * nbre_col; compteur++)
	{
		if (param[compteur + num_col * 10].type != 't' && param[compteur + num_col * 10].type != '0')
		{
			if (zone == compteur + 10)
			{rect_arrondi(50 + col * 550, 50 + compteur * 50 - col * 500, 500, 40, couleur_selection, fond, rend);}
			else
			{rect_arrondi(50 + col * 550, 50 + compteur * 50 - col * 500, 500, 40, couleur_boutons, fond, rend);}
		}
		if (param[compteur + num_col * 10].type != '0')
		{afficher_txt(param[compteur + num_col * 10].nom, 60 + col * 550, 60 + compteur * 50 - col * 500, 500, police, couleur_texte, rend);}
		if (param[compteur + num_col * 10].type == 'b')
		{
			if (*((bool*)param[compteur + num_col * 10].ptr_int))
			{afficher_txt("ON", 500 + col * 550, 60 + compteur * 50 - col * 500, 50, police, couleur_texte, rend);}
			else
			{afficher_txt("OFF", 500 + col * 550, 60 + compteur * 50 - col * 500, 50, police, couleur_texte, rend);}
		}
		if ((compteur + 1) % 10 == 0 && compteur + 1 != 0)
		{col++;}
	}
	
	//Affichage du bouton "plus d'options":
	if (nbre_col < NBRE_PARAM / 10)
	{
		if (zone == '+')
		{rect_arrondi(xmax - 210, ymax - 155, 190, 40, couleur_selection, fond, rend);}
		else
		{rect_arrondi(xmax - 210, ymax - 155, 190, 40, couleur_boutons, fond, rend);}
		afficher_txt_centre("Plus d'options", xmax - 210, xmax - 20, ymax - 145, police, couleur_texte, rend);
		
		sprintf(buffer, "Page %d / %d", num_col + 1, NBRE_PARAM / 10 - nbre_col + 1);
		afficher_txt_centre(buffer, xmax - 210, xmax - 20, ymax - 185, police, couleur_texte, rend);
	}
	
	//Affichage des boutons "terminé" et "réinitialiser":
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
	
	//Affichage de la version du jeu:
	afficher_txt("CMC Version", 5, ymax - 25, 200, police, couleur_texte, rend);
	afficher_txt(VERSION, afficher_txt("CMC Version", 0, 0, 1000, police, blanc, NULL) + 10, ymax - 25, 100, police, couleur_texte, rend);
	
	//Affichage à l'écran:
	SDL_RenderPresent(rend);
}


void reinitialiser_reglages ()
//Remet tous les réglages à leur valeur par défaut en supprimant fconfig.
//Fait ensuite redémarrer le programme via appliquer_reglages.
{
	char buffer[sizeof(nom_fconfig) + 5];
	
	if (debogage)
	{printf("Suppression du fichier de config (%s)...\n", nom_fconfig);}
	
	sprintf(buffer, "rm %s", nom_fconfig);
	system(buffer);
	appliquer_reglages();
}


void appliquer_reglages ()
//Redémarre l'application pour appliquer les réglages.
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "CMC - Appliquer les paramètres", "L'application va redémarrer pour appliquer vos nouveaux réglages.", fenetre);
	if (debogage)
	{printf("Redémarrage du programme...\n\n"); execl("./cmc", "cmc", "--deboguer", (char*) NULL);}
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
	
	if (debogage)
	{printf("Fichier de config créé (en %s).\n", nom_fconfig);}
	
	fprintf(fconfig, "* Fichier de configuration (réglages) de l'éditeur de mots croisés CMC.\n* \"cmc --aide\" pour plus de détails.\n*\n");
	fprintf(fconfig, "* Ce fichier est généré automatiquement et son contenu est normalement effacé et remplacé à chaque ouverture des réglages de l'application.\n* Modifiez à vos risques et périls.\n\n");
	fprintf(fconfig, "* Si vous voulez être sûr(e) que l'application ne modifiera pas ce fichier, vous pouvez ajouter une ligne avec le paramètre \"modification_manuelle = 1\" (sans les guillemets).\n");
	fprintf(fconfig, "* L'ordre des paramètres n'a pas d'importance.\n");
	fprintf(fconfig, "* Toute ligne précédée d'un * séparé du texte par un espace sera ignorée par le programme.\n* Si vous supprimez un paramètre du fichier, sa valeur par défaut sera utilisée.\n");
	fprintf(fconfig, "* Ne mettez qu'un seul paramètre par ligne.\n\n\n");
	
	fprintf(fconfig, "CMC_Version = %s %s\n\n", VERSION, NOM_VERSION);
	
	fprintf(fconfig, "Dimensions_Fenetre = %d x %d\n\n", largeur_fenetre, hauteur_fenetre);
	fprintf(fconfig, "Police = %s\n", nom_police_principale);
	fprintf(fconfig, "Police_Principale = %d\nPetite_Police = %d\nPolice_Grille = %d\nPolice_Nombres = %d\n\n", taille_police_principale, taille_petite_police, taille_police_grille, taille_police_nbre_grille);
	fprintf(fconfig, "Symbole_Modifier = %s\nSymbole_Taille_Police = %s\nSymbole_Orientation = %s\n\n", fichier_symbole_modifier, fichier_symbole_taille_police, fichier_symbole_orientation);
	fprintf(fconfig, "Delai_Validation = %d\nColor_Picker = %s\nLog_Erreurs = %d\n\n", delai_validation_mot, color_picker, errlog);
	
	fprintf(fconfig, "Couleur_Fond = [%d, %d, %d, %d]\nCouleur_Grille = [%d, %d, %d, %d]\n", fond.r, fond.g, fond.b, fond.a, couleur_grille.r, couleur_grille.g, couleur_grille.b, couleur_grille.a);
	fprintf(fconfig, "Couleur_Texte = [%d, %d, %d, %d]\nCouleur_Boutons = [%d, %d, %d, %d]\n", couleur_texte.r, couleur_texte.g, couleur_texte.b, couleur_texte.a, couleur_boutons.r, couleur_boutons.g, couleur_boutons.b, \
		couleur_boutons.a);
	fprintf(fconfig, "Couleur_Selection = [%d, %d, %d, %d]\nCouleur_Survol = [%d, %d, %d, %d]\n", couleur_selection.r, couleur_selection.g, couleur_selection.b, couleur_selection.a, couleur_focus.r, couleur_focus.g, \
		couleur_focus.b, couleur_focus.a);
	fprintf(fconfig, "Couleur_Ombrage = [%d, %d, %d, %d]\nCouleur_Valide = [%d, %d, %d, %d]\n", couleur_ombre.r, couleur_ombre.g, couleur_ombre.b, couleur_ombre.a, couleur_valide.r, couleur_valide.g, couleur_valide.b, \
		couleur_valide.a);
	fprintf(fconfig, "Couleur_Invalide = [%d, %d, %d, %d]\n", couleur_invalide.r, couleur_invalide.g, couleur_invalide.b, couleur_invalide.a);
	
	fclose(fconfig);
}


bool lire_reglages ()
//Lit les réglages depuis fconfig.
//Utilisé uniquement au démarrage de l'application.
//Renvoie 0 en cas d'échec et 1 en cas de succès (ou si le fichier n'existe pas).
{
	char ligne[500];
	char* buffer;
	Uint8 couleur[4] = {0, 0, 0, 0};
	FILE* fconfig = fopen(nom_fconfig, "r");
	
	if (fconfig == NULL)
	{
		if (debogage)
		{printf("Aucun fichier de réglages trouvé en %s.\nUtilisation des paramètres par défaut.\n", nom_fconfig);}
		return 1;
	}
	
	if (debogage)
	{printf("Ouverture du fichier de config (%s)...\n", nom_fconfig);}
	
	while (fgets(ligne, sizeof(ligne), fconfig) != NULL)
	{
		enlever_majuscule(ligne);
		buffer = strtok(ligne, " =:;,[]\n");
		
		if (buffer == NULL || !strcmp(buffer, "*") || !strcmp(buffer, "cmc_version")) {}
		
		else if (!strcmp(buffer, "dimensions_fenetre"))
		{
			buffer = strtok(NULL, " =:;,[]\n");
			if (buffer == NULL || !sscanf(buffer, "%d", &largeur_fenetre))
			{
				erreur(26, "Fichier de réglages invalide.\nLes réglages par défaut seront utilisés.\nOuvrez le programme en mode débogage pour en savoir plus.");
				if (debogage)
				{printf("Fichier de config invalide:\nDans la ligne débutant par \"%s\", le mot \"%s\" devrait être un nombre.\n\n", ligne, buffer);}
				return 0;
			}
			buffer = strtok(NULL, " =:;,[]\n");
			if (!strcmp(buffer, "x") || !strcmp(buffer, "par") || !strcmp(buffer, "sur"))
			{buffer = strtok(NULL, " =:;,[]\n");}
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
			buffer = strtok(NULL, " =:;,[]\n");
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
			buffer = strtok(NULL, " =:;,[]\n");
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
			buffer = strtok(NULL, " =:;,[]\n");
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
			buffer = strtok(NULL, " =:;,[]\n");
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
			buffer = strtok(NULL, " =:;,[]\n");
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
			buffer = strtok(NULL, " =:;,[]\n");
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
			buffer = strtok(NULL, " =:;,[]\n");
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
			buffer = strtok(NULL, " =:;,[]\n");
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
			buffer = strtok(NULL, " =:;,[]\n");
			if (buffer == NULL || !sscanf(buffer, "%d", &delai_validation_mot))
			{
				erreur(26, "Fichier de réglages invalide.\nLes réglages par défaut seront utilisés.\nOuvrez le programme en mode débogage pour en savoir plus.");
				if (debogage)
				{printf("Fichier de config invalide:\nDans la ligne débutant par \"%s\", le mot \"%s\" devrait être un nombre.\n\n", ligne, buffer);}
				return 0;
			}
		}
		
		else if (!strcmp(buffer, "color_picker"))
		{
			buffer = strtok(NULL, " =:;,[]\n");
			if (buffer == NULL)
			{
				erreur(26, "Fichier de réglages invalide.\nLes réglages par défaut seront utilisés.\nOuvrez le programme en mode débogage pour en savoir plus.");
				if (debogage)
				{printf("Fichier de config invalide:\nIl manque une valeur à l'identifiant \"%s\" (seul l'identifiant est détecté).\n\n", ligne);}
				return 0;
			}
			color_picker[0] = '\000';
			while (buffer != NULL)
			{
				strcat(color_picker, buffer);
				buffer = strtok(NULL, " =:;,[]\n");
				if (buffer != NULL)
				{strcat(color_picker, " ");}
			}
		}
		
		else if (!strcmp(buffer, "log_erreurs"))
		{
			buffer = strtok(NULL, " =:;,[]\n");
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
		
		else if (!strcmp(buffer, "couleur_fond"))
		{
			for (int compteur = 0; compteur < 4; compteur++)
			{
				buffer = strtok(NULL, " =:;,[]\n");
				if (buffer == NULL || !sscanf(buffer, "%hhd", &couleur[compteur]))
				{
					erreur(26, "Fichier de réglages invalide.\nLes réglages par défaut seront utilisés.\nOuvrez le programme en mode débogage pour en savoir plus.");
					if (debogage)
					{printf("Fichier de config invalide:\nDans la ligne débutant par \"%s\", le mot \"%s\" devrait être le %de nombre de la couleur.\n\n", ligne, buffer, compteur);}
					return 0;
				}
			}
			fond.r = couleur[0];
			fond.g = couleur[1];
			fond.b = couleur[2];
			fond.a = couleur[3];
		}
		
		else if (!strcmp(buffer, "couleur_grille"))
		{
			for (int compteur = 0; compteur < 4; compteur++)
			{
				buffer = strtok(NULL, " =:;,[]\n");
				if (buffer == NULL || !sscanf(buffer, "%hhd", &couleur[compteur]))
				{
					erreur(26, "Fichier de réglages invalide.\nLes réglages par défaut seront utilisés.\nOuvrez le programme en mode débogage pour en savoir plus.");
					if (debogage)
					{printf("Fichier de config invalide:\nDans la ligne débutant par \"%s\", le mot \"%s\" devrait être le %de nombre de la couleur.\n\n", ligne, buffer, compteur);}
					return 0;
				}
			}
			couleur_grille.r = couleur[0];
			couleur_grille.g = couleur[1];
			couleur_grille.b = couleur[2];
			couleur_grille.a = couleur[3];
		}
		
		else if (!strcmp(buffer, "couleur_texte"))
		{
			for (int compteur = 0; compteur < 4; compteur++)
			{
				buffer = strtok(NULL, " =:;,[]\n");
				if (buffer == NULL || !sscanf(buffer, "%hhd", &couleur[compteur]))
				{
					erreur(26, "Fichier de réglages invalide.\nLes réglages par défaut seront utilisés.\nOuvrez le programme en mode débogage pour en savoir plus.");
					if (debogage)
					{printf("Fichier de config invalide:\nDans la ligne débutant par \"%s\", le mot \"%s\" devrait être le %de nombre de la couleur.\n\n", ligne, buffer, compteur);}
					return 0;
				}
			}
			couleur_texte.r = couleur[0];
			couleur_texte.g = couleur[1];
			couleur_texte.b = couleur[2];
			couleur_texte.a = couleur[3];
		}
		
		else if (!strcmp(buffer, "couleur_boutons"))
		{
			for (int compteur = 0; compteur < 4; compteur++)
			{
				buffer = strtok(NULL, " =:;,[]\n");
				if (buffer == NULL || !sscanf(buffer, "%hhd", &couleur[compteur]))
				{
					erreur(26, "Fichier de réglages invalide.\nLes réglages par défaut seront utilisés.\nOuvrez le programme en mode débogage pour en savoir plus.");
					if (debogage)
					{printf("Fichier de config invalide:\nDans la ligne débutant par \"%s\", le mot \"%s\" devrait être le %de nombre de la couleur.\n\n", ligne, buffer, compteur);}
					return 0;
				}
			}
			couleur_boutons.r = couleur[0];
			couleur_boutons.g = couleur[1];
			couleur_boutons.b = couleur[2];
			couleur_boutons.a = couleur[3];
		}
		
		else if (!strcmp(buffer, "couleur_selection"))
		{
			for (int compteur = 0; compteur < 4; compteur++)
			{
				buffer = strtok(NULL, " =:;,[]\n");
				if (buffer == NULL || !sscanf(buffer, "%hhd", &couleur[compteur]))
				{
					erreur(26, "Fichier de réglages invalide.\nLes réglages par défaut seront utilisés.\nOuvrez le programme en mode débogage pour en savoir plus.");
					if (debogage)
					{printf("Fichier de config invalide:\nDans la ligne débutant par \"%s\", le mot \"%s\" devrait être le %de nombre de la couleur.\n\n", ligne, buffer, compteur);}
					return 0;
				}
			}
			couleur_selection.r = couleur[0];
			couleur_selection.g = couleur[1];
			couleur_selection.b = couleur[2];
			couleur_selection.a = couleur[3];
		}
		
		else if (!strcmp(buffer, "couleur_survol"))
		{
			for (int compteur = 0; compteur < 4; compteur++)
			{
				buffer = strtok(NULL, " =:;,[]\n");
				if (buffer == NULL || !sscanf(buffer, "%hhd", &couleur[compteur]))
				{
					erreur(26, "Fichier de réglages invalide.\nLes réglages par défaut seront utilisés.\nOuvrez le programme en mode débogage pour en savoir plus.");
					if (debogage)
					{printf("Fichier de config invalide:\nDans la ligne débutant par \"%s\", le mot \"%s\" devrait être le %de nombre de la couleur.\n\n", ligne, buffer, compteur);}
					return 0;
				}
			}
			couleur_focus.r = couleur[0];
			couleur_focus.g = couleur[1];
			couleur_focus.b = couleur[2];
			couleur_focus.a = couleur[3];
		}
		
		else if (!strcmp(buffer, "couleur_ombrage"))
		{
			for (int compteur = 0; compteur < 4; compteur++)
			{
				buffer = strtok(NULL, " =:;,[]\n");
				if (buffer == NULL || !sscanf(buffer, "%hhd", &couleur[compteur]))
				{
					erreur(26, "Fichier de réglages invalide.\nLes réglages par défaut seront utilisés.\nOuvrez le programme en mode débogage pour en savoir plus.");
					if (debogage)
					{printf("Fichier de config invalide:\nDans la ligne débutant par \"%s\", le mot \"%s\" devrait être le %de nombre de la couleur.\n\n", ligne, buffer, compteur);}
					return 0;
				}
			}
			couleur_ombre.r = couleur[0];
			couleur_ombre.g = couleur[1];
			couleur_ombre.b = couleur[2];
			couleur_ombre.a = couleur[3];
		}
		
		else if (!strcmp(buffer, "couleur_valide"))
		{
			for (int compteur = 0; compteur < 4; compteur++)
			{
				buffer = strtok(NULL, " =:;,[]\n");
				if (buffer == NULL || !sscanf(buffer, "%hhd", &couleur[compteur]))
				{
					erreur(26, "Fichier de réglages invalide.\nLes réglages par défaut seront utilisés.\nOuvrez le programme en mode débogage pour en savoir plus.");
					if (debogage)
					{printf("Fichier de config invalide:\nDans la ligne débutant par \"%s\", le mot \"%s\" devrait être le %de nombre de la couleur.\n\n", ligne, buffer, compteur);}
					return 0;
				}
			}
			couleur_valide.r = couleur[0];
			couleur_valide.g = couleur[1];
			couleur_valide.b = couleur[2];
			couleur_valide.a = couleur[3];
		}
		
		else if (!strcmp(buffer, "couleur_invalide"))
		{
			for (int compteur = 0; compteur < 4; compteur++)
			{
				buffer = strtok(NULL, " =:;,[]\n");
				if (buffer == NULL || !sscanf(buffer, "%hhd", &couleur[compteur]))
				{
					erreur(26, "Fichier de réglages invalide.\nLes réglages par défaut seront utilisés.\nOuvrez le programme en mode débogage pour en savoir plus.");
					if (debogage)
					{printf("Fichier de config invalide:\nDans la ligne débutant par \"%s\", le mot \"%s\" devrait être le %de nombre de la couleur.\n\n", ligne, buffer, compteur);}
					return 0;
				}
			}
			couleur_invalide.r = couleur[0];
			couleur_invalide.g = couleur[1];
			couleur_invalide.b = couleur[2];
			couleur_invalide.a = couleur[3];
		}
		
		else if (!strcmp(buffer, "modification_manuelle") || !strcmp(buffer, "modifications_manuelles") || !strcmp(buffer, "reglages_bloques"))
		{
			buffer = strtok(NULL, " =:;,[]\n");
			if (buffer == NULL || (strcmp(buffer, "0") != 0 && strcmp(buffer, "1") != 0))
			{
				erreur(26, "Fichier de réglages invalide.\nLes réglages par défaut seront utilisés.\nOuvrez le programme en mode débogage pour en savoir plus.");
				if (debogage)
				{printf("Fichier de config invalide:\nDans la ligne débutant par \"%s\", le mot \"%s\" devrait être 0 ou 1.\n\n", ligne, buffer);}
				return 0;
			}
			if (!strcmp(buffer, "0"))
			{reglages_bloques = 0;}
			else
			{
				reglages_bloques = 1;
				if (debogage)
				{printf("Réglages bloqués. Il y a quelqu'un qui sait qu'est-ce qu'il fait...\n");}
			}
		}
		
		else
		{
			erreur(102, "Entrée invalide dans le fichier de réglages.\nCette entrée sera ignorée.");
			if (debogage)
			{printf("Entrée invalide dans le fichier de config:\n\"%s\" n'est pas un identifiant valide.\nCette ligne sera ignorée.\n\n", ligne);}
		}
	}
	
	if (debogage)
	{printf("Fermeture du fichier de config...\n\n");}
	
	fclose(fconfig);
	return 1;
}