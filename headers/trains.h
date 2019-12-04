#include <stdlib.h>
#include <stdbool.h>

typedef struct temps
{
    int heure;
    int minute;
}Temps;

typedef struct train
{
    int numero;
    char *villeDepart;
    char *villeArrivee;
    Temps heureDep;
    Temps heureArr;
    double prix;
    char *reduc;
}Train;

typedef struct cellule{
    Train leTrain;
    struct cellule* suivant;
}Cellule;
typedef Cellule * PCellule;
//inserTete(&(dernier->suivant), train)
Temps stringToTemps(char*);
Temps duree(Train );
Cellule * getTBTrains(char *, char *, char* ,Cellule *);
Cellule * getTHTrains(char *, char *, char* ,Cellule *);
Cellule * getTVTrains(char *, char *, Cellule *);

int inferieur(Temps t1, Temps t2);
int superieur(Temps t1, Temps t2);