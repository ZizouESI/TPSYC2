#ifndef __TPSYC2__
#define __TPSYC2__
/**
    les prototypes des fonctions et procedures
**/
int SiFichierExiste(char nom_fichier[]);
void list_disques(char d[20][20]);
void lire_secteur(char disque_physique[],int num_secteur,unsigned char buffer[512]);
void afficher_secteur(char disque_physique[],int num_secteur);

#endif // __TPSYC2__
