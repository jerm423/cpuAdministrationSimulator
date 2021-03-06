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
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

/*

    It is not possible, at least on a single core computer. But why do you want that? 
    Even if you were able to start two threads at exactly the same second, they will progress differently 
    because scheduling is not in your control.

*/
void *addCpuOcioso();
int  *threadTiempo();
void  flush ();
void *startSimulator ();
void *jobSchedulerTask ();
void *cpuSchedulerTask (void* arg);
void *cpuSJFSchedulerTask (void* arg);
void *cpuHPFSchedulerTask (void* arg);
void *cpuRRSchedulerTask (void* arg);

pthread_mutex_t mutexReady;
struct Process *primero,*ultimo;
struct Tat_Wt_Table *primeroTable,*ultimoTable;
int reloj = 0;
int cpuOcioso = 0;
int executedProcessess = 0;
int readyQueueLenght = 0;
int tableLenght = 0;
int algorithm = 0;

void printReadyQueueLenght(){
    printf(COLOR_YELLOW"Largo de la lista de ready: %d\n"COLOR_RESET, readyQueueLenght);
}

void printTableLenght(){
    printf(COLOR_YELLOW"Largo de la lista de TAT y WT es : %d\n"COLOR_RESET, tableLenght);
}

int addElementToReadyQueue(int p_Pid, int p_Burst, int p_Prioridad, int p_Tiempo_Llegada) {
    struct Process *nuevo;
    /* reservamos memoria para el nuevo elemento */
    nuevo = (struct Process *) malloc (sizeof(struct Process));
    if (nuevo==NULL){
        printf( COLOR_RED"\nNo hay memoria disponible!\n"COLOR_RESET);
        return 0;
    }

    //Se agreagan los datos al nuevo struct
    nuevo->pid = p_Pid;
    nuevo->burst = p_Burst;
    nuevo->burstRestante = p_Burst;
    nuevo->burstTotal = p_Burst;
    nuevo->prioridad = p_Prioridad;
    nuevo->posicion = 0;
    nuevo->t_llegada = p_Tiempo_Llegada;
    nuevo->enabled = 1;

    /* el campo siguiente va a ser NULL por ser el último elemento
     de la lista */
    nuevo->nextNode = NULL;
    nuevo->previousNode = ultimo;

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
        nuevo->posicion = 0;
        primero->t_llegada = p_Tiempo_Llegada;
        nuevo->enabled = 1;
        //primero->nextNode = NULL;
        ultimo = primero;
     }
    else {
       /* el que hasta ahora era el último tiene que apuntar al nuevo */
       ultimo->nextNode = nuevo;
       nuevo->previousNode = ultimo;
       /* hacemos que el nuevo sea ahora el último */
       ultimo = nuevo;
       ultimo->nextNode = NULL;
    }

    readyQueueLenght ++;
    //free(nuevo);
}


int addElementToTatTable(int p_Pid, int p_TurnAroundTime, int p_WaitingTime, int p_Tiempo_Llegada, int p_Tiempo_Salida) {
    struct Tat_Wt_Table *nuevo;
    /* reservamos memoria para el nuevo elemento */
    nuevo = (struct Tat_Wt_Table *) malloc (sizeof(struct Tat_Wt_Table));
    if (nuevo==NULL){
        printf(COLOR_RED"\nNo hay memoria disponible!\n"COLOR_RESET);
        return 0;
    }

    //Se agreagan los datos al nuevo struct
    nuevo->pid = p_Pid;
    nuevo->turnAroundTime = p_TurnAroundTime;
    nuevo->waitingTime = p_WaitingTime;
    nuevo->t_llegada = p_Tiempo_Llegada;
    nuevo->t_salida = p_Tiempo_Salida;
   

    /* el campo siguiente va a ser NULL por ser el último elemento
     de la lista */
    nuevo->nextNode = NULL;

    /* ahora metemos el nuevo elemento en la lista. lo situamos
     al final de la lista */
    /* comprobamos si la lista está vacía. si primeroTable==NULL es que no
     hay ningún elemento en la lista. también vale ultimoTable==NULL */
    if (primeroTable==NULL) {
        primeroTable = nuevo;
        primeroTable->pid = p_Pid;
        primeroTable->waitingTime = p_WaitingTime;
        primeroTable->turnAroundTime = p_TurnAroundTime;
        nuevo->t_llegada = p_Tiempo_Llegada;
        nuevo->t_salida = p_Tiempo_Salida;

        ultimoTable = primeroTable;
     }
    else {
       /* el que hasta ahora era el último tiene que apuntar al nuevo */
       ultimoTable->nextNode = nuevo;
       /* hacemos que el nuevo sea ahora el último */
       ultimoTable = nuevo;
       ultimoTable->nextNode = NULL;
    }

    tableLenght ++;
    //free(nuevo);
}



