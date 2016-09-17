#include "params.h"

int getId(params_t *p)
{
    int id;
    pthread_mutex_lock(&(p->mutex));
    id = p->id;
    pthread_mutex_unlock(&(p->mutex));

    return id;

}

ProcessList * getProcessessList(params_t *p)
{
    ProcessList * pList;
    pthread_mutex_lock(&(p->mutex));
    pList = p->processList;
    pthread_mutex_unlock(&(p->mutex));

    return pList;
}

void setId(params_t *p, int val)
{
    pthread_mutex_lock(&(p->mutex));
    p->id = val;
    pthread_mutex_unlock(&(p->mutex));
}

void setAlgorithm(params_t *p, int alg)
{
    pthread_mutex_lock(&(p->mutex));
    p->algorithm = alg;
    pthread_mutex_unlock(&(p->mutex));
}

void incrementId(params_t *p)
{
    pthread_mutex_lock(&(p->mutex));
    p->id++;
    pthread_mutex_unlock(&(p->mutex));
}

/// <summary>
/// Method to insert a new thread node to the thread list
/// </summary>
void insertNode(Process *pProcess,params_t *p)
{
    ProcessNode *processNode = (ProcessNode *)malloc(sizeof(ProcessNode));

    processNode->process=pProcess;

    processNode->nextNode = 0;

    /*
    printf("el len de esta vara es \n");
    printf("Hello from %d\n", p->processList->len);
    */
    
    if(p->processList->len == 0){
        p->processList->firstNode = processNode;
        //pList->firstNode = pNode;
    }
    
    else{
        ProcessNode * nextNode = p->processList->firstNode;
        while(nextNode->nextNode){
            nextNode = nextNode->nextNode;
        }
        nextNode->nextNode = processNode;
    }
    p->processList->len++;
    
}

void* hello(void* arg){
    params_t *p = (params_t*)(arg);
    incrementId(p);
    int id = getId(p);

    // This could possibly be quite messy since it
    // could print the data for multiple threads at once
    printf("Hello from %d\n", id);
}









/// <summary>
/// Method print the thread node values
/// </summary>
void printNode(ProcessNode* pNode)
{
    printf("pid: %d, burst: %d, priority: %d",pNode->process->pid,pNode->process->burst,pNode->process->priority);
}



/// <summary>
/// Method to print the threadlist nodes information
/// </summary>
void printList(ProcessList* pList)
{
    ProcessNode * nextNode =pList->firstNode;

    while(nextNode)
    {
        printNode(nextNode);
        printf("%c",' ');
        nextNode = nextNode->nextNode;
        printf("\n");
    }

}






