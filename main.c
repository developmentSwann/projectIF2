#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include <windows.h>
#include <conio.h>
#include <unistd.h>
#include <time.h>

void textColor(int couleurDuTexte,int couleurDeFond)
{
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,couleurDeFond*16+couleurDuTexte);
}

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
    char icon;
    int color;
    int posX;
    int posY;
    int hasUsedSpecial;
};



void affichagePlateau(int n,int p,struct Case plateau[n][n],struct Joueur *joueur, int selecType,int gamePhase){
    int i,j;
    int playerPosX = joueur->posX;
    int playerPosY = joueur->posY;
    for (i=1;i<(n+1);i++){
        printf("\n");
        for (j=1;j<n+1;j++){

            if (plateau[i-1][j-1].isEmpty == true){
                //permet de mettre en couleur la case que le joueur est entrain de parcourir jusqu'au moment où il la selectionne
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
    printf("---------------------\n");
    printf("Informations du joueur :\n");
    printf("Equipe : %d\n",joueur->equipe);
    printf("Pions places : %d\n",joueur->nbPion);
    printf("Pions restants : %d\n",(p-joueur->nbPion));

    printf("---------------------");
    printf("\n");
    printf("[TAB] pour valider");
    if (gamePhase == 2){
        printf(" | [S] Coup special");
    }
    printf("\n[%c] pour monter",(char)24);
    printf("\n[%c] pour descendre",(char)25);
    printf("\n[->] pour aller a droite");
    printf("\n[<-] pour aller a gauche");

}

//Fonction qui permet d'enregistrer la partie en cours en gardant les informations deja contenues dans le fichier (statut : mulitjoueur ou AI, taille du plateau, informations des joueurs : pions etc..)
void saveGame(int n,int p, int x,struct Joueur joueur1, struct Joueur joueur2, struct Case plateau[n][n], struct Pion pions[2][p], int gameStatut){
    FILE *fichier = fopen("save.txt", "w");

    //Enregistrement sous la forme :
    // 1- Date et heure de la partie (format : jj/mm/aaaa hh:mm:ss)
    // 2- Statut de la partie (multi ou AI)
    // 3- Informations du plateau (longueur, nombre dce pions a aligner, positions des pions)
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
        fprintf(fichier, "Nombre de pions a aligner : %d \n", x);

        fprintf(fichier, "Positions des pions : \n");

        int i,j;
        for (i = 0; i < n; i++){
            for (j = 0; j < n; j++){
                if (plateau[i][j].isEmpty == false){

                    fprintf(fichier, "%d ,Pion %d, equipe %d - X: %d - Y: %d  - Voisins : N: %d - E: %d - S: %d - O: %d - NE: %d - SE: %d - SO: %d - NO: %d \n",plateau[i][j].isEmpty , plateau[i][j].pion->id, plateau[i][j].pion->equipe->equipe, plateau[i][j].posX-1, plateau[i][j].posY-1, plateau[i][j].voisinN, plateau[i][j].voisinE, plateau[i][j].voisinS, plateau[i][j].voisinO, plateau[i][j].voisinNE, plateau[i][j].voisinSE, plateau[i][j].voisinSO, plateau[i][j].voisinNO);
                }else{

                    fprintf(fichier, "%d ,Case vide - X: %d - Y: %d - Voisins : N: %d - E: %d - S: %d - O: %d - NE: %d - SE: %d - SO: %d - NO: %d \n",plateau[i][j].isEmpty , plateau[i][j].posX-1, plateau[i][j].posY-1, plateau[i][j].voisinN, plateau[i][j].voisinE, plateau[i][j].voisinS, plateau[i][j].voisinO, plateau[i][j].voisinNE, plateau[i][j].voisinSE, plateau[i][j].voisinSO, plateau[i][j].voisinNO);
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
// Detection grâce aux fleches directionnelles et entree
void shouldSave(int n,int p, int x, struct Case plateau[n][n], struct Joueur *joueur1, struct Joueur *joueur2, struct Pion pions[2][p],int gameStatut){
    int choix = 0;
    clearScreen();
    printf("Voulez-vous sauvegarder la partie ?\n");
    choixSave(choix);
    bool hasSelected = false;
    do{
        if (GetAsyncKeyState(VK_UP) != 0){
            choix --;
            if (choix > 2){
                choix = 0;
            }else if (choix < 0){
                choix = 2;
            }
            clearScreen();
            printf("Voulez-vous sauvegarder la partie ?\n");
            choixSave(choix);
            Sleep(250);

        }
        if (GetAsyncKeyState(VK_DOWN) != 0){
            choix ++;
            if (choix > 2){
                choix = 0;
            }else if (choix < 0){
                choix = 2;
            }

            clearScreen();
            printf("Voulez-vous sauvegarder la partie ?\n");
            choixSave(choix);
            Sleep(250);

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
    //Verifie vers le Nord si il y a un pion de la même equipe et incremente le compteur tant qu'il y'en a un, puis retourne a la position de base pour verifier vers le Sud si il y a un pion de la même equipe et incremente le compteur tant qu'il y'en a un
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
    //Verifie vers l'Est si il y a un pion de la même equipe et incremente le compteur tant qu'il y'en a un, puis retourne a la position de base pour verifier vers l'Ouest si il y a un pion de la même equipe et incremente le compteur tant qu'il y'en a un
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
    //Verifie vers le Nord-Est si il y a un pion de la même equipe et incremente le compteur tant qu'il y'en a un, puis retourne a la position de base pour verifier vers le Sud-Ouest si il y a un pion de la même equipe et incremente le compteur tant qu'il y'en a un
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
    //Verifie vers le Nord-Ouest si il y a un pion de la même equipe et incremente le compteur tant qu'il y'en a un, puis retourne a la position de base pour verifier vers le Sud-Est si il y a un pion de la même equipe et incremente le compteur tant qu'il y'en a un
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
    // Verification de pions positionnes en carres
    compteur = 0;
    posY = defPosY;
    posX = defPosX;
    actualCase = plateau[posY][posX];
    // Carre #1 :
    //Un pion a droite du pion actuel et un pion au dessus du pion actuel et de celui de droite
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
    // Carre #2 :
    //Un pion a gauche du pion actuel et un pion au dessus du pion actuel et de celui de gauche
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
    // Carre #3 :
    //Un pion a droite du pion actuel et un pion en dessous du pion actuel et de celui de droite
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
    // Carre #4 :
    //Un pion a gauche du pion actuel et un pion en dessous du pion actuel et de celui de gauche
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

void CaseSelector2(int n,int p,int x,struct Case plateau[n][n], struct Joueur *joueur,struct Pion pions[2][p], int *posX, int *posY,int selecType,int gamePhase){
    bool hasMoove = false;
    bool hasSelected = false;
    bool hasWin = false;
    int pressedScore = 0;
    //Boucle qui permet de naviguer sur les cases grâce a une detection automatique des fleches directionnelles, puis en actualisant l'affichage du plateau et de selectionner une case avec la touche entree
    while (!hasSelected) {
        //Detecte si la touche fleche du haut est pressee
        if (GetAsyncKeyState(VK_RIGHT) && plateau[joueur->posY][joueur->posX].voisinE != 0){
            pressedScore ++;
            if (pressedScore ==1){
                joueur->posX = joueur->posX + 1;
                clearScreen();
                affichagePlateau(n,p,plateau,joueur,selecType,gamePhase);
                Sleep(250);

                pressedScore=0;

            }
        }
        //Detecte si la touche fleche du bas est pressee
        if (GetAsyncKeyState(VK_LEFT) && plateau[joueur->posY][joueur->posX].voisinO != 0){
            pressedScore ++;
            if (pressedScore ==1){
                joueur->posX = joueur->posX - 1;
                clearScreen();
                affichagePlateau(n,p,plateau,joueur,selecType,gamePhase);
                Sleep(250);

                pressedScore=0;
            }
        }
        //Detecte si la touche fleche du haut est pressee
        if (GetAsyncKeyState(VK_UP) && plateau[joueur->posY][joueur->posX].voisinN != 0){
            pressedScore ++;
            if (pressedScore ==1){
                joueur->posY = joueur->posY - 1;
                clearScreen();
                affichagePlateau(n,p,plateau,joueur,selecType,gamePhase);
                Sleep(250);

                pressedScore=0;
            }
        }
        //Detecte si la touche fleche du bas est pressee
        if (GetAsyncKeyState(VK_DOWN) && plateau[joueur->posY][joueur->posX].voisinS != 0){
            pressedScore ++;
            if (pressedScore ==1){
                joueur->posY = joueur->posY + 1;
                clearScreen();
                affichagePlateau(n,p,plateau,joueur,selecType,gamePhase);
                Sleep(250);

                pressedScore=0;
            }
        }
        if (gamePhase == 2 && joueur->hasUsedSpecial == 0){
            //Detecte si la touche S est pressée et qu'il se situe sur une case occupée par l'un de ses pions
            if (GetAsyncKeyState(0x53) && plateau[joueur->posY][joueur->posX].pion != NULL && plateau[joueur->posY][joueur->posX].pion->equipe->equipe == joueur->equipe){
                pressedScore = pressedScore +1;
                if (pressedScore >= 1) {
                    //On supprime le pion du plateau et on enlève un pion au compteur du joueur
                    int idPion = plateau[joueur->posY][joueur->posX].pion->id;
                    plateau[joueur->posY][joueur->posX].pion = NULL;
                    plateau[joueur->posY][joueur->posX].isEmpty = true;
                    joueur->nbPion = joueur->nbPion - 1;
                    joueur->hasUsedSpecial = 1;
                    clearScreen();
                    affichagePlateau(n,p,plateau,joueur,selecType,gamePhase);
                    Sleep(250);

                    pressedScore=0;
                    hasSelected = true;


                }
            }

        }
        //Detecte si la touche entree est pressee
        if (GetAsyncKeyState(VK_TAB)){
            pressedScore = pressedScore +1;
            if (pressedScore >= 1) {
                //Si on veut placer le pion
                if (selecType == 0) {
                    if (plateau[joueur->posY][joueur->posX].isEmpty == true) {
                        *posX = joueur->posX;
                        *posY = joueur->posY;
                        pions[joueur->equipe - 1][joueur->nbPion] = (struct Pion) {joueur->nbPion, joueur,&plateau[joueur->posY][joueur->posX]};
                        plateau[*posY][*posX]
                        .pion = &pions[joueur->equipe - 1][joueur->nbPion];
                        plateau[*posY][*posX].isEmpty = false;
                        joueur->nbPion++;
                        clearScreen();
                        Sleep(250);
                        pressedScore = 0;
                        hasSelected = true;
                    }
                    //Si on veut sélectionenr le pion à déplacer
                }else if (selecType == 1){
                    if (plateau[joueur->posY][joueur->posX].isEmpty == false && plateau[joueur->posY][joueur->posX].pion->equipe->equipe == joueur->equipe){
                        *posX = joueur->posX;
                        *posY = joueur->posY;
                        hasSelected = true;
                        pressedScore = 0;
                    }
                    //Si on veut sélectionner la case de destination
                }else{
                    //Change la position du pion de la coordonnee (Y,X) a la coordonnee (joueur->posY,joueur->posX),
                    if (plateau[joueur->posY][joueur->posX].isEmpty == true) {
                        //on vérifie que la case sélectionnée se situe bien dans lun rayon de 1 autour du pion
                        if (abs(*posX - joueur->posX) <= 1 && abs(*posY - joueur->posY) <= 1) {

                            plateau[joueur->posY][joueur->posX].pion = plateau[*posY][*posX].pion;
                            plateau[joueur->posY][joueur->posX].isEmpty = false;
                            plateau[*posY][*posX].isEmpty = true;
                            plateau[*posY][*posX].pion = NULL;
                            *posX = joueur->posX;
                            *posY = joueur->posY;
                            clearScreen();
                            affichagePlateau(n, p, plateau, joueur, 0, gamePhase);
                            hasSelected = true;
                            pressedScore = 0;
                        }
                    }
                }

            }
        }
    }
}

bool placePion2(int n,int p,struct Case plateau[n][n], struct Joueur *joueur,struct Pion pions[2][p],int x){
    bool hasWin = false;
    int posX,posY = 0;
    affichagePlateau(n,p,plateau,joueur,0,1);
    printf("\n");
    printf("---------------------");
    printf("\n");
    printf("-> Joueur %d, placez votre pion\n", joueur->equipe);
    CaseSelector2(n,p, x,plateau, joueur, pions,&posX, &posY,0,1);
    hasWin = scorePoint2(n,x,plateau,plateau[posY][posX],posX,posY);

        return hasWin;

}

bool movePion2(int n,int p,struct Case plateau[n][n], struct Joueur *joueur,struct Pion pions[2][p],int x){
    bool hasWin = false;
    int posX,posY = 0;
    affichagePlateau(n, p,plateau,joueur,1,2);
    printf("\n");
    printf("---------------------");
    printf("\n");
    printf("-> Joueur %d, deplacez votre pion\n", joueur->equipe);
    CaseSelector2(n,p,x, plateau, joueur, pions,&posX, &posY,1,2);
    affichagePlateau(n,p, plateau,joueur,2,2);
    if (joueur->hasUsedSpecial == 0) {
        printf("\n");
        printf("---------------------");
        printf("\n");
        printf("-> Joueur %d, deplacez votre pion\n", joueur->equipe);
        CaseSelector2(n, p, x, plateau, joueur, pions, &posX, &posY, 2, 2);
        hasWin = scorePoint2(n, x, plateau, plateau[posY][posX], posX, posY);
    }
        return hasWin;

}










//Fonction "CreatePlateau" qui automatise la creation du plateau de jeu
void CreatePlateau(int n, struct Case plateau[n][n]){
    int i,j;
    for (i=1;i<(n+1);i++){
        printf("\n");
        for (j=1;j<n+1;j++){
            plateau[i-1][j-1] = (struct Case){i*(n)+j-n,j,i,0,0,0,0,0,0,0,0,true,NULL};
            //Voisin nord = a 0 si on est sur la premiere ligne
            if (i == 1){
                plateau[i-1][j-1].voisinN = 0;
            }
            else{
                plateau[i-1][j-1].voisinN = plateau[i-1][j-1].id -n;
            }
            //Voisin sud = a 0 si on est sur la derniere ligne
            if (i == n){
                plateau[i-1][j-1].voisinS = 0;
            }
            else{
                plateau[i-1][j-1].voisinS = plateau[i-1][j-1].id +n;
            }
            //Voisin est = a 0 si on est sur la derniere colonne
            if (j == n){
                plateau[i-1][j-1].voisinE = 0;
            }
            else{
                plateau[i-1][j-1].voisinE = plateau[i-1][j-1].id +1;
            }
            //Voisin ouest = a 0 si on est sur la premiere colonne
            if (j == 1){
                plateau[i-1][j-1].voisinO = 0;
            }
            else{
                plateau[i-1][j-1].voisinO = plateau[i-1][j-1].id -1;
            }
            //Voisin nord-est = a 0 si on est sur la premiere ligne ou la derniere colonne
            if (i == 1 || j == n){
                plateau[i-1][j-1].voisinNE = 0;
            }
            else{
                plateau[i-1][j-1].voisinNE = plateau[i-1][j-1].id -n +1;
            }
            //Voisin nord-ouest = a 0 si on est sur la premiere ligne ou la premiere colonne
            if (i == 1 || j == 1){
                plateau[i-1][j-1].voisinNO = 0;
            }
            else{
                plateau[i-1][j-1].voisinNO = plateau[i-1][j-1].id -n -1;
            }
            //Voisin sud-est = a 0 si on est sur la derniere ligne ou la derniere colonne
            if (i == n || j == n){
                plateau[i-1][j-1].voisinSE = 0;
            }
            else{
                plateau[i-1][j-1].voisinSE = plateau[i-1][j-1].id +n +1;
            }
            //Voisin sud-ouest = a 0 si on est sur la derniere ligne ou la premiere colonne
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
            //Check si la case est occupee par un pion du joueur 1
            if (plateau[i][j].isEmpty == false && plateau[i][j].pion->equipe->equipe == 1) {
                //Les cases qui l'entourent prennent la valeur 1 sauf si elles sont deja occupees par un pion du joueur 1 ou du robot
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
                //On check si le joueur 1 est entrain de former une diaonale avec deux pions, et si c'est le cas on incremente la valeur des cases aux extremites de la diagonale si elles sont libres
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
                //On check si le joueur 1 a forme une ligne verticale avec nbPionAligner-1 pions et si c'est le cas, on incremente de 1000 la valeur de la case qu'il manque pour completer la ligne de nbPionAligner pions, si elle est libre
                //Vers le Nord :
                // On va vers le haut tant qu'il y a un voisin Nord different de 0 et que la case n'est pas vide, et qu'un pion de l'equipe 1 est dessus, et on incremente un compteur
                int compteur = 0;
                int k = i;
                while (plateau[k][j].voisinN != 0 && plateau[k][j].isEmpty == false && plateau[k][j].pion->equipe->equipe == 1) {
                    compteur++;
                    k--;
                }
                //Si le compteur est egal a nbPionAligner-2, on incremente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 2 && plateau[k][j].isEmpty == true) {
                    valeurCase[k][j] += 1000;
                }
                //Vers le Sud :
                // On va vers le bas tant qu'il y a un voisin Sud different de 0 et que la case n'est pas vide, et qu'un pion de l'equipe 1 est dessus, et on incremente un compteur
                compteur = 0;
                k = i;
                while (plateau[k][j].voisinS != 0 && plateau[k][j].isEmpty == false && plateau[k][j].pion->equipe->equipe == 1) {
                    compteur++;
                    k++;
                }
                //Si le compteur est egal a nbPionAligner-2, on incremente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 2 && plateau[k][j].isEmpty == true) {
                    valeurCase[k][j] += 1000;
                }
                //Vers l'Est :
                // On va vers la droite tant qu'il y a un voisin Est different de 0 et que la case n'est pas vide, et qu'un pion de l'equipe 1 est dessus, et on incremente un compteur
                compteur = 0;
                k = j;
                while (plateau[i][k].voisinE != 0 && plateau[i][k].isEmpty == false && plateau[i][k].pion->equipe->equipe == 1) {
                    compteur++;
                    k++;
                }
                //Si le compteur est egal a nbPionAligner-2, on incremente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 2 && plateau[i][k].isEmpty == true) {
                    valeurCase[i][k] += 1000;
                }
                //Vers l'Ouest :
                // On va vers la gauche tant qu'il y a un voisin Ouest different de 0 et que la case n'est pas vide, et qu'un pion de l'equipe 1 est dessus, et on incremente un compteur
                compteur = 0;
                k = j;
                while (plateau[i][k].voisinO != 0 && plateau[i][k].isEmpty == false && plateau[i][k].pion->equipe->equipe == 1) {
                    compteur++;
                    k--;
                }
                //Si le compteur est egal a nbPionAligner-2, on incremente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 2 && plateau[i][k].isEmpty == true) {
                    valeurCase[i][k] += 1000;
                }
                //Diagonale Nord-Ouest :
                // On va vers la gauche et vers le haut tant qu'il y a un voisin Nord-Ouest different de 0 et que la case n'est pas vide, et qu'un pion de l'equipe 1 est dessus, et on incremente un compteur
                compteur = 0;
                k = i;
                int l = j;
                while (plateau[k][l].voisinNO != 0 && plateau[k][l].isEmpty == false && plateau[k][l].pion->equipe->equipe == 1) {
                    compteur++;
                    k--;
                    l--;
                }
                //Si le compteur est egal a nbPionAligner-2, on incremente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 2 && plateau[k][l].isEmpty == true) {
                    valeurCase[k][l] += 1000;
                }
                //Diagonale Nord-Est :
                // On va vers la droite et vers le haut tant qu'il y a un voisin Nord-Est different de 0 et que la case n'est pas vide, et qu'un pion de l'equipe 1 est dessus, et on incremente un compteur
                compteur = 0;
                k = i;
                l = j;
                while (plateau[k][l].voisinNE != 0 && plateau[k][l].isEmpty == false && plateau[k][l].pion->equipe->equipe == 1) {
                    compteur++;
                    k--;
                    l++;
                }
                //Si le compteur est egal a nbPionAligner-2, on incremente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 2 && plateau[k][l].isEmpty == true) {
                    valeurCase[k][l] += 1000;
                }
                //Diagonale Sud-Ouest :
                // On va vers la gauche et vers le bas tant qu'il y a un voisin Sud-Ouest different de 0 et que la case n'est pas vide, et qu'un pion de l'equipe 1 est dessus, et on incremente un compteur
                compteur = 0;
                k = i;
                l = j;
                while (plateau[k][l].voisinSO != 0 && plateau[k][l].isEmpty == false && plateau[k][l].pion->equipe->equipe == 1) {
                    compteur++;
                    k++;
                    l--;
                }
                //Si le compteur est egal a nbPionAligner-2, on incremente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 2 && plateau[k][l].isEmpty == true) {
                    valeurCase[k][l] += 1000;
                }
                //Diagonale Sud-Est :
                // On va vers la droite et vers le bas tant qu'il y a un voisin Sud-Est different de 0 et que la case n'est pas vide, et qu'un pion de l'equipe 1 est dessus, et on incremente un compteur
                compteur = 0;
                k = i;
                l = j;
                while (plateau[k][l].voisinSE != 0 && plateau[k][l].isEmpty == false && plateau[k][l].pion->equipe->equipe == 1) {
                    compteur++;
                    k++;
                    l++;
                }
                //Si le compteur est egal a nbPionAligner-2, on incremente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 2 && plateau[k][l].isEmpty == true) {
                    valeurCase[k][l] += 1000;
                }
            }
            //On check les cases qui sont occupees par un pion du robot
            if (plateau[i][j].isEmpty == false && plateau[i][j].pion->equipe->equipe == 2) {
                //Les cases qui l'entourent prennent la valeur 1 sauf si elles sont deja occupees par un pion du joueur 1 ou du robot
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
                //On check si le joueur 2 (robot) a deja forme une ligne verticale de nbPionAligner -1 pions, si c'est le cas, on incremente de 10000 la valeur de la case qu'il manque pour completer la ligne de nbPionAligner pions si elles sont libres
                //Ligne Nord :
                // On va vers le haut tant qu'il y a un voisin Nord different de 0 et que la case n'est pas vide, et qu'un pion de l'equipe 2 est dessus, et on incremente un compteur
                int compteur = 0;
                int k = i;
                int l = j;
                while (plateau[k][l].voisinN != 0 && plateau[k][l].isEmpty == false && plateau[k][l].pion->equipe->equipe == 2) {
                    compteur++;
                    k--;
                }
                //Si le compteur est egal a nbPionAligner-1, on incremente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 1 && plateau[k][l].isEmpty == true) {
                    valeurCase[k][l] += 10000;
                }
                //Ligne Sud :
                // On va vers le bas tant qu'il y a un voisin Sud different de 0 et que la case n'est pas vide, et qu'un pion de l'equipe 2 est dessus, et on incremente un compteur
                compteur = 0;
                k = i;
                l = j;
                while (plateau[k][l].voisinS != 0 && plateau[k][l].isEmpty == false && plateau[k][l].pion->equipe->equipe == 2) {
                    compteur++;
                    k++;
                }
                //Si le compteur est egal a nbPionAligner-1, on incremente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 1 && plateau[k][l].isEmpty == true) {
                    valeurCase[k][l] += 10000;
                }
                //Ligne Est :
                // On va vers la droite tant qu'il y a un voisin Est different de 0 et que la case n'est pas vide, et qu'un pion de l'equipe 2 est dessus, et on incremente un compteur
                compteur = 0;
                k = i;
                l = j;
                while (plateau[k][l].voisinE != 0 && plateau[k][l].isEmpty == false && plateau[k][l].pion->equipe->equipe == 2) {
                    compteur++;
                    l++;
                }
                //Si le compteur est egal a nbPionAligner-1, on incremente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 1 && plateau[k][l].isEmpty == true) {
                    valeurCase[k][l] += 10000;
                }
                //Ligne Ouest :
                // On va vers la gauche tant qu'il y a un voisin Ouest different de 0 et que la case n'est pas vide, et qu'un pion de l'equipe 2 est dessus, et on incremente un compteur
                compteur = 0;
                k = i;
                l = j;
                while (plateau[k][l].voisinO != 0 && plateau[k][l].isEmpty == false && plateau[k][l].pion->equipe->equipe == 2) {
                    compteur++;
                    l--;
                }
                //Si le compteur est egal a nbPionAligner-1, on incremente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 1 && plateau[k][l].isEmpty == true) {
                    valeurCase[k][l] += 10000;
                }
                //Ligne Nord-Est :
                // On va vers le haut et la droite tant qu'il y a un voisin Nord-Est different de 0 et que la case n'est pas vide, et qu'un pion de l'equipe 2 est dessus, et on incremente un compteur
                compteur = 0;
                k = i;
                l = j;
                while (plateau[k][l].voisinNE != 0 && plateau[k][l].isEmpty == false && plateau[k][l].pion->equipe->equipe == 2) {
                    compteur++;
                    k--;
                    l++;
                }
                //Si le compteur est egal a nbPionAligner-1, on incremente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 1 && plateau[k][l].isEmpty == true) {
                    valeurCase[k][l] += 10000;
                }
                //Ligne Nord-Ouest :
                // On va vers le haut et la gauche tant qu'il y a un voisin Nord-Ouest different de 0 et que la case n'est pas vide, et qu'un pion de l'equipe 2 est dessus, et on incremente un compteur
                compteur = 0;
                k = i;
                l = j;
                while (plateau[k][l].voisinNO != 0 && plateau[k][l].isEmpty == false && plateau[k][l].pion->equipe->equipe == 2) {
                    compteur++;
                    k--;
                    l--;
                }
                //Si le compteur est egal a nbPionAligner-1, on incremente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 1 && plateau[k][l].isEmpty == true) {
                    valeurCase[k][l] += 10000;
                }
                //Ligne Sud-Est :
                // On va vers le bas et la droite tant qu'il y a un voisin Sud-Est different de 0 et que la case n'est pas vide, et qu'un pion de l'equipe 2 est dessus, et on incremente un compteur
                compteur = 0;
                k = i;
                l = j;
                while (plateau[k][l].voisinSE != 0 && plateau[k][l].isEmpty == false && plateau[k][l].pion->equipe->equipe == 2) {
                    compteur++;
                    k++;
                    l++;
                }
                //Si le compteur est egal a nbPionAligner-1, on incremente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 1 && plateau[k][l].isEmpty == true) {
                    valeurCase[k][l] += 10000;
                }
                //Ligne Sud-Ouest :
                // On va vers le bas et la gauche tant qu'il y a un voisin Sud-Ouest different de 0 et que la case n'est pas vide, et qu'un pion de l'equipe 2 est dessus, et on incremente un compteur
                compteur = 0;
                k = i;
                l = j;
                while (plateau[k][l].voisinSO != 0 && plateau[k][l].isEmpty == false && plateau[k][l].pion->equipe->equipe == 2) {
                    compteur++;
                    k++;
                    l--;
                }
                //Si le compteur est egal a nbPionAligner-1, on incremente la valeur de la case au dessus de la ligne de nbPionAligner pions si elle est libre
                if (compteur == nbPionAligner - 1 && plateau[k][l].isEmpty == true) {
                    valeurCase[k][l] += 10000;
                }





            }
        }
    }
}

