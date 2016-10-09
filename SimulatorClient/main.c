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
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

void sendProcessToServer(int min, int max);
int  getRandomNumber();
void moveFileIntoArray(ProcessList *processList);
void createSenderThreads(ProcessList *pList, ThreadList *tList);
void joinSenderThreads(ThreadList *tList);


int getInt()
{
  int n = 0;
  char buffer[128];
  fgets(buffer,sizeof(buffer),stdin);
  n = atoi(buffer);
  return ( n > 100 || n < 1 ) ? 0 : n;
}


void makePause(){
    int i;
    for (i = 5; i > 0; --i)
    {
        printf("%d...\n",i);
        sleep(1);
    }
}

int main(int argc, char *argv[])
{


    while(1){

        printf("\033[2J\033[1;1H");
        printf(COLOR_GREEN"Bienvenido al Cliente del Simulador de Algoritmos de Procesos!"COLOR_RESET"\n");
        printf("\n");
        printf("Seleccione el metodo de funcionamiento del cliente:\n");
        printf("\n");
        printf(COLOR_GREEN"1.)"COLOR_RESET" Cargar los procesos desde el archivo\n");
        printf(COLOR_GREEN"2.)"COLOR_RESET" Cargar los procesos manualmente\n\n");

        //printf(COLOR_BLUE "%c"COLOR_RESET,tmpSquare->identifier );


        printf("Su opcion: ");
        int  option = getInt();

        //flush();
        if(option == 1){
            flush();
            printf("\033[2J\033[1;1H");

            printf(COLOR_GREEN"Se esta iniciando la carga del archivo de procesos "COLOR_RESET"\n");

            ProcessList *processList = createProcessList();
            moveFileIntoArray(processList);
            ThreadList *threadList = createThreadList();
            createSenderThreads(processList, threadList);
            detachThreadList(threadList);

            //scanf("%s", NULL);

            //flush();

        }
        else if(option == 2){
            //flush();
            printf("\033[2J\033[1;1H");

            printf("\nIngrese el tiempo de ramp-up (entre 0 y 100) en segundos para la creacion de los procesos: ");
            int  rampUp = getInt();


            if(rampUp >= 1 && rampUp <= 100 ){

                printf("\n\nEl ramp-up para la creacion de los procesos es %d\n", rampUp);
                printf(COLOR_YELLOW"\nPresione ENTER en cualquier momento para salir."COLOR_RESET"\n");
                printf("\n"COLOR_GREEN"Generando prcesos aleatoriamente..."COLOR_RESET"\n");


                int burst,priority;
                int loopCounter = 0;

                while(1){
                    burst = getRandomNumber(1,21);
                    priority = getRandomNumber(1,6);

                    char processInfo[1024];
                    printf("\n== Enviando proceso aleatorio con PID %d, BURST %d, PRIORITY %d==\n", loopCounter, burst, priority );
                    sprintf( processInfo, "%d,%d,%d ",loopCounter, burst, priority );
                    Process * process = createProcess(loopCounter,burst,priority);
                    pthread_t thread;
                    pthread_create(&thread, 0, sendProcessToServer, process);
                    sleep(rampUp);
                    loopCounter++;
                }
                
            }
            else{
                printf(COLOR_RED"El valor ingresado para el ramp-up no es valido. Intente de nuevo."COLOR_RESET"\n");
                makePause();
            }






        }
        else{
            printf(COLOR_RED"La opcion no es valida"COLOR_RESET"\n");
            makePause();
        }

        //flush();

    }

    return 0;
}


void moveFileIntoArray(ProcessList *processList){

    //We open the file and read it
    char fileAddress[256] = "";
    //flush();
    printf("Ingrese la direccion en la que se enuentra el archivo con la informacion de los procesos:\n\n" );
    //fgets(fileAddress,sizeof(fileAddress),stdin);
    scanf("%s",&fileAddress);
    //printf("%s\n", fileAddress);
    //FILE* file = fopen("/home/joseph/Desktop/cpuAdministrationSimulator/SimulatorClient/entryFiles/source.txt", "r");
    FILE* file = fopen(fileAddress, "r");
    if (file == NULL)
    {
        printf(COLOR_RED"Error opening file"COLOR_RESET"\n");
        makePause();
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


        Process * process = createProcess(atoi(parameters[0]),atoi(parameters[1]),atoi(parameters[2]));
        insertProcess(process,processList);

    }

    fclose(file);

}

void createSenderThreads(ProcessList *pList, ThreadList *tList){



    Process * nextNode = pList->firstNode;

    while(nextNode)
    {
        printf("\nEnviando al servidor un proceso con ID %d ", (int) nextNode->pid);
        printf(" , BURST %d ", (int) nextNode->burst);
        printf("y PRIORIDAD %d ", (int) nextNode->priority);
        printf("\n");
        createThread(tList,nextNode);
        nextNode = nextNode->nextNode;
        printf("\n");

    }


}



int getRandomNumber(int min, int max){

    srand(time(NULL));
    return(rand()%(max-min)+min);

}


void flush()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}
