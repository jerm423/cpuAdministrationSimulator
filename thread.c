#include "thread.h"

// Necessary sleep variables
#define NANO_SECOND_MULTIPLIER  1000000  // 1 millisecond = 1,000,000 Nanoseconds
pthread_mutex_t mut;











/// <summary>
/// Method to wait all threads to finish!
/// </summary>
void joinThreadList(ThreadList* pList)
{
	Node * nextNode = pList->firstNode;
	while(nextNode){
        pthread_join(nextNode->thread->thread,0);
		nextNode = nextNode->nextNode;
	}
}

void *runThread(void  *threadArg)
{

    Thread *thread = (Thread*)threadArg;
    printf("Molitaa!\n");
    /*
    sleepValue.tv_nsec = INTERVAL_MS;
    int step = thread->direction == 0 || thread->direction == 2 ? -1 : 1;

    while (thread->isActive)
    {
        // Mark the square as the thread passes
        thread->currentSquare->directionStatuses[thread->direction] = true;
        thread->currentSquare->lastDirection = thread->direction;


        if(thread->currentSquare->identifier == '/')
        {
            printf("------------------------------------------------------ \n");
            printf("Thread Id: %d, found exit in direction: %d \n",thread->id,thread->direction);
            printf("Thread Id: %d, X: %d, Y: %d, Direction: %d , Steps: %d, Square Value: %c \n",
                thread->id,thread->coordinates->lastCoordinate->xCoord,
                thread->coordinates->lastCoordinate->yCoord,thread->direction,
                thread->steps, thread->currentSquare->identifier);
            printf("------------------------------------------------------ \n");
            break;
        }

        if(isValidMove(thread->coordinates->lastCoordinate,thread->direction,step,thread->squareMatrix))
        {
            changeThreadPosition(thread,step);
            assignThreadSquare(thread);
            thread->steps++;
            if(thread->currentSquare->identifier != '/') createChildThreads(thread);
        }
        else
        {

            break;
        }

        //nanosleep(&sleepValue, NULL);
    }
    pthread_exit(NULL);
    return NULL;
    */
}


/// <summary>
/// Creates a struct with a thread and put it on pList
/// </summary>
Thread* createThread()
{
	Thread *thread = (Thread*)malloc(sizeof(Thread));

	thread->isActive = true;

    if(pthread_create(&thread->thread, 0, runThread, thread))
    {
		fprintf(stderr, "Error creating thread\n");
	}

	return thread;
}





/// <summary>
/// Locks the thread
/// </summary>
void lock()
{
    pthread_mutex_lock(&mut);
}

/// <summary>
/// Unlocks the thread
/// </summary>
void unlock()
{
    pthread_mutex_unlock(&mut);
}



