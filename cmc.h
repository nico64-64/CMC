#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>


#define VERSION "0.4" //numéro "officiel"
#define NOM_VERSION "LETTRES DÉFORMÉES" //surnom amusant =)

//Dimensions minimales de la fenêtre:
#define LARGEUR_MIN	800 //largeur minimale de la fenêtre
#define HAUTEUR_MIN	600 //hauteur minimale de la fenêtre

//Définitions facilitant la lisibilité du code:
#define VERTICAL	1
#define HORIZONTAL	0


//Structure d'une case de la grille de mots croisés:
struct _case
{
	bool vide; //indique si la case doit rester vide
	char solution[3]; //lettre qui va dans cette case (0 = aucune lettre)
	char essai[3]; //lettre entrée par le joueur (0 = aucune lettre)
	char validation; //indique si la réponse entrée par le joueur est la bonne (0 = ne s'applique pas, 'v' = valide, 'i' = invalide)
};

//Structure d'un mot inscrit dans la grille de mots croisés:
typedef struct mot
{
	struct mot* prec; //mot précédent
	int x; //coordonnée x de la case où débute le mot
	int y; //coordonnée y de la case où débute le mot
	char mot[20]; //mot recherché (à inscrire dans la grille de mots croisés)
	int nbre_car; //nombre de caractères que contient le mot
	bool orientation; //utiliser HORIZONTAL ou VERTICAL (définis plus tôt)
	char legende[300]; //description du mot affichée dans la légende
	struct mot* suiv; //mot suivant
} mot;

//Zones de la fenêtre affichant la grille:
enum zones
{
	//jeu:
	//position inconnue = 0,
	sur_grille = 1,
	annuler,
	verifier,
	terminer,
	mod_orientation,
	mod_taille_police,
	verification,
	verification_finale,
	
	//création:
	nouv_grille = 100, //position inconnue
	nouv_sur_grille,
	nouv_modifier_titre,
	nouv_annuler,
	nouv_identifier,
	nouv_terminer,
	nouv_mod_orientation,
	nouv_mod_taille_police
};

enum zones_menu
{
	//0 = indéterminé
	creer = 1,
	jouer,
	ouvrir_aide,
	ouvrir_reglages
};


//Palette de couleurs par défaut:
const SDL_Color transparent = {0, 0, 0, 0};
const SDL_Color noir = {0, 0, 0, 255};
const SDL_Color blanc = {255, 255, 255, 255};
const SDL_Color gris = {167, 170, 170, 255};
const SDL_Color gris_fonce = {108, 112, 116, 255};
const SDL_Color gris_pale = {209, 209, 209, 255};
const SDL_Color bleu = {36, 128, 206, 255};
const SDL_Color bleu_efface = {121, 187, 243, 100};
const SDL_Color vert_pale = {172, 212, 157, 255};
const SDL_Color jaune_pale = {252, 255, 145, 255};
const SDL_Color jaune_orange = {255, 229, 0, 255};
const SDL_Color orange = {255, 176, 21, 255};
const SDL_Color orange_fonce = {255, 148, 25, 255};
const SDL_Color rouge = {255, 95, 51, 255};
const SDL_Color rouge_fonce = {172, 26, 26, 255};
const SDL_Color rouge_tres_fonce = {80, 11, 11, 255};

//Couleurs utilisées (dynamiques):
SDL_Color fond = blanc; //arrière-plan
SDL_Color couleur_grille = noir; //contours de la grille et de ses cases
SDL_Color couleur_texte = noir; //texte dans la grille (et ailleurs...)
SDL_Color couleur_boutons = gris_pale; //boutons cliquables
SDL_Color couleur_focus = bleu_efface; //focus souris (hovering)
SDL_Color couleur_selection = bleu; //sélection clavier ou clic de souris
SDL_Color couleur_ombre = gris_pale; //ombrage pour le reste du mot sur la grille
SDL_Color couleur_valide = vert_pale; //case ayant la bonne lettre
SDL_Color couleur_invalide = orange_fonce; //case n'ayant pas la bonne lettre

//Valeurs modifiables:
int  largeur_fenetre = 900; //valeur par défaut
int  hauteur_fenetre = 700; //valeur par défaut
int  xmax = 900; //valeur variable
int  ymax = 700; //valeur variable
int  marge_gauche; //coordonnée x où commence la grille
int  marge_droite; //coordonnée x 20 pixels à droite de la grille (où commencent les boutons)
int  marge_bas; //coordonnée y 20 pixels en bas de la grille (où commencent les boutons)  /!\ Vaut zéro s'il manque de place en bas /!\ .
int  largeur_grille; //largeur (et hauteur) de la grille
int  largeur_case; //largeur (et hauteur) d'une case de la grille
char nom_police_principale[40] = "./source/dejavu_sans.ttf";
int  taille_police_principale = 20;
int  taille_petite_police = 18;
int  taille_police_grille = 30;
int  taille_police_nbre_grille = 20;
char fichier_symbole_modifier[40] = "./source/edit.png";
char fichier_symbole_taille_police[40] = "./source/taille_police.png";
char fichier_symbole_orientation[40] = "./source/orientation.png";
int  focus_x = 0; //indique la coordonnée x ayant en ce moment le focus souris
int  focus_y = 0; //indique la coordonnée y ayant en ce moment le focus souris
int  selection_x = -1; //indique la coordonnée x qui est sélectionnée en ce moment (via clavier ou clic de souris)
int  selection_y = -1; //indique la coordonnée y qui est sélectionnée en ce moment (via clavier ou clic de souris)
bool orientation = HORIZONTAL; //indique l'orientation actuelle dans la grille
int  delai_validation_mot = 1500; //délai (en ms) pendant lequel la validation du mot est visible à l'écran
char nom_fconfig[40] = "./source/reglages.txt"; //nom et chemin d'accès au fichier de sauvegarde des réglages
char color_picker[150] = "zenity --color-selection"; //commande du color-picker
bool ne_pas_afficher = false; //indique aux fonction d'affichage de ne pas renderer à l'écran (utilisé par les fonctions d'aide)

