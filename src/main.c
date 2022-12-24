#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include <windows.h>
#include <conio.h>
#include <unistd.h>
#include <time.h>

void clearScreen()
{
    system("cls");
}

struct Case {
    int id;
    int posX;
    int posY;
    int voisinN;
    int voisinE;
    int voisinS;
    int voisinO;
    int voisinNE;
    int voisinSE;
    int voisinSO;
    int voisinNO;
    bool isEmpty;
    struct Pion *pion;
};
struct Pion {
    int id;
    struct Joueur *equipe;
    struct Case *pos;

};
struct Joueur {
    int id;
    int equipe;
    int nbPion;
    int score;
    char icon;
    int color;
    int posX;
    int posY;
    bool hasPlacedInZero;
};
void textColor(int couleurDuTexte,int couleurDeFond)
{
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,couleurDeFond*16+couleurDuTexte);
}


void affichagePlateau(int n,struct Case plateau[n][n],struct Joueur *joueur, int selecType){
    int i,j;
    int playerPosX = joueur->posX;
    int playerPosY = joueur->posY;
    for (i=1;i<(n+1);i++){
        printf("\n");
        for (j=1;j<n+1;j++){

            if (plateau[i-1][j-1].isEmpty == true){
                //permet de mettre en couleur la case que le joueur est entrain de parcourir jusqu'au moment où il la sélectionne
                if (i-1 == playerPosY && j-1 == playerPosX){
                    textColor(joueur->color,0);
                }
                printf(" %c",(char)254)  ;
                textColor(15,0);

            }
            else{

                textColor(plateau[i - 1][j - 1].pion->equipe->color, 0);
                if (i - 1 == playerPosY && j - 1 == playerPosX) {
                    if (selecType == 0) {
                        textColor(8, 0);
                        printf(" %c", (char) 254);
                    } else {
                        if (plateau[i - 1][j - 1].pion->equipe->id == joueur->id) {
                            textColor(1, 0);
                        } else {
                            textColor(8, 0);
                        }
                        printf(" %c", plateau[i - 1][j - 1].pion->equipe->icon);

                    }
                } else {
                    printf(" %c", plateau[i - 1][j - 1].pion->equipe->icon);
                }
                textColor(15, 0);
            }


        }
    }
    printf("\n\n");
    printf("---------------------");
    printf("\n");
    printf("[TAB] pour valider");
    printf("\n[%c] pour monter",(char)24);
    printf("\n[%c] pour descendre",(char)25);
    printf("\n[->] pour aller a droite");
    printf("\n[<-] pour aller a gauche");


}

//Fonction qui permet d'enregistrer la partie en cours (statut : mulitjoueur ou AI, taille du plateau, informations des joueurs : pions etc..)
void saveGame(int n,int p, int x,struct Joueur joueur1, struct Joueur joueur2, struct Case plateau[n][n], struct Pion pions[2][p], int gameStatut){
    FILE *fichier = NULL;
    fichier = fopen("save.txt", "w");
    //Enregistrement sous la forme :
    // 1- Date et heure de la partie (format : jj/mm/aaaa hh:mm:ss)
    // 2- Statut de la partie (multi ou AI)
    // 3- Informations du plateau (longueur, nombre dce pions à aligner, positions des pions)
    // 4.1 - Informations du joueur 1
    // 4.2 - Informations du joueur 2

    //1
    time_t now = time(NULL);
    struct tm tm = *localtime(&now);
    fprintf(fichier, "Date et heure de la partie : %d/%d/%d %d:%d:%d \n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    //2
    if (gameStatut == 1){
        fprintf(fichier, "Statut de la partie : Multi-joueur \n");
    }else{
        fprintf(fichier, "Statut de la partie : AI \n");
    }
    //3
    fprintf(fichier, "Informations du plateau : \n");
    fprintf(fichier, "Longueur du plateau : %d \n", n);
    fprintf(fichier, "Nombre de pions à aligner : %d \n", x);
    fprintf(fichier, "Positions des pions : \n");
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (plateau[i][j].isEmpty == false){
                fprintf(fichier, "Pion %d, équipe %d - X: %d - Y: %d \n", plateau[i][j].pion->id, plateau[i][j].pion->equipe, plateau[i][j].posX, plateau[i][j].posY);
            }else{
                fprintf(fichier, "Case vide - X: %d - Y: %d \n", plateau[i][j].posX, plateau[i][j].posY);
            }
        }
    }
    //4.1
    fprintf(fichier, "Informations du joueur 1 : \n");
    fprintf(fichier, "Nombre de pions : %d \n", joueur1.nbPion);
    //4.2
    fprintf(fichier, "Informations du joueur 2 : \n");
    fprintf(fichier, "Nombre de pions : %d \n", joueur2.nbPion);
    fclose(fichier);



}

void choixSave(int choix){
    switch (choix){
        case 1:
            printf("[");
            textColor(4,0);
            printf("%c",(char) 219);
            textColor(15,0);
            printf("] Continuer\n");
            printf("[ ] Sauvegarder et quitter\n");
            printf("[ ] Quitter sans sauvegarder\n");
            break;
        case 2:
            printf("[ ] Continuer\n");
            printf("[");
            textColor(4,0);
            printf("%c",(char) 219);
            textColor(15,0);
            printf("] Sauvegarder et quitter\n");
            printf("[ ] Quitter sans sauvegarder\n");
            break;
        case 3:
            printf("[ ] Continuer\n");
            printf("[ ] Sauvegarder et quitter\n");
            printf("[");
            textColor(4,0);
            printf("%c",(char) 219);
            textColor(15,0);
            printf("] Quitter sans sauvegarder\n");
            break;
        default:

            break;

    }
}


//Fonction shouldSave qui propose au joueur de sauvegarder la partie ou de continuer
// Détection grâce aux flèches directionnelles et entrée
void shouldSave(int n,int p, int x, struct Case plateau[n][n], struct Joueur *joueur1, struct Joueur *joueur2, struct Pion pions[2][p],int gameStatut){
    int choix = 0;
    printf("Voulez-vous sauvegarder la partie ?\n");
    choixSave(choix);
    bool hasSelected = false;
    do{
        if (GetAsyncKeyState(VK_UP) != 0){
            choix = 1;
            clearScreen();
            printf("Voulez-vous sauvegarder la partie ?\n");
            choixSave(choix);
        }
        if (GetAsyncKeyState(VK_DOWN) != 0){
            choix = 2;
            clearScreen();
            printf("Voulez-vous sauvegarder la partie ?\n");
            choixSave(choix);
        }
        if (GetAsyncKeyState(VK_RIGHT) != 0){
            choix = 3;
            clearScreen();
            printf("Voulez-vous sauvegarder la partie ?\n");
            choixSave(choix);
        }
        if (GetAsyncKeyState(VK_RETURN) != 0){
            printf("-> Vous avez choisi l'option %d\n", choix);
            sleep(1);
            clearScreen();
            hasSelected = true;
        }
    }while (!hasSelected);
    switch (choix){
        case 1:
            break;
        case 2:
            saveGame(n,p,x,*joueur1,*joueur2,plateau,pions,gameStatut);
            break;
        case 3:
            exit(0);
            break;
        default:
            break;
    }

}

