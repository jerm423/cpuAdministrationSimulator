#ifndef PARAMS_H_INCLUDED
#define PARAMS_H_INCLUDED
#endif // PARAMS_H_INCLUDED
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "process.h"





/// <summary>
/// Struct to handle the matrix threads
/// </summary>
typedef struct Process {

    int pid;
    int burst;
    int priority;
    int executeCounter;
    int waitingCounter;
    bool isActive;


} Process;

/// <summary>
/// Strcut to handle the thread list nodes
/// </summary>
typedef struct ProcessNode {

	Process *process;
	struct ProcessNode* nextNode;

} ProcessNode;

/// <summary>
/// Struct to handle the Thread list that loops the matrix
/// </summary>
typedef struct ProcessList {

	ProcessNode * firstNode;
	int len;

} ProcessList;



struct params {
        pthread_mutex_t mutex;
        int id, algorithm;
        ProcessList *processList;

};

typedef struct params params_t;
