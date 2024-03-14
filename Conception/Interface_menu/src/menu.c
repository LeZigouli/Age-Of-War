#include "../lib/menu.h"


/***************/
/*--Fonctions--*/
/***************/

/*Fonction d'initialisation*/
int initialisation()
{
    /*Initialiser SDL*/
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        SDL_Log("Erreur lors de l'initialisation de SDL : %s", SDL_GetError());
        return 1;
    }
    /*Initialiser SDL_image*/
    if (IMG_Init(IMG_INIT_PNG) < 0) {
        SDL_Log("Erreur lors de l'initialisation de SDL_image : %s", IMG_GetError());
        SDL_Quit();
        return 1;
    }
    /*Initialiser SDL_ttf*/
    if (TTF_Init() < 0) {
        SDL_Log("Erreur lors de l'initialisation de SDL_ttf : %s", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    /*Initialisation SDL_Mixer*/
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("Erreur: %s\n", Mix_GetError());
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    } 
    /*Initialisation variables pour la gestion du volume*/
    volumeBar = malloc(sizeof(SDL_Rect));
    volumeCursor = malloc(sizeof(SDL_Rect));
    volume = MIX_MAX_VOLUME / 2;
    estLache = SDL_FALSE;
    return 0;   
}

/*Création de rectangle dynamique*/
SDL_Rect creationRectangle(SDL_Window* fenetre, int x, int y, int largeur, int hauteur)
{
    int w, h;

    SDL_GetWindowSize(fenetre, &w, &h);
    /*Calcul des proportions relatives*/
    float widthFactor = (float)w / WINDOW_WIDTH;
    float heightFactor = (float)h / WINDOW_HEIGHT;
    
    /*Définition du rectangle en fonction des proportions relatives*/
    SDL_Rect rect = {x * widthFactor, y * heightFactor, largeur * widthFactor, hauteur * heightFactor};

    return rect;
}

/*Fonction pour afficher le titre du jeu*/
void afficherTitre(SDL_Renderer * rendu, TTF_Font* police, SDL_Window* fenetre, int x, int y, int largeur, int hauteur)
{
    SDL_Texture* texture_titre = IMG_LoadTexture(rendu, "img/Battle_of_Time.png");

    SDL_Rect rect_titre = creationRectangle(fenetre, x, y,largeur,hauteur);
    SDL_RenderCopy(rendu, texture_titre, NULL, &rect_titre);

    SDL_DestroyTexture(texture_titre);
}

/*Fonction d'affichage du menu*/
void afficherMenu(SDL_Renderer* rendu, TTF_Font* police, SDL_Window* fenetre, const char* texte, int x, int y, int largeur, int hauteur)
{
    SDL_Color couleurTexte = COLOR;  // Couleur du texte 

    SDL_Surface* surfaceTexte = TTF_RenderUTF8_Solid(police, texte, couleurTexte);
    SDL_Texture* textureTexte = SDL_CreateTextureFromSurface(rendu, surfaceTexte);

    SDL_Rect rectangleMenu = creationRectangle(fenetre, x, y,largeur,hauteur);
    SDL_RenderCopy(rendu, textureTexte, NULL, &rectangleMenu);

    SDL_FreeSurface(surfaceTexte);
    SDL_DestroyTexture(textureTexte);
}

/*Fonction d'affichage des sous-menus*/
void afficherSousMenu(SDL_Renderer* rendu, TTF_Font* police, SDL_Window* fenetre, const char* texte1, const char* texte2, const char* texte3, const char* texte4,const char* texte5)
{
    int menuX = (WINDOW_WIDTH - MENU_WIDTH) / 2;  // Position horizontale centrée pour le sous-menu
    int menuY = (WINDOW_HEIGHT -  ( 4 * MENU_HEIGHT + SPACING)) / 2;  // Position verticale centrée pour le sous-menu

    // Afficher les onglets du sous-menu avec espacement
    afficherMenu(rendu, police, fenetre, texte1, menuX, menuY, MENU_WIDTH, MENU_HEIGHT);
    afficherMenu(rendu, police, fenetre, texte2, menuX, menuY + MENU_HEIGHT + SPACING, MENU_WIDTH, MENU_HEIGHT);
    afficherMenu(rendu, police, fenetre, texte3, menuX, menuY + 2 * (MENU_HEIGHT + SPACING), MENU_WIDTH, MENU_HEIGHT);
    afficherMenu(rendu, police, fenetre, texte4, menuX, menuY + 3 * (MENU_HEIGHT + SPACING), MENU_WIDTH, MENU_HEIGHT);
    afficherMenu(rendu, police, fenetre, texte5, menuX, menuY + 4 * (MENU_HEIGHT + SPACING), MENU_WIDTH, MENU_HEIGHT);
}

