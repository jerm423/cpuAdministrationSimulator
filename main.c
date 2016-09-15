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

void *startSimulator (void* arg);

void *jobSchedulerTask (void* arg);

void *cpuSchedulerTask (void* arg);



int main(int argc, char *argv[])
{
    //printf("Hello world!\n");

    pthread_t simulatorControllerThread, thread2;


    params_t params;

    params.id = 0;
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


    printf("Opciones Administrativas!\n");
    printf("\n");
    printf("Seleccione el tipo de algoritmo con el que desea correr la simulacion:\n");
    printf("\n");
    printf("1.) FIFO\n");
    printf("2.) SJF\n");
    printf("3.) HPF\n");
    printf("4.) Round Robin\n");

}

void *jobSchedulerTask (void* arg){
    /*
    params_t *p = (params_t*)(arg);
    incrementId(p);
    int id = getId(p);



    printf("PARAMS 2 ARE %d\n", id);

    */

    int sock, connected, bytes_recieved;  
    int trues = 1;

    char send_data [1024] , recv_data[1024];       

        struct sockaddr_in server_addr,client_addr;    
        int sin_size;
        
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("Socket");
            exit(1);
        }

        if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&trues,sizeof(int)) == -1) {
            perror("Setsockopt");
            exit(1);
        }
        
        server_addr.sin_family = AF_INET;         
        server_addr.sin_port = htons(5000);     
        server_addr.sin_addr.s_addr = INADDR_ANY; 
        bzero(&(server_addr.sin_zero),8); 

        if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))
                                                                       == -1) {
            perror("Unable to bind");
            exit(1);
        }

        if (listen(sock, 5) == -1) {
            perror("Listen");
            exit(1);
        }
        
    printf("\nTCPServer Waiting for client on port 5000");
        fflush(stdout);


        while(1)
        {  

            sin_size = sizeof(struct sockaddr_in);

            connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);

            printf("\n I got a connection from (%s , %d)",
                   inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

            while (1)
            {
              printf("\n SEND (q or Q to quit) : ");
              gets(send_data);
              
              if (strcmp(send_data , "q") == 0 || strcmp(send_data , "Q") == 0)
              {
                send(connected, send_data,strlen(send_data), 0); 
                close(connected);
                break;
              }
               
              else
                 send(connected, send_data,strlen(send_data), 0);  

              bytes_recieved = recv(connected,recv_data,1024,0);

              recv_data[bytes_recieved] = '\0';

              if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0)
              {
                close(connected);
                break;
              }

              else 
              printf("\n RECIEVED DATA = %s " , recv_data);
              fflush(stdout);
            }
        }       

      close(sock);
      return 0;
        


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
    //pthread_join(jobSheduler,NULL);


    cpuSchedulerRef = pthread_create(&cpuScheduler, NULL, jobSchedulerTask, params);
    pthread_join(jobSheduler,NULL);



	//return NULL;
}


