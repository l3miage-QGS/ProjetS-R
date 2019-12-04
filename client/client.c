#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "../headers/trains.h"

#define MAX 40000

Cellule *recupTrain(int, int);
void afficherTrains(Cellule *trains);

int main(int argc, char *argv[], char *arge[])
{
	int connection = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(7777);
	struct hostent *host = gethostbyname("localhost");
	memcpy(&sin.sin_addr.s_addr, host->h_addr_list[0], sizeof(sin.sin_addr.s_addr));
	int echange;
	echange = connect(connection, (struct sockaddr *)&sin, sizeof(sin));
	printf("Je suis connecté\n");
	char tampon[MAX];
	char hor[MAX];
	char reponse[MAX];
	char req[MAX];
	char dep[MAX];
	char arr[MAX];
	int nbLus;
	printf("Recherche avec ville seul(TV), avec une horraire (TH), avec deux horraires(TB) ? ");
	fscanf(stdin, "%s", req);
	printf("Ville de départ : ");
	fscanf(stdin, "%s;", dep);
	printf("Ville d'arrivé : ");
	fscanf(stdin, "%s", arr);
	if (strcmp(req, "TV") == 0)
	{
		sprintf(reponse, "%s;%s;%s\n", req, dep, arr);
	}
	if (strcmp(req, "TH") == 0)
	{
		printf("Horraire (XX:XX): ");
		fscanf(stdin, "%s", hor);
		sprintf(reponse, "%s;%s;%s;%s\n", req, dep, arr, hor);
	}
	if (strcmp(req, "TB") == 0)
	{
		printf("Horraire (XX:XX-XX:XX) : ");
		fscanf(stdin, "%s", hor);
		sprintf(reponse, "%s;%s;%s;%s\n", req, dep, arr, hor);
	}
	write(connection, reponse, strlen(reponse) + 1);

	nbLus = read(connection, tampon, MAX);
	int nbTrains = atoi(tampon);
	printf("%s\n", tampon);

	if (nbTrains==0){
		printf("Aucun trains disponible\n");
	}else{
		Cellule *trains = recupTrain(nbTrains, connection);
		afficherTrains(trains);
	}
}

Cellule *recupTrain(int nbTrains, int connection)
{
	int i = 0;
	Cellule *trains = malloc(sizeof(Cellule));

	while (i < nbTrains)
	{
		Train *t = malloc(sizeof(Train));
		char tampon[MAX];

		char h1[5];
		char h2[5];
		char numero[MAX];
		char vDep[MAX];
		char vAr[MAX];
		char prix[MAX];

		char *ptr;



		int nbLus = read(connection, tampon, MAX);
		sscanf(tampon, "%[^;\n];%[^;\n];%[^;\n];%[^;\n];%[^;\n];%[^;\n]",
			   numero, vDep, vAr, h1, h2, prix);
		t->villeDepart = vDep;
		t->villeArrivee = vAr;
		t->numero = atoi(numero);
		t->prix = strtod(prix, &ptr);
		t->heureArr = stringToTemps(h2);
		t->heureDep = stringToTemps(h1);

		printf("%s\n",t->villeArrivee);

		inserTete(&trains, *t);
		i++;
	}
	return trains;
}

void afficherTrains(Cellule *trains)
{
	Cellule * t = trains; 
	char reponse[MAX];

	sprintf(reponse, "Trains disponibles : \n  N \t\tDepart \t\t   Arrivee\t\tHeure D \t Heure A \t  Prix\n");

	while (t->suivant != NULL)
	{
		char chaine[MAX];
		sprintf(chaine, "%d %20s  %20s \t %02d:%02d \t\t  %02d:%02d \t %3.2f€\n",
				t->leTrain.numero, t->leTrain.villeDepart, t->leTrain.villeArrivee,
				t->leTrain.heureDep.heure, t->leTrain.heureDep.minute,
				t->leTrain.heureArr.heure, t->leTrain.heureArr.minute,
				t->leTrain.prix);
		strcat(reponse, chaine);
		t = t->suivant;
	}
	printf("%s", reponse);
}