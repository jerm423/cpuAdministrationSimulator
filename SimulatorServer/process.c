#include "params.h"
#include "process.h"



ProcessList* createProcessList()
{
    ProcessList* _ProcesssList = (ProcessList*)malloc(sizeof(ProcessList));
    _ProcesssList->firstNode = 0;
    _ProcesssList->len = 0;
    return _ProcesssList;
}



/// <summary>
/// Creates a struct with a thread and put it on pList
/// </summary>


Process* createProcess(int pPid, int pBurst, int pPriority, void* arg)
{


    params_t *params = (params_t*)(arg);
    //int processLenght1 = params->processList->len;
    //printf("prcess 1 :    %d\n", processLenght1);    


    Process *process = (Process*)malloc(sizeof(Process));
    process->pid = pPid;
    process->burst = pBurst;
    process->priority = pPriority;

    insertNode(process,params);

    //int processLenght2 = params->processList->len;
    //printf("prcess 2 :    %d\n", processLenght2);  

    //printList(params->processList);

    //return thread;
}

void append(char* s, char c)
{
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
}



