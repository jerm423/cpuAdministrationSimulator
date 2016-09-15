#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED
#endif // THREAD_H_INCLUDED
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include "LabyrinthStructures.h"




/// <summary>
/// Struct to handle the matrix threads
/// </summary>
typedef struct Thread {

    int id;

    bool isActive;

    //int xCoord;
    //int yCoord;
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