bool scorePoint2(int n,int x ,struct Case plateau[n][n],struct Case actualCase,int posX,int posY){
    int equipe = actualCase.pion->equipe->equipe;
    int compteur = 0;
    int defPosX = posX;
    int defPosY = posY;
    //Vérifie vers le Nord si il y a un pion de la même équipe et incrémente le compteur tant qu'il y'en a un, puis retourne à la position de base pour vérifier vers le Sud si il y a un pion de la même équipe et incrémente le compteur tant qu'il y'en a un
    // Verification vers le Nord
    while (actualCase.voisinN != 0 && plateau[posY-1][posX].pion != NULL && plateau[posY-1][posX].pion->equipe->equipe == equipe){
        compteur++;
        posY--;
        actualCase = plateau[posY][posX];
    }
    // Verification vers le Sud
    posY = defPosY;
    actualCase = plateau[posY][posX];
    while (actualCase.voisinS != 0 && plateau[posY+1][posX].pion != NULL && plateau[posY+1][posX].pion->equipe->equipe == equipe){

        compteur++;
        posY++;
        actualCase = plateau[posY][posX];
    }

    if (compteur >= x-1){
        return true;
    }
    compteur = 0;
    posY = defPosY;
    actualCase = plateau[posY][posX];
    //Vérifie vers l'Est si il y a un pion de la même équipe et incrémente le compteur tant qu'il y'en a un, puis retourne à la position de base pour vérifier vers l'Ouest si il y a un pion de la même équipe et incrémente le compteur tant qu'il y'en a un
    // Verification vers l'Est
    while (actualCase.voisinE != 0 && plateau[posY][posX+1].pion != NULL && plateau[posY][posX+1].pion->equipe->equipe == equipe){
        compteur++;
        posX++;
        actualCase = plateau[posY][posX];
    }
    // Verification vers l'Ouest
    posX = defPosX;
    actualCase = plateau[posY][posX];
    while (actualCase.voisinO != 0 && plateau[posY][posX-1].pion != NULL && plateau[posY][posX-1].pion->equipe->equipe == equipe){
        compteur++;
        posX--;
        actualCase = plateau[posY][posX];
    }
    if (compteur >= x-1){
        return true;
    }
    compteur = 0;
    posX = defPosX;
    actualCase = plateau[posY][posX];
    //Vérifie vers le Nord-Est si il y a un pion de la même équipe et incrémente le compteur tant qu'il y'en a un, puis retourne à la position de base pour vérifier vers le Sud-Ouest si il y a un pion de la même équipe et incrémente le compteur tant qu'il y'en a un
    // Verification vers le Nord-Est
    while (actualCase.voisinNE != 0 && plateau[posY-1][posX+1].pion != NULL && plateau[posY-1][posX+1].pion->equipe->equipe == equipe){
        compteur++;
        posY--;
        posX++;
        actualCase = plateau[posY][posX];
    }
    // Verification vers le Sud-Ouest
    posY = defPosY;
    posX = defPosX;
    actualCase = plateau[posY][posX];
    while (actualCase.voisinSO != 0 && plateau[posY+1][posX-1].pion != NULL && plateau[posY+1][posX-1].pion->equipe->equipe == equipe){
        compteur++;
        posY++;
        posX--;
        actualCase = plateau[posY][posX];
    }
    if (compteur >= x-1){
        return true;
    }
    compteur = 0;
    posY = defPosY;
    posX = defPosX;
    actualCase = plateau[posY][posX];
    //Vérifie vers le Nord-Ouest si il y a un pion de la même équipe et incrémente le compteur tant qu'il y'en a un, puis retourne à la position de base pour vérifier vers le Sud-Est si il y a un pion de la même équipe et incrémente le compteur tant qu'il y'en a un
    // Verification vers le Nord-Ouest
    while (actualCase.voisinNO != 0 && plateau[posY-1][posX-1].pion != NULL && plateau[posY-1][posX-1].pion->equipe->equipe == equipe){
        compteur++;
        posY--;
        posX--;
        actualCase = plateau[posY][posX];
    }
    // Verification vers le Sud-Est
    posY = defPosY;
    posX = defPosX;
    actualCase = plateau[posY][posX];
    while (actualCase.voisinSE != 0 && plateau[posY+1][posX+1].pion != NULL && plateau[posY+1][posX+1].pion->equipe->equipe == equipe){
        compteur++;
        posY++;
        posX++;
        actualCase = plateau[posY][posX];
    }
    if (compteur >= x-1){
        return true;
    }
    // Vérification de pions positionnés en carrés
    compteur = 0;
    posY = defPosY;
    posX = defPosX;
    actualCase = plateau[posY][posX];
    // Carré #1 :
    //Un pion à droite du pion actuel et un pion au dessus du pion actuel et de celui de droite
    if (actualCase.voisinE != 0 && actualCase.voisinN != 0 && actualCase.voisinNE != 0 && plateau[posY-1][posX+1].pion != NULL && plateau[posY-1][posX+1].pion->equipe->equipe == equipe && plateau[posY][posX+1].pion != NULL && plateau[posY][posX+1].pion->equipe->equipe == equipe && plateau[posY-1][posX].pion != NULL && plateau[posY-1][posX].pion->equipe->equipe == equipe){
        compteur++;
    }
    if (compteur >= x-1){
        return true;
    }
    compteur = 0;
    posY = defPosY;
    posX = defPosX;
    actualCase = plateau[posY][posX];
    // Carré #2 :
    //Un pion à gauche du pion actuel et un pion au dessus du pion actuel et de celui de gauche
    if (actualCase.voisinO != 0 && actualCase.voisinN != 0 && actualCase.voisinNO != 0 && plateau[posY-1][posX-1].pion != NULL && plateau[posY-1][posX-1].pion->equipe->equipe == equipe && plateau[posY][posX-1].pion != NULL && plateau[posY][posX-1].pion->equipe->equipe == equipe && plateau[posY-1][posX].pion != NULL && plateau[posY-1][posX].pion->equipe->equipe == equipe){
        compteur++;
    }
    if (compteur >= x-1){
        return true;
    }
    compteur = 0;
    posY = defPosY;
    posX = defPosX;
    actualCase = plateau[posY][posX];
    // Carré #3 :
    //Un pion à droite du pion actuel et un pion en dessous du pion actuel et de celui de droite
    if (actualCase.voisinE != 0 && actualCase.voisinS != 0 && actualCase.voisinSE != 0 && plateau[posY+1][posX+1].pion != NULL && plateau[posY+1][posX+1].pion->equipe->equipe == equipe && plateau[posY][posX+1].pion != NULL && plateau[posY][posX+1].pion->equipe->equipe == equipe && plateau[posY+1][posX].pion != NULL && plateau[posY+1][posX].pion->equipe->equipe == equipe){
        compteur++;
    }
    if (compteur >= x-1){
        return true;
    }
    compteur = 0;
    posY = defPosY;
    posX = defPosX;
    actualCase = plateau[posY][posX];
    // Carré #4 :
    //Un pion à gauche du pion actuel et un pion en dessous du pion actuel et de celui de gauche
    if (actualCase.voisinO != 0 && actualCase.voisinS != 0 && actualCase.voisinSO != 0 && plateau[posY+1][posX-1].pion != NULL && plateau[posY+1][posX-1].pion->equipe->equipe == equipe && plateau[posY][posX-1].pion != NULL && plateau[posY][posX-1].pion->equipe->equipe == equipe && plateau[posY+1][posX].pion != NULL && plateau[posY+1][posX].pion->equipe->equipe == equipe){
        compteur++;
    }
    if (compteur >= x-1){
        return true;
    }

    if (compteur >= x-1){
        return true;
    }
    return false;


}

