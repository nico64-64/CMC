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
	afficher_txt_centre(titre, 0, xmax, 5, police, noir, rend);
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
		for (int compteur = 0; compteur < nbre_cases; compteur++)
		{rectangle(marge_gauche + largeur_case * compteur, 40 + largeur_case * selection_y, largeur_case, largeur_case, 0, couleur_ombre, fond, rend);}
	}
	else if (selection_x >= 0)
	{
		for (int compteur = 0; compteur < nbre_cases; compteur++)
		{rectangle(marge_gauche + largeur_case * selection_x, 40 + largeur_case * compteur, largeur_case, largeur_case, 0, couleur_ombre, fond, rend);}
	}
	
	//Case sélectionnée:
	if (zone == sur_grille || zone == nouv_sur_grille)
	{rectangle(marge_gauche + largeur_case * focus_x, 40 + largeur_case * focus_y, largeur_case, largeur_case, 0, couleur_focus, fond, rend);}
	if (selection_x >= 0 && selection_y >= 0)
	{rectangle(marge_gauche + largeur_case * selection_x, 40 + largeur_case * selection_y, largeur_case, largeur_case, 0, couleur_selection, fond, rend);}
	
	//Grille:
	rectangle(marge_gauche, 40, largeur_grille, largeur_grille, 5, noir, fond, rend);
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
				else if (grille[a][b].essai[0] != 0)
				{afficher_txt_centre(grille[a][b].essai, marge_gauche + largeur_case * a, marge_gauche + largeur_case * (a - 1), 40 + largeur_case * b + 10, police_grille, couleur_texte, rend);}
			}
		}
	}
	
	//Légende:
	TTF_SetFontStyle(police, TTF_STYLE_UNDERLINE);
	afficher_txt("Horizontalement:", marge_droite, 40, xmax - marge_droite - 20, police, noir, rend);
	afficher_txt("Verticalement:", marge_droite, 300, xmax - marge_droite - 20, police, noir, rend);
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
	{afficher_txt_centre("Vérifier", marge_gauche + 140, marge_gauche + 250, marge_bas + 10, police, couleur_texte, rend);}
	
	rect_arrondi(marge_gauche + 330, marge_bas, 50, 40, couleur_boutons, fond, rend);
	if (zone == nouv_mod_taille_police || zone == mod_taille_police)
	{rect_arrondi(marge_gauche + 330, marge_bas, 50, 40, couleur_selection, fond, rend);}
	SDL_RenderCopy(rend, symbole_taille_police, NULL, &rect_symbole_taille_police);
	
	rect_arrondi(marge_gauche + 400, marge_bas, 110, 40, couleur_boutons, fond, rend);
	if (zone == nouv_terminer || zone == terminer)
	{rect_arrondi(marge_gauche + 400, marge_bas, 110, 40, couleur_selection, fond, rend);}
	afficher_txt_centre("Terminer", marge_gauche + 400, marge_gauche + 510, marge_bas + 10, police, couleur_texte, rend);
	
	SDL_RenderPresent(rend);
}