//Fonction "placePionIA", qui permet de placer les pions de l'IA de maniere intelligente afin de bloquer le joueur 1 (detection du placement des pions du joueur 1 et prise de decision) et de gagner le plus rapidement possible (detection des pions du robot et prise de decision)
//Dans un premier temps, l'intelligence artificelle analyse le placement des pions du joueur 1, et attribue une valeur a chaque case du plateau de jeu en fonction de la position des pions du joueur 1, (+ il peut bloquer le joueur 1 dans sa progression, + la valeur de la case est elevee)
// Dans un second temps, l'intelligence artificielle analyse la position des pions du joueur 2 (robot), et attribue une valeur a chaque case du plateau de jeu en fonction de la position des pions du joueur 2 (+ le robot peut se rapprocher de la victoire, + la valeur de la case est elevee)
// Enfin, l'intelligence artificielle compare les valeurs des cases du plateau de jeu, et place le pion de l'IA sur la case qui a la valeur la plus elevee
bool placePionIA2(int n,int p, struct Joueur *joueur, struct Case plateau[n][n], struct Pion pions[2][p], int NbPionAligner) {

    int i, j;
    int valeurCase[n][n];
    //Initialisation du tableau de valeurs a 0
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            valeurCase[i][j] = 0;

        }

    }
    AIeval2(n, plateau,*joueur, valeurCase, NbPionAligner);

    //On evalue les cases qui ont le plus de valeur et on pose un pion sur la case qui a la plus grande valeur (si la case est vide)
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

    //On check si le BOT a gagne grâce a la fonction scorePoint
    bool hasWin = false;
    hasWin = scorePoint2(n,NbPionAligner,plateau,plateau[botPosY][botPosX],botPosX,botPosY);
    // On reinitialise les valeurs des cases
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++){
            valeurCase[i][j] = 0;
        }
    }
    affichagePlateau(n,p, plateau,joueur,0,0);
    Sleep(1000);
    return hasWin;



}

