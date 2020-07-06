#include <stdio.h>
#include "TPSYC2.c"

int main()
{
    char disques_con[20][20];
	printf("\nLa liste des disques connectes : \n\n");
    list_disques(disques_con);
    printf("\naffichage du contenu d'un secteur : \n\n");
    afficher_secteur(disques_con[1],0); //secteur 0
	printf("\nAffichage des repertoires et fichiers d'une partition :\n\n ");
    char disk[20]="/dev/sdb";
    afficher_fdel(disk,1);
    return 0;
}
