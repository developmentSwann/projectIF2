#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include <windows.h>

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
};
void textColor(int couleurDuTexte,int couleurDeFond)
{
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,couleurDeFond*16+couleurDuTexte);
}
void affichagePlateau(int n,struct Case plateau[n][n]){
    int i,j;
    printf(" ");
    for (i=1;i<n+1;i++){
        printf(" %d",i);

    }
    for (i=1;i<(n+1);i++){
        printf("\n");
        printf("%d ",i);
        for (j=1;j<n+1;j++){
            if (plateau[i-1][j-1].isEmpty == true){
                printf(" %c",(char)254)    ;
            }
            else{
                textColor(plateau[i-1][j-1].pion->equipe->color,0);
                printf(" %c",plateau[i-1][j-1].pion->equipe->icon);
                textColor(15,0);

            }
        }
    }
    printf("\n");
    printf("---------------------");
    printf("\n");
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
    // Verification vers le Nord
    while (actualCase.voisinN != 0 && plateau[posY-1][posX].pion != NULL && plateau[posY-1][posX].pion->equipe->equipe == equipe){
        compteur++;
        posY--;
        actualCase = plateau[posY][posX];
    }
    // Verification vers l'Est
    posY = defPosY;
    actualCase = plateau[posY][posX];
    while (actualCase.voisinE != 0 && plateau[posY][posX+1].pion != NULL && plateau[posY][posX+1].pion->equipe->equipe == equipe){
        compteur++;
        posX++;
        actualCase = plateau[posY][posX];
    }
    // Verification vers le Nord-Est
    posX = defPosX;
    actualCase = plateau[posY][posX];
    while (actualCase.voisinNE != 0 && plateau[posY-1][posX+1].pion != NULL && plateau[posY-1][posX+1].pion->equipe->equipe == equipe){
        compteur++;
        posY--;
        posX++;
        actualCase = plateau[posY][posX];
    }
    if (compteur >= 3){
        return true;
    }
    compteur = 0;
    posY = defPosY;
    posX = defPosX;
    actualCase = plateau[posY][posX];
    // Carré #2 :
    // Verification vers le Nord
    while (actualCase.voisinN != 0 && plateau[posY-1][posX].pion != NULL && plateau[posY-1][posX].pion->equipe->equipe == equipe){
        compteur++;
        posY--;
        actualCase = plateau[posY][posX];
    }
    // Verification vers l'Ouest
    posY = defPosY;
    actualCase = plateau[posY][posX];
    while (actualCase.voisinO != 0 && plateau[posY][posX-1].pion != NULL && plateau[posY][posX-1].pion->equipe->equipe == equipe){
        compteur++;
        posX--;
        actualCase = plateau[posY][posX];
    }
    // Verification vers le Nord-Ouest
    posX = defPosX;
    actualCase = plateau[posY][posX];
    while (actualCase.voisinNO != 0 && plateau[posY-1][posX-1].pion != NULL && plateau[posY-1][posX-1].pion->equipe->equipe == equipe){
        compteur++;
        posY--;
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
    // Carré #3 :
    // Verification vers le Sud
    while (actualCase.voisinS != 0 && plateau[posY+1][posX].pion != NULL && plateau[posY+1][posX].pion->equipe->equipe == equipe){
        compteur++;
        posY++;
        actualCase = plateau[posY][posX];
    }
    // Verification vers l'Ouest
    posY = defPosY;
    actualCase = plateau[posY][posX];
    while (actualCase.voisinO != 0 && plateau[posY][posX-1].pion != NULL && plateau[posY][posX-1].pion->equipe->equipe == equipe){
        compteur++;
        posX--;
        actualCase = plateau[posY][posX];
    }
    // Verification vers le Sud-Ouest
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
    // Carré #4 :
    // Verification vers le Sud
    while (actualCase.voisinS != 0 && plateau[posY+1][posX].pion != NULL && plateau[posY+1][posX].pion->equipe->equipe == equipe){
        compteur++;
        posY++;
        actualCase = plateau[posY][posX];
    }
    // Verification vers l'Est
    posY = defPosY;
    actualCase = plateau[posY][posX];
    while (actualCase.voisinE != 0 && plateau[posY][posX+1].pion != NULL && plateau[posY][posX+1].pion->equipe->equipe == equipe){
        compteur++;
        posX++;
        actualCase = plateau[posY][posX];
    }
    // Verification vers le Sud-Est
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
    return false;

}
bool placePion(int n,struct Joueur *joueur, struct Case plateau[n][n], struct Pion pions[4][4]){
    bool hasWin = false;
    printf("-> Joueur %d : Veuillez saisir les coordonnées d'une case vide (x,y): ",joueur->equipe);
    int x,y;
    scanf("%d,%d", &x, &y);
    while (x>n || y>n || x<1 || y<1){
        printf("-> Veuillez saisir des coordonnées valides (x,y): ");
        scanf("%d,%d", &x, &y);
    };

    while (plateau[y-1][x-1].isEmpty == false){
        printf("-> Veuillez saisir les coordonnées d'une case vide (x,y): ");
        scanf("%d,%d", &x, &y);
    };
    joueur->nbPion++;
    pions[joueur->equipe-1][joueur->nbPion-1] = (struct Pion){joueur->nbPion,joueur,&plateau[y-1][x-1]};
    plateau[y-1][x-1].isEmpty = false;
    plateau[y-1][x-1].pion = &pions[joueur->equipe-1][joueur->nbPion-1];

    affichagePlateau(n,plateau);
    hasWin = scorePoint(n,plateau,plateau[y-1][x-1],x-1,y-1);

    if (joueur->nbPion > 3){
        if (hasWin == true){
            printf("\nLe joueur %d a gagné",joueur->equipe);
        }
    }
    return hasWin;
}
bool movePion(int n,struct Joueur *joueur, struct Case plateau[n][n], struct Pion pions[4][4]){
    printf("\033[1;31m");
    printf("\n-> Déplacement des pions");
    printf("\033[0m");
    printf("\n-> Joueur %d : Veuillez saisir les coordonnées d'un de vos pions (x,y): ",joueur->equipe);
    int x,y;
    scanf("%d,%d", &x, &y);
    while (x>n || y>n || x<1 || y<1){
        printf("-> Veuillez saisir des coordonnées valides (x,y): ");
        scanf("%d,%d", &x, &y);
    };
    while (plateau[y-1][x-1].isEmpty == true || plateau[y-1][x-1].pion->equipe->equipe != joueur->equipe){
        printf("-> Veuillez saisir les coordonnées d'un de vos pions (x,y): ");
        scanf("%d,%d", &x, &y);
    };
    printf("-> Joueur %d : Veuillez saisir les coordonnées d'une case vide (x,y): ",joueur->equipe);
    int x2,y2;
    scanf("%d,%d", &x2, &y2);
    while (x2>n || y2>n || x2<1 || y2<1){
        printf("-> Veuillez saisir des coordonnées valides (x,y): ");
        scanf("%d,%d", &x2, &y2);
    };
    while (plateau[y2-1][x2-1].isEmpty == false){
        printf("-> Veuillez saisir les coordonnées d'une case vide (x,y): ");
        scanf("%d,%d", &x2, &y2);
    };
    plateau[y2-1][x2-1].isEmpty = false;
    plateau[y2-1][x2-1].pion = plateau[y-1][x-1].pion;
    plateau[y-1][x-1].isEmpty = true;
    plateau[y-1][x-1].pion = NULL;
    affichagePlateau(n,plateau);
    return scorePoint(n,plateau,plateau[y2-1][x2-1],x2-1,y2-1);




}

void MultiJoueur(){
    int n, i,j;;
    while (n < 4){
        printf("-> Veuillez saisir une longueur supérieure ou égale à 4 : ");
        scanf("%d", &n);
    }

    struct Case plateau[n][n];
    struct Pion pions[4][4];
    printf(" ");
    for (i=1;i<n+1;i++){
        printf(" %d",i);

    }
    for (i=1;i<(n+1);i++){
        printf("\n");
        printf("%d ",i);
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
            printf(" %c",(char)254);
        }
    }
    printf("\n");
    printf("---------------------");
    printf("\n");


    struct Joueur joueur1 = (struct Joueur){1,1,0,0,'X',12};
    struct Joueur joueur2 = (struct Joueur){2,2,0,0,'O',2};
    bool hasWin = false;
    while (joueur2.nbPion != 4 && joueur1.nbPion != 4 && hasWin == false){
        hasWin = placePion(n,&joueur1,plateau,pions);
        if (hasWin == false){
            hasWin = placePion(n,&joueur2,plateau,pions);
        }
    }
    while (hasWin == false){
            hasWin = movePion(n,&joueur1,plateau,pions);
        if (hasWin == false){
            hasWin = movePion(n,&joueur2,plateau,pions);
            if (hasWin == true){
                printf("Le joueur 2 a gagné !");
            }
        }else{
            printf("Le joueur 1 a gagné !");
        }
    }





}


void IA(){


}

int main()
{
    MultiJoueur();
    return 0;

}