/*Fonction d'affichage des sous-menus*/
void afficherSousMenuOption(SDL_Renderer* rendu, TTF_Font* police, SDL_Window* fenetre, const char* texte1, const char* texte2, const char* texte3)
{
    int menuX = (WINDOW_WIDTH - MENU_WIDTH) / 2;  // Position horizontale centrée pour le sous-menu
    int menuY = (WINDOW_HEIGHT -  (2 * MENU_HEIGHT + SPACING)) / 2;  // Position verticale centrée pour le sous-menu

    // Afficher les onglets du sous-menu avec espacement
    afficherMenu(rendu, police, fenetre, texte1, menuX, menuY, MENU_WIDTH, MENU_HEIGHT);
    afficherMenu(rendu, police, fenetre, texte2, menuX, menuY + MENU_HEIGHT + SPACING, MENU_WIDTH, MENU_HEIGHT);
    afficherMenu(rendu, police, fenetre, texte3, menuX, menuY + 2 * (MENU_HEIGHT + SPACING), MENU_WIDTH, MENU_HEIGHT);
}

/*Affichage du sous-menu résolution*/
void afficherSousMenuResolution(SDL_Renderer* rendu, TTF_Font* police, SDL_Window* fenetre)
{
    int menuX = (WINDOW_WIDTH - MENU_WIDTH) / 2; //Position horizontale
    int menuY = (WINDOW_HEIGHT - (MENU_HEIGHT + SPACING)) / 2; //Position verticale

    /*Afficher les flèches de sélections*/
    //Flèche droite
    SDL_Texture* texture_fleche_d = IMG_LoadTexture(rendu, "img/fleche_menu_droite.png");
    SDL_Rect rect_flechD = creationRectangle(fenetre, menuX + MENU_WIDTH, menuY, MENU_WIDTH / 3, MENU_HEIGHT * 1.5);
    SDL_RenderCopy(rendu, texture_fleche_d, NULL, &rect_flechD);
    SDL_DestroyTexture(texture_fleche_d);

    //Flèche gauche
    SDL_Texture* texture_fleche_g = IMG_LoadTexture(rendu, "img/fleche_menu_gauche.png");
    SDL_Rect rect_flechG = creationRectangle(fenetre, menuX - 90, menuY, MENU_WIDTH / 3, MENU_HEIGHT * 1.5);
    SDL_RenderCopy(rendu, texture_fleche_g, NULL, &rect_flechG);
    SDL_DestroyTexture(texture_fleche_g);

    //Afficher les onglets
    afficherMenu(rendu, police, fenetre, "Plein écran", menuX, menuY + 1.5 * (MENU_HEIGHT + SPACING), MENU_WIDTH, MENU_HEIGHT);
    afficherMenu(rendu, police, fenetre, "Retour", menuX, menuY + 2.5 * (MENU_HEIGHT + SPACING), MENU_WIDTH, MENU_HEIGHT);
}

