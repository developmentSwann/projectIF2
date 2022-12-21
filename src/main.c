#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include <windows.h>
#include <conio.h>
#include <unistd.h>

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
bool scorePoint(int n,struct Case plateau[n][n],struct Case actualCase,int posX,int posY){
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

    if (compteur >= 3){
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
    if (compteur >= 3){
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
    if (compteur >= 3){
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
    if (compteur >= 3){
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
    if (compteur >= 1){
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
    if (compteur >= 1){
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
    if (compteur >= 1){
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
    if (compteur >= 1){
        return true;
    }

    if (compteur >= 3){
        return true;
    }
    return false;

}
//selecType = 0 pour placement des pions, 1 pour déplacement des pions
void CaseSelector(int n,struct Case plateau[n][n], struct Joueur *joueur,struct Pion pions[4][4], int *posX, int *posY,int selecType){
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
                sleep(0.5);
                pressedScore = 0;
            }
        }





    }
}



bool placePion(int n,struct Joueur *joueur, struct Case plateau[n][n], struct Pion pions[4][4]){
    bool hasWin = false;
    int posX,posY = 0;
    affichagePlateau(n, plateau,joueur,0);
    printf("\n");
    printf("---------------------");
    printf("\n");
    printf("-> Joueur %d, placez votre pion\n", joueur->equipe);
    CaseSelector(n, plateau, joueur, pions,&posX, &posY,0);
    hasWin = scorePoint(n,plateau,plateau[posY][posX],posX,posY);
    {if (hasWin == true) {
        printf("\nLe joueur %d a gagné", joueur->equipe);
    }
        return hasWin;
    }


}

bool movePion(int n,struct Joueur *joueur, struct Case plateau[n][n], struct Pion pions[4][4]){
    printf("\033[1;31m");
    printf("\n-> Déplacement des pions");
    printf("\033[0m");
    //Sélectionne une case avec un pion du joueur actuel et déplace ce pion sur une case vide
    int posX,posY,posX2,posY2 = 0;
    affichagePlateau(n, plateau,joueur,1);
    printf("\n");
    printf("---------------------");
    printf("\n");
    printf("-> Joueur %d, choisissez votre pion\n", joueur->equipe);
    CaseSelector(n, plateau, joueur, pions,&posX, &posY,1);
    printf("\n");
    printf("---------------------");
    printf("\n");
    printf("-> Joueur %d, choisissez la case de destination\n", joueur->equipe);
    CaseSelector(n, plateau, joueur, pions,&posX, &posY,2);
    return scorePoint(n,plateau,plateau[posY][posX],posX,posY);





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


void MultiJoueur(){
    int n, i,j;;
    while (n < 4){
        printf("-> Veuillez saisir une longueur supérieure ou égale à 4 : ");
        scanf("%d", &n);
    }
    //Création du plateau de jeu
    struct Case plateau[n][n];
    CreatePlateau(n, plateau);
    struct Pion pions[4][4];

    //Création des joueurs
    struct Joueur joueur1 = (struct Joueur){1,1,0,0,'X',12, 0,0,false};
    struct Joueur joueur2 = (struct Joueur){2,2,0,0,'O',2,0,0,false};
    bool hasWin = false;
    int winner= 0;
    while (joueur2.nbPion <=3){
            hasWin = placePion(n,&joueur1,plateau,pions);
            if (hasWin == false){
                hasWin = placePion(n,&joueur2,plateau,pions);
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
        hasWin = movePion(n,&joueur1,plateau,pions);
        if (hasWin == false){
            hasWin = movePion(n,&joueur2,plateau,pions);
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
    sleep(15);
}


void AIeval(int n, struct Case plateau[n][n], struct Joueur joueur, int valeurCase[n][n]){
    //Attribution de valeurs aux cases du plateau de jeu en fonction de la position des pions du joueur 1
    int i,j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            //Check si la case est occupée par un pion du joueur 1
            if (plateau[i][j].isEmpty == false && plateau[i][j].pion->equipe->equipe == 1) {
                //Les cases qui l'entourent prennent la valeur 1 sauf si elles sont déjà occupées par un pion du joueur 1 ou du robot
                if (plateau[i][j].voisinN != 0 && plateau[i - 1][j].isEmpty == true) {
                    valeurCase[i - 1][j]++;
                }
                if (plateau[i][j].voisinS != 0 && plateau[i + 1][j].isEmpty == true) {
                    valeurCase[i + 1][j] ++;
                }
                if (plateau[i][j].voisinE != 0 && plateau[i][j + 1].isEmpty == true) {
                    valeurCase[i][j + 1] ++;
                }
                if (plateau[i][j].voisinO != 0 && plateau[i][j - 1].isEmpty == true) {
                    valeurCase[i][j - 1] ++;
                }
                if (plateau[i][j].voisinNE != 0 && plateau[i - 1][j + 1].isEmpty == true) {
                    valeurCase[i - 1][j + 1] ++;
                }
                if (plateau[i][j].voisinNO != 0 && plateau[i - 1][j - 1].isEmpty == true) {
                    valeurCase[i - 1][j - 1] ++;
                }
                if (plateau[i][j].voisinSE != 0 && plateau[i + 1][j + 1].isEmpty == true) {
                    valeurCase[i + 1][j + 1] ++;
                }
                if (plateau[i][j].voisinSO != 0 && plateau[i + 1][j - 1].isEmpty == true) {
                    valeurCase[i + 1][j - 1] ++;
                }
                //On check si le joueur 1 est entrain de former une diaonale avec deux pions, et si c'est le cas on incrémente la valeur des cases aux extrémités de la diagonale si elles sont libres
                //Diagonale Nord-Est :
                if (plateau[i][j].voisinNE != 0 && plateau[i - 1][j + 1].isEmpty == false && plateau[i - 1][j + 1].pion->equipe->equipe == 1) {
                    if (plateau[i-1][j-1].voisinNE != 0 && plateau[i - 2][j + 2].isEmpty == true) {
                        valeurCase[i - 2][j + 2] ++;

                    }
                    if (plateau[i][j].voisinSO != 0 && plateau[i + 1][j - 1].isEmpty == true) {
                        valeurCase[i + 1][j - 1] ++;
                    }
                }
                //Diagonale Nord-Ouest :
                if (plateau[i][j].voisinNO != 0 && plateau[i - 1][j - 1].isEmpty == false && plateau[i - 1][j - 1].pion->equipe->equipe == 1) {
                    if (plateau[i-1][j+1].voisinNO != 0 && plateau[i - 2][j - 2].isEmpty == true) {
                        valeurCase[i - 2][j - 2] ++;
                    }
                    if (plateau[i][j].voisinSE != 0 && plateau[i + 1][j + 1].isEmpty == true) {
                        valeurCase[i + 1][j + 1] ++;
                    }
                }
                //Diagonale Sud-Est :
                if (plateau[i][j].voisinSE != 0 && plateau[i + 1][j + 1].isEmpty == false && plateau[i + 1][j + 1].pion->equipe->equipe == 1) {
                    if (plateau[i+1][j-1].voisinSE != 0 && plateau[i + 2][j + 2].isEmpty == true) {
                        valeurCase[i + 2][j + 2] ++;
                    }
                    if (plateau[i][j].voisinNO != 0 && plateau[i - 1][j - 1].isEmpty == true) {
                        valeurCase[i - 1][j - 1] ++;
                    }
                }
                //Diagonale Sud-Ouest :
                if (plateau[i][j].voisinSO != 0 && plateau[i + 1][j - 1].isEmpty == false && plateau[i + 1][j - 1].pion->equipe->equipe == 1) {
                    if (plateau[i+1][j+1].voisinSO != 0 && plateau[i + 2][j - 2].isEmpty == true) {
                        valeurCase[i + 2][j - 2] ++;
                    }
                    if (plateau[i][j].voisinNE != 0 && plateau[i - 1][j + 1].isEmpty == true) {
                        valeurCase[i - 1][j + 1] ++;
                    }
                }
                //On check si le joueur 1 a formé une ligne verticale avec 3 pions et si c'est le cas, on incrémente de 10000 la valeur de la case qu'il manque pour compléter la ligne de 4 si elle est libre
                //En haut :
                if (plateau[i][j].voisinN != 0 && plateau[i - 1][j].isEmpty == false && plateau[i - 1][j].pion->equipe->equipe == 1) {
                    if (plateau[i-1][j].voisinN != 0  && plateau[i - 2][j].isEmpty == false && plateau[i - 2][j].pion->equipe->equipe == 1) {
                        if (plateau[i-2][j].voisinN != 0 && plateau[i - 3][j].isEmpty == true) {
                            valeurCase[i - 3][j] += 10000;
                        }
                    }
                }
                //En bas :
                if (plateau[i][j].voisinS != 0 && plateau[i + 1][j].isEmpty == false && plateau[i + 1][j].pion->equipe->equipe == 1) {
                    if (plateau[i+1][j].voisinS != 0  && plateau[i + 2][j].isEmpty == false && plateau[i + 2][j].pion->equipe->equipe == 1) {
                        if (plateau[i+2][j].voisinS != 0 && plateau[i + 3][j].isEmpty == true) {
                            valeurCase[i + 3][j] += 10000;
                        }
                    }
                }
                //A droite :
                if (plateau[i][j].voisinE != 0 && plateau[i][j + 1].isEmpty == false && plateau[i][j + 1].pion->equipe->equipe == 1) {
                    if (plateau[i][j+1].voisinE != 0  && plateau[i][j + 2].isEmpty == false && plateau[i][j + 2].pion->equipe->equipe == 1) {
                        if (plateau[i][j+2].voisinE != 0 && plateau[i][j + 3].isEmpty == true) {
                            valeurCase[i][j + 3] += 10000;
                        }
                    }
                }
                //A gauche :
                if (plateau[i][j].voisinO != 0 && plateau[i][j - 1].isEmpty == false && plateau[i][j - 1].pion->equipe->equipe == 1) {
                    if (plateau[i][j-1].voisinO != 0  && plateau[i][j - 2].isEmpty == false && plateau[i][j - 2].pion->equipe->equipe == 1) {
                        if (plateau[i][j-2].voisinO != 0 && plateau[i][j - 3].isEmpty == true) {
                            valeurCase[i][j - 3] += 10000;
                        }
                    }
                }
                //Diagonale Nord-Est :
                if (plateau[i][j].voisinNE != 0 && plateau[i - 1][j + 1].isEmpty == false && plateau[i - 1][j + 1].pion->equipe->equipe == 1) {
                    if (plateau[i-1][j+1].voisinNE != 0  && plateau[i - 2][j + 2].isEmpty == false && plateau[i - 2][j + 2].pion->equipe->equipe == 1) {
                        if (plateau[i-2][j+2].voisinNE != 0 && plateau[i - 3][j + 3].isEmpty == true) {
                            valeurCase[i - 3][j + 3] += 10000;
                        }
                    }
                }
                //Diagonale Nord-Ouest :
                if (plateau[i][j].voisinNO != 0 && plateau[i - 1][j - 1].isEmpty == false && plateau[i - 1][j - 1].pion->equipe->equipe == 1) {
                    if (plateau[i-1][j-1].voisinNO != 0  && plateau[i - 2][j - 2].isEmpty == false && plateau[i - 2][j - 2].pion->equipe->equipe == 1) {
                        if (plateau[i-2][j-2].voisinNO != 0 && plateau[i - 3][j - 3].isEmpty == true) {
                            valeurCase[i - 3][j - 3] += 10000;
                        }
                    }
                }
                //Diagonale Sud-Est :
                if (plateau[i][j].voisinSE != 0 && plateau[i + 1][j + 1].isEmpty == false && plateau[i + 1][j + 1].pion->equipe->equipe == 1) {
                    if (plateau[i+1][j+1].voisinSE != 0  && plateau[i + 2][j + 2].isEmpty == false && plateau[i + 2][j + 2].pion->equipe->equipe == 1) {
                        if (plateau[i+2][j+2].voisinSE != 0 && plateau[i + 3][j + 3].isEmpty == true) {
                            valeurCase[i + 3][j + 3] += 10000;
                        }
                    }
                }
                //Diagonale Sud-Ouest :
                if (plateau[i][j].voisinSO != 0 && plateau[i + 1][j - 1].isEmpty == false && plateau[i + 1][j - 1].pion->equipe->equipe == 1) {
                    if (plateau[i+1][j-1].voisinSO != 0  && plateau[i + 2][j - 2].isEmpty == false && plateau[i + 2][j - 2].pion->equipe->equipe == 1) {
                        if (plateau[i+2][j-2].voisinSO != 0 && plateau[i + 3][j - 3].isEmpty == true) {
                            valeurCase[i + 3][j - 3] += 10000;
                        }
                    }
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
                //On check si le joueur 2 (robot) a déjà formé une ligne verticale de 3 pions, si c'est le cas, on incrémente de 1000 la valeur de la case qu'il manque pour compléter la ligne de 4 si elles sont libres
                //Vers le haut
                if (plateau[i][j].voisinN != 0 && plateau[i - 1][j].isEmpty == false && plateau[i - 1][j].pion->equipe->equipe == 2) {
                    if (plateau[i-1][j].voisinN != 0 && plateau[i - 2][j].isEmpty == false && plateau[i - 2][j].pion->equipe->equipe == 2) {
                        if (plateau[i-2][j].voisinN != 0 && plateau[i - 3][j].isEmpty == true) {
                            valeurCase[i - 3][j] += 1000;
                        }
                    }
                }
                //Vers le bas
                if (plateau[i][j].voisinS != 0 && plateau[i + 1][j].isEmpty == false && plateau[i + 1][j].pion->equipe->equipe == 2) {
                    if (plateau[i+1][j].voisinS != 0 && plateau[i + 2][j].isEmpty == false && plateau[i + 2][j].pion->equipe->equipe == 2) {
                        if (plateau[i+2][j].voisinS != 0 && plateau[i + 3][j].isEmpty == true) {
                            valeurCase[i + 3][j] += 1000;
                        }
                    }
                }
                //Vers la droite
                if (plateau[i][j].voisinE != 0 && plateau[i][j + 1].isEmpty == false && plateau[i][j + 1].pion->equipe->equipe == 2) {
                    if (plateau[i][j+1].voisinE != 0 && plateau[i][j + 2].isEmpty == false && plateau[i][j + 2].pion->equipe->equipe == 2) {
                        if (plateau[i][j+2].voisinE != 0 && plateau[i][j + 3].isEmpty == true) {
                            valeurCase[i][j + 3] += 1000;
                        }
                    }
                }
                //Vers la gauche
                if (plateau[i][j].voisinO != 0 && plateau[i][j - 1].isEmpty == false && plateau[i][j - 1].pion->equipe->equipe == 2) {
                    if (plateau[i][j-1].voisinO != 0 && plateau[i][j - 2].isEmpty == false && plateau[i][j - 2].pion->equipe->equipe == 2) {
                        if (plateau[i][j-2].voisinO != 0 && plateau[i][j - 3].isEmpty == true) {
                            valeurCase[i][j - 3] += 1000;
                        }
                    }
                }
                //Diagonale Nord-Est :
                if (plateau[i][j].voisinNE != 0 && plateau[i - 1][j + 1].isEmpty == false && plateau[i - 1][j + 1].pion->equipe->equipe == 2) {
                    if (plateau[i-1][j+1].voisinNE != 0 && plateau[i - 2][j + 2].isEmpty == false && plateau[i - 2][j + 2].pion->equipe->equipe == 2) {
                        if (plateau[i-2][j+2].voisinNE != 0 && plateau[i - 3][j + 3].isEmpty == true) {
                            valeurCase[i - 3][j + 3] += 1000;
                        }
                    }
                }
                //Diagonale Nord-Ouest :
                if (plateau[i][j].voisinNO != 0 && plateau[i - 1][j - 1].isEmpty == false && plateau[i - 1][j - 1].pion->equipe->equipe == 2) {
                    if (plateau[i-1][j-1].voisinNO != 0 && plateau[i - 2][j - 2].isEmpty == false && plateau[i - 2][j - 2].pion->equipe->equipe == 2) {
                        if (plateau[i-2][j-2].voisinNO != 0 && plateau[i - 3][j - 3].isEmpty == true) {
                            valeurCase[i - 3][j - 3] += 1000;
                        }
                    }
                }
                //Diagonale Sud-Est :
                if (plateau[i][j].voisinSE != 0 && plateau[i + 1][j + 1].isEmpty == false && plateau[i + 1][j + 1].pion->equipe->equipe == 2) {
                    if (plateau[i+1][j+1].voisinSE != 0 && plateau[i + 2][j + 2].isEmpty == false && plateau[i + 2][j + 2].pion->equipe->equipe == 2) {
                        if (plateau[i+2][j+2].voisinSE != 0 && plateau[i + 3][j + 3].isEmpty == true) {
                            valeurCase[i + 3][j + 3] += 1000;
                        }
                    }
                }
                //Diagonale Sud-Ouest :
                if (plateau[i][j].voisinSO != 0 && plateau[i + 1][j - 1].isEmpty == false && plateau[i + 1][j - 1].pion->equipe->equipe == 2) {
                    if (plateau[i+1][j-1].voisinSO != 0 && plateau[i + 2][j - 2].isEmpty == false && plateau[i + 2][j - 2].pion->equipe->equipe == 2) {
                        if (plateau[i+2][j-2].voisinSO != 0 && plateau[i + 3][j - 3].isEmpty == true) {
                            valeurCase[i + 3][j - 3] += 1000;
                        }
                    }
                }
            }
        }
    }
}
//Fonction "placePionIA", qui permet de placer les pions de l'IA de manière intelligente afin de bloquer le joueur 1 (détection du placement des pions du joueur 1 et prise de décision) et de gagner le plus rapidement possible (détection des pions du robot et prise de décision)
//Dans un premier temps, l'intelligence artificelle analyse le placement des pions du joueur 1, et attribue une valeur à chaque case du plateau de jeu en fonction de la position des pions du joueur 1, (+ il peut bloquer le joueur 1 dans sa progression, + la valeur de la case est élevée)
// Dans un second temps, l'intelligence artificielle analyse la position des pions du joueur 2 (robot), et attribue une valeur à chaque case du plateau de jeu en fonction de la position des pions du joueur 2 (+ le robot peut se rapprocher de la victoire, + la valeur de la case est élevée)
// Enfin, l'intelligence artificielle compare les valeurs des cases du plateau de jeu, et place le pion de l'IA sur la case qui a la valeur la plus élevée
bool placePionIA(int n, struct Joueur *joueur, struct Case plateau[n][n], struct Pion pions[4][4]) {

    int i, j;
    int valeurCase[n][n];
    //Initialisation du tableau de valeurs à 0
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            valeurCase[i][j] = 0;

        }

    }
    AIeval(n, plateau,*joueur, valeurCase);

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
    hasWin = scorePoint(n,plateau,plateau[botPosY][botPosX],botPosX,botPosY);
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


bool movePionIA(int n, struct Joueur *joueur, struct Case plateau[n][n], struct Pion pions[4][4]) {
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
        // Si le pion a comme voisin de droite une ligne horizontale de 3 pions du joueur 1, alors on lui attribue une valeur négative égale à -1000
        if (botPosX < n - 3) {
            if (plateau[botPosY][botPosX + 1].isEmpty == false && plateau[botPosY][botPosX + 2].isEmpty == false && plateau[botPosY][botPosX + 3].isEmpty == false && plateau[botPosY][botPosX + 1].pion->equipe->equipe == 1 && plateau[botPosY][botPosX + 2].pion->equipe->equipe == 1 && plateau[botPosY][botPosX + 3].pion->equipe->equipe == 1) {
                valeurPion[i] --;
            }
        }
         //Si le pion a comme voisin de gauche une ligne horizontale de 3 pions du joueur 1, alors on lui attribue une valeur négative égale à -1000
        if (botPosX > 2) {
            if (plateau[botPosY][botPosX - 1].isEmpty == false && plateau[botPosY][botPosX - 2].isEmpty == false && plateau[botPosY][botPosX - 3].isEmpty == false && plateau[botPosY][botPosX - 1].pion->equipe->equipe == 1 && plateau[botPosY][botPosX - 2].pion->equipe->equipe == 1 && plateau[botPosY][botPosX - 3].pion->equipe->equipe == 1) {
                valeurPion[i] --;
            }
        }
        //Si le pion a comme voisin du haut une ligne verticale de 3 pions du joueur 1, alors on lui attribue une valeur négative égale à -1000
        if (botPosY > 2) {
            if (plateau[botPosY - 1][botPosX].isEmpty == false && plateau[botPosY - 2][botPosX].isEmpty == false && plateau[botPosY - 3][botPosX].isEmpty == false && plateau[botPosY - 1][botPosX].pion->equipe->equipe == 1 && plateau[botPosY - 2][botPosX].pion->equipe->equipe == 1 && plateau[botPosY - 3][botPosX].pion->equipe->equipe == 1) {
                valeurPion[i] = valeurPion[i] - 1000;
            }
        }
        //Si le pion a comme voisin du bas une ligne verticale de 3 pions du joueur 1, alors on lui attribue une valeur négative égale à -1000
        if (botPosY < n - 3) {
            if (plateau[botPosY + 1][botPosX].isEmpty == false && plateau[botPosY + 2][botPosX].isEmpty == false && plateau[botPosY + 3][botPosX].isEmpty == false && plateau[botPosY + 1][botPosX].pion->equipe->equipe == 1 && plateau[botPosY + 2][botPosX].pion->equipe->equipe == 1 && plateau[botPosY + 3][botPosX].pion->equipe->equipe == 1) {
                valeurPion[i] = valeurPion[i] - 1000;
            }
        }
        //Si le pion a comme voisin du haut à droite une diagonale de 3 pions du joueur 1, alors on lui attribue une valeur négative égale à -1000
        if (botPosX < n - 3 && botPosY > 2) {
            if (plateau[botPosY - 1][botPosX + 1].isEmpty == false && plateau[botPosY - 2][botPosX + 2].isEmpty == false && plateau[botPosY - 3][botPosX + 3].isEmpty == false && plateau[botPosY - 1][botPosX + 1].pion->equipe->equipe == 1 && plateau[botPosY - 2][botPosX + 2].pion->equipe->equipe == 1 && plateau[botPosY - 3][botPosX + 3].pion->equipe->equipe == 1) {
                valeurPion[i] = valeurPion[i] - 1000;
            }
        }
        //Si le pion a comme voisin du bas à gauche une diagonale de 3 pions du joueur 1, alors on lui attribue une valeur négative égale à -1000
        if (botPosX > 2 && botPosY < n - 3) {
            if (plateau[botPosY + 1][botPosX - 1].isEmpty == false && plateau[botPosY + 2][botPosX - 2].isEmpty == false && plateau[botPosY + 3][botPosX - 3].isEmpty == false && plateau[botPosY + 1][botPosX - 1].pion->equipe->equipe == 1 && plateau[botPosY + 2][botPosX - 2].pion->equipe->equipe == 1 && plateau[botPosY + 3][botPosX - 3].pion->equipe->equipe == 1) {
                valeurPion[i] = valeurPion[i] - 1000;
            }
        }
        //Si le pion a comme voisin du haut à gauche une diagonale de 3 pions du joueur 1, alors on lui attribue une valeur négative égale à -1000
        if (botPosX > 2 && botPosY > 2) {
            if (plateau[botPosY - 1][botPosX - 1].isEmpty == false && plateau[botPosY - 2][botPosX - 2].isEmpty == false && plateau[botPosY - 3][botPosX - 3].isEmpty == false && plateau[botPosY - 1][botPosX - 1].pion->equipe->equipe == 1 && plateau[botPosY - 2][botPosX - 2].pion->equipe->equipe == 1 && plateau[botPosY - 3][botPosX - 3].pion->equipe->equipe == 1) {
                valeurPion[i] = valeurPion[i] - 1000;
            }
        }
        //Si le pion a comme voisin du bas à droite une diagonale de 3 pions du joueur 1, alors on lui attribue une valeur négative égale à -1000
        if (botPosX < n - 3 && botPosY < n - 3) {
            if (plateau[botPosY + 1][botPosX + 1].isEmpty == false && plateau[botPosY + 2][botPosX + 2].isEmpty == false && plateau[botPosY + 3][botPosX + 3].isEmpty == false && plateau[botPosY + 1][botPosX + 1].pion->equipe->equipe == 1 && plateau[botPosY + 2][botPosX + 2].pion->equipe->equipe == 1 && plateau[botPosY + 3][botPosX + 3].pion->equipe->equipe == 1) {
                valeurPion[i] = valeurPion[i] - 1000;
            }
        }
    }
    sleep(1);

    // On cherche le pion avec la plus grande valeur
    for (i = 0; i < 4; i++) {
        if (valeurPion[i] > valeurPionMax) {
            valeurPionMax = valeurPion[i];
            pionMax = i;
        }
    }
    sleep(1);

    //Comme dans la fonction "placePionIA" on évalue les cases en fonction du placement des pions du joueur 1
    AIeval(n,plateau,*joueur,valeurCase);
    sleep(1);

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
    sleep(1);
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
    hasWin = scorePoint(n,plateau,plateau[botPosY][botPosX],botPosX,botPosY);
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

void IA(){
    int n, i,j;;
    while (n < 4){
        printf("-> Veuillez saisir une longueur supérieure ou égale à 4 : ");
        scanf("%d", &n);
    }

    //Création du plateau de jeu
    struct Case plateau[n][n];
    CreatePlateau(n, plateau);
    struct Pion pions[4][4];

    //Création des joueurs
    struct Joueur joueur1 = (struct Joueur){1,1,0,0,'X',12, 0,0,false};
    struct Joueur joueur2 = (struct Joueur){2,2,0,0,'O',2,0,0,false};
    bool hasWin = false;
    int winner= 0;
    //Placement des pions du joueur 1 et intelligence artificielle pour un placement automatique des pions du joueur 2
    while (joueur2.nbPion <=3){
            hasWin = placePion(n,&joueur1,plateau,pions);
            if (hasWin == false){
                hasWin = placePionIA(n,&joueur2,plateau,pions);
                if (hasWin){
                    winner = 2;
                    break;
                }
            }else{
                winner = 1;
                break;
            }
        }
 //   Déplacement des pions du joueur 1 et intelligence artificielle pour un déplacement automatique des pions du joueur 2
    do {
        if (!hasWin){
            hasWin = movePion(n,&joueur1,plateau,pions);
            if (hasWin == false){
                hasWin = movePionIA(n,&joueur2,plateau,pions);
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
            IA();
            break;
        case 2:
            MultiJoueur();
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
