#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "serverStructures.h"

void *addCpuOcioso();
int  *threadTiempo();
void  flush ();
void *startSimulator ();
void *jobSchedulerTask ();
void *cpuFIFOSchedulerTask (void* arg);
void *cpuSJFSchedulerTask (void* arg);
void *cpuHPFSchedulerTask (void* arg);
void *cpuRRSchedulerTask (void* arg);

pthread_mutex_t mutexReady;
struct Process *primero,*ultimo;
int reloj = 0;
int cpuOcioso = 0;
int readyQueueLenght = 0;

void printReadyQueueLenght(){
    printf("Largo de la lista de ready: %d\n", readyQueueLenght);
}

int addElementToReadyQueue(int p_Pid, int p_Burst, int p_Prioridad, int p_Tiempo_Llegada) {
    struct Process *nuevo;
    /* reservamos memoria para el nuevo elemento */
    nuevo = (struct Process *) malloc (sizeof(struct Process));
    if (nuevo==NULL){
        printf( "No hay memoria disponible!\n");
        return 0;
    }

    //Se agreagan los datos al nuevo struct
    nuevo->pid = p_Pid;
    nuevo->burst = p_Burst;
    nuevo->burstRestante = p_Burst;
    nuevo->burstTotal = p_Burst;
    nuevo->prioridad = p_Prioridad;
    nuevo->t_llegada = p_Tiempo_Llegada;

    /* el campo siguiente va a ser NULL por ser el último elemento
     de la lista */
    nuevo->nextNode = NULL;

    /* ahora metemos el nuevo elemento en la lista. lo situamos
     al final de la lista */
    /* comprobamos si la lista está vacía. si primero==NULL es que no
     hay ningún elemento en la lista. también vale ultimo==NULL */
    if (primero==NULL) {
        primero = nuevo;
        primero->pid = p_Pid;
        primero->burst = p_Burst;
        primero->burstRestante = p_Burst;
        primero->burstTotal = p_Burst;
        primero->prioridad = p_Prioridad;
        primero->t_llegada = p_Tiempo_Llegada;
        primero->nextNode = NULL;
        ultimo = primero;
     }
    else {
       /* el que hasta ahora era el último tiene que apuntar al nuevo */
       ultimo->nextNode = nuevo;
       /* hacemos que el nuevo sea ahora el último */
       ultimo = nuevo;
       ultimo->nextNode = NULL;
    }

    readyQueueLenght ++;    
    //free(nuevo);
}

void printReadyQueue() {
    
    //En caso de cola vacía
    if (primero == NULL) {
        printf("--- Cola vacia! ---\n\n");
        return 0;
    }

    printf("\n\n--------- Procesos en la cola de ready ---------\n\n");

    struct Process *actual = primero;

    while (actual != ultimo) {
        printf("--- ID: %d ---\n", actual->pid);
        printf("--- Burst: %d ---\n", actual->burst);
        printf("--- Prioridad: %d ---\n", actual->prioridad);
        printf("--- Tiempo de llegada: %d ---\n", actual->t_llegada);
        printf("\n-----------------------------------------------\n\n");
        actual = actual->nextNode;
    }
    //Se imprime el ultimo proceso
    printf("--- ID: %d ---\n", actual->pid);
    printf("--- Burst: %d ---\n", actual->burst);
    printf("--- Prioridad: %d ---\n", actual->prioridad);
    printf("--- Tiempo de llegada: %d ---\n\n", actual->t_llegada);
}

int main(int argc, char *argv[])
{
    //printf("Hello world!\n");
    pthread_t simulatorControllerThread;
    pthread_mutex_init (&mutexReady , NULL);
    pthread_create(&simulatorControllerThread, NULL, startSimulator, NULL);
    pthread_join(simulatorControllerThread,NULL);
    //Show stats
    return 0;
}


