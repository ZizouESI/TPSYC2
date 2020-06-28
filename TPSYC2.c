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
    long int secteur=512*(num_secteur-1); //calcul de début de secteur
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
