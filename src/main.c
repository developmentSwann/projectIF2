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
bool scorePoint(int n,struct Case plateau[n][n],int line,int column,int equipe){
    int i,j,score;
    // Allignement horizontal
    for (i=0;i<n;i++){
        if (plateau[line][i].isEmpty == false) {
            if (plateau[line][i].pion->equipe == equipe) {
                score++;
            }
            if (score == 4) {
                return true;
            }
        }
    }
    // Allignement vertical
    for (i=0;i<n;i++){
        if (plateau[i][column].isEmpty == false) {
            if (plateau[i][column].pion->equipe == equipe) {
                score++;
            }
            if (score == 4) {
                return true;
            }
        }
    }
    // Allignement diagonal haut gauche vers bas droite
    for (i=0;i<n;i++){
        for (j=0;j<n;j++){
            if (i==j){
                if (plateau[j][i].isEmpty == false) {
                    if (plateau[j][i].pion->equipe == equipe) {
                        score++;
                    }
                    if (score == 4) {
                        printf("Bien ouej");
                        return true;
                    }
                }
            }
        }
    }


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
            plateau[i-1][j-1] = (struct Case){i*(n)+j+1,j,i,(i*j)-n,(i*j)+1,(i*j)+n,(i*j)-1,(i*j)-n+1,(i*j)+n+1,(i*j)+n-1,(i*j)-n-1,true};
            printf(" X");
        }
    }
    printf("\n");
    printf("---------------------");
    printf("\n");
    struct Joueur joueur1 = (struct Joueur){1,1,0};
    struct Joueur joueur2 = (struct Joueur){2,2,0};
    while (joueur2.nbPion != 4 && joueur1.nbPion != 4){
        printf("-> Joueur 1 : Veuillez saisir les coordonnées d'une case vide (x,y): ");
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
        joueur1.nbPion++;
        pions[joueur1.equipe-1][joueur1.nbPion-1] = (struct Pion){joueur1.nbPion,joueur1.equipe,&plateau[y-1][x-1]};
        plateau[y-1][x-1].isEmpty = false;
        plateau[y-1][x-1].pion = &pions[joueur1.equipe-1][joueur1.nbPion-1];
        affichagePlateau(n,plateau);
        scorePoint(n,plateau,y-1,x-1,joueur1.equipe);
//        printf("-> Joueur 2 : Veuillez saisir les coordonnées d'une case vide (x,y): ");
//        scanf("%d,%d", &x, &y);
//        while (x>n || y>n || x<1 || y<1){
//            printf("-> Veuillez saisir des coordonnées valides (x,y): ");
//            scanf("%d,%d", &x, &y);
//        };
//        while (plateau[y-1][x-1].isEmpty == false){
//            printf("-> Veuillez saisir les coordonnées d'une case vide (x,y): ");
//            scanf("%d,%d", &x, &y);
//        };
//        joueur2.nbPion++;
//        pions[joueur2.equipe-1][joueur2.nbPion-1] = (struct Pion){joueur2.nbPion,joueur2.equipe,&plateau[y-1][x-1]};
//        plateau[y-1][x-1].isEmpty = false;
//        plateau[y-1][x-1].pion = &pions[joueur2.equipe-1][joueur2.nbPion-1];
//        affichagePlateau(n,plateau);


    }




}




int main()
{
    MultiJoueur();
    return 0;
}
