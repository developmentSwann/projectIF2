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

//Fonction qui permet d'enregistrer la partie en cours en gardant les informations déjà contenues dans le fichier (statut : mulitjoueur ou AI, taille du plateau, informations des joueurs : pions etc..)
void saveGame(int n,int p, int x,struct Joueur joueur1, struct Joueur joueur2, struct Case plateau[n][n], struct Pion pions[2][p], int gameStatut){
    FILE *fichier = fopen("save.txt", "w");

    //Enregistrement sous la forme :
    // 1- Date et heure de la partie (format : jj/mm/aaaa hh:mm:ss)
    // 2- Statut de la partie (multi ou AI)
    // 3- Informations du plateau (longueur, nombre dce pions à aligner, positions des pions)
    // 4.1 - Informations du joueur 1
    // 4.2 - Informations du joueur 2
    if (fichier != NULL){
        //1
        fprintf(fichier,"\n\n");
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
        fprintf(fichier, "Nombre de pions par joueur : %d \n", p);
        fprintf(fichier, "Nombre de pions à aligner : %d \n", x);

        fprintf(fichier, "Positions des pions : \n");
        int i,j;
        for (i = 0; i < n; i++){
            for (j = 0; j < n; j++){
                if (plateau[i][j].isEmpty == false){

                    fprintf(fichier, "%d ,Pion %d, equipe %d - X: %d - Y: %d  - Voisins : N: %d - E: %d - S: %d - O: %d - NE: %d - SE: %d - SO: %d - NO: %d \n",plateau[i][j].isEmpty , plateau[i][j].pion->id, plateau[i][j].pion->equipe->equipe, plateau[i][j].posX, plateau[i][j].posY, plateau[i][j].voisinN, plateau[i][j].voisinE, plateau[i][j].voisinS, plateau[i][j].voisinO, plateau[i][j].voisinNE, plateau[i][j].voisinSE, plateau[i][j].voisinSO, plateau[i][j].voisinNO);
                }else{

                    fprintf(fichier, "%d ,Case vide - X: %d - Y: %d - Voisins : N: %d - E: %d - S: %d - O: %d - NE: %d - SE: %d - SO: %d - NO: %d \n",plateau[i][j].isEmpty , plateau[i][j].posX, plateau[i][j].posY, plateau[i][j].voisinN, plateau[i][j].voisinE, plateau[i][j].voisinS, plateau[i][j].voisinO, plateau[i][j].voisinNE, plateau[i][j].voisinSE, plateau[i][j].voisinSO, plateau[i][j].voisinNO);
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

    }else{
        printf("Erreur lors de l'ouverture du fichier");
    }



}


void choixSave(int choix){
    switch (choix){
        case 0:
            printf("[");
            textColor(4,0);
            printf("%c",(char) 219);
            textColor(15,0);
            printf("] Continuer\n");
            printf("[ ] Sauvegarder et quitter\n");
            printf("[ ] Quitter sans sauvegarder\n");
            printf("------------------\n[SPACE] pour valider\n");

            break;
        case 1:
            printf("[ ] Continuer\n");
            printf("[");
            textColor(4,0);
            printf("%c",(char) 219);
            textColor(15,0);
            printf("] Sauvegarder et quitter\n");
            printf("[ ] Quitter sans sauvegarder\n");
            printf("------------------\n[SPACE] pour valider\n");
            break;
        case 2:
            printf("[ ] Continuer\n");
            printf("[ ] Sauvegarder et quitter\n");
            printf("[");
            textColor(4,0);
            printf("%c",(char) 219);
            textColor(15,0);
            printf("] Quitter sans sauvegarder\n");
            printf("------------------\n[SPACE] pour valider\n");
            break;
        default:

            break;

    }
}


//Fonction shouldSave qui propose au joueur de sauvegarder la partie ou de continuer
// Détection grâce aux flèches directionnelles et entrée
void shouldSave(int n,int p, int x, struct Case plateau[n][n], struct Joueur *joueur1, struct Joueur *joueur2, struct Pion pions[2][p],int gameStatut){
    int choix = 0;
    printf("\n\n\n");
    printf("Voulez-vous sauvegarder la partie ?\n");
    choixSave(choix);
    bool hasSelected = false;
    do{
        if (GetAsyncKeyState(VK_UP) != 0){
            choix --;
            if (choix > 3){
                choix = 3;
            }else if (choix < 0){
                choix = 0;
            }
            clearScreen();
            printf("Voulez-vous sauvegarder la partie ?\n");
            choixSave(choix);
        }
        if (GetAsyncKeyState(VK_DOWN) != 0){
            choix ++;
            if (choix > 3){
                choix = 3;
            }else if (choix < 0){
                choix = 0;
            }
            clearScreen();
            printf("Voulez-vous sauvegarder la partie ?\n");
            choixSave(choix);
        }
        if (GetAsyncKeyState(VK_SPACE) != 0){
            printf("-> Vous avez choisi l'option %d\n", choix);
            sleep(1);
            clearScreen();
            hasSelected = true;
        }

    }while (!hasSelected);
    switch (choix){
        case 0:
            break;
        case 1:
            saveGame(n,p,x,*joueur1,*joueur2,plateau,pions,gameStatut);
            exit(0);

            break;
        case 2:
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


//Fonction "restartGame" qui permet de lancer la dernière partie sauvegardée dans le fichier save.txt
//On restaure les paramètres du plateau, et des joueurs à partir du contenu du fichier save.txt
//Exemple de sauvegarde :
//
//
//Date et heure de la partie : 27/12/2022 14:2:14
//Statut de la partie : AI
//Informations du plateau :
//Longueur du plateau : 10
//Nombre de pions à aligner : 4
//Positions des pions :
//0 ,Pion 0, equipe 1 - X: 1 - Y: 1  - Voisins : N: 0 - E: 2 - S: 11 - O: 0 - NE: 0 - SE: 12 - SO: 0 - NO: 0
//0 ,Pion 0, equipe 2 - X: 2 - Y: 1  - Voisins : N: 0 - E: 3 - S: 12 - O: 1 - NE: 0 - SE: 13 - SO: 11 - NO: 0
//1 ,Case vide - X: 3 - Y: 1 - Voisins : N: 0 - E: 4 - S: 13 - O: 2 - NE: 0 - SE: 14 - SO: 12 - NO: 0
//1 ,Case vide - X: 4 - Y: 1 - Voisins : N: 0 - E: 5 - S: 14 - O: 3 - NE: 0 - SE: 15 - SO: 13 - NO: 0
//1 ,Case vide - X: 5 - Y: 1 - Voisins : N: 0 - E: 6 - S: 15 - O: 4 - NE: 0 - SE: 16 - SO: 14 - NO: 0
//1 ,Case vide - X: 6 - Y: 1 - Voisins : N: 0 - E: 7 - S: 16 - O: 5 - NE: 0 - SE: 17 - SO: 15 - NO: 0
//1 ,Case vide - X: 7 - Y: 1 - Voisins : N: 0 - E: 8 - S: 17 - O: 6 - NE: 0 - SE: 18 - SO: 16 - NO: 0
//1 ,Case vide - X: 8 - Y: 1 - Voisins : N: 0 - E: 9 - S: 18 - O: 7 - NE: 0 - SE: 19 - SO: 17 - NO: 0
//1 ,Case vide - X: 9 - Y: 1 - Voisins : N: 0 - E: 10 - S: 19 - O: 8 - NE: 0 - SE: 20 - SO: 18 - NO: 0
//1 ,Case vide - X: 10 - Y: 1 - Voisins : N: 0 - E: 0 - S: 20 - O: 9 - NE: 0 - SE: 0 - SO: 19 - NO: 0
//1 ,Case vide - X: 1 - Y: 2 - Voisins : N: 1 - E: 12 - S: 21 - O: 0 - NE: 2 - SE: 22 - SO: 0 - NO: 0
//1 ,Case vide - X: 2 - Y: 2 - Voisins : N: 2 - E: 13 - S: 22 - O: 11 - NE: 3 - SE: 23 - SO: 21 - NO: 1
//1 ,Case vide - X: 3 - Y: 2 - Voisins : N: 3 - E: 14 - S: 23 - O: 12 - NE: 4 - SE: 24 - SO: 22 - NO: 2
//1 ,Case vide - X: 4 - Y: 2 - Voisins : N: 4 - E: 15 - S: 24 - O: 13 - NE: 5 - SE: 25 - SO: 23 - NO: 3
//1 ,Case vide - X: 5 - Y: 2 - Voisins : N: 5 - E: 16 - S: 25 - O: 14 - NE: 6 - SE: 26 - SO: 24 - NO: 4
//1 ,Case vide - X: 6 - Y: 2 - Voisins : N: 6 - E: 17 - S: 26 - O: 15 - NE: 7 - SE: 27 - SO: 25 - NO: 5
//1 ,Case vide - X: 7 - Y: 2 - Voisins : N: 7 - E: 18 - S: 27 - O: 16 - NE: 8 - SE: 28 - SO: 26 - NO: 6
//1 ,Case vide - X: 8 - Y: 2 - Voisins : N: 8 - E: 19 - S: 28 - O: 17 - NE: 9 - SE: 29 - SO: 27 - NO: 7
//1 ,Case vide - X: 9 - Y: 2 - Voisins : N: 9 - E: 20 - S: 29 - O: 18 - NE: 10 - SE: 30 - SO: 28 - NO: 8
//1 ,Case vide - X: 10 - Y: 2 - Voisins : N: 10 - E: 0 - S: 30 - O: 19 - NE: 0 - SE: 0 - SO: 29 - NO: 9
//1 ,Case vide - X: 1 - Y: 3 - Voisins : N: 11 - E: 22 - S: 31 - O: 0 - NE: 12 - SE: 32 - SO: 0 - NO: 0
//1 ,Case vide - X: 2 - Y: 3 - Voisins : N: 12 - E: 23 - S: 32 - O: 21 - NE: 13 - SE: 33 - SO: 31 - NO: 11
//1 ,Case vide - X: 3 - Y: 3 - Voisins : N: 13 - E: 24 - S: 33 - O: 22 - NE: 14 - SE: 34 - SO: 32 - NO: 12
//1 ,Case vide - X: 4 - Y: 3 - Voisins : N: 14 - E: 25 - S: 34 - O: 23 - NE: 15 - SE: 35 - SO: 33 - NO: 13
//1 ,Case vide - X: 5 - Y: 3 - Voisins : N: 15 - E: 26 - S: 35 - O: 24 - NE: 16 - SE: 36 - SO: 34 - NO: 14
//1 ,Case vide - X: 6 - Y: 3 - Voisins : N: 16 - E: 27 - S: 36 - O: 25 - NE: 17 - SE: 37 - SO: 35 - NO: 15
//1 ,Case vide - X: 7 - Y: 3 - Voisins : N: 17 - E: 28 - S: 37 - O: 26 - NE: 18 - SE: 38 - SO: 36 - NO: 16
//1 ,Case vide - X: 8 - Y: 3 - Voisins : N: 18 - E: 29 - S: 38 - O: 27 - NE: 19 - SE: 39 - SO: 37 - NO: 17
//1 ,Case vide - X: 9 - Y: 3 - Voisins : N: 19 - E: 30 - S: 39 - O: 28 - NE: 20 - SE: 40 - SO: 38 - NO: 18
//1 ,Case vide - X: 10 - Y: 3 - Voisins : N: 20 - E: 0 - S: 40 - O: 29 - NE: 0 - SE: 0 - SO: 39 - NO: 19
//1 ,Case vide - X: 1 - Y: 4 - Voisins : N: 21 - E: 32 - S: 41 - O: 0 - NE: 22 - SE: 42 - SO: 0 - NO: 0
//1 ,Case vide - X: 2 - Y: 4 - Voisins : N: 22 - E: 33 - S: 42 - O: 31 - NE: 23 - SE: 43 - SO: 41 - NO: 21
//1 ,Case vide - X: 3 - Y: 4 - Voisins : N: 23 - E: 34 - S: 43 - O: 32 - NE: 24 - SE: 44 - SO: 42 - NO: 22
//1 ,Case vide - X: 4 - Y: 4 - Voisins : N: 24 - E: 35 - S: 44 - O: 33 - NE: 25 - SE: 45 - SO: 43 - NO: 23
//1 ,Case vide - X: 5 - Y: 4 - Voisins : N: 25 - E: 36 - S: 45 - O: 34 - NE: 26 - SE: 46 - SO: 44 - NO: 24
//1 ,Case vide - X: 6 - Y: 4 - Voisins : N: 26 - E: 37 - S: 46 - O: 35 - NE: 27 - SE: 47 - SO: 45 - NO: 25
//1 ,Case vide - X: 7 - Y: 4 - Voisins : N: 27 - E: 38 - S: 47 - O: 36 - NE: 28 - SE: 48 - SO: 46 - NO: 26
//1 ,Case vide - X: 8 - Y: 4 - Voisins : N: 28 - E: 39 - S: 48 - O: 37 - NE: 29 - SE: 49 - SO: 47 - NO: 27
//1 ,Case vide - X: 9 - Y: 4 - Voisins : N: 29 - E: 40 - S: 49 - O: 38 - NE: 30 - SE: 50 - SO: 48 - NO: 28
//1 ,Case vide - X: 10 - Y: 4 - Voisins : N: 30 - E: 0 - S: 50 - O: 39 - NE: 0 - SE: 0 - SO: 49 - NO: 29
//1 ,Case vide - X: 1 - Y: 5 - Voisins : N: 31 - E: 42 - S: 51 - O: 0 - NE: 32 - SE: 52 - SO: 0 - NO: 0
//1 ,Case vide - X: 2 - Y: 5 - Voisins : N: 32 - E: 43 - S: 52 - O: 41 - NE: 33 - SE: 53 - SO: 51 - NO: 31
//1 ,Case vide - X: 3 - Y: 5 - Voisins : N: 33 - E: 44 - S: 53 - O: 42 - NE: 34 - SE: 54 - SO: 52 - NO: 32
//1 ,Case vide - X: 4 - Y: 5 - Voisins : N: 34 - E: 45 - S: 54 - O: 43 - NE: 35 - SE: 55 - SO: 53 - NO: 33
//1 ,Case vide - X: 5 - Y: 5 - Voisins : N: 35 - E: 46 - S: 55 - O: 44 - NE: 36 - SE: 56 - SO: 54 - NO: 34
//1 ,Case vide - X: 6 - Y: 5 - Voisins : N: 36 - E: 47 - S: 56 - O: 45 - NE: 37 - SE: 57 - SO: 55 - NO: 35
//1 ,Case vide - X: 7 - Y: 5 - Voisins : N: 37 - E: 48 - S: 57 - O: 46 - NE: 38 - SE: 58 - SO: 56 - NO: 36
//1 ,Case vide - X: 8 - Y: 5 - Voisins : N: 38 - E: 49 - S: 58 - O: 47 - NE: 39 - SE: 59 - SO: 57 - NO: 37
//1 ,Case vide - X: 9 - Y: 5 - Voisins : N: 39 - E: 50 - S: 59 - O: 48 - NE: 40 - SE: 60 - SO: 58 - NO: 38
//1 ,Case vide - X: 10 - Y: 5 - Voisins : N: 40 - E: 0 - S: 60 - O: 49 - NE: 0 - SE: 0 - SO: 59 - NO: 39
//1 ,Case vide - X: 1 - Y: 6 - Voisins : N: 41 - E: 52 - S: 61 - O: 0 - NE: 42 - SE: 62 - SO: 0 - NO: 0
//1 ,Case vide - X: 2 - Y: 6 - Voisins : N: 42 - E: 53 - S: 62 - O: 51 - NE: 43 - SE: 63 - SO: 61 - NO: 41
//1 ,Case vide - X: 3 - Y: 6 - Voisins : N: 43 - E: 54 - S: 63 - O: 52 - NE: 44 - SE: 64 - SO: 62 - NO: 42
//1 ,Case vide - X: 4 - Y: 6 - Voisins : N: 44 - E: 55 - S: 64 - O: 53 - NE: 45 - SE: 65 - SO: 63 - NO: 43
//1 ,Case vide - X: 5 - Y: 6 - Voisins : N: 45 - E: 56 - S: 65 - O: 54 - NE: 46 - SE: 66 - SO: 64 - NO: 44
//1 ,Case vide - X: 6 - Y: 6 - Voisins : N: 46 - E: 57 - S: 66 - O: 55 - NE: 47 - SE: 67 - SO: 65 - NO: 45
//1 ,Case vide - X: 7 - Y: 6 - Voisins : N: 47 - E: 58 - S: 67 - O: 56 - NE: 48 - SE: 68 - SO: 66 - NO: 46
//1 ,Case vide - X: 8 - Y: 6 - Voisins : N: 48 - E: 59 - S: 68 - O: 57 - NE: 49 - SE: 69 - SO: 67 - NO: 47
//1 ,Case vide - X: 9 - Y: 6 - Voisins : N: 49 - E: 60 - S: 69 - O: 58 - NE: 50 - SE: 70 - SO: 68 - NO: 48
//1 ,Case vide - X: 10 - Y: 6 - Voisins : N: 50 - E: 0 - S: 70 - O: 59 - NE: 0 - SE: 0 - SO: 69 - NO: 49
//1 ,Case vide - X: 1 - Y: 7 - Voisins : N: 51 - E: 62 - S: 71 - O: 0 - NE: 52 - SE: 72 - SO: 0 - NO: 0
//1 ,Case vide - X: 2 - Y: 7 - Voisins : N: 52 - E: 63 - S: 72 - O: 61 - NE: 53 - SE: 73 - SO: 71 - NO: 51
//1 ,Case vide - X: 3 - Y: 7 - Voisins : N: 53 - E: 64 - S: 73 - O: 62 - NE: 54 - SE: 74 - SO: 72 - NO: 52
//1 ,Case vide - X: 4 - Y: 7 - Voisins : N: 54 - E: 65 - S: 74 - O: 63 - NE: 55 - SE: 75 - SO: 73 - NO: 53
//1 ,Case vide - X: 5 - Y: 7 - Voisins : N: 55 - E: 66 - S: 75 - O: 64 - NE: 56 - SE: 76 - SO: 74 - NO: 54
//1 ,Case vide - X: 6 - Y: 7 - Voisins : N: 56 - E: 67 - S: 76 - O: 65 - NE: 57 - SE: 77 - SO: 75 - NO: 55
//1 ,Case vide - X: 7 - Y: 7 - Voisins : N: 57 - E: 68 - S: 77 - O: 66 - NE: 58 - SE: 78 - SO: 76 - NO: 56
//1 ,Case vide - X: 8 - Y: 7 - Voisins : N: 58 - E: 69 - S: 78 - O: 67 - NE: 59 - SE: 79 - SO: 77 - NO: 57
//1 ,Case vide - X: 9 - Y: 7 - Voisins : N: 59 - E: 70 - S: 79 - O: 68 - NE: 60 - SE: 80 - SO: 78 - NO: 58
//1 ,Case vide - X: 10 - Y: 7 - Voisins : N: 60 - E: 0 - S: 80 - O: 69 - NE: 0 - SE: 0 - SO: 79 - NO: 59
//1 ,Case vide - X: 1 - Y: 8 - Voisins : N: 61 - E: 72 - S: 81 - O: 0 - NE: 62 - SE: 82 - SO: 0 - NO: 0
//1 ,Case vide - X: 2 - Y: 8 - Voisins : N: 62 - E: 73 - S: 82 - O: 71 - NE: 63 - SE: 83 - SO: 81 - NO: 61
//1 ,Case vide - X: 3 - Y: 8 - Voisins : N: 63 - E: 74 - S: 83 - O: 72 - NE: 64 - SE: 84 - SO: 82 - NO: 62
//1 ,Case vide - X: 4 - Y: 8 - Voisins : N: 64 - E: 75 - S: 84 - O: 73 - NE: 65 - SE: 85 - SO: 83 - NO: 63
//1 ,Case vide - X: 5 - Y: 8 - Voisins : N: 65 - E: 76 - S: 85 - O: 74 - NE: 66 - SE: 86 - SO: 84 - NO: 64
//1 ,Case vide - X: 6 - Y: 8 - Voisins : N: 66 - E: 77 - S: 86 - O: 75 - NE: 67 - SE: 87 - SO: 85 - NO: 65
//1 ,Case vide - X: 7 - Y: 8 - Voisins : N: 67 - E: 78 - S: 87 - O: 76 - NE: 68 - SE: 88 - SO: 86 - NO: 66
//1 ,Case vide - X: 8 - Y: 8 - Voisins : N: 68 - E: 79 - S: 88 - O: 77 - NE: 69 - SE: 89 - SO: 87 - NO: 67
//1 ,Case vide - X: 9 - Y: 8 - Voisins : N: 69 - E: 80 - S: 89 - O: 78 - NE: 70 - SE: 90 - SO: 88 - NO: 68
//1 ,Case vide - X: 10 - Y: 8 - Voisins : N: 70 - E: 0 - S: 90 - O: 79 - NE: 0 - SE: 0 - SO: 89 - NO: 69
//1 ,Case vide - X: 1 - Y: 9 - Voisins : N: 71 - E: 82 - S: 91 - O: 0 - NE: 72 - SE: 92 - SO: 0 - NO: 0
//1 ,Case vide - X: 2 - Y: 9 - Voisins : N: 72 - E: 83 - S: 92 - O: 81 - NE: 73 - SE: 93 - SO: 91 - NO: 71
//1 ,Case vide - X: 3 - Y: 9 - Voisins : N: 73 - E: 84 - S: 93 - O: 82 - NE: 74 - SE: 94 - SO: 92 - NO: 72
//1 ,Case vide - X: 4 - Y: 9 - Voisins : N: 74 - E: 85 - S: 94 - O: 83 - NE: 75 - SE: 95 - SO: 93 - NO: 73
//1 ,Case vide - X: 5 - Y: 9 - Voisins : N: 75 - E: 86 - S: 95 - O: 84 - NE: 76 - SE: 96 - SO: 94 - NO: 74
//1 ,Case vide - X: 6 - Y: 9 - Voisins : N: 76 - E: 87 - S: 96 - O: 85 - NE: 77 - SE: 97 - SO: 95 - NO: 75
//1 ,Case vide - X: 7 - Y: 9 - Voisins : N: 77 - E: 88 - S: 97 - O: 86 - NE: 78 - SE: 98 - SO: 96 - NO: 76
//1 ,Case vide - X: 8 - Y: 9 - Voisins : N: 78 - E: 89 - S: 98 - O: 87 - NE: 79 - SE: 99 - SO: 97 - NO: 77
//1 ,Case vide - X: 9 - Y: 9 - Voisins : N: 79 - E: 90 - S: 99 - O: 88 - NE: 80 - SE: 100 - SO: 98 - NO: 78
//1 ,Case vide - X: 10 - Y: 9 - Voisins : N: 80 - E: 0 - S: 100 - O: 89 - NE: 0 - SE: 0 - SO: 99 - NO: 79
//1 ,Case vide - X: 1 - Y: 10 - Voisins : N: 81 - E: 92 - S: 0 - O: 0 - NE: 82 - SE: 0 - SO: 0 - NO: 0
//1 ,Case vide - X: 2 - Y: 10 - Voisins : N: 82 - E: 93 - S: 0 - O: 91 - NE: 83 - SE: 0 - SO: 0 - NO: 81
//1 ,Case vide - X: 3 - Y: 10 - Voisins : N: 83 - E: 94 - S: 0 - O: 92 - NE: 84 - SE: 0 - SO: 0 - NO: 82
//1 ,Case vide - X: 4 - Y: 10 - Voisins : N: 84 - E: 95 - S: 0 - O: 93 - NE: 85 - SE: 0 - SO: 0 - NO: 83
//1 ,Case vide - X: 5 - Y: 10 - Voisins : N: 85 - E: 96 - S: 0 - O: 94 - NE: 86 - SE: 0 - SO: 0 - NO: 84
//1 ,Case vide - X: 6 - Y: 10 - Voisins : N: 86 - E: 97 - S: 0 - O: 95 - NE: 87 - SE: 0 - SO: 0 - NO: 85
//1 ,Case vide - X: 7 - Y: 10 - Voisins : N: 87 - E: 98 - S: 0 - O: 96 - NE: 88 - SE: 0 - SO: 0 - NO: 86
//1 ,Case vide - X: 8 - Y: 10 - Voisins : N: 88 - E: 99 - S: 0 - O: 97 - NE: 89 - SE: 0 - SO: 0 - NO: 87
//1 ,Case vide - X: 9 - Y: 10 - Voisins : N: 89 - E: 100 - S: 0 - O: 98 - NE: 90 - SE: 0 - SO: 0 - NO: 88
//1 ,Case vide - X: 10 - Y: 10 - Voisins : N: 90 - E: 0 - S: 0 - O: 99 - NE: 0 - SE: 0 - SO: 0 - NO: 89
//Informations du joueur 1 :
//Nombre de pions : 1
//Informations du joueur 2 :
//Nombre de pions : 1

void restartGame() {
    //On ouvre le fichier
    FILE *fichier = fopen("save.txt", "r");
    int gameMode,boardSize,maxPawns,winPawns;
    // On lit les lignes une par une

    for (int i = 0; i < 111; i++) {
        char ligne[100];
        fgets(ligne, 100, fichier);
        //Si il s'agit de la 2eme ligne, on récupère le statut de la partie ("AI" "ou "Multi-joueur")
        if (i == 1) {
            printf("%d - %s",i,ligne);

            if (ligne[22] == 'A') {
                gameMode = 1;

            } else {
                gameMode = 2;
            }
        }
        //Si il s'agit de la 4eme ligne, on récupère la longueur du plateau
        if (i == 3){
            printf("%d - %s",i,ligne);

            //On récupère la longueur du plateau (a partir de la 23 eme lettre de la ligne), et on la convertit en int
            boardSize = ligne[22] - '0';
            //On check si la longueur du plateau est supérieur à 9
            int j = 1;
            while (ligne[22 + j] != ' '){
                boardSize = boardSize * 10 + ligne[22 + j] - '0';
                j++;
            }
        }
        //Si il s'agit de la 5eme ligne, on récupère le nombre de pions max par joueur
        if (i == 4){
            printf("%d - %s",i,ligne);

            //On récupère le nombre de pions max par joueur (a partir de la 30 eme lettre de la ligne), et on le convertit en int
            maxPawns = ligne[29] - '0';
            //On check si le nombre de pions max par joueur est supérieur à 9
            int j = 1;
            while (ligne[29 + j] != ' '){
                maxPawns = maxPawns * 10 + ligne[29 + j] - '0';
                j++;
            }
        }
        //Si
        // il s'agit de la 6eme ligne, on récupère le nombre de pions à aligner pour gagner
        if (i == 5){
            printf("%d - %s",i,ligne);
            //On récupère le nombre de pions à aligner pour gagner (a partir de la 29 eme lettre de la ligne), et on le convertit en int
//            winPawns = ligne[28] - '0';
//            //On check si le nombre de pions à aligner pour gagner est supérieur à 9
//            int j = 1;
//            while (lig
//            ne[28 + j] != ' '){
//                winPawns = winPawns * 10 + ligne[28 + j] - '0';
//                j++;
//
//            }
//            printf("Nombre de pions à aligner pour gagner : %d",winPawns);

        }




    }


}






//        // On lit les informations des joueurs
//        for (int i = 0; i < n; i++) {
//            fscanf(save, "%d %d", &joueurs[i].nbPions, &joueurs[i].nbPionsRestants);
//        }
//
//        // On lit les informations des pions
//        for (int i = 0; i < p; i++) {
//            fscanf(save, "%d %d %d", &pions[i].joueur, &pions[i].x, &pions[i].y);
//        }
//
//        // On lit les informations du plateau
//        for (int i = 0; i < N; i++) {
//            for (int j = 0; j < N; j++) {
//                fscanf(save, "%d", &plateau[i][j]);
//            }
//        }





//        //On lance la partie
//        if (gameStatut == 0){
//            bool hasWin = false;
//            int winner = 0;
//            //Placement des pions du joueur 1 et intelligence artificielle pour un placement automatique des pions du joueur 2
//            while (joueur2->nbPion <= p - 1) {
//                hasWin = placePion2(n, p,  plateau, &joueur1,pions,x);
//                if (hasWin == false) {
//                    hasWin = placePionIA2(n, p, &joueur2, plateau, pions, x);
//                    if (hasWin) {
//                        winner = 2;
//                        break;
//                    }
//
//                } else {
//                    winner = 1;
//                    break;
//                }
//            }
//            do {
//                if (!hasWin){
//                    hasWin = movePion2(n,p,plateau,&joueur1,pions,x);
//                    if (hasWin == false){
//                        hasWin = movePionIA2(n,p,&joueur2,plateau,pions,x);
//                        if (hasWin){
//                            winner = 2;
//                            break;
//                        }
//                    }else{
//                        winner = 1;
//                        break;
//                    }
//                }else {
//                    break;
//                }
//            } while (hasWin == false);
//
//            printf("\n");
//            printf("---------------------");
//            printf("\n");
//            printf("-> Le joueur %d a gagné\n", winner);
//            sleep(15);
//        }else{
//

//        }


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
        shouldSave(n,p,x,plateau,&joueur1,&joueur2,pions,1);
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
        shouldSave(n,p,x,plateau,&joueur1,&joueur2,pions,1);
    } while (hasWin == false);
    printf("\n");
    printf("---------------------");
    printf("\n");
    printf("-> Le joueur %d a gagné\n", winner);
    Sleep(15);
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
        shouldSave(n,p,x,plateau,&joueur1,&joueur2,pions,0);
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
        shouldSave(n,p,x,plateau,&joueur1,&joueur2,pions,0);
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
            printf("[ ] Charger une partie sauvegardée\n");
            break;
        case 2:
            printf("[ ] Jouer contre l'ordinateur\n");
            printf("[");
            textColor(4,0);
            printf("%c",(char) 219);
            textColor(15,0);
            printf("] Jouer contre un autre joueur\n");
            printf("[ ] Charger une partie sauvegardée\n");
            break;
        case 3:
            printf("[ ] Jouer contre l'ordinateur\n");
            printf("[ ] Jouer contre un autre joueur\n");
            printf("[");
            textColor(4,0);
            printf("%c",(char) 219);
            textColor(15,0);
            printf("] Charger une partie sauvegardée\n");
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
    printf("-> Bienvenue dans le jeu du Teeko\n");
    printf("-> Veuillez choisir une option\n");
    AffichageMenu(choix);


    do{
        if (GetAsyncKeyState(VK_UP) != 0){
            choix --;
            if (choix < 1){
                choix = 3;
            }
            clearScreen();
            printf("-> Bienvenue dans le jeu du Teeko\n");
            printf("-> Veuillez choisir une option\n");
            AffichageMenu(choix);

        }
        if (GetAsyncKeyState(VK_DOWN) != 0){
            choix  ++;
            if (choix > 3){
                choix = 1;
            }
            clearScreen();
            printf("-> Bienvenue dans le jeu du Teeko\n");
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
        case 3:
            restartGame();
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





