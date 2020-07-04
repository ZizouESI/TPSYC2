#include <stdio.h>


int main()
{
    /*char disques_con[20][20];
    list_disques(disques_con);
    printf("%s \n", disques_con[0]);
    afficher_secteur(disques_con[0],1);*/
    char disk[20]="/dev/sdb";
    afficher_fdel(disk,1);

    return 0;
}
