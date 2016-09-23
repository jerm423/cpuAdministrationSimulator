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

void sendProcessToServer ();
int getRandomNumber();
void moveFileIntoArray(ProcessList *processList);
void createSenderThreads(ProcessList *pList, ThreadList *tList);
void joinSenderThreads(ThreadList *tList);




int main(int argc, char *argv[])
{

    
    while(1){

        flush();
        
        printf("Bienvenido al Cliente del Simulador de Algoritmos de Procesos!\n");
        printf("\n");
        printf("Seleccione el metodo de funcionamiento del cliente:\n");
        printf("\n");
        printf("1.) Cargar los procesos desde el archivo\n");
        printf("2.) Cargar los procesos manualmente\n");

        int  option;

        scanf("%d", &option);

        if(option == 1){
            flush();
            printf("Se esta iniciando la carga del archivo de procesos %s\n");

            ProcessList *processList = createProcessList();

            moveFileIntoArray(processList);

            ThreadList *threadList = createThreadList();

            createSenderThreads(processList, threadList);


            detachThreadList(threadList);

        }
        else if(option == 2){
            printf("Ingrese el burst que desea para el nuevo proceso\n");
        }
        else{
            printf("La opcion es valida\n");
        }

        

    }

    return 0;
}


void moveFileIntoArray(ProcessList *processList){

    //We open the file and read it
    FILE* file = fopen("entryFiles/source.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return(0);
    }

    char line[256];

    char *p;
    char *parameters[3];
    //Get the first line
    fgets(line, sizeof(line), file);
    //Get the rest of the lines
    int i = 0;

    while (fgets(line, sizeof(line), file)) {

        i = 0;
        p = strtok (line, " ");
        char pid, burst, priority;


        while (p != NULL)
        {
            parameters[i++] = p;
            p = strtok (NULL, " ");
        }

        p = NULL;

        char * comma = ",";
        char * processInfo = (char *) malloc(1 + strlen(parameters[0])+ strlen(parameters[1]) + strlen(parameters[2]) );
        strcpy(processInfo, parameters[0]);
        strcat(processInfo, comma);
        strcat(processInfo, parameters[1]);
        strcat(processInfo, comma);
        strcat(processInfo, parameters[2]);

        Process * process = createProcess(processInfo);
        insertProcess(process,processList);

    }

    fclose(file);

}

void createSenderThreads(ProcessList *pList, ThreadList *tList){

    Process * nextNode = pList->firstNode;

    while(nextNode)
    {
        printf("Enviando al servidor un proceso con parametros: %s\n", nextNode->processInfo);
        createThread(tList,nextNode->processInfo);
        nextNode = nextNode->nextNode;
        printf("\n");
    }

}


int getRandomNumber(){
    int r, i;
    int N = 1,
    M = 20;
    /* initialize random seed: */
    srand (time(NULL));
    /* generate secret number between 1 and 20: */
    r = M + rand() / (RAND_MAX / (N - M + 1) + 1);
    return r;
}


void flush()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}
