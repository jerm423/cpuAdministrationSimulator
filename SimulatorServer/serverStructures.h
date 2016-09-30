#ifndef SERVERSTRUCTURES_H_INCLUDED
#define SERVERSTRUCTURES_H_INCLUDED
#endif // SERVERSTRUCTURES_H_INCLUDED
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include "params.h"


typedef struct Process {
    int pid;
    int burst;
    int burstRestante;
    int burstTotal;
    int prioridad;
    int posicion;
    int t_llegada;
    int enabled;
    struct Process *nextNode;
} Process;



typedef struct Tat_Wt_Table {
    int pid;
    int turnAroundTime;
    int t_llegada;
    int t_salida;
    int waitingTime;
    struct Tat_Wt_Table *nextNode;
} Tat_Wt_Table;