void CaseSelector2(int n,int p,struct Case plateau[n][n], struct Joueur *joueur,struct Pion pions[2][p], int *posX, int *posY,int selecType){
    bool hasMoove = false;
    bool hasSelected = false;
    bool hasWin = false;
    int pressedScore = 0;
    //Boucle qui permet de naviguer sur les cases grâce à une détection automatique des fleches directionnelles, puis en actualisant l'affichage du plateau et de sélectionner une case avec la touche entrée
    while (!hasSelected) {
        //Détecte si la touche fleche du haut est pressée
        if (GetAsyncKeyState(VK_RIGHT) && plateau[joueur->posY][joueur->posX].voisinE != 0){
            pressedScore ++;
            if (pressedScore ==1){
                joueur->posX = joueur->posX + 1;
                clearScreen();
                affichagePlateau(n,plateau,joueur,selecType);
                pressedScore=0;
            }
        }
        //Détecte si la touche fleche du bas est pressée
        if (GetAsyncKeyState(VK_LEFT) && plateau[joueur->posY][joueur->posX].voisinO != 0){
            pressedScore ++;
            if (pressedScore ==1){
                joueur->posX = joueur->posX - 1;
                clearScreen();
                affichagePlateau(n,plateau,joueur,selecType);
                pressedScore=0;
            }
        }
        //Détecte si la touche fleche du haut est pressée
        if (GetAsyncKeyState(VK_UP) && plateau[joueur->posY][joueur->posX].voisinN != 0){
            pressedScore ++;
            if (pressedScore ==1){
                joueur->posY = joueur->posY - 1;
                clearScreen();
                affichagePlateau(n,plateau,joueur,selecType);
                pressedScore=0;
            }
        }
        //Détecte si la touche fleche du bas est pressée
        if (GetAsyncKeyState(VK_DOWN) && plateau[joueur->posY][joueur->posX].voisinS != 0){
            pressedScore ++;
            if (pressedScore ==1){
                joueur->posY = joueur->posY + 1;
                clearScreen();
                affichagePlateau(n,plateau,joueur,selecType);
                pressedScore=0;
            }
        }
        //Détecte si la touche entrée est pressée
        if (GetAsyncKeyState(VK_TAB)){
            pressedScore = pressedScore +1;
            if (pressedScore >= 1) {
                if (selecType == 0) {
                    if (plateau[joueur->posY][joueur->posX].isEmpty == true) {
                        *posX = joueur->posX;
                        *posY = joueur->posY;
                        pions[joueur->equipe - 1][joueur->nbPion] = (struct Pion) {joueur->nbPion, joueur,&plateau[joueur->posY][joueur->posX]};
                        plateau[*posY][*posX].pion = &pions[joueur->equipe - 1][joueur->nbPion];
                        plateau[*posY][*posX].isEmpty = false;
                        joueur->nbPion++;
                        clearScreen();
                        hasSelected = true;
                    }
                }else if (selecType == 1){
                    if (plateau[joueur->posY][joueur->posX].isEmpty == false && plateau[joueur->posY][joueur->posX].pion->equipe->equipe == joueur->equipe){
                        *posX = joueur->posX;
                        *posY = joueur->posY;
                        hasSelected = true;
                    }
                }else{
                    //Change la position du pion de la coordonnée (Y,X) à la coordonnée (joueur->posY,joueur->posX)
                    if (plateau[joueur->posY][joueur->posX].isEmpty == true) {
                        plateau[joueur->posY][joueur->posX].pion = plateau[*posY][*posX].pion;
                        plateau[joueur->posY][joueur->posX].isEmpty = false;
                        plateau[*posY][*posX].isEmpty = true;
                        plateau[*posY][*posX].pion = NULL;
                        *posX = joueur->posX;
                        *posY = joueur->posY;
                        clearScreen();
                        affichagePlateau(n,plateau,joueur,0);
                        hasSelected = true;
                    }
                }
                Sleep(0.5);
                pressedScore = 0;
            }
        }
    }
}

bool placePion2(int n,int p,struct Case plateau[n][n], struct Joueur *joueur,struct Pion pions[2][p],int x){
    bool hasWin = false;
    int posX,posY = 0;
    affichagePlateau(n, plateau,joueur,0);
    printf("\n");
    printf("---------------------");
    printf("\n");
    printf("-> Joueur %d, placez votre pion\n", joueur->equipe);
    CaseSelector2(n,p, plateau, joueur, pions,&posX, &posY,0);
    hasWin = scorePoint2(n,x,plateau,plateau[posY][posX],posX,posY);
    {if (hasWin == true) {
            printf("\nLe joueur %d a gagné", joueur->equipe);
        }
        return hasWin;
    }
}

bool movePion2(int n,int p,struct Case plateau[n][n], struct Joueur *joueur,struct Pion pions[2][p],int x){
    bool hasWin = false;
    int posX,posY = 0;
    affichagePlateau(n, plateau,joueur,1);
    printf("\n");
    printf("---------------------");
    printf("\n");
    printf("-> Joueur %d, déplacez votre pion\n", joueur->equipe);
    CaseSelector2(n,p, plateau, joueur, pions,&posX, &posY,1);
    affichagePlateau(n, plateau,joueur,2);
    printf("\n");
    printf("---------------------");
    printf("\n");
    printf("-> Joueur %d, déplacez votre pion\n", joueur->equipe);
    CaseSelector2(n,p, plateau, joueur, pions,&posX, &posY,2);
    hasWin = scorePoint2(n,x,plateau,plateau[posY][posX],posX,posY);
    {if (hasWin == true) {
            printf("\nLe joueur %d a gagné", joueur->equipe);
        }
        return hasWin;
    }
}










