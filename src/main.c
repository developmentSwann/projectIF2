#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
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
    int equipe;
    struct Case *pos;
};
struct Joueur {
    int id;
    int equipe;
    int nbPion;
    int score;
};

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
                printf(" X");
            }
            else{
                printf(" %d",plateau[i-1][j-1].pion->equipe);
            }
        }
    }
    printf("\n");
    printf("---------------------");
    printf("\n");
}

bool scorePoint(int n,struct Case plateau[n][n],struct Case actualCase,int posX,int posY){
    //Allignement horizontal
    //Check si le vosin de gauche est de la même équipe que la case actuelle tant qu'il y a un voisin de gauche de la même équipe, puis check si le voisin de droite est de la même équipe que la case actuelle tant qu'il y a un voisin de droite de la même équipe
    int equipe = actualCase.pion->equipe;
    int compteur = 0;
    //Allignement horizontal
    while (actualCase.voisinE != 0){
        if (plateau[posY][posX+1].pion){
            if (plateau[posY][posX+1].pion->equipe == equipe){

            compteur = compteur + 1;
            posX++;
}           else{
                break;
            }
        }
        else{
            break;
        }
    }
    while (actualCase.voisinO != 0){
        if (plateau[posY][posX-1].pion){
            if (plateau[posY][posX-1].pion->equipe == equipe){
            compteur = compteur + 1;
            posX--;
}           else{
                break;
            }

        }
        else{
            break;
        }
    }

    //Allignement vertical
    while (actualCase.voisinN != 0){
        if (plateau[posY+1][posX].pion){
            if (plateau[posY+1][posX].pion->equipe == equipe){
            compteur = compteur + 1;
            posY++;
}           else{
                break;
            }
        }
        else{
            break;
        }
    }
    while (actualCase.voisinS != 0){
        if (plateau[posY-1][posX].pion){
            if (plateau[posY-1][posX].pion->equipe == equipe){
            compteur = compteur + 1;
            posY--;
}           else{
                break;
            }
        }
        else{
            break;
        }
    }

    printf("Compteur : %d",compteur);

//    //Allignement diagonal haut droite
//    while (actualCase.voisinNE != (n+1)+n*posY){
//        if (plateau[posY+1][posX+1].pion){
//            if (plateau[posY+1][posX+1].pion->equipe == equipe){
//            compteur = compteur + 1;
//            posY++;
//            posX++;
//}           else{
//                break;
//            }
//        }
//        else{
//            break;
//        }
//    }
//    while (actualCase.voisinSO != (0)+n*posY){
//        if (plateau[posY-1][posX-1].pion){
//            if (plateau[posY-1][posX-1].pion->equipe == equipe){
//            compteur = compteur + 1;
//            posY--;
//            posX--;
//}           else{
//                break;
//            }
//        }
//        else{
//            break;
//        }
//    }
//
//    //Allignement diagonal haut gauche
//    while (actualCase.voisinNO != (0)+n*posY){
//        if (plateau[posY-1][posX+1].pion){
//            if (plateau[posY-1][posX+1].pion->equipe == equipe){
//            compteur = compteur + 1;
//            posY--;
//            posX++;
//}           else{
//                break;
//            }
//        }
//        else{
//            break;
//        }
//    }
//    while (actualCase.voisinSE != (n+1)+n*posY){
//        if (plateau[posY+1][posX-1].pion){
//            if (plateau[posY+1][posX-1].pion->equipe == equipe){
//            compteur = compteur + 1;
//            posY++;
//            posX--;
//}           else{
//                break;
//            }
//        }
//        else{
//            break;
//        }
//    }


    if (compteur > 3) {
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
    pions[joueur->equipe-1][joueur->nbPion-1] = (struct Pion){joueur->nbPion,joueur->equipe,&plateau[y-1][x-1]};
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
void MultiJoueur(){
    int n = 0,i,j;
    printf("-> Veuillez saisir la longueur du cote du plateau : ");
    scanf("%d", &n);
    struct Case plateau[n][n];
    struct Pion pions[4][4];
    while (n < 4){
        printf("-> Veuillez saisir une longueur supérieure ou égale à 4 : ");
        scanf("%d", &n);
    }
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
            printf(" X");
        }
    }
    printf("\n");
    printf("---------------------");
    printf("\n");

    struct Joueur joueur1 = (struct Joueur){1,1,0,0};
    struct Joueur joueur2 = (struct Joueur){2,2,0,0};
    bool hasWin = false;
    while (joueur2.nbPion != 4 && joueur1.nbPion != 4 && hasWin == false){
        hasWin = placePion(n,&joueur1,plateau,pions);
        if (hasWin == false){
            hasWin = placePion(n,&joueur2,plateau,pions);
        }
    }




}
int main()
{
    MultiJoueur();
    return 0;

}
