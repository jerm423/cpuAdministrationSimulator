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
    int t_llegada;
    struct Process *nextNode;
} Process;


typedef struct ProcessList {
	pthread_mutex_t mutex;
    Process * firstNode, *lastNode ;
    int len;

} ProcessList;


typedef struct Params {
        //pthread_mutex_t mutex;
        //ProcessList *processList;

} Params;