//Fonction "CreatePlateau" qui automatise la création du plateau de jeu
void CreatePlateau(int n, struct Case plateau[n][n]){
    int i,j;
    for (i=1;i<(n+1);i++){
        printf("\n");
        for (j=1;j<n+1;j++){
            plateau[i-1][j-1] = (struct Case){i*(n)+j-n,j,i,0,0,0,0,0,0,0,0,true,NULL};
            //Voisin nord = à 0 si on est sur la première ligne
            if (i == 1){
                plateau[i-1][j-1].voisinN = 0;
            }
            else{
                plateau[i-1][j-1].voisinN = plateau[i-1][j-1].id -n;
            }
            //Voisin sud = à 0 si on est sur la dernière ligne
            if (i == n){
                plateau[i-1][j-1].voisinS = 0;
            }
            else{
                plateau[i-1][j-1].voisinS = plateau[i-1][j-1].id +n;
            }
            //Voisin est = à 0 si on est sur la dernière colonne
            if (j == n){
                plateau[i-1][j-1].voisinE = 0;
            }
            else{
                plateau[i-1][j-1].voisinE = plateau[i-1][j-1].id +1;
            }
            //Voisin ouest = à 0 si on est sur la première colonne
            if (j == 1){
                plateau[i-1][j-1].voisinO = 0;
            }
            else{
                plateau[i-1][j-1].voisinO = plateau[i-1][j-1].id -1;
            }
            //Voisin nord-est = à 0 si on est sur la première ligne ou la dernière colonne
            if (i == 1 || j == n){
                plateau[i-1][j-1].voisinNE = 0;
            }
            else{
                plateau[i-1][j-1].voisinNE = plateau[i-1][j-1].id -n +1;
            }
            //Voisin nord-ouest = à 0 si on est sur la première ligne ou la première colonne
            if (i == 1 || j == 1){
                plateau[i-1][j-1].voisinNO = 0;
            }
            else{
                plateau[i-1][j-1].voisinNO = plateau[i-1][j-1].id -n -1;
            }
            //Voisin sud-est = à 0 si on est sur la dernière ligne ou la dernière colonne
            if (i == n || j == n){
                plateau[i-1][j-1].voisinSE = 0;
            }
            else{
                plateau[i-1][j-1].voisinSE = plateau[i-1][j-1].id +n +1;
            }
            //Voisin sud-ouest = à 0 si on est sur la dernière ligne ou la première colonne
            if (i == n || j == 1){
                plateau[i-1][j-1].voisinSO = 0;
            }
            else{
                plateau[i-1][j-1].voisinSO = plateau[i-1][j-1].id +n -1;
            }
        }
    }
    printf("\n");
    printf("---------------------");
    printf("\n");
    clearScreen();
}


void MultiJoueur2(){
    int n,p, i,j;
    do{
        printf("\n-> Veuillez saisir une longueur supérieure ou égale à 4 : ");
        scanf("%d", &n);
    }while (n < 4 || n < 0);
    // On demande le nombre de pions strictement positif et inérieur à la longueur du plateau divisée par deux
    do{
        printf("\n-> Veuillez saisir le nombre de pions  strictement positif et inférieur à %d : ", n/2);
        scanf("%d", &p);
    }while (p < 1 || p > (n/2)-1);
    //On demande le nombre de pions à aligner  positif et inférieur ou égal à p
    int x;
    do{
        printf("\n-> Veuillez saisir le nombre de pions à aligner strictement positif et inférieur ou égal à %d : ", p);
        scanf("%d", &x);
    }while (x < 1 || x > p);





    //Création du plateau de jeu
    struct Case plateau[n][n];
    CreatePlateau(n, plateau);
    struct Pion pions[2][p];

    //Création des joueurs
    struct Joueur joueur1 = (struct Joueur){1,1,0,0,'X',12, 0,0,false};
    struct Joueur joueur2 = (struct Joueur){2,2,0,0,'O',2,0,0,false};
    bool hasWin = false;
    int winner= 0;
    while (joueur2.nbPion <= p-1){
            hasWin = placePion2(n,p,plateau,&joueur1,pions,x);
            if (hasWin == false){
                hasWin = placePion2(n,p,plateau,&joueur2,pions,x);
                if (hasWin){
                    winner = 2;
                    break;
                }
            }else{
                winner = 1;
                break;
            }
        }
    do {
        if (hasWin)
            break;
        hasWin = movePion2(n,p,plateau,&joueur1,pions,x);
        if (hasWin == false){
            hasWin = movePion2(n,p,plateau,&joueur2,pions,x);
            if (hasWin){
                winner = 2;
                break;
            }
        }else{
            winner = 1;
            break;
        }
    } while (hasWin == false);
    printf("\n");
    printf("---------------------");
    printf("\n");
    printf("-> Le joueur %d a gagné\n", winner);
    Sleep(15);
}



