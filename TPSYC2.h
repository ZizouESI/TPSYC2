#ifndef __TPSYC2__
#define __TPSYC2__
#include <unistd.h>
/**
    les prototypes des fonctions et procedures
**/
void afficher_fdel(char disque_physique[20],int num_partition);
int SiFichierExiste(char nom_fichier[]);
void list_disques(char d[20][20]);
void lire_secteur(char disque_physique[],int num_secteur,unsigned char buffer[512]);
void lire_clusters(char disque_physique[20],int16_t secteur_resreve,int32_t secteur_par_cluster,int32_t deplacement_vers_racine_rep,int32_t premier_cluster_rep_racine,int32_t cluster_parent);
void afficher_secteur(char disque_physique[],int num_secteur);
/** structure d'une entrée en format court **/
typedef struct Entree_format_court {
	unsigned char nom_fichier[8];
	unsigned char extension[3];
	int8_t attribut;
	int8_t reserver_WindowsNT;
	int8_t millisecondes_creation;
	int16_t heure_creation;
	int16_t date_creation;
	int16_t date_dernier_acces;
	int16_t numero_premier_cluster_fat_pfort; //poids fort pour FAT32
	int16_t heure_derniere_modification;
	int16_t date_derniere_modification;
	int16_t numero_premier_cluster_fat_pfaible; //poids faible
	int32_t taille_fichier;
} Entree_format_court;

#endif // __TPSYC2__
