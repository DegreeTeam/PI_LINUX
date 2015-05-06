#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
#define MSGSIZE 1024
#define SIZE sizeof(struct sockaddr_in)
//the thread function
void *connection_handler(void *);
struct _Param{
	int *new_sock;
	struct sockaddr_in sockaddr;
}; 
int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , *n_sock;
    int client_len = SIZE;
    struct _Param *param;
    struct sockaddr_in server , client;
    char msg[MSGSIZE];
    //Create socket
    socket_desc = socket(AF_INET , SOCK_DGRAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(2007);
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
 
    while( 0 < (recvfrom(socket_desc, (void *)&msg, MSGSIZE, 0, (struct sockaddr *)&client,&client_len )))
 // while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
        param = malloc(sizeof(struct _Param)); 
        pthread_t sniffer_thread;
        n_sock = malloc(sizeof(int));
	char buffer[6];
	buffer[0] = msg[0];
	buffer[1] = msg[1];
	buffer[2] = msg[2];
	buffer[3] = msg[3];
	buffer[4] = msg[4];
	buffer[5] = '\0';

        *n_sock =atoi(buffer);
        param->new_sock =n_sock;
	param->sockaddr =client; 
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) param) < 0)
        {
            perror("could not create thread");
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }
     
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
     
    return 0;
}
 
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *pa)
{
    //Get the socket descriptor
    struct _Param param =*(struct _Param*)pa;    

    int sock =*(param.new_sock);
    struct sockaddr_in client = param.sockaddr;
    int read_size;
    char message[1024]; 
    int test;
 
    //Send some messages to the client
    message[0] = 'a';
    test = sendto(sock ,&message , strlen(message),0,(struct sockaddr *)&client, sizeof(client));
    puts(message); 
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
    //Free the socket pointer
    free(pa);    
    return 0;
}
