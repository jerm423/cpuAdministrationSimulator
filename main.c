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
#include "params.h"




void flush ();

void *startSimulator (void* arg);

void *jobSchedulerTask (void* arg);

void *cpuSchedulerTask (void* arg);



int main(int argc, char *argv[])
{
    //printf("Hello world!\n");

    pthread_t simulatorControllerThread, thread2;


    params_t params;

    params.id = 0;
    params.processList = createProcessList();
    pthread_mutex_init (&params.mutex , NULL);

    //char *msg1 = "First message";
    //char *msg2 = "Second message";

    int thread1ref, thread2ref;

    thread1ref = pthread_create(&simulatorControllerThread, NULL, startSimulator, &params);
    pthread_join(simulatorControllerThread,NULL);

    //thread2ref = pthread_create(&thread2, NULL, test, &params);
    //pthread_join(thread2,NULL);

    //ret2 = pthread_create(&thread2, NULL, myfunc, (void *) msg2);

    //printf("Main function after pthread_create\n");





    printf("Two threads executed\n");

    return 0;

}


void *cpuSchedulerTask (void* arg){
    //printf("\033[2J");
    




}

void *jobSchedulerTask (void* arg){
    
    params_t *p = (params_t*)(arg);
    
    /*
    incrementId(p);
    int id = getId(p);



    printf("PARAMS 2 ARE %d\n", id);

    

    createProcess(8, 5, 1, p);

    createProcess(7, 2, 6, p);

    */


    
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;

    char sendBuff[1025];
    time_t ticks;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);

    while(1)
    {
        //printf("listening\n");
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

        ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
        write(connfd, sendBuff, strlen(sendBuff));

        close(connfd);
        sleep(1);
     }
    

}



void *startSimulator (void* arg)
{
    params_t *params = (params_t*)(arg);
    incrementId(params);
    int id = getId(params);



    printf("PARAMS ARE %d\n", id);

	printf("Bienvenido al simulador de algoritmos de procesos!\n");
	printf("\n");
	printf("Seleccione el tipo de algoritmo con el que desea correr la simulacion:\n");
    printf("\n");
    printf("1.) FIFO\n");
    printf("2.) SJF\n");
    printf("3.) HPF\n");
    printf("4.) Round Robin\n");

    int  option;

    scanf("%d", &option);

    if(option == 1){
        printf("El simulador correra con el algoritmo FIFO\n");
    }
    else if(option == 2){
        printf("El simulador correra con el algoritmo SJF\n");
    }
    else if(option == 3){
        printf("El simulador correra con el algoritmo HPF\n");
    }
    else if(option == 4){
        printf("El simulador correra con el algoritmo Round Robin\n");
    }
    else{
        printf("La opcion seleccionada no es valida. Abortando!\n");
    }


    pthread_t jobSheduler, cpuScheduler;
    int jobShedulerRef, cpuSchedulerRef;

    printf("\n");
    printf("\n");

    printf("//////////////INICIANDO HILOS HIJOS//////////////\n");
    jobShedulerRef = pthread_create(&jobSheduler, NULL, jobSchedulerTask, params);
    pthread_join(jobSheduler,NULL);


    cpuSchedulerRef = pthread_create(&cpuScheduler, NULL, cpuSchedulerTask, params);




    //pthread_join(cpuScheduler,NULL);

    /*
    flush();
    while (1)
    {
        printf("\033[2J");
        char user_option [1024];

        printf("Opciones Administrativas!\n");
        printf("\n");
        printf("\n");
        printf("1.) Imprimir cantidad de procesos ejecutados\n");
        printf("2.) \n");
        printf("3.) \n");
        printf("4.) \n");
        printf("\n Escribir (q or Q) para salir : ");

      gets(user_option);
      printf(user_option);

      if (strcmp(user_option , "q") == 0 || strcmp(user_option , "Q") == 0)
      {
        break;
      }


    }
    */

	//return NULL;
}


void flush()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}