void *startSimulator ()
{

    printf("Bienvenido al simulador de algoritmos de procesos!\n");
    printf("\n");
    printf("Seleccione el tipo de algoritmo con el que desea correr la simulacion:\n");
    printf("\n");
    printf("1.) FIFO\n");
    printf("2.) SJF\n");
    printf("3.) HPF\n");
    printf("4.) Round Robin\n");
    //Scan the entered option from stdin
    int  option;
    scanf("%d", &option);

    //Check if the entered option is valid
    if( (option >= 1) && (option <= 4)){
        //Clear the terminal
        printf("\033[2J\033[1;1H");
        //Create the threads declaration
        pthread_t jobSheduler, cpuScheduler, clockThread;
        pthread_create(&jobSheduler, NULL, jobSchedulerTask, NULL);
        pthread_detach(jobSheduler);
        pthread_create(&clockThread, NULL, threadTiempo, NULL);
        pthread_detach(clockThread);

        if(option == 1){
            printf("El simulador correra con el algoritmo FIFO\n");
            //Hilo cpuScheduler con metodo fifo
            pthread_create(&cpuScheduler, NULL, cpuFIFOSchedulerTask, NULL);
        }
        else if(option == 2){
            printf("El simulador correra con el algoritmo SJF\n");
            //Hilo cpuScheduler con metodo sjf
            //cpuSchedulerRef = pthread_create(&cpuScheduler, NULL, cpuSchedulerTask, params);
        }
        else if(option == 3){
            printf("El simulador correra con el algoritmo HPF\n");
            //Hilo cpuScheduler con metodo hpf
            //cpuSchedulerRef = pthread_create(&cpuScheduler, NULL, cpuSchedulerTask, params);
        }
        else if(option == 4){
            printf("El simulador correra con el algoritmo Round Robin\n");
            //Hilo cpuScheduler con metodo rr
            //cpuSchedulerRef = pthread_create(&cpuScheduler, NULL, cpuSchedulerTask, params);
        }

        //Loop for checking simulator variables
        while(1){

            flush();
            printf("\n");
            printf("Seleccione el metodo de funcionamiento del cliente:\n");
            printf("\n");
            printf("1.) Imprimir procesos ejecutados\n");
            printf("2.) Imprimir largo de la lista de procesos\n");
            printf("3.) Imprimir procesos en la cola\n");
            printf("9.) SALIR\n");
            //Read user input from stdin
            int  simulatorOption;
            scanf("%d", &simulatorOption);

            if(simulatorOption == 1){
                printf("El valor de las varas ejecutadas es %s\n");
            }
            else if(simulatorOption == 2){
                printReadyQueueLenght();
            }
            else if(simulatorOption == 3){
                //printf("El valor de las  otras varas es %s\n");
                //printList(processList->firstNode);
                printReadyQueue();
            }
            else if(simulatorOption == 9){
                //QUIT
                break;
            }
            else{
                printf("La opcion no es valida. Intente de nuevo\n");
            }

            printf("\n\n");

            //Clear the terminal
            //printf("\033[2J\033[1;1H");
        }//While true
    }
    return 0;
}


void *jobSchedulerTask (){

    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    int n;
    char buffer[256];
    int bytes_recieved = 1;
    char sendBuff[1025], recv_data[1024];
    time_t ticks;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(listenfd, 10);
    int i = 0;
    char *p;
    char *parameters[3];
    char pid, burst, priority;
    while(1)
    {
        //Here we accept the new connection
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        //Prepare to receive the buffer
        bzero(buffer,256);
        n = read(connfd,buffer,255);
        if (n < 0)
            printf("ERROR reading from socket");

        printf("\n\n +++ Proceso con ID %d ", (int) buffer[0]);
        printf("con BURST %d ", (int) buffer[1]);
        printf("y PRIORIDAD %d ", (int) buffer[2]);
        printf("ha llegado al server. +++ \n\n");
        //printf("RECEIVED -> %s\n", parameters[0]);
        //Process * process = createProcess(parameters[0]);
        //printf("status -> %s\n", process->nextNode);
        //insertProcess(pList->firstNode,parameters[0]);
        pthread_mutex_lock(&mutexReady);
        addElementToReadyQueue((int) buffer[0], (int) buffer[1], (int) buffer[2], reloj);
        pthread_mutex_unlock(&mutexReady);

        close(connfd);
     }
}

void *cpuFIFOSchedulerTask (void* arg){
    //printf("\033[2J");
    while(1){
        if(readyQueueLenght==0){
            cpuOcioso ++;
        }
        else{
            
        }
        sleep(1);
    }
}

void *cpuSJFSchedulerTask (void* arg){
    //printf("\033[2J");
}

void *cpuHPFSchedulerTask (void* arg){
    //printf("\033[2J");
}

void *cpuRRSchedulerTask (void* arg){
    //printf("\033[2J");
}



int* threadTiempo() {
    while (1) {
        sleep(1);
        reloj ++;
    }
}



void flush()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}