//Gestion des erreurs, débogage, etc.:
bool reglages_bloques = false; //impossible de modifier les réglages dans l'application (édition manuelle de fconfig seulement)
bool debogage = false; //mode débogage
bool errlog = true; //log les erreurs dans ferreur
char nom_ferreur[30] = "erreurs.txt";

//Grille:
struct _case** grille = NULL; //ptr vers un array 2D symbolisant la grille de mots croisés
struct mot* mots = NULL; //ptr vers un array contenant la liste des mots inscrits dans la grille
int nbre_cases = 10;
int nbre_mots = 0;
char titre[200] = "Grille sans titre";

//Variables SDL opaques:
SDL_Window* fenetre = NULL;
SDL_Renderer* rend = NULL;
TTF_Font* police = NULL;
TTF_Font* police_grille = NULL;
TTF_Font* petite_police = NULL;
TTF_Font* police_nbre_grille = NULL;
SDL_Cursor* curseur_normal = NULL;
SDL_Cursor* curseur_txt = NULL;
SDL_Texture* symbole_modifier = NULL;
SDL_Texture* symbole_taille_police = NULL;
SDL_Texture* symbole_orientation = NULL;

//Autres variables d'intérêt public:
SDL_MessageBoxButtonData boutons_oui_non[2] =
{
	{SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Non"},
	{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Oui"}
};


//Macro permettant de changer la couleur des formes dessinées avec SDL de manière plus simple et intelligente:
#define SDL_SetColor(couleur, renderer)									SDL_SetRenderDrawColor(renderer, couleur.r, couleur.g, couleur.b, couleur.a)

//Macro permettant de dessiner un rectangle plein aux coins arrondis:
#define rect_arrondi(x, y, largeur, hauteur, couleur, fond, renderer)	rectangle(x + 1, y + 1, largeur, hauteur, 0, couleur, fond, renderer); rectangle(x+2, y+2, largeur-2, hauteur-2, 5, couleur, fond, renderer)

//Macros permettant de trouver la longueur d'une ligne de texte:
#define longueur_txt(txt, longueur_max, police)							afficher_txt(txt, 0, 0, longueur_max, police, transparent, NULL)
#define longueur_txt_centre(txt, x_gauche, x_droite, police)			afficher_txt_centre(txt, x_gauche, x_droite, 0, police, transparent, NULL)

//Macro de débogage (dompe l'état des variables dans ferreur):
#define debug()															gestion_arguments("--deboguer")


//Liste des fonctions (en ordre alphabétique et classées par fichier):
//cmc.c:
void afficher_menu(enum zones_menu zone);
void erreur(int code, char msg[]);
void gestion_arguments(char arg[]);
void init();
int main(int argc, char* argv[]);
void menu();
void quitter();
//reglages.c:
void appliquer_reglages();
void enregistrer_reglages();
bool lire_reglages();
void rafraichir_reglages(char zone);
void reglages();
void reinitialiser_reglages();
//aide.c:
void aide();
void aide_creation();
void aide_jeu();
//creation.c:
void identifier_mot();
void nouvelle_grille();
bool sauvegarder_grille();
//jeu.c:
bool ouvrir_grille(char nom[]);
void partie();
void rafraichir(enum zones zone);
void verifier_grille();
void verifier_mot();
//backend.c:
void afficher_versions_SDL(FILE* ferreur);
int afficher_txt(char txt[], int x, int y, int longueur_max, TTF_Font* police, SDL_Color couleur, SDL_Renderer* renderer);
int afficher_txt_centre(char txt[], int x_gauche, int x_droite, int y, TTF_Font* police, SDL_Color couleur, SDL_Renderer* renderer);
bool demander_couleur(char nom_couleur[], SDL_Color* couleur, SDL_Window* fenetre_source);
int demander_nbre(char titre_recu[], char explications[], int valeur_initiale, SDL_Window* fenetre_source);
bool demander_txt(char titre_recu[], char explications[], char input[], int max, SDL_Window* fenetre_source);
char* enlever_majuscule(char string[]);
void extraire_rgba(char src[], char r[], char g[], char b[], char a[]);
void liberer_memoire();
void rectangle(int x, int y, int largeur, int hauteur, int epaisseur, SDL_Color couleur, SDL_Color fond, SDL_Renderer* renderer);
void simuler_mvm_souris();
void tronquer(char txt[]);