void AIeval2(int n, struct Case plateau[n][n], struct Joueur joueur, int valeurCase[n][n],int nbPionAligner) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            //Check si la case est occupée par un pion du joueur 1
            if (plateau[i][j].isEmpty == false && plateau[i][j].pion->equipe->equipe == 1) {
                //Les cases qui l'entourent prennent la valeur 1 sauf si elles sont déjà occupées par un pion du joueur 1 ou du robot
                if (plateau[i][j].voisinN != 0 && plateau[i - 1][j].isEmpty == true) {
                    valeurCase[i - 1][j]++;
                }
                if (plateau[i][j].voisinS != 0 && plateau[i + 1][j].isEmpty == true) {
                    valeurCase[i + 1][j]++;
                }
                if (plateau[i][j].voisinE != 0 && plateau[i][j + 1].isEmpty == true) {
                    valeurCase[i][j + 1]++;
                }
                if (plateau[i][j].voisinO != 0 && plateau[i][j - 1].isEmpty == true) {
                    valeurCase[i][j - 1]++;
                }
                if (plateau[i][j].voisinNE != 0 && plateau[i - 1][j + 1].isEmpty == true) {
                    valeurCase[i - 1][j + 1]++;
                }
                if (plateau[i][j].voisinNO != 0 && plateau[i - 1][j - 1].isEmpty == true) {
                    valeurCase[i - 1][j - 1]++;
                }
                if (plateau[i][j].voisinSE != 0 && plateau[i + 1][j + 1].isEmpty == true) {
                    valeurCase[i + 1][j + 1]++;
                }
                if (plateau[i][j].voisinSO != 0 && plateau[i + 1][j - 1].isEmpty == true) {
                    valeurCase[i + 1][j - 1]++;
                }
                //On check si le joueur 1 est entrain de former une diaonale avec deux pions, et si c'est le cas on incrémente la valeur des cases aux extrémités de la diagonale si elles sont libres
                //Diagonale Nord-Est :
                if (plateau[i][j].voisinNE != 0 && plateau[i - 1][j + 1].isEmpty == false &&
                    plateau[i - 1][j + 1].pion->equipe->equipe == 1) {
                    if (plateau[i - 1][j - 1].voisinNE != 0 && plateau[i - 2][j + 2].isEmpty == true) {
                        valeurCase[i - 2][j + 2]++;

                    }
                    if (plateau[i][j].voisinSO != 0 && plateau[i + 1][j - 1].isEmpty == true) {
                        valeurCase[i + 1][j - 1]++;
                    }
                }
                //Diagonale Nord-Ouest :
                if (plateau[i][j].voisinNO != 0 && plateau[i - 1][j - 1].isEmpty == false &&
                    plateau[i - 1][j - 1].pion->equipe->equipe == 1) {
                    if (plateau[i - 1][j + 1].voisinNO != 0 && plateau[i - 2][j - 2].isEmpty == true) {
                        valeurCase[i - 2][j - 2]++;
                    }
                    if (plateau[i][j].voisinSE != 0 && plateau[i + 1][j + 1].isEmpty == true) {
                        valeurCase[i + 1][j + 1]++;
                    }
                }
                //Diagonale Sud-Est :
                if (plateau[i][j].voisinSE != 0 && plateau[i + 1][j + 1].isEmpty == false &&
                    plateau[i + 1][j + 1].pion->equipe->equipe == 1) {
                    if (plateau[i + 1][j - 1].voisinSE != 0 && plateau[i + 2][j + 2].isEmpty == true) {
                        valeurCase[i + 2][j + 2]++;
                    }
                    if (plateau[i][j].voisinNO != 0 && plateau[i - 1][j - 1].isEmpty == true) {
                        valeurCase[i - 1][j - 1]++;
                    }
                }
                //Diagonale Sud-Ouest :
                if (plateau[i][j].voisinSO != 0 && plateau[i + 1][j - 1].isEmpty == false &&
                    plateau[i + 1][j - 1].pion->equipe->equipe == 1) {
                    if (plateau[i + 1][j + 1].voisinSO != 0 && plateau[i + 2][j - 2].isEmpty == true) {
                        valeurCase[i + 2][j - 2]++;
                    }
                    if (plateau[i][j].voisinNE != 0 && plateau[i - 1][j + 1].isEmpty == true) {
                        valeurCase[i - 1][j + 1]++;
                    }
                }
                //On check si le joueur 1 a formé une ligne verticale avec nbPionAligner-1 pions et si c'est le cas, on incrémente de 1000 la valeur de la case qu'il manque pour compléter la ligne de nbPionAligner pions, si elle est libre
                //Vers le Nord :
                // On va vers le haut tant qu'il y a un voisin Nord différent de 0 et que la case n'est pas vide, et qu'un pion de l'équipe 1 est dessus, et on incrémente un compteur
                int compteur = 0;
                int k = i;
                while (plateau[k][j].voisinN != 0 && plateau[k][j].isEmpty == false && plateau[k][j].pion->equipe->equipe == 1) {
                    compteur++;
                    k--;
                }
                //Si le compteur est égal à nbPionAligner-2, on incrémente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 2 && plateau[k][j].isEmpty == true) {
                    valeurCase[k][j] += 1000;
                }
                //Vers le Sud :
                // On va vers le bas tant qu'il y a un voisin Sud différent de 0 et que la case n'est pas vide, et qu'un pion de l'équipe 1 est dessus, et on incrémente un compteur
                compteur = 0;
                k = i;
                while (plateau[k][j].voisinS != 0 && plateau[k][j].isEmpty == false && plateau[k][j].pion->equipe->equipe == 1) {
                    compteur++;
                    k++;
                }
                //Si le compteur est égal à nbPionAligner-2, on incrémente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 2 && plateau[k][j].isEmpty == true) {
                    valeurCase[k][j] += 1000;
                }
                //Vers l'Est :
                // On va vers la droite tant qu'il y a un voisin Est différent de 0 et que la case n'est pas vide, et qu'un pion de l'équipe 1 est dessus, et on incrémente un compteur
                compteur = 0;
                k = j;
                while (plateau[i][k].voisinE != 0 && plateau[i][k].isEmpty == false && plateau[i][k].pion->equipe->equipe == 1) {
                    compteur++;
                    k++;
                }
                //Si le compteur est égal à nbPionAligner-2, on incrémente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 2 && plateau[i][k].isEmpty == true) {
                    valeurCase[i][k] += 1000;
                }
                //Vers l'Ouest :
                // On va vers la gauche tant qu'il y a un voisin Ouest différent de 0 et que la case n'est pas vide, et qu'un pion de l'équipe 1 est dessus, et on incrémente un compteur
                compteur = 0;
                k = j;
                while (plateau[i][k].voisinO != 0 && plateau[i][k].isEmpty == false && plateau[i][k].pion->equipe->equipe == 1) {
                    compteur++;
                    k--;
                }
                //Si le compteur est égal à nbPionAligner-2, on incrémente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 2 && plateau[i][k].isEmpty == true) {
                    valeurCase[i][k] += 1000;
                }
                //Diagonale Nord-Ouest :
                // On va vers la gauche et vers le haut tant qu'il y a un voisin Nord-Ouest différent de 0 et que la case n'est pas vide, et qu'un pion de l'équipe 1 est dessus, et on incrémente un compteur
                compteur = 0;
                k = i;
                int l = j;
                while (plateau[k][l].voisinNO != 0 && plateau[k][l].isEmpty == false && plateau[k][l].pion->equipe->equipe == 1) {
                    compteur++;
                    k--;
                    l--;
                }
                //Si le compteur est égal à nbPionAligner-2, on incrémente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 2 && plateau[k][l].isEmpty == true) {
                    valeurCase[k][l] += 1000;
                }
                //Diagonale Nord-Est :
                // On va vers la droite et vers le haut tant qu'il y a un voisin Nord-Est différent de 0 et que la case n'est pas vide, et qu'un pion de l'équipe 1 est dessus, et on incrémente un compteur
                compteur = 0;
                k = i;
                l = j;
                while (plateau[k][l].voisinNE != 0 && plateau[k][l].isEmpty == false && plateau[k][l].pion->equipe->equipe == 1) {
                    compteur++;
                    k--;
                    l++;
                }
                //Si le compteur est égal à nbPionAligner-2, on incrémente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 2 && plateau[k][l].isEmpty == true) {
                    valeurCase[k][l] += 1000;
                }
                //Diagonale Sud-Ouest :
                // On va vers la gauche et vers le bas tant qu'il y a un voisin Sud-Ouest différent de 0 et que la case n'est pas vide, et qu'un pion de l'équipe 1 est dessus, et on incrémente un compteur
                compteur = 0;
                k = i;
                l = j;
                while (plateau[k][l].voisinSO != 0 && plateau[k][l].isEmpty == false && plateau[k][l].pion->equipe->equipe == 1) {
                    compteur++;
                    k++;
                    l--;
                }
                //Si le compteur est égal à nbPionAligner-2, on incrémente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 2 && plateau[k][l].isEmpty == true) {
                    valeurCase[k][l] += 1000;
                }
                //Diagonale Sud-Est :
                // On va vers la droite et vers le bas tant qu'il y a un voisin Sud-Est différent de 0 et que la case n'est pas vide, et qu'un pion de l'équipe 1 est dessus, et on incrémente un compteur
                compteur = 0;
                k = i;
                l = j;
                while (plateau[k][l].voisinSE != 0 && plateau[k][l].isEmpty == false && plateau[k][l].pion->equipe->equipe == 1) {
                    compteur++;
                    k++;
                    l++;
                }
                //Si le compteur est égal à nbPionAligner-2, on incrémente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 2 && plateau[k][l].isEmpty == true) {
                    valeurCase[k][l] += 1000;
                }
            }
            //On check les cases qui sont occupées par un pion du robot
            if (plateau[i][j].isEmpty == false && plateau[i][j].pion->equipe->equipe == 2) {
                //Les cases qui l'entourent prennent la valeur 1 sauf si elles sont déjà occupées par un pion du joueur 1 ou du robot
                if (plateau[i][j].voisinN != 0 && plateau[i - 1][j].isEmpty == true) {
                    valeurCase[i - 1][j]++;
                }
                if (plateau[i][j].voisinS != 0 && plateau[i + 1][j].isEmpty == true) {
                    valeurCase[i + 1][j]++;
                }
                if (plateau[i][j].voisinE != 0 && plateau[i][j + 1].isEmpty == true) {
                    valeurCase[i][j + 1]++;
                }
                if (plateau[i][j].voisinO != 0 && plateau[i][j - 1].isEmpty == true) {
                    valeurCase[i][j - 1]++;
                }
                if (plateau[i][j].voisinNE != 0 && plateau[i - 1][j + 1].isEmpty == true) {
                    valeurCase[i - 1][j + 1]++;
                }
                if (plateau[i][j].voisinNO != 0 && plateau[i - 1][j - 1].isEmpty == true) {
                    valeurCase[i - 1][j - 1]++;
                }
                if (plateau[i][j].voisinSE != 0 && plateau[i + 1][j + 1].isEmpty == true) {
                    valeurCase[i + 1][j + 1]++;
                }
                if (plateau[i][j].voisinSO != 0 && plateau[i + 1][j - 1].isEmpty == true) {
                    valeurCase[i + 1][j - 1]++;
                }
                //On check si le joueur 2 (robot) a déjà formé une ligne verticale de nbPionAligner -1 pions, si c'est le cas, on incrémente de 10000 la valeur de la case qu'il manque pour compléter la ligne de nbPionAligner pions si elles sont libres
                //Ligne Nord :
                // On va vers le haut tant qu'il y a un voisin Nord différent de 0 et que la case n'est pas vide, et qu'un pion de l'équipe 2 est dessus, et on incrémente un compteur
                int compteur = 0;
                int k = i;
                int l = j;
                while (plateau[k][l].voisinN != 0 && plateau[k][l].isEmpty == false && plateau[k][l].pion->equipe->equipe == 2) {
                    compteur++;
                    k--;
                }
                //Si le compteur est égal à nbPionAligner-1, on incrémente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 1 && plateau[k][l].isEmpty == true) {
                    valeurCase[k][l] += 10000;
                }
                //Ligne Sud :
                // On va vers le bas tant qu'il y a un voisin Sud différent de 0 et que la case n'est pas vide, et qu'un pion de l'équipe 2 est dessus, et on incrémente un compteur
                compteur = 0;
                k = i;
                l = j;
                while (plateau[k][l].voisinS != 0 && plateau[k][l].isEmpty == false && plateau[k][l].pion->equipe->equipe == 2) {
                    compteur++;
                    k++;
                }
                //Si le compteur est égal à nbPionAligner-1, on incrémente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 1 && plateau[k][l].isEmpty == true) {
                    valeurCase[k][l] += 10000;
                }
                //Ligne Est :
                // On va vers la droite tant qu'il y a un voisin Est différent de 0 et que la case n'est pas vide, et qu'un pion de l'équipe 2 est dessus, et on incrémente un compteur
                compteur = 0;
                k = i;
                l = j;
                while (plateau[k][l].voisinE != 0 && plateau[k][l].isEmpty == false && plateau[k][l].pion->equipe->equipe == 2) {
                    compteur++;
                    l++;
                }
                //Si le compteur est égal à nbPionAligner-1, on incrémente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 1 && plateau[k][l].isEmpty == true) {
                    valeurCase[k][l] += 10000;
                }
                //Ligne Ouest :
                // On va vers la gauche tant qu'il y a un voisin Ouest différent de 0 et que la case n'est pas vide, et qu'un pion de l'équipe 2 est dessus, et on incrémente un compteur
                compteur = 0;
                k = i;
                l = j;
                while (plateau[k][l].voisinO != 0 && plateau[k][l].isEmpty == false && plateau[k][l].pion->equipe->equipe == 2) {
                    compteur++;
                    l--;
                }
                //Si le compteur est égal à nbPionAligner-1, on incrémente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 1 && plateau[k][l].isEmpty == true) {
                    valeurCase[k][l] += 10000;
                }
                //Ligne Nord-Est :
                // On va vers le haut et la droite tant qu'il y a un voisin Nord-Est différent de 0 et que la case n'est pas vide, et qu'un pion de l'équipe 2 est dessus, et on incrémente un compteur
                compteur = 0;
                k = i;
                l = j;
                while (plateau[k][l].voisinNE != 0 && plateau[k][l].isEmpty == false && plateau[k][l].pion->equipe->equipe == 2) {
                    compteur++;
                    k--;
                    l++;
                }
                //Si le compteur est égal à nbPionAligner-1, on incrémente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 1 && plateau[k][l].isEmpty == true) {
                    valeurCase[k][l] += 10000;
                }
                //Ligne Nord-Ouest :
                // On va vers le haut et la gauche tant qu'il y a un voisin Nord-Ouest différent de 0 et que la case n'est pas vide, et qu'un pion de l'équipe 2 est dessus, et on incrémente un compteur
                compteur = 0;
                k = i;
                l = j;
                while (plateau[k][l].voisinNO != 0 && plateau[k][l].isEmpty == false && plateau[k][l].pion->equipe->equipe == 2) {
                    compteur++;
                    k--;
                    l--;
                }
                //Si le compteur est égal à nbPionAligner-1, on incrémente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 1 && plateau[k][l].isEmpty == true) {
                    valeurCase[k][l] += 10000;
                }
                //Ligne Sud-Est :
                // On va vers le bas et la droite tant qu'il y a un voisin Sud-Est différent de 0 et que la case n'est pas vide, et qu'un pion de l'équipe 2 est dessus, et on incrémente un compteur
                compteur = 0;
                k = i;
                l = j;
                while (plateau[k][l].voisinSE != 0 && plateau[k][l].isEmpty == false && plateau[k][l].pion->equipe->equipe == 2) {
                    compteur++;
                    k++;
                    l++;
                }
                //Si le compteur est égal à nbPionAligner-1, on incrémente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 1 && plateau[k][l].isEmpty == true) {
                    valeurCase[k][l] += 10000;
                }
                //Ligne Sud-Ouest :
                // On va vers le bas et la gauche tant qu'il y a un voisin Sud-Ouest différent de 0 et que la case n'est pas vide, et qu'un pion de l'équipe 2 est dessus, et on incrémente un compteur
                compteur = 0;
                k = i;
                l = j;
                while (plateau[k][l].voisinSO != 0 && plateau[k][l].isEmpty == false && plateau[k][l].pion->equipe->equipe == 2) {
                    compteur++;
                    k++;
                    l--;
                }
                //Si le compteur est égal à nbPionAligner-1, on incrémente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 1 && plateau[k][l].isEmpty == true) {
                    valeurCase[k][l] += 10000;
                }





            }
        }
    }
}

//Fonction "placePionIA", qui permet de placer les pions de l'IA de manière intelligente afin de bloquer le joueur 1 (détection du placement des pions du joueur 1 et prise de décision) et de gagner le plus rapidement possible (détection des pions du robot et prise de décision)
//Dans un premier temps, l'intelligence artificelle analyse le placement des pions du joueur 1, et attribue une valeur à chaque case du plateau de jeu en fonction de la position des pions du joueur 1, (+ il peut bloquer le joueur 1 dans sa progression, + la valeur de la case est élevée)
// Dans un second temps, l'intelligence artificielle analyse la position des pions du joueur 2 (robot), et attribue une valeur à chaque case du plateau de jeu en fonction de la position des pions du joueur 2 (+ le robot peut se rapprocher de la victoire, + la valeur de la case est élevée)
// Enfin, l'intelligence artificielle compare les valeurs des cases du plateau de jeu, et place le pion de l'IA sur la case qui a la valeur la plus élevée
bool placePionIA2(int n,int p, struct Joueur *joueur, struct Case plateau[n][n], struct Pion pions[2][p], int NbPionAligner) {

    int i, j;
    int valeurCase[n][n];
    //Initialisation du tableau de valeurs à 0
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            valeurCase[i][j] = 0;

        }

    }
    AIeval2(n, plateau,*joueur, valeurCase, NbPionAligner);

    //On évalue les cases qui ont le plus de valeur et on pose un pion sur la case qui a la plus grande valeur (si la case est vide)
    int max = 0;
    int botPosX = 0;
    int botPosY = 0;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (valeurCase[i][j] > max && plateau[i][j].isEmpty == true){
                max = valeurCase[i][j];
                botPosX = j;
                botPosY = i;
            }
        }
    }
    printf("\n");
    //On place le pion sur la case qui a la plus grande valeur
    //On pose le pion
    if (plateau[botPosY][botPosX].isEmpty == true) {
        //Creation du pion
        struct Pion *pion = malloc(sizeof(struct Pion));
        pion->equipe = joueur;
        pion->pos = &plateau[botPosY][botPosX];
        pion->id = joueur->nbPion;
        pions[1][joueur->nbPion] = *pion;
        //On ajoute le pion au plateau
        plateau[botPosY][botPosX].pion = pion;
        plateau[botPosY][botPosX].isEmpty = false;

        //On ajoute le pion au joueur
        joueur->nbPion++;
    }

    //On check si le BOT a gagné grâce à la fonction scorePoint
    bool hasWin = false;
    hasWin = scorePoint2(n,NbPionAligner,plateau,plateau[botPosY][botPosX],botPosX,botPosY);
    // On réinitialise les valeurs des cases
    for (i = 0; i < n; i++) {
        printf("\n");
        for (j = 0; j < n; j++){
            printf(" %d", valeurCase[i][j]);
            valeurCase[i][j] = 0;
        }
    }
    return hasWin;



}

