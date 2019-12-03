#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../headers/server.h"


Cellule * getTrains(char* depart, char* arrivee, char* heure, Cellule* trains){
    Temps * hDebReq = malloc(sizeof(Temps));
    Temps * hFinReq = malloc(sizeof(Temps));
    printf("Aurevoir\n");
    char* p = strtok(heure,";");
    * hDebReq = stringToTemps(p);
    p = strtok(NULL,";");

    if (p!=NULL){
       * hFinReq = stringToTemps(p);
    }

    Cellule * t = trains;
    Cellule * bonTrains = malloc(sizeof(Cellule));
    int i = 0;
    while(t!=NULL){
        printf("i = %d\n",i);
        if(strcmp(t->leTrain.villeDepart, depart)==0){
            if(strcmp(t->leTrain.villeArrivee, arrivee)==0){
                if(superieur(t->leTrain.heureDep,*hDebReq)){
                    if(hFinReq==NULL||inferieur(t->leTrain.heureDep,*hDebReq)){
                        inserTete(&bonTrains,t->leTrain);
                        printf("bonj%d\n",i);
                    }
                }
            }
        }
        i++;
        t = t->suivant;
    }

    return bonTrains;
}

Temps duree(Train t)
{
    int tDepMin = t.heureDep.heure * 60 + t.heureDep.minute;
    int tArrMin = t.heureArr.heure * 60 + t.heureArr.minute;
    int durre = tArrMin - tDepMin;
    Temps dure;
    dure.heure=durre / 60;
    dure.minute= durre % 60;
    return dure;
}