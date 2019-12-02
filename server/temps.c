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

Temps stringToTemps(char* heure){
    Temps h;
    h.heure = atoi(strtok(heure,":"));
    h.minute = atoi(strtok(NULL,":"));
    return h;
}