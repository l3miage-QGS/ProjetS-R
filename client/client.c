#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "../headers/trains.h"
#include "../headers/dialogue.h"
#include "../headers/statistique.h"
#define MAX 400

int main(int argc, char *argv[], char *arge[])
{	
	int nbserv = argc - 1;

	// demande admin si seulement 1 serveur
	if (nbserv == 1)
	{
		admin();
	}

	//recuperation des numeros de serveur auxquels se connecter 
	int serv[nbserv];
	for (int i = 1; i < argc; i++)
	{
		serv[i - 1] = atoi(argv[i]);
	}

	//Configuration des ports et connection aux serveurs
	int connection[nbserv];
	struct sockaddr_in sin[nbserv];
	struct hostent *host[nbserv];
	for (int i = 0; i < nbserv; i++)
	{
		connection[i] = socket(AF_INET, SOCK_STREAM, 0);
		sin[i].sin_family = AF_INET;
		sin[i].sin_port = htons(PORT + serv[i]);
		host[i] = gethostbyname("localhost");
		memcpy(&sin[i].sin_addr.s_addr, host[i]->h_addr_list[0], sizeof(sin[i].sin_addr.s_addr));
		int echange = connect(connection[i], (struct sockaddr *)&sin[i], sizeof(sin[i]));
	}

	int nbLus;
	Cellule *trains = malloc(30*sizeof(Cellule));
	int nbtrains = 0;
	char *reponse = malloc(128*sizeof(char));

	//demande des parametres de la requete qui seront ecris dans "reponse"
	makeRequete(&reponse);

	//traitement de la requete avec les serveurs
	for (int i = 0; i < nbserv; i++)
	{
		int nbtrainsserv;
		//envoi de la requete au serveur i
		write(connection[i], reponse, strlen(reponse) + 1);
		//recuperation des resultats de la requete dans "trains"
		getRequete(connection[i], &trains, &nbtrainsserv);
		nbtrains += nbtrainsserv;
	}
	free(reponse);

	//affichage des resultats
	printRequete(nbserv, trains, nbtrains);
}
