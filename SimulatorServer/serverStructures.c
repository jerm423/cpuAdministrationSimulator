#include "serverStructures.h"


/*
PROCESSESS!!!!!!!!!!!!!!!!!
*/

ProcessList* createProcessList()
{
    ProcessList* processList = (ProcessList*)malloc(sizeof(ProcessList));
    processList->firstNode = processList->lastNode = NULL;
    processList->len = 0;
    return processList;
}





void printList(Process * head)
{
    Process * current = head;


    while (current != NULL) {
        printf("%d\n", current->pid);
        current = current->nextNode;
    }
    

}

