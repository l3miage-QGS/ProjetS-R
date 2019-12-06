#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../headers/trains.h"
#include "../headers/server.h"
#include "../headers/dialogue.h"

void reponseRequete(Cellule *trains, int echange, int nbtrains)
{
	char reponse[10];
	sprintf(reponse, "%d\n", nbtrains);
	write(echange, reponse, sizeof(reponse));
	while (trains->suivant != NULL)
	{
		char chaine[MAX];
		double prixReduc = getReduc(trains->leTrain);

		sprintf(chaine, "%d;%s;%s;%d:%d;%d:%d;%f;",
				trains->leTrain.numero, trains->leTrain.villeDepart, trains->leTrain.villeArrivee,
				trains->leTrain.heureDep.heure, trains->leTrain.heureDep.minute,
				trains->leTrain.heureArr.heure, trains->leTrain.heureArr.minute,
				prixReduc);
		trains = trains->suivant;
		printf("%s\n", chaine);
		write(echange, chaine, sizeof(chaine));
	}
}

Cellule *recupTrain(int nbTrains, int connection, Cellule **trains)
{
	int i = 0;

	for (int i = 0; i < nbTrains; i++)
	{
		Train *t = malloc(sizeof(Train));
		char tampon[MAX];

		char h1[MAX];
		char h2[MAX];
		char numero[MAX];
		char vDep[MAX];
		char vAr[MAX];
		char prix[MAX];
		char *ptr = malloc(sizeof(char));

		int nbLus = read(connection, tampon, MAX);
		printf("read: '%s'",tampon);
		sscanf(tampon, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;\n]",
			   numero, vDep, vAr, h1, h2, prix);
		t->villeDepart = vDep;
		t->villeArrivee = vAr;
		printf("test'%s'",h1);

		t->numero = atoi(numero);
		t->prix = strtod(prix, &ptr);
		t->heureArr = stringToTemps(h2);
		t->heureDep = stringToTemps(h1);
		printf("'%d'", t->heureDep.minute);

		inserTete(trains, *t);
	}
}

void makeRequete(char **reponse)
{
	char hor[MAX];
	char req[MAX];
	char dep[MAX];
	char arr[MAX];
	char rep[MAX];

	printf(PURPLE "Recherche avec ville seul(" YELLOW "TV" PURPLE "), avec une horraire (" 
	YELLOW "TH" PURPLE "), avec deux horraires(" YELLOW "TB" PURPLE ") ? ");
	printf(YELLOW);
	fscanf(stdin, "%s", req);
	printf(PURPLE "Ville de départ : ");
	printf(DEFAULT);
	fscanf(stdin, "%s;", dep);
	printf(PURPLE "Ville d'arrivé : ");
	printf(DEFAULT);
	fscanf(stdin, "%s", arr);
	if (strcmp(req, "TV") == 0)
	{
		sprintf(*reponse, "%s;%s;%s\n", req, dep, arr);
	}
	if (strcmp(req, "TH") == 0)
	{
		printf(PURPLE "Horraire (XX:XX): ");
		printf(DEFAULT);
		fscanf(stdin, "%s", hor);
		sprintf(*reponse, "%s;%s;%s;%s\n", req, dep, arr, hor);
	}
	if (strcmp(req, "TB") == 0)
	{
		printf(PURPLE "Horraire (XX:XX-XX:XX) : ");
		printf(DEFAULT);
		fscanf(stdin, "%s", hor);
		sprintf(*reponse, "%s;%s;%s;%s\n", req, dep, arr, hor);
	}
}

Cellule *getRequete(int connection, Cellule **trains, int *nbtrains)
{
	int nbLus;
	int nbtrainsAdd = 0;

	char tampon[MAX];
	nbLus = read(connection, tampon, MAX);
	*nbtrains = atoi(tampon);
	if(*nbtrains==-1){
		printf(RED"Erreur : protocole invalide\n");
		exit(EXIT_SUCCESS);
	}else{
		recupTrain(*nbtrains, connection, trains);
	}
}

void printRequete(int nbserv, Cellule *trains, int nbtrains)
{
	switch (nbtrains)
	{

	case 0:
		printf(CYAN "Il n'y a aucun train qui repond à vos attentes\n"DEFAULT);
		break;

	case 1:
		afficherTrains(CYAN "Seulement 1 train disponible", trains, nbtrains);
		break;

	default:
		if (nbserv == 1)
		{
			afficherTrains(CYAN "Voici Les trains disponibles ", trains, nbtrains);
		}
		else
		{
			printf(CYAN "Il y a %d trains disponible.\n"DEFAULT, nbtrains);
		}

		if (nbtrains > 1)
		{
			char ans[MAX];
			printf(CYAN "Voulez vous le trajet le plus rapide(" YELLOW "R" CYAN ") ? Le moins cher(" 
			YELLOW "P" CYAN ") ? " YELLOW "Q" CYAN " pour quitter\n");
			printf(YELLOW);
			fscanf(stdin, "%s", ans);
			if (strcmp(ans, "R") == 0)
			{
				Cellule *tMin = malloc(sizeof(Cellule));
				Train t1 = trierParTemps(trains, nbtrains);
				inserTete(&tMin, t1);
				afficherTrains(CYAN "Le train le plus rapide est", tMin, 1);
			}
			if (strcmp(ans, "P") == 0)
			{
				Train t1 = trierParPrix(trains, nbtrains);
				Cellule *tMinprix = malloc(sizeof(Cellule));
				inserTete(&tMinprix, t1);
				afficherTrains(YELLOW "Le train le moins cher est", tMinprix, 1);
			}
			if (strcmp(ans, "Q") == 0)
			{
				//close(connection);
				exit(EXIT_SUCCESS);
			}
		}
	}
}