//Fonction "movePionIA" qui permet de deplacer un pion du joueur 2 (intelligence artificielle) de maniere a bloquer le joueur 1 si il est sur le point de gagner ou de deplacer un pion du robot si il est sur le point de gagner
//De même que pour la fonction "placePionIA", on va evaluer les cases du plateau de jeu et on va deplacer le pion sur la case qui a la plus grande valeur
//Ici, on procede a une double evaluation, car il faut aussi evaluer le meilleur pion a deplacer.  Pour cela on analyse si il ne s'agit pas d'une case qui permet deja de bloquer le joueur 1 ou de gagner le jeu pour le joueur 2
bool movePionIA2(int n,int p, struct Joueur *joueur, struct Case plateau[n][n], struct Pion pions[2][p], int x) {
    //D'abord on evalue le pion a deplacer
    int i, j, k, l;
    int valeurCase[n][n];
    int valeurPion[4];
    int valeurPionMax = 0;
    int pionMax = 0;
    int botPosX = 0;
    int botPosY = 0;


    //On initialise les valeurs des cases a 0
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            valeurCase[i][j] = 0;
        }
    }
    //On initialise les valeurs des pions a 0
    for (i = 0; i < 4; i++) {
        valeurPion[i] = 0;
    }

    //Si le pion permet de bloquer le joueur 1, alors il prend une valeur negative egale a -1000
    //On part de la position du pion et on analyse les cases autour du pion
    for (i = 0; i < 4; i++) {
        int botPosX = pions[1][i].pos->posX -1;
        int botPosY = pions[1][i].pos->posY -1;
        //Si le pion est entre deux pions du joueur 1, alors on lui attribue une valeur negative egale a -1
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
        // Si le pion a comme voisin de droite une ligne horizontale de x-1 pions du joueur 1, alors on lui attribue une valeur negative egale a -1000
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
        // Si le pion a comme voisin de gauche une ligne horizontale de x-1 pions du joueur 1, alors on lui attribue une valeur negative egale a -1000
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

    //Comme dans la fonction "placePionIA" on evalue les cases en fonction du placement des pions du joueur 1
    AIeval2(n,plateau,*joueur,valeurCase,x);
    Sleep(1);

    //On evalue les cases qui ont le plus de valeur et on pose un pion sur la case qui a la plus grande valeur (si la case est vide)
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
    //On deplace le pion avec la plus grande valeur sur la case qui a la plus grande valeur
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
    // On reinitialise les valeurs des cases
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            valeurCase[i][j] = 0;
        }
    }
    // On reinitialise les valeurs des pions
    for (i = 0; i < 4; i++) {
        valeurPion[i] = 0;
    }
    return hasWin;
}




void afficheWinner(struct Joueur *joueur){
    //"Décoration" de victoire et affiche le gagnant
    printf("\n");
    printf("**********************************************\n");
    printf("*                                            *\n");
    printf("*          Felicitations joueur %d!          *\n", joueur->equipe);
    printf("*                                            *\n");
    printf("**********************************************\n");

    sleep(15);  //On attend 15 secondes avant de revenir au menu principal



}
int compte()
{
    FILE *fichier = fopen("save.txt", "r");

    int c;
    int nLignes = 0;
    int c2 = '\0';

    while((c=fgetc(fichier)) != EOF)
    {
        if(c=='\n')
            nLignes++;
        c2 = c;
    }

    /* Ici, c2 est egal au caractere juste avant le EOF. */
    if(c2 != '\n')
        nLignes++; /* Derniere ligne non finie */


    return nLignes;
}
//Fonction "restartGame" qui permet de lancer la derniere partie sauvegardee dans le fichier save.txt
//On restaure les parametres du plateau, et des joueurs a partir du contenu du fichier save.txt
void restartGame() {
    //On ouvre le fichier
    FILE *fichier = fopen("save.txt", "r");
    int gameMode,boardSize,maxPawns,winPawns,nbP1,nbP2;
    // On lit les lignes une par une
    int nbLigne = compte();
    for (int i = 0; i < nbLigne; i++) {
        char ligne[100];
        fgets(ligne, 100, fichier);
        //Si il s'agit de la 4eme ligne, on recupere le statut de la partie ("AI" "ou "Multi-joueur")
        if(i == 3) {
            printf("%d - %s | ",i,ligne);
            if (ligne[22] == 'A') {
                gameMode = 1;

            } else {
                gameMode = 2;
            }
        }
        //Si il s'agit de la 6eme ligne, on recupere la longueur du plateau
        if (i == 5){

            //On recupere la longueur du plateau (a partir de la 23 eme lettre de la ligne), et on la convertit en int
            boardSize = ligne[22] - '0';
            //On check si la longueur du plateau est superieur a 9
            int j = 1;
            while (ligne[22 + j] != ' '){
                boardSize = boardSize * 10 + ligne[22 + j] - '0';
                j++;
            }
        }
        //Si il s'agit de la 7eme ligne, on recupere le nombre de pions max par joueur
        if (i == 6){
            printf("%d - %s",i,ligne);

            //On recupere le nombre de pions max par joueur (a partir de la 30 eme lettre de la ligne), et on le convertit en int
            maxPawns = ligne[29] - '0';

            //On check si le nombre de pions max par joueur est superieur a 9
            int j = 1;
            while (ligne[29 + j] != ' '){
                maxPawns = maxPawns * 10 + ligne[29 + j] - '0';
                j++;
            }
        }
        //Si
        // il s'agit de la 8eme ligne, on recupere le nombre de pions a aligner pour gagner
        if (i == 7){
            printf("%d - %s",i,ligne);
           // On recupere le nombre de pions a aligner pour gagner (a partir de la 29 eme lettre de la ligne), et on le convertit en int
            winPawns = ligne[28] - '0';
            //On check si le nombre de pions max par joueur est superieur a 9
            int j = 1;
            while (ligne[28 + j] != ' '){
                winPawns = winPawns * 10 + ligne[29 + j] - '0';
                j++;
            }
        }
    }
    //On ferme le fichier
    fclose(fichier);
    //On creer le plateau
    struct Case plateau[boardSize][boardSize];
    //CreatePlateau(boardSize, plateau);
    struct Pion pions[2][maxPawns];
    CreatePlateau(boardSize, plateau);

    //Creation des joueurs
    struct Joueur joueur1 = (struct Joueur) {1, 1,  0, 'X', 12, 0, 0, 0};
    struct Joueur joueur2 = (struct Joueur) {2, 2,  0, 'O', 2, 0, 0, 0};
    int i = 0;
    FILE *fichier2 = fopen("save.txt", "r");
    for (i=0; i < nbLigne; i++){
        char ligne[150];
        fgets(ligne, 10+(boardSize * boardSize), fichier2);
        //On check de la ligne 8 a la ligne 8 + n*n pour recuperer les informations des cases
        if (i >= 8 && i <= 8 + (boardSize * boardSize)){

            //Si la premiere lettre de la ligne est un
            // "0" alors la case contient un pion, on recupere les informations du pion, on l'attribue au joueur concerne et on le place sur le plateau

            if (ligne[0] == '0') {

                int idPion, idEquipe, x, y;
                // a partir 9eme lettre de la ligne = idPion
                idPion = ligne[8] - '0';
                int j = 1;
                while (ligne[8 + j] != ',') {
                    idPion = idPion * 10 + ligne[8 + j] - '0';

                    j++;
                }
                // 19eme lettre de la ligne = idEquipe
                idEquipe = ligne[18] - '0';
                // a partir 26eme lettre de la ligne = x
                x = ligne[25] - '0';
                j = 1;
                while (ligne[25 + j] != ' ') {
                    x = x * 10 + ligne[25 + j] - '0';
                    j++;
                }
                // a partir 33eme lettre de la ligne = y
                y = ligne[32] - '0';
                j = 1;
                while (ligne[32 + j] != ' ') {
                    y = y * 10 + ligne[32 + j] - '0';
                    j++;
                }
                //On creer le pion
                if (idEquipe == 1) {
                    pions[0][idPion] = (struct Pion) {idPion, &joueur1, &plateau[y][x]};
                    plateau[y][x].pion = &pions[0][idPion];
                    plateau[y][x].isEmpty = false;
                    joueur1.nbPion++;


                } else {
                    pions[1][idPion] = (struct Pion) {idPion, &joueur2, &plateau[y][x]};
                    plateau[y][x].pion = &pions[1][idPion];
                    plateau[y][x].isEmpty = false;
                    joueur2.nbPion++;
                }
            }


            }
        }

    fclose(fichier2);

    //On recommence le jeu
    if (gameMode == 1) {
        bool hasWin = false;
        int winner = 0;
        //Placement des pions du joueur 1 et intelligence artificielle pour un placement automatique des pions du joueur 2
        while (joueur2.nbPion <= maxPawns - 1) {
            hasWin = placePion2(boardSize, maxPawns, plateau, &joueur1, pions, winPawns);

            if (hasWin == false) {
                hasWin = placePionIA2(boardSize, maxPawns, &joueur2, plateau, pions,winPawns);
                if (hasWin) {
                    winner = 2;
                    break;
                }
            } else {
                winner = 1;
                break;
            }
            shouldSave(boardSize, maxPawns, winPawns, plateau, &joueur1, &joueur2, pions, 0);
        }
        do {
            if (!hasWin) {
                if (joueur1.hasUsedSpecial == 1){
                    hasWin = placePion2(boardSize, maxPawns, plateau, &joueur1, pions, winPawns);
                    joueur1.hasUsedSpecial = 0;
                }else{
                    hasWin = movePion2(boardSize, maxPawns, plateau, &joueur1, pions, winPawns);
                }
                if (hasWin == false) {
                    hasWin = movePionIA2(boardSize, maxPawns, &joueur2, plateau, pions, winPawns);
                    if (hasWin) {
                        winner = 2;
                        break;
                    }
                } else {
                    winner = 1;
                    break;
                }
            } else {
                break;
            }
            shouldSave(boardSize, maxPawns, winPawns, plateau, &joueur1, &joueur2, pions, 0);
        } while (hasWin == false);{
            if (winner == 1) {
                afficheWinner(&joueur1);

            } else {
                afficheWinner(&joueur2);

            }
            Sleep(5000);
        }


    }else{
        bool hasWin = false;
        int winner= 0;
        while (joueur2.nbPion <= maxPawns-1){

            hasWin = placePion2(boardSize, maxPawns,plateau,&joueur1,pions,winPawns);
            if (hasWin == false){

                hasWin = placePion2(boardSize, maxPawns,plateau,&joueur2,pions,winPawns);
                if (hasWin){
                    winner = 2;
                    break;
                }
            }else{
                winner = 1;
                break;
            }
            shouldSave(boardSize, maxPawns,winPawns,plateau,&joueur1,&joueur2,pions,1);
        }
        do {
            if (hasWin)
                break;
            if (joueur1.hasUsedSpecial == 1){
                hasWin = placePion2(boardSize, maxPawns, plateau, &joueur1, pions, winPawns);
                joueur1.hasUsedSpecial = 0;
            }else{
                hasWin = movePion2(boardSize, maxPawns,plateau,&joueur1,pions,winPawns);

            }
            if (hasWin == false){
                if (joueur2.hasUsedSpecial == 1){
                    hasWin = placePion2(boardSize, maxPawns, plateau, &joueur2, pions, winPawns);
                    joueur2.hasUsedSpecial = 0;
                }else{
                    hasWin = movePion2(boardSize, maxPawns,plateau,&joueur2,pions,winPawns);
                }
                if (hasWin){
                    winner = 2;
                    break;
                }
            }else{
                winner = 1;
                break;
            }
            shouldSave(boardSize, maxPawns,winPawns,plateau,&joueur1,&joueur2,pions,1);
        } while (hasWin == false);
        if (winner == 1) {
            afficheWinner(&joueur1);

        } else {
            afficheWinner(&joueur2);

        }
        Sleep(15);
    }


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
            printf("[ ] Charger une partie sauvegardee\n");
            printf("[ ] Quitter le jeu\n");
            break;
        case 2:
            printf("[ ] Jouer contre l'ordinateur\n");
            printf("[");
            textColor(4,0);
            printf("%c",(char) 219);
            textColor(15,0);
            printf("] Jouer contre un autre joueur\n");
            printf("[ ] Charger une partie sauvegardee\n");
            printf("[ ] Quitter le jeu\n");
            break;
        case 3:
            printf("[ ] Jouer contre l'ordinateur\n");
            printf("[ ] Jouer contre un autre joueur\n");
            printf("[");
            textColor(4,0);
            printf("%c",(char) 219);
            textColor(15,0);
            printf("] Charger une partie sauvegardee\n");
            printf("[ ] Quitter le jeu\n");
            break;
        case 4:
            printf("[ ] Jouer contre l'ordinateur\n");
            printf("[ ] Jouer contre un autre joueur\n");
            printf("[ ] Charger une partie sauvegardee\n");
            printf("[");
            textColor(4,0);
            printf("%c",(char) 219);
            textColor(15,0);
            printf("] Quitter le jeu\n");
            break;
        default:

            break;
    }
}

