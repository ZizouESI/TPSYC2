#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TPSYC2.h"

/***
    SiFichierExiste : vérifie si un fichier avec "nom_fichier" donné existe ou non,
    elle retourne 1 si le fichier existe 0 si non.
    Entrées : nom du fichier
    Sorties : 1 ou 0
***/
int SiFichierExiste(char nom_fichier[]){
    FILE *fichier=NULL;
    //tester l'existance d'un fichier avec la fonction prédefinie "fopen"
	fichier = fopen(nom_fichier, "rb");
    if (fichier != NULL){
        fclose(fichier);
        return 1;
    }
    return 0;
}
/***
    list_disques : Affiche la liste des disques physiques connectés.
    Entrées : tableau de chaine de caractères (dans lequel on va récupérer les disque existants)
    Sorties : void
***/
void list_disques(char d[20][20]){
    //déclarations des chaines à utiliser
    char prefix[]="/dev/sd";
    char tab_disk[]="abcdefghij";
    char alpha;
    int i=0,exist=0,nbMax=10;
    //composer les noms des fichiers et tester s'il existe
    for(i=0;i<nbMax;i++){

        strcpy(prefix,"/dev/sd");
        alpha=tab_disk[i];
        char alphaNull[2]={alpha,'\0'};
        strcat(prefix,alphaNull);
		//tester l'existence du fichier
        exist=SiFichierExiste(prefix);
        if (exist){
            strcpy(d[i],prefix);
			printf("%s \n",prefix);
        }else{
			break;
		}
    }
}
/***
    lire_secteur : Lire le secteur ayant le numéro Num_secteur du disque physique disque_physique.
    Entrées : nom du disque physique , numéro du secteur dans le disque , buffer dans lequel on va récuperer les données
    Sorties : void
***/
void lire_secteur(char disque_physique[],int num_secteur,unsigned char buffer[512]){
    int depl,lect;
    long int secteur=512*num_secteur; //calcul de début de secteur
    FILE *fichier=NULL;
    fichier=fopen(disque_physique,"rb");
    if (fichier == NULL){
        printf("Erreur d'ouverture du disque physique %s",disque_physique);
		exit(1);
    }else{
        //déplacement par rapport au début du fichier
        depl=fseek(fichier,secteur,SEEK_SET);
        if (depl != 0){
            printf("Erreur dans le déplacement");
			exit(1);
        }else{
            //lecture du secteur
            lect=fread(buffer,512,1,fichier);
            if(lect<=0){
                printf("Erreur de lecture , n = %d",lect);
				exit(1);
            }
        }
    }
}
/***
    afficher_secteur : Appelle la fonction "lire_secteur" pour lire le secteur Num_secteur et affiche son contenu, en hexadécimal.
    Entrées : disque_physique et le numéro du secteur .
    Sorties : void , affiche directement les données en hexa du secteur demandé .
***/
void afficher_secteur(char disque_physique[],int num_secteur){
    //appel pour la fonction de lecture de secteur
    unsigned char buffer[512];
    lire_secteur(disque_physique,num_secteur,buffer);
    //récupération de la taille du buffer pour l'affichage
    size_t taille=sizeof(buffer);
    //un tableau pour mettre les caractères ascii pour l'affichage
    //on affiche 16 par 16
	char ascii[17];
	size_t i, j;
	ascii[16] = '\0';
	//parcourir le buffer
	for (i = 0; i < taille; ++i) {
        //affichage de l'adresse
        if (i % 16 == 0){
            printf("%04d ", i);
        }
        //imprimer en héxadecimal chaque caractère
		printf("%02X ", buffer[i]);
        //controller les extrimités du codage ascii et remplissage du tableau "ascii" s'il y'a eu caractère
		if (buffer[i] >= ' ' && buffer[i] <= '~') {
			ascii[i % 16] = buffer[i];
		} else {
		    //si NULL on met un point
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == taille) {
			printf(" ");
			if ((i+1) % 16 == 0) {
                //affichage ascii
				printf("|  %s \n", ascii);
			} else if (i+1 == taille) {
			    //dernier caractère
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
					printf(" ");
				}
                //compléter avec des caractères vides si taille % 16 != 0 , et puis afficher ascii
				for (j = (i+1) % 16; j < 16; ++j) {
					printf("   ");
				}
				printf("|  %s \n", ascii);
			}
		}
	}
}