//Fonction "movePionIA" qui permet de déplacer un pion du joueur 2 (intelligence artificielle) de manière à bloquer le joueur 1 si il est sur le point de gagner ou de déplacer un pion du robot si il est sur le point de gagner
//De même que pour la fonction "placePionIA", on va évaluer les cases du plateau de jeu et on va déplacer le pion sur la case qui a la plus grande valeur
//Ici, on procède à une double évaluation, car il faut aussi évaluer le meilleur pion à déplacer.  Pour cela on analyse si il ne s'agit pas d'une case qui permet déjà de bloquer le joueur 1 ou de gagner le jeu pour le joueur 2


bool movePionIA2(int n,int p, struct Joueur *joueur, struct Case plateau[n][n], struct Pion pions[2][p], int x) {
    //D'abord on évalue le pion à déplacer
    int i, j, k, l;
    int valeurCase[n][n];
    int valeurPion[4];
    int valeurPionMax = 0;
    int pionMax = 0;
    int botPosX = 0;
    int botPosY = 0;


    //On initialise les valeurs des cases à 0
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            valeurCase[i][j] = 0;
        }
    }
    //On initialise les valeurs des pions à 0
    for (i = 0; i < 4; i++) {
        valeurPion[i] = 0;
    }

    //Si le pion permet de bloquer le joueur 1, alors il prend une valeur négative égale à -1000
    //On part de la position du pion et on analyse les cases autour du pion
    for (i = 0; i < 4; i++) {
        int botPosX = pions[1][i].pos->posX -1;
        int botPosY = pions[1][i].pos->posY -1;
        //Si le pion est entre deux pions du joueur 1, alors on lui attribue une valeur négative égale à -1
        if (botPosX > 0 && botPosX < n - 1) {
            if (plateau[botPosY][botPosX - 1].isEmpty == false && plateau[botPosY][botPosX + 1].isEmpty == false &&  plateau[botPosY][botPosX - 1].pion->equipe->equipe == 1 && plateau[botPosY][botPosX + 1].pion->equipe->equipe == 1) {
                valeurPion[i] --;
            }
        }
        if (botPosY > 0 && botPosY < n - 1) {
            if (plateau[botPosY - 1][botPosX].isEmpty == false && plateau[botPosY + 1][botPosX].isEmpty == false && plateau[botPosY - 1][botPosX].pion->equipe->equipe == 1 && plateau[botPosY + 1][botPosX].pion->equipe->equipe == 1) {
                valeurPion[i] --;
            }
        }
        // Si le pion a comme voisin de droite une ligne horizontale de x-1 pions du joueur 1, alors on lui attribue une valeur négative égale à -1000
        //Vers la droite
        if (botPosX < n - x) {
            int score = 0;
            for (k = 1; k < x; k++) {
                if (plateau[botPosY][botPosX + k].isEmpty == false && plateau[botPosY][botPosX + k].pion->equipe->equipe == 1) {
                    score++;
                }
            }
            if (score == x - 1) {
                valeurPion[i] -= 1000;
            }
        }
        // Si le pion a comme voisin de gauche une ligne horizontale de x-1 pions du joueur 1, alors on lui attribue une valeur négative égale à -1000
        //Vers la gauche
        if (botPosX > x - 2) {
            int score = 0;
            for (k = 1; k < x; k++) {
                if (plateau[botPosY][botPosX - k].isEmpty == false && plateau[botPosY][botPosX - k].pion->equipe->equipe == 1) {
                    score++;
                }
            }
            if (score == x - 1) {
                valeurPion[i] -= 1000;
            }
        }
        //Vers le haut
        if (botPosY > x - 2) {
            int score = 0;
            for (k = 1; k < x; k++) {
                if (plateau[botPosY - k][botPosX].isEmpty == false && plateau[botPosY - k][botPosX].pion->equipe->equipe == 1) {
                    score++;
                }
            }
            if (score == x - 1) {
                valeurPion[i] -= 1000;
            }
        }
        //Vers le bas
        if (botPosY < n - x) {
            int score = 0;
            for (k = 1; k < x; k++) {
                if (plateau[botPosY + k][botPosX].isEmpty == false && plateau[botPosY + k][botPosX].pion->equipe->equipe == 1) {
                    score++;
                }
            }
            if (score == x - 1) {
                valeurPion[i] -= 1000;
            }
        }
        //Diagonale haut gauche
        if (botPosX > x - 2 && botPosY > x - 2) {
            int score = 0;
            for (k = 1; k < x; k++) {
                if (plateau[botPosY - k][botPosX - k].isEmpty == false && plateau[botPosY - k][botPosX - k].pion->equipe->equipe == 1) {
                    score++;
                }
            }
            if (score == x - 1) {
                valeurPion[i] -= 1000;
            }
        }
        //Diagonale haut droite
        if (botPosX < n - x && botPosY > x - 2) {
            int score = 0;
            for (k = 1; k < x; k++) {
                if (plateau[botPosY - k][botPosX + k].isEmpty == false && plateau[botPosY - k][botPosX + k].pion->equipe->equipe == 1) {
                    score++;
                }
            }
            if (score == x - 1) {
                valeurPion[i] -= 1000;
            }
        }
        //Diagonale bas gauche
        if (botPosX > x - 2 && botPosY < n - x) {
            int score = 0;
            for (k = 1; k < x; k++) {
                if (plateau[botPosY + k][botPosX - k].isEmpty == false && plateau[botPosY + k][botPosX - k].pion->equipe->equipe == 1) {
                    score++;
                }
            }
            if (score == x - 1) {
                valeurPion[i] -= 1000;
            }
        }
        //Diagonale bas droite
        if (botPosX < n - x && botPosY < n - x) {
            int score = 0;
            for (k = 1; k < x; k++) {
                if (plateau[botPosY + k][botPosX + k].isEmpty == false && plateau[botPosY + k][botPosX + k].pion->equipe->equipe == 1) {
                    score++;
                }
            }
            if (score == x - 1) {
                valeurPion[i] -= 1000;
            }
        }



    }
    Sleep(1);

    // On cherche le pion avec la plus grande valeur
    for (i = 0; i < 4; i++) {
        if (valeurPion[i] > valeurPionMax) {
            valeurPionMax = valeurPion[i];
            pionMax = i;
        }
    }
    Sleep(1);

    //Comme dans la fonction "placePionIA" on évalue les cases en fonction du placement des pions du joueur 1
    AIeval2(n,plateau,*joueur,valeurCase,x);
    Sleep(1);

    //On évalue les cases qui ont le plus de valeur et on pose un pion sur la case qui a la plus grande valeur (si la case est vide)
    int max = 0;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (valeurCase[i][j] > max && plateau[i][j].isEmpty == true){

                max = valeurCase[i][j];
                botPosX = j;
                botPosY = i;
            }
        }
    }
    Sleep(1);
    //On déplace le pion avec la plus grande valeur sur la case qui a la plus grande valeur
    int oldPosX = pions[joueur->equipe - 1][pionMax].pos->posX -1;
    int oldPosY = pions[joueur->equipe - 1][pionMax].pos->posY -1;
    plateau[oldPosY][oldPosX].isEmpty = true;
    plateau[oldPosY][oldPosX].pion = NULL;
    plateau[botPosY][botPosX].isEmpty = false;
    plateau[botPosY][botPosX].pion = &pions[joueur->equipe - 1][pionMax];
    pions[joueur->equipe - 1][pionMax].pos->posX = botPosX + 1;
    pions[joueur->equipe - 1][pionMax].pos->posY = botPosY + 1;
    bool hasWin = false;
    hasWin = scorePoint2(n,x,plateau,plateau[botPosY][botPosX],botPosX,botPosY);
    // On réinitialise les valeurs des cases
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            valeurCase[i][j] = 0;
        }
    }
    // On réinitialise les valeurs des pions
    for (i = 0; i < 4; i++) {
        valeurPion[i] = 0;
    }
    return hasWin;
}

