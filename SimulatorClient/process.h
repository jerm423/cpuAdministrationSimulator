#ifndef PROCESS_H_INCLUDED
#define PROCESS_H_INCLUDED
#endif // PROCESS_H_INCLUDED
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Process {
    int pid;
    int burst;
    int priority;
    struct Process *nextNode;
} Process;


typedef struct ProcessList {

    Process * firstNode	;
    int len;

} ProcessList;



typedef struct Thread {

    int id;
    pthread_t thread;


} Thread;

/// <summary>
/// Strcut to handle the thread list nodes
/// </summary>
typedef struct Node {

	Thread *thread;
	struct Node* nextNode;

} Node;

/// <summary>
/// Struct to handle the Thread list that loops the matrix
/// </summary>
typedef struct ThreadList {

	Node * firstNode;
	int len;

} ThreadList;
