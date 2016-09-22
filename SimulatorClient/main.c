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

void sendProcessToServer ();

int getRandomNumber();

int main(int argc, char *argv[])
{

    //getRandomNumber();

    //sendProcessToServer();



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

        //printf("PID : %s\n", parameters[0]);
        //printf("Burst : %s\n", parameters[1]);
        //printf("Priority : %s\n", parameters[2]);

        char * comma = ",";
        char * processInfo = (char *) malloc(1 + strlen(parameters[0])+ strlen(parameters[1]) + strlen(parameters[2]) );
        strcpy(processInfo, parameters[0]);
        strcat(processInfo, comma);
        strcat(processInfo, parameters[1]);
        strcat(processInfo, comma);
        strcat(processInfo, parameters[2]);



        printf("Enviando al servidor un proceso con parametros: %s\n", processInfo);

        sleep(1);
        //int sleepNumber = getRandomNumber();
        pthread_t sendProcessThread;
        int sendProcessRef;

        sendProcessRef = pthread_create(&sendProcessThread, NULL, sendProcessToServer, processInfo);
        //pthread_join(sendProcessThread,NULL);

        //sendProcessToServer(processInfo );

    }

    fclose(file);


    return 0;
}

int getRandomNumber(){
    int r, i;
    int N = 1,
    M = 20;
    /* initialize random seed: */
    srand (time(NULL));

    /* generate secret number between 1 and 10: */
    r = M + rand() / (RAND_MAX / (N - M + 1) + 1);
    //printf("\n%d", r);
    //printf("\n") ;

    return r;
}

void sendProcessToServer (char * processInfo){

    int sleepNumber = getRandomNumber();

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