void MultiJoueur2(){
    int n,p, i,j;
    do{
        printf("\n-> Veuillez saisir une longueur superieure ou egale a 4 : ");
        scanf("%d", &n);
    }while (n < 4 || n < 0);
    // On demande le nombre de pions strictement positif et inerieur a la longueur du plateau divisee par deux
    do{
        printf("\n-> Veuillez saisir le nombre de pions  strictement positif et inferieur a %d : ", n/2);
        scanf("%d", &p);
    }while (p < 1 || p > (n/2)-1);
    //On demande le nombre de pions a aligner  positif et inferieur ou egal a p
    int x;
    do{
        printf("\n-> Veuillez saisir le nombre de pions a aligner strictement positif et inferieur ou egal a %d : ", p);
        scanf("%d", &x);
    }while (x < 1 || x > p);

    //Creation du plateau de jeu
    struct Case plateau[n][n];
    CreatePlateau(n, plateau);
    struct Pion pions[2][p];

    //Creation des joueurs
    struct Joueur joueur1 = (struct Joueur){1,1,0,'X',12, 0,0,0};

    struct Joueur joueur2 = (struct Joueur){2,2,0,'O',2,0,0,0};
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
        if (hasWin){
            break;
        }
        if (joueur1.hasUsedSpecial == 1){

            hasWin = placePion2(n,p,plateau,&joueur1,pions,x);
            joueur1.hasUsedSpecial = 0;
        }else {

            hasWin = movePion2(n,p,plateau,&joueur1,pions,x);
        }
        if (hasWin == false){
            if (joueur2.hasUsedSpecial == 1){
                hasWin = placePion2(n,p,plateau,&joueur2,pions,x);
                joueur2.hasUsedSpecial = 0;
            }else {
                hasWin = movePion2(n,p,plateau,&joueur2,pions,x);
            }
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
    if (winner == 1) {
        afficheWinner(&joueur1);

    } else {
        afficheWinner(&joueur2);

    }
    Sleep(5000);

}


void IA2() {
    int x, n, i, j, p;
    do {
        printf("\n-> Veuillez saisir une longueur superieure ou egale a 4 : ");
        scanf("%d", &n);
    } while (n < 4 || n < 0);
    // On demande le nombre de pions strictement positif et inerieur a la longueur du plateau divisee par deux
    do {
        printf("\n-> Veuillez saisir un nombre de pions strictement positif et inferieur a %d : ", n / 2);
        scanf("%d", &p);
    } while (p < 1 || p > (n / 2)-1);
    //On demande au joueur de choisir le nombre de pions a aligner ( inferieur ou egal a p)
    do {
        printf("\n-> Veuillez saisir un nombre de pions a aligner strictement positif et inferieur ou egal a %d : ", p);
        scanf("%d", &x);
    } while (x < 1 || x > p);

    //Creation du plateau de jeu
    struct Case plateau[n][n];
    CreatePlateau(n, plateau);
    struct Pion pions[2][p];

    //Creation des joueurs
    struct Joueur joueur1 = (struct Joueur) {1, 1,  0, 'X', 12, 0, 0, false};
    struct Joueur joueur2 = (struct Joueur) {2, 2,  0, 'O', 2, 0, 0, false};
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
            if (joueur1.hasUsedSpecial == 1){
                hasWin = placePion2(n,p,plateau,&joueur1,pions,x);
                joueur1.hasUsedSpecial = 0;
            }else {
                hasWin = movePion2(n,p,plateau,&joueur1,pions,x);
            }
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
    if (winner == 1) {
        afficheWinner(&joueur1);

    } else {
        afficheWinner(&joueur2);

    }
    Sleep(5000);


}


//Fonction menu qui permet de selectionner entre jouer contre l'ordinateur ou contre un autre joueur
//Naviguation avec les fleches du clavier et validation avec la touche entree
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
                choix = 4;
            }
            system("cls");
            printf("-> Bienvenue dans le jeu du Teeko\n");
            printf("-> Veuillez choisir une option\n");
            AffichageMenu(choix);
            Sleep(250);


        }
        if (GetAsyncKeyState(VK_DOWN) != 0){
            choix  ++;
            if (choix > 4){
                choix = 1;
            }
            system("cls");
            printf("-> Bienvenue dans le jeu du Teeko\n");
            printf("-> Veuillez choisir une option\n");
            AffichageMenu(choix);
            Sleep(250);


        }
        if (GetAsyncKeyState(VK_RETURN) != 0){
            printf("-> Vous avez choisi l'option %d\n", choix);
            Sleep(250);
            system("cls");
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
        case 4:
            exit(0);
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