void IA2() {
    int x, n, i, j, p;
    do {
        printf("\n-> Veuillez saisir une longueur supérieure ou égale à 4 : ");
        scanf("%d", &n);
    } while (n < 4 || n < 0);
    // On demande le nombre de pions strictement positif et inérieur à la longueur du plateau divisée par deux
    do {
        printf("\n-> Veuillez saisir un nombre de pions strictement positif et inférieur à %d : ", n / 2);
        scanf("%d", &p);
    } while (p < 1 || p > (n / 2)-1);
    //On demande au joueur de choisir le nombre de pions à aligner ( inférieur ou égal à p)
    do {
        printf("\n-> Veuillez saisir un nombre de pions à aligner strictement positif et inférieur ou égal à %d : ", p);
        scanf("%d", &x);
    } while (x < 1 || x > p);

    //Création du plateau de jeu
    struct Case plateau[n][n];
    CreatePlateau(n, plateau);
    struct Pion pions[2][p];

    //Création des joueurs
    struct Joueur joueur1 = (struct Joueur) {1, 1, 0, 0, 'X', 12, 0, 0, false};
    struct Joueur joueur2 = (struct Joueur) {2, 2, 0, 0, 'O', 2, 0, 0, false};
    bool hasWin = false;
    int winner = 0;
    //Placement des pions du joueur 1 et intelligence artificielle pour un placement automatique des pions du joueur 2
    while (joueur2.nbPion <= p - 1) {
        hasWin = placePion2(n, p,  plateau, &joueur1,pions,x);
        if (hasWin == false) {
            hasWin = placePionIA2(n, p, &joueur2, plateau, pions, x);
            if (hasWin) {
                winner = 2;
                break;
            }
        } else {
            winner = 1;
            break;
        }
    }
    do {
        if (!hasWin){
            hasWin = movePion2(n,p,plateau,&joueur1,pions,x);
            if (hasWin == false){
                hasWin = movePionIA2(n,p,&joueur2,plateau,pions,x);
                if (hasWin){
                    winner = 2;
                    break;
                }
            }else{
                winner = 1;
                break;
            }
        }else {
            break;
        }
    } while (hasWin == false);

    printf("\n");
    printf("---------------------");
    printf("\n");
    printf("-> Le joueur %d a gagné\n", winner);
    sleep(15);
}


void AffichageMenu(int choix){
    switch (choix){
        case 1:
            printf("[");
            textColor(4,0);
            printf("%c",(char) 219);
            textColor(15,0);
            printf("] Jouer contre l'ordinateur\n");
            printf("[ ] Jouer contre un autre joueur\n");
            break;
        case 2:
            printf("[ ] Jouer contre l'ordinateur\n");
            printf("[");
            textColor(4,0);
            printf("%c",(char) 219);
            textColor(15,0);
            printf("] Jouer contre un autre joueur\n");
            break;
        default:

            break;
    }
}


//Fonction menu qui permet de sélectionner entre jouer contre l'ordinateur ou contre un autre joueur
//Naviguation avec les flèches du clavier et validation avec la touche entrée
void menu(){
    int choix = 1;
    bool hasSelected = false;

    system("cls");
    printf("-> Bienvenue dans le jeu de Morpion\n");
    printf("-> Veuillez choisir une option\n");
    AffichageMenu(choix);


    do{
        if (GetAsyncKeyState(VK_UP) != 0){
            choix = 1;
            clearScreen();
            printf("-> Bienvenue dans le jeu de Morpion\n");
            printf("-> Veuillez choisir une option\n");
            AffichageMenu(choix);

        }
        if (GetAsyncKeyState(VK_DOWN) != 0){
            choix = 2;
            clearScreen();
            printf("-> Bienvenue dans le jeu de Morpion\n");
            printf("-> Veuillez choisir une option\n");
            AffichageMenu(choix);

        }
        if (GetAsyncKeyState(VK_RETURN) != 0){
            printf("-> Vous avez choisi l'option %d\n", choix);
            sleep(1);
            clearScreen();
            hasSelected = true;
        }




    }while (!hasSelected);
    switch (choix){
        case 1:
            IA2();
            break;
        case 2:
            MultiJoueur2();
            break;
        default:
            break;
    }
}



int main()
{
    menu();
    return 0;

}







