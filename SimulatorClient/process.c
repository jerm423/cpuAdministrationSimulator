#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <time.h>
#include "process.h"


void sendProcessToServer (char * processInfo){


    int sleepNumber = getRandomNumber();
    sleep(sleepNumber);
    //sleep(sleepNumber);

    //printf("Estoy en el socket con:\n");
    //printf("Proceso: %s\n", processInfo);
    //printf("Sleep: %d\n", sleepNumber);


    int sockfd = 0, n = 0;
    char send_data[1024], recvBuff[1024];
    struct sockaddr_in serv_addr;


    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {

        printf("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    }


    //printf("\nSEND a message (q or Q to quit) : ");
    //gets(send_data);

    send(sockfd,processInfo,strlen(processInfo), 0);

    close(sockfd);

}


//Processess


/// <summary>
/// Method to allocate memory for the thread list to use
/// </summary>
ProcessList* createProcessList()
{
    ProcessList* processList = (ProcessList*)malloc(sizeof(ProcessList));
    processList->firstNode = 0;
    processList->len = 0;
    return processList;
}



Process* createProcess(char *pProcessInfo)
{
    
    Process* process = (Process*)malloc(sizeof(Process));
    process->processInfo = pProcessInfo;
    process->nextNode = 0;
    return process;
}


void insertProcess(char *pProcess,ProcessList *pList)
{
    
    if(!pList->firstNode){
        pList->firstNode = pProcess;
    }
    else{
        Process * nextNode = pList->firstNode;
        while(nextNode->nextNode){
            nextNode = nextNode->nextNode;
        }
        nextNode->nextNode = pProcess;
    }
    pList->len++;
}



void printList(ProcessList* pList)
{
    Process * nextNode = pList->firstNode;

    while(nextNode)
    {
        printf("process: %s",nextNode->processInfo);
        printf("%c",' ');
        nextNode = nextNode->nextNode;
        printf("\n");
    }

}



//Threads


void insertNode(Thread *pThread,ThreadList *pList)
{
    Node *pNode = (Node *)malloc(sizeof(Node));
    pNode->thread=pThread;
    pNode->nextNode = 0;
    if(!pList->firstNode){
        pList->firstNode = pNode;
    }
    else{
        Node * nextNode = pList->firstNode;
        while(nextNode->nextNode){
            nextNode = nextNode->nextNode;
        }
        nextNode->nextNode = pNode;
    }
    pList->len++;
}

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

void detachThreadList(ThreadList* pList)
{
    Node * nextNode = pList->firstNode;
    while(nextNode){
        pthread_detach(nextNode->thread->thread);
        nextNode = nextNode->nextNode;
    }
}



ThreadList* createThreadList()
{
    ThreadList* _ThreadList = (ThreadList*)malloc(sizeof(ThreadList));
    _ThreadList->firstNode = 0;
    _ThreadList->len = 0;
    return _ThreadList;
}

/// <summary>
/// Creates a struct with a thread and put it on pList
/// </summary>
Thread* createThread(ThreadList *threadList, char *processInfo)
{
    Thread *thread = (Thread*)malloc(sizeof(Thread));
    thread->id = threadList->len;
    insertNode(thread,threadList);

    sleep(1);
    
    if(pthread_create(&thread->thread, 0, sendProcessToServer, processInfo))
    {
        fprintf(stderr, "Error creating thread\n");
    }

    return thread;
}