/*Initialiser du bloc avec les résolutions*/
void initElements(SDL_Renderer* rendu, SDL_Window* fenetre, Element* elm_reso) 
{

    int menuX = (WINDOW_WIDTH - MENU_WIDTH) / 2; // Coordonnée x pour centrer les éléments horizontalement
    int menuY = (WINDOW_HEIGHT - (MENU_HEIGHT + SPACING)) / 2; // Coordonnée y pour centrer les éléments verticalement

    // Informations à afficher pour chaque élément
    char* infos[4] = {"800x600", "1024x768", "1280x720", "1920x1080"};
    int largeur[4] = {800, 1024, 1080, 1920};
    int hauteur[4] = {600, 768, 720, 1080};

    for (int i = 0; i < 4; ++i) {
        elm_reso[i].rect = creationRectangle(fenetre, menuX, menuY, MENU_WIDTH, MENU_HEIGHT * 1.5);
        elm_reso[i].info = infos[i];
        elm_reso[i].w = largeur[i];
        elm_reso[i].h = hauteur[i];
    }
}
/*Affichage du bloc résolution dans le sous-menu résolution*/
void resolution(SDL_Renderer* rendu, SDL_Window* fenetre , TTF_Font* police, Element* elm_reso, int* selecElement)
{
    //Dessiner l'élément sélectionné
    SDL_Rect shiftedRect = {elm_reso[(*selecElement)].rect.x, elm_reso[(*selecElement)].rect.y, elm_reso[(*selecElement)].rect.w, elm_reso[(*selecElement)].rect.h};
    // Dessiner le texte pour l'élément sélectionné
    SDL_Surface* textSurface = TTF_RenderText_Solid(police, elm_reso[(*selecElement)].info, COLOR);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(rendu, textSurface);
    SDL_Rect textRect = creationRectangle(fenetre, shiftedRect.x, shiftedRect.y, shiftedRect.w, shiftedRect.h);
    SDL_RenderCopy(rendu, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

/*Affichage du sous menu crédits*/
void afficherSousMenuCredits(SDL_Renderer* rendu, SDL_Window* fenetre, TTF_Font* police)
{
    int img_w = 700 / 1.2;
    int img_h = 549 / 1.25;
    int menuX_img = (WINDOW_WIDTH - img_w) / 2; //Position horizontale
    int menuY_img = (WINDOW_HEIGHT - img_h) / 2; //Position verticale

    int menuX = (WINDOW_WIDTH - MENU_WIDTH) / 2;
    int menuY = (WINDOW_HEIGHT - MENU_HEIGHT) / 2;

    //Chargement de l'image
    SDL_Texture* texture_credit = NULL;
    texture_credit = IMG_LoadTexture(rendu, "img/credits.png");
    if(texture_credit == NULL){
        SDL_Log("Erreur lors du chargement image des crédits : %s\n", SDL_GetError());
    }

    SDL_Rect rect_credit = creationRectangle(fenetre, menuX_img, menuY_img + 100, img_w, img_h);
    SDL_RenderCopy(rendu, texture_credit, NULL, &rect_credit);
    SDL_DestroyTexture(texture_credit);

    afficherMenu(rendu, police, fenetre, "Retour", menuX, menuY + 335, MENU_WIDTH, MENU_HEIGHT - 20);
}

/*Affichage du sous menu son*/
void afficherSousMenuSon(SDL_Renderer* rendu, TTF_Font* police, SDL_Window* fenetre, const char* texte)
{
    int menuX = (WINDOW_WIDTH - MENU_WIDTH) / 2; //Position horizontale
    int menuY = (WINDOW_HEIGHT - (MENU_HEIGHT + SPACING)) / 2; //Position verticale

    // Variables pour gérer le curseur de volume
    (*volumeBar)  = creationRectangle(fenetre, menuX, menuY + 2.5, MENU_WIDTH, 25);
    (*volumeCursor) = creationRectangle(fenetre, menuX + volume * 2, menuY - 2.5, 15, 35);

    // Dessiner la barre de volume
    SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
    SDL_RenderFillRect(rendu, volumeBar);

    // Dessiner le curseur de volume
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
    SDL_RenderFillRect(rendu, volumeCursor);


    afficherMenu(rendu, police, fenetre, "Musique :", menuX, menuY - MENU_HEIGHT, MENU_WIDTH, MENU_HEIGHT);
    afficherMenu(rendu, police, fenetre, "Effets sonores :", menuX, menuY + MENU_HEIGHT, MENU_WIDTH, MENU_HEIGHT);
    afficherMenu(rendu, police, fenetre, texte, menuX, menuY + 2 * MENU_HEIGHT, MENU_WIDTH, MENU_HEIGHT);
    afficherMenu(rendu ,police, fenetre, "Retour", menuX, menuY + 3 * MENU_HEIGHT + SPACING, MENU_WIDTH, MENU_HEIGHT);
}
