#include <stdio.h>
#include "TPSYC2.c"

int main()
{
    char disques_con[20][20];
    list_disques(disques_con);
    printf("%s \n", disques_con[0]);
    afficher_secteur(disques_con[0],1);
    return 0;
}