/***
    lire_clusters : une procédure récursive pour lire toutes la hièrarchie des répértoires et fichier à partir de la "/"
    Entrées: fichier , nombre de secteurs resérvés , secteurs par cluster, déplacement vers la racine des répertoires , premier cluster du répertoire racine actuel ,cluster parent.
    Sorties: void , affiche la hièrarchie .

***/
void lire_clusters(char disque_physique[20],int16_t secteur_resreve,int32_t secteur_par_cluster,int32_t deplacement_vers_racine_rep,int32_t premier_cluster_rep_racine,int32_t cluster_parent){

    unsigned char buffer[512];
    int32_t cluster_actuel = premier_cluster_rep_racine;
    const int EOC = 0x0FFFFFF8;
    char nom_rep_pere[50]="/";
    //parcours des clusters
    while(cluster_actuel < EOC){
            printf("cluster = %p\n", cluster_actuel);
            //parcours des secteurs dans le cluster
            for(int i=0 ;i<secteur_par_cluster;i++){

                int num_secteur=(deplacement_vers_racine_rep/512) + (cluster_actuel - premier_cluster_rep_racine/*2*/) * secteur_par_cluster + i ;
                lire_secteur(disque_physique,num_secteur,buffer);
                Entree_format_court* entree = (Entree_format_court*) buffer; //découpage du buffer par entrees
                for(int j=0 ; j< 512/sizeof(Entree_format_court);j++){
                    if(entree[j].nom_fichier[0] != '\0'){ //pas une entree libre
                        //écrasement de vide entre fichier et extension dans l'entrée "nom_fichier" et vide après "extension"
                        entree[j].nom_fichier[strcspn(entree[j].nom_fichier,"\x20")]=0;
                        entree[j].extension[strcspn(entree[j].extension,"\x20")]=0;

                        if(entree[j].nom_fichier[0] != 0xE5 && entree[j].attribut != 0xf && !(entree[j].attribut & 0x10)){ //fichier non supprimé logiquement , bon à lire et ce n'est pas un répertoire
                            printf("\n   Fichier existant \n\tLe nom du fichier: %s\n\tExtension: %s\n\tTaille du fichier : %d octets \n\tNom du repertoire pere :  %s \n\tNumero de premier cluster dans la FAT : %p\n\n",entree[j].nom_fichier,entree[j].extension,entree[j].taille_fichier,nom_rep_pere,entree[j].numero_premier_cluster_fat_pfaible);
                        }else if (entree[j].nom_fichier[0] == 0xE5 && entree[j].attribut != 0xf){
                            printf("\n   Fichier supprime trouve \n\tLe nom du fichier:%s\n\tExtension: %s\n\tTaille du fichier: %d octets \n\tNom du repertoire pere :  %s \n\tNumero de premier cluster dans la FAT : %p\n\n",entree[j].nom_fichier,entree[j].extension,entree[j].taille_fichier,nom_rep_pere,entree[j].numero_premier_cluster_fat_pfaible);
                        }
                        if(entree[j].attribut & 0x10){//c'est un répertoire , test de bit 4
                            int32_t cluster_du_rep_actuel=entree[j].numero_premier_cluster_fat_pfaible | (entree[j].numero_premier_cluster_fat_pfort << 0x10);
                            //tester si on est pas dans . ou ..
                            if(cluster_du_rep_actuel != cluster_actuel && cluster_du_rep_actuel != cluster_parent && cluster_du_rep_actuel != 0x00000000){
                                //affichage des info du répertoire
                                printf("\n   repertoire existant \n\tLe nom du repertoire : %s\n\tNom de repertoire pere : %s\n\tNumero de premier cluster dans la FAT: %p \n\n ",entree[j].nom_fichier,nom_rep_pere,entree[j].numero_premier_cluster_fat_pfaible);
                                strcpy(nom_rep_pere,entree[j].nom_fichier);
                                //appel récursif pour lire les autres répertoires
                                lire_clusters(disque_physique,secteur_resreve,secteur_par_cluster,deplacement_vers_racine_rep+ (cluster_actuel - premier_cluster_rep_racine/*2*/) * secteur_par_cluster,cluster_du_rep_actuel,cluster_actuel);
                                lire_secteur(disque_physique,(deplacement_vers_racine_rep/512) + (cluster_actuel - premier_cluster_rep_racine/*2*/) * secteur_par_cluster+i,buffer);
                            }
                        }
                    }else{
                        //pas la peine de suivre , l'entrée n'existe pas
                        break;
                    }
                }
            }
            //lecture dans la fat pour pouvoir calculer le cluster suivant
            lire_secteur(disque_physique,secteur_resreve+ cluster_actuel*4/512,buffer);
            //calcul du cluster
            cluster_actuel=*(int32_t *) (buffer+((cluster_actuel*4)%512));
    }
}
/***
    Afficher_fdel : Afficher les fichiers/répertoires de la partition spécifiée en entrée de type FAT32
     en donnant pour chacun le nom en format court (nom en format long optionnel), sa taille en octets,
     son N° du premier cluster dans la FAT et le nom du répertoire père.
     Entrées: disque physique et la partition
     Sorties: void
***/
void afficher_fdel(char disque_physique[20],int num_partition){
    unsigned char secteur_boot[512];
    char nom_partition[20];
    int disque_physique_len=strlen(disque_physique);
    char num_partition_str[5];
    //itoa(num_partition,num_partition_str,10);
	sprintf(num_partition_str,"%d",num_partition);
    strcat(disque_physique,num_partition_str);
    // lecture du secteur boot
    lire_secteur(disque_physique,0,secteur_boot);
    //positionnement dans les différents paramètres de la partition FAT32
    int16_t octets_par_secteur= *(int16_t *)(secteur_boot+11);
    int8_t secteur_par_cluster= *(int8_t *)(secteur_boot+13);
    int16_t secteur_resreve=*(int16_t *)(secteur_boot+14);
    int8_t nombre_FAT= *(int8_t *)(secteur_boot+16);
    int32_t secteurs_par_fat= * (int32_t *)(secteur_boot+36);
    int32_t premier_cluster_rep_racine= *(int32_t *)(secteur_boot+ 44);
    int32_t deplacement_fat= secteur_resreve * octets_par_secteur;
    int32_t deplacement_vers_racine_rep= deplacement_fat + (nombre_FAT * secteurs_par_fat * octets_par_secteur);
    printf("FAT's Sont A %p \n",deplacement_fat);
    printf("Racine Des Repertoire A %p \n ",deplacement_vers_racine_rep);
    printf("\n***********************************************************\n");
    printf("Les informations globales\n");
    printf("Octets Par Secteur : %u\n",octets_par_secteur);
    printf("Secteurs Par Cluster : %u\n",secteur_par_cluster);
    printf("Secteurs Reserve: %u\n",secteur_resreve);
    printf("Nombre De FAT :%u \n",nombre_FAT);
    printf("Secteur Par FAT : %u\n",secteurs_par_fat);
    printf("Premier Cluster Du Repertoire Racine : %u",premier_cluster_rep_racine);
    printf("\n***********************************************************\n");
    //la procédure lire_cluster qui lit l'arborescence des répertoires et fichiers et les affiche en console
    lire_clusters(disque_physique,secteur_resreve,secteur_par_cluster,deplacement_vers_racine_rep,premier_cluster_rep_racine,premier_cluster_rep_racine);
}