void printReadyQueue() {

    //En caso de cola vacía
    if (primero == NULL) {
        printf(COLOR_BLUE"\n\n--- Cola vacia! ---\n\n"COLOR_RESET);
        return 0;
    }

    printf(COLOR_CYAN"\n\n--------- Procesos en la cola de ready ---------\n\n"COLOR_RESET);

    struct Process *actual = primero;

    while (actual) {

        if(actual->enabled == 1)
        {

            printf(COLOR_GREEN"--- ID: %d ---\n"COLOR_RESET, actual->pid);
            printf(COLOR_GREEN"--- Burst: %d ---\n"COLOR_RESET, actual->burst);
            printf(COLOR_GREEN"--- Prioridad: %d ---\n"COLOR_RESET, actual->prioridad);
            printf(COLOR_GREEN"--- Tiempo de llegada: %d ---\n"COLOR_RESET, actual->t_llegada);
            printf(COLOR_GREEN"--- Enabled: %d ---\n"COLOR_RESET, actual->enabled);
            printf(COLOR_CYAN"\n-----------------------------------------------\n\n"COLOR_RESET);

        }

        
        actual = actual->nextNode;
    }

}

void printTat_Wt_Table() {

    //En caso de cola vacía
    if (primeroTable == NULL) {
        printf(COLOR_BLUE"\n\n--- Cola de TAT y WT vacia! ---\n\n"COLOR_RESET);
        return 0;
    }

    printf(COLOR_CYAN"\n\n--------- Procesos en la tabla de TAT y WT ---------\n\n"COLOR_RESET);

    struct Tat_Wt_Table *actual = primeroTable;

    while (actual) {

        printf(COLOR_GREEN"--- ID: %d ---\n"COLOR_RESET, actual->pid);
        printf(COLOR_GREEN"--- turnAroundTime: %d ---\n"COLOR_RESET, actual->turnAroundTime);
        printf(COLOR_GREEN"--- waitingTime: %d ---\n"COLOR_RESET, actual->waitingTime);
        printf(COLOR_GREEN"--- tiempoLlegada: %d ---\n"COLOR_RESET, actual->t_llegada);
        printf(COLOR_GREEN"--- TiempoSalida: %d ---\n"COLOR_RESET, actual->t_salida);
        printf(COLOR_CYAN"\n-----------------------------------------------\n\n"COLOR_RESET);

        actual = actual->nextNode;
    }

}

void printExecutedProcessess(){
    printf(COLOR_CYAN"\nLa cantidad de procesos ejecutados es %d\n"COLOR_RESET,executedProcessess);
}

void printIdleCPU(){
    printf(COLOR_CYAN"\nLa cantidad de segundos de CPU ocioso es %d\n"COLOR_RESET,cpuOcioso);
}

void printWaitingTimeAverage(){
    
    
    //En caso de cola vacía
    if (primeroTable == NULL) {
        printf(COLOR_RED"\nEl promedio de waiting time es 0\n"COLOR_RESET);
    }

    struct Tat_Wt_Table *actual = primeroTable;
    int waitingTimeTotal = 0;
    float waitingTimeAverage = 0.0;
    int loopCounter = 0;
    while (actual) {
        waitingTimeTotal = waitingTimeTotal + actual->waitingTime;
        actual = actual->nextNode;
        loopCounter ++;
    }

    waitingTimeAverage = (float)((float)waitingTimeTotal/(float)loopCounter);

    printf(COLOR_RED"\nEl promedio de waiting time es %f\n"COLOR_RESET, waitingTimeAverage);

}

void printTatTimeAverage(){
    
    
    //En caso de cola vacía
    if (primeroTable == NULL) {
        printf(COLOR_RED"\nEl promedio de waiting time es 0\n"COLOR_RESET);
    }

    struct Tat_Wt_Table *actual = primeroTable;
    int tatTimeTime = 0;
    float tatTimeAverage = 0.0;
    int loopCounter = 0;
    while (actual) {
        tatTimeTime = tatTimeTime + actual->turnAroundTime;
        actual = actual->nextNode;
        loopCounter ++;
    }

    tatTimeAverage = (float)((float)tatTimeTime/(float)loopCounter);

    printf(COLOR_RED"\nEl promedio de turn around time es %f\n"COLOR_RESET, tatTimeAverage);

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

    printf(COLOR_GREEN"Bienvenido al simulador de algoritmos de procesos!\n"COLOR_RESET);
    printf("\n");
    printf("Seleccione el tipo de algoritmo con el que desea correr la simulacion:\n");
    printf("\n");
    printf(COLOR_GREEN"1.)"COLOR_RESET" FIFO\n");
    printf(COLOR_GREEN"2.)"COLOR_RESET" SJF\n");
    printf(COLOR_GREEN"3.)"COLOR_RESET" HPF\n");
    printf(COLOR_GREEN"4.)"COLOR_RESET" Round Robin\n");
    //Scan the entered option from stdin
    int  option;
    scanf("%d", &option);

    //Check if the entered option is valid
    if( (option >= 1) && (option <= 4)){
        //Clear the terminal
        printf("\033[2J\033[1;1H");
        //Create the threads declaration
        pthread_t jobSheduler, cpuScheduler, clockThread;

        //Clock thread
        pthread_create(&clockThread, NULL, threadTiempo, NULL);
        pthread_detach(clockThread);

        //Job scheduler
        pthread_create(&jobSheduler, NULL, jobSchedulerTask, NULL);
        pthread_detach(jobSheduler);
        
        

        if(option == 1){
            printf(COLOR_RED"\nEl simulador correra con el algoritmo FIFO\n"COLOR_RESET);
            algorithm = 1;
            pthread_create(&cpuScheduler, NULL, cpuSchedulerTask, NULL);
            pthread_detach(cpuScheduler);
            //Hilo cpuScheduler con metodo fifo
            
        }
        else if(option == 2){
            algorithm = 2;
            printf(COLOR_RED"\nEl simulador correra con el algoritmo SJF\n"COLOR_RESET);
            pthread_create(&cpuScheduler, NULL, cpuSchedulerTask, NULL);
            pthread_detach(cpuScheduler);
            //Hilo cpuScheduler con metodo sjf
            //cpuSchedulerRef = pthread_create(&cpuScheduler, NULL, cpuSchedulerTask, params);
        }
        else if(option == 3){
            algorithm = 3;
            printf(COLOR_RED"\nEl simulador correra con el algoritmo HPF\n"COLOR_RESET);
            pthread_create(&cpuScheduler, NULL, cpuSchedulerTask, NULL);
            pthread_detach(cpuScheduler);
            //Hilo cpuScheduler con metodo hpf
            //cpuSchedulerRef = pthread_create(&cpuScheduler, NULL, cpuSchedulerTask, params);
        }
        else if(option == 4){
            algorithm = 4;
            printf(COLOR_RED"\nEl simulador correra con el algoritmo Round Robin\n"COLOR_RESET);
            pthread_create(&cpuScheduler, NULL, cpuRRSchedulerTask, NULL);
            pthread_detach(cpuScheduler);
            //Hilo cpuScheduler con metodo rr
            //cpuSchedulerRef = pthread_create(&cpuScheduler, NULL, cpuSchedulerTask, params);
        }


        

        //Loop for checking simulator variables
        while(1){

            flush();
            printf("\n");
            printf(COLOR_GREEN"Seleccione el metodo de funcionamiento del cliente:\n"COLOR_RESET);
            printf("\n");
            printf(COLOR_GREEN"1.)"COLOR_RESET" Imprimir procesos ejecutados\n");
            printf(COLOR_GREEN"2.)"COLOR_RESET" Imprimir largo de la lista de procesos\n");
            printf(COLOR_GREEN"3.)"COLOR_RESET" Imprimir procesos en la cola\n");
            printf(COLOR_GREEN"9.)"COLOR_RESET" Mostrar resultados de la simulacion y salir\n");
            //Read user input from stdin
            int  simulatorOption;
            scanf("%d", &simulatorOption);

            if(simulatorOption == 1){
                printExecutedProcessess();
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
                printExecutedProcessess();
                printIdleCPU();
                printTat_Wt_Table();
                printWaitingTimeAverage();
                printTatTimeAverage();
                break;
            }
            else{
                printf(COLOR_RED"La opcion no es valida. Intente de nuevo\n"COLOR_RESET);
            }

            printf("\n\n");

            //Clear the terminal
            //printf("\033[2J\033[1;1H");
        }//While true

        


    }
    return 0;
}

void eliminar_elemento(int posicion) {    
    if (primero == NULL) {        
        return;
    }
    if (primero->nextNode == NULL) {        
        primero = NULL;
        ultimo = NULL;
        return;
    }
    struct Process *actual = primero;
    struct Process *aux = primero;    
    for (int i=1; i < posicion; i++) {   //Se hace menor estricto para eliminar el elemento que sigue de la condicion
        actual = actual->nextNode;      //de parada del ciclo  
    }
    if (actual == primero && posicion == 0) {        
        primero = actual->nextNode;
        primero->previousNode = NULL;
        actual = NULL;
    }
    else {
        aux = actual->nextNode;
        actual->nextNode = actual->nextNode->nextNode;
        actual->nextNode->previousNode = actual;
        if (actual->nextNode == NULL) {
            ultimo = actual;
        }
        aux = NULL;
    }
    //free(aux);
    readyQueueLenght--;
}

void deleteProcessById(int pId) {   

    //If the list is empty 
    if (primero == NULL) {        
        return;
    }

    //If there is only one element in the list 
    if ( (primero->nextNode == NULL) && (primero->pid == pId ) ) {       
        primero = NULL;
        ultimo = NULL;
        return;
    }


    //If the element to be deleted is the first one and there are more than one processess in the list
    if ( (primero->nextNode != NULL) && (primero->pid == pId ) ) {        
        primero = primero->nextNode;
        return;
    }


    //If there are several elements and the one to be deleted is not the first one
    struct Process *actual = primero;
    struct Process *aux = primero;    

    while(actual->nextNode->pid != pId){

        actual = actual->nextNode;
    }
        
    aux = actual->nextNode;
    actual->nextNode = actual->nextNode->nextNode;

    aux = NULL;
    //free(aux);
    readyQueueLenght--;
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
            printf(COLOR_RED"ERROR reading from socket"COLOR_RESET);
        printf("\n +++ Proceso con ID %d con BURST %d y PRIORIDAD %d ha llegado al server. +++\n", (int) buffer[0], (int) buffer[1], (int) buffer[2] );

        //Insert incoming process in ready queue
        pthread_mutex_lock(&mutexReady);
        addElementToReadyQueue((int) buffer[0], (int) buffer[1], (int) buffer[2], reloj);
        pthread_mutex_unlock(&mutexReady);

        close(connfd);
     }
}

Process *findFIFOCandidate(){

    int contadorLista, posGanador;
    struct Process *candidato;
    struct Process *ganador;

    //En caso de que no haya procesos
    
    if (primero == NULL) {
        return NULL;
    }

    //Recorremos cola y seleccionamos procesos por PID más pequeño
    candidato = primero->nextNode;    
    ganador = primero;
    contadorLista = 0;
    posGanador = 0;
    while (candidato != NULL) {
        contadorLista++;
        if (candidato->pid < ganador->pid) {
            ganador = candidato;
            posGanador = contadorLista;
        }
        candidato = candidato->nextNode;        
    }

    ganador->posicion = posGanador;
    
    return ganador;

    
    
}

Process *findSJFCandidate(){

    int contadorLista, posGanador;
    struct Process *candidato;
    struct Process *ganador;

    //En caso de que no haya procesos
    
    if (primero == NULL) {
        return NULL;
    }

    //Recorremos cola y seleccionamos procesos por PID más pequeño
    candidato = primero->nextNode;    
    ganador = primero;
    contadorLista = 0;
    posGanador = 0;
    while (candidato != NULL) {
        contadorLista++;
        if (candidato->burst < ganador->burst) {
            ganador = candidato;
            posGanador = contadorLista;
        }
        candidato = candidato->nextNode;        
    }

    ganador->posicion = posGanador;
    
    return ganador;

    
    
}

Process *findHPFCandidate(){

    int contadorLista, posGanador;
    struct Process *candidato;
    struct Process *ganador;

    //En caso de que no haya procesos
    
    if (primero == NULL) {
        return NULL;
    }

    //Recorremos cola y seleccionamos procesos por PID más pequeño
    candidato = primero->nextNode;    
    ganador = primero;
    contadorLista = 0;
    posGanador = 0;
    while (candidato != NULL) {
        contadorLista++;
        if (candidato->prioridad < ganador->prioridad) {
            ganador = candidato;
            posGanador = contadorLista;
        }
        candidato = candidato->nextNode;        
    }

    ganador->posicion = posGanador;
    
    return ganador;

    
    
}



Process *executeProcess(Process *process){
    
    
    //poner burst restante en cero
    process->burstRestante = 0;
    
    

    eliminar_elemento(process->posicion);

    //aumentamos en uno la cantidad de procesos ejecutados
    executedProcessess++;

    sleep(process->burst);
    int tiempoFinalizacion = reloj;

    //aqui hay que meterlo en las tablas de WT y TAT
    printf(COLOR_GREEN"Proceso ID: %d llego en el segundo %d entra en ejecucion\n"COLOR_RESET,process->pid ,process->t_llegada);
    
    
    int turnAroundTime = tiempoFinalizacion - process->t_llegada;
    int waitingTime = turnAroundTime - process->burst;
    addElementToTatTable(process->pid, turnAroundTime, waitingTime, process->t_llegada, tiempoFinalizacion);
}


void *cpuSchedulerTask (void* arg){
    //printf("\033[2J");
    while(1){

        

        struct Process *candidate;

        
        if(algorithm == 1){
            pthread_mutex_lock(&mutexReady);
            candidate = findFIFOCandidate();
            pthread_mutex_unlock(&mutexReady);   
        }
        else if(algorithm == 2){
            pthread_mutex_lock(&mutexReady);
            candidate = findSJFCandidate();
            pthread_mutex_unlock(&mutexReady);   
        }
        else if(algorithm == 3){
            pthread_mutex_lock(&mutexReady);
            candidate = findSJFCandidate();
            pthread_mutex_unlock(&mutexReady);   
        }

        
        

        if(candidate == NULL){
            cpuOcioso ++;
            printf(COLOR_RED"CPU OCIOSO!! \n"COLOR_RESET);
        }
        else{
            executeProcess(candidate);
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
  
    int contadorLista, posGanador;
    int quantum = 3;
    //En caso de que no haya procesos

    while(1){
        if (primero == NULL) {
            cpuOcioso ++;
            printf(COLOR_RED"CPU OCIOSO!! \n"COLOR_RESET);

        }
        else{
            
            struct Process *actual = primero;

            if(primero->nextNode == NULL){
                

                if(primero->burstRestante <= quantum){

                    if(primero->burstRestante == quantum){
                        sleep(quantum);
                    }
                    else{
                        sleep(quantum - primero->burstRestante);
                    }
                    int tiempoFinalizacion = reloj;
                    //aqui hay que meterlo en las tablas de WT y TAT
                    printf(COLOR_GREEN"Proceso RR ID: %d llego en el segundo : %d \n"COLOR_RESET,primero->pid ,primero->t_llegada);
                    
                    printReadyQueue();

                    int turnAroundTime = tiempoFinalizacion - primero->t_llegada;
                    int waitingTime = turnAroundTime - primero->burst;
                    addElementToTatTable(primero->pid, turnAroundTime, waitingTime, primero->t_llegada, tiempoFinalizacion);
                    //printf("pasa antes del print\n");
                    //Eliminar por pid
                    deleteProcessById(primero->pid);
                }
                else{
                    sleep(quantum);
                    primero->burstRestante = primero->burstRestante - quantum;
                }
            }


            while(actual->nextNode != NULL){
                
              

                //Recorremos cola y seleccionamos procesos por PID más pequeño
                if(actual->burstRestante <= quantum){
                    if(actual->burstRestante == quantum){
                        sleep(quantum);
                    }
                    else{
                        sleep(quantum - actual->burstRestante);
                    }
                    int tiempoFinalizacion = reloj;
                    //aqui hay que meterlo en las tablas de WT y TAT
                    printf(COLOR_GREEN"Proceso RR ID: %d llego en el segundo : %d \n"COLOR_RESET,actual->pid ,actual->t_llegada);
                    
                    printReadyQueue();

                    int turnAroundTime = tiempoFinalizacion - actual->t_llegada;
                    int waitingTime = turnAroundTime - actual->burst;
                    addElementToTatTable(actual->pid, turnAroundTime, waitingTime, actual->t_llegada, tiempoFinalizacion);
                    //printf("pasa antes del print\n");
                    //Eliminar por pid
                    deleteProcessById(actual->pid);
                }
                else{
                    sleep(quantum);
                    actual->burstRestante = actual->burstRestante - quantum;
                }

                actual=actual->nextNode;
            }
            

        }

        sleep(1);
    }
    
    

    
}



int* threadTiempo() {
    while (1) {
        sleep(1);
        //printf("reloj: %d\n", reloj);
        reloj ++;
    }
}




void flush()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}
