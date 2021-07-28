//Example code: A simple server side code, which echos back the received message.
//Handle multiple socket connections with select and fd_set on Linux 
#include <stdio.h> 
#include <string.h>   //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include <pthread.h>
#include <unistd.h>   //close 
#include <arpa/inet.h>    //close 
#include <sys/types.h> 
#include <curl/curl.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 

struct MemoryStruct {
  char *memory;
  size_t size;
};

extern struct MemoryStruct send_request(void);

#define TRUE   1 
#define FALSE  0 
#define PORT 8888 
#define THREAD_NUM 4
#define max_c 30
typedef struct Task {
    int Tsd, Tok;
} Task;
Task taskQueue[256];
int taskCount = 0;
char respuesta[20]="";



pthread_mutex_t mutexQueue;
pthread_cond_t condQueue;

void executeTask(Task* task) {
	struct MemoryStruct chunk;
	if(task->Tok ==1){
		printf("OK %d\n\r",task->Tsd);
		sprintf(respuesta,"OK, Cli %d\n\r",task->Tsd);
		send(task->Tsd , respuesta , strlen(respuesta) , 0 );
		chunk=send_request();
		send(task->Tsd , chunk.memory, strlen(chunk.memory) , 0);
	}else{
		printf("NOK %d\n\r",task->Tsd);
		sprintf(respuesta,"FALSE, Cli %d\n\r",task->Tsd);
		send(task->Tsd, respuesta , strlen(respuesta) , 0 ); 
	}
}

void submitTask(Task task) {
    pthread_mutex_lock(&mutexQueue);
    taskQueue[taskCount] = task;
    taskCount++;
    pthread_mutex_unlock(&mutexQueue);
    pthread_cond_signal(&condQueue);
}

// 1 2 3 4 5
// 2 3 4 5

void* startThread(void* args) {
    while (1) {
        Task task;
        pthread_mutex_lock(&mutexQueue);
        while (taskCount == 0) {
            pthread_cond_wait(&condQueue, &mutexQueue);
        }
        task = taskQueue[0];
        int i;
        for (i = 0; i < taskCount - 1; i++) {
            taskQueue[i] = taskQueue[i + 1];
        }
        taskCount--;
        pthread_mutex_unlock(&mutexQueue);
        executeTask(&task);
    }
}
     
int main(int argc , char *argv[])  
{  
    int opt = TRUE;  
    int master_socket, addrlen, new_socket, client_socket[max_c], 
          max_clients = max_c, activity, i, valread, sd;  
    int max_sd;  
    struct sockaddr_in address;  
    char buffer[1025];  //data buffer of 1K 
    //set of socket descriptors 
    fd_set readfds;  
    //a message 
    char *message = "ECHO Daemon v1.0 \r\n";  
	pthread_t th[THREAD_NUM];
    pthread_mutex_init(&mutexQueue, NULL);
    pthread_cond_init(&condQueue, NULL);
    int j;
    for (i = 0; j < THREAD_NUM; j++){
        if (pthread_create(&th[j], NULL, &startThread, NULL) != 0){
            perror("Failed to create the thread");
        }
    }
    //initialise all client_socket[] to 0 so not checked 
    for (i = 0; i < max_clients; i++){  
        client_socket[i] = 0;  
    }      
    //create a master socket 
    if((master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0){  
        perror("socket failed");  
        exit(EXIT_FAILURE);  
    }
    //set master socket to allow multiple connections , 
    //this is just a good habit, it will work without this 
    if(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
          sizeof(opt)) < 0 ){  
        perror("setsockopt");  
        exit(EXIT_FAILURE);  
    }    
    //type of socket created 
    address.sin_family = AF_INET;  
    address.sin_addr.s_addr = INADDR_ANY;  
    address.sin_port = htons( PORT );       
    //bind the socket to localhost port 8888 
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0){  
        perror("bind failed");  
        exit(EXIT_FAILURE);  
    }  
    printf("Listener on port %d \n", PORT);    
    //try to specify maximum of 3 pending connections for the master socket 
    if (listen(master_socket, 3) < 0){  
        perror("listen");  
        exit(EXIT_FAILURE);  
    }
    //accept the incoming connection 
    addrlen = sizeof(address);  
    puts("Waiting for connections ...");    
    while(TRUE)  
    {  
        //clear the socket set 
        FD_ZERO(&readfds);  
 
        //add master socket to set 
        FD_SET(master_socket, &readfds);  
        max_sd = master_socket;  
             
        //add child sockets to set 
        for ( i = 0 ; i < max_clients ; i++){  
			//socket descriptor 
            sd = client_socket[i];  
			
            //if valid socket descriptor then add to read list 
            if(sd > 0)  
                FD_SET( sd , &readfds);  
                 
            //highest file descriptor number, need it for the select function 
            if(sd > max_sd)  
                max_sd = sd;  
        }  
     
        //wait for an activity on one of the sockets , timeout is NULL , 
        //so wait indefinitely 
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  
       
        if ((activity < 0) && (errno!=EINTR)){  
            printf("select error");  
        }  
             
        //If something happened on the master socket , 
        //then its an incoming connection 
        if (FD_ISSET(master_socket, &readfds)){  
            if ((new_socket = accept(master_socket, 
                    (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){  
                perror("accept");  
                exit(EXIT_FAILURE);  
            }  
             
            //inform user of socket number - used in send and receive commands 
            printf("New connection, socket fd is %d, ip is : %s, port: %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs
                  (address.sin_port));  
           
            //send new connection greeting message 
            if( send(new_socket, message, strlen(message), 0) != strlen(message) ){  
                perror("send");  
            }  
                 
            puts("Welcome message sent successfully");  
                 
            //add new socket to array of sockets 
            for (i = 0; i < max_clients; i++){  
                //if position is empty 
                if( client_socket[i] == 0 ){  
                    client_socket[i] = new_socket;  
                    printf("Adding to list of sockets as %d\n" , i);  
                         
                    break;  
                }  
            }  
        }  
             
        //else its some IO operation on some other socket
        for (i = 0; i < max_clients; i++){
            sd = client_socket[i];  
            if (FD_ISSET( sd , &readfds)){ 
                //Check if it was for closing , and also read the 
                //incoming message 
                if ((valread = read(sd,buffer,1024)) == 0){  
                    //Somebody disconnected , get his details and print 
                    getpeername(sd,(struct sockaddr*)&address,\
                        (socklen_t*)&addrlen);  
                    printf("Host disconnected , ip %s , port %d \n" , 
                          inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                    //Close the socket and mark as 0 in list for reuse 
                    close( sd );
                    client_socket[i] = 0;
                }
                //Echo back the message that came in 
                else{
					if(strncmp(buffer,"GET",strlen("GET"))==0){
						Task t = {
							.Tsd = sd,
							.Tok = 1
						};
						submitTask(t);
					}else{
						Task t = {
							.Tsd = sd,
							.Tok = 0
						};
						submitTask(t);
					}
                    //set the string terminating NULL byte on the end 
                    //of the data read 
                    //buffer[valread] = '\0';  
                    //send(sd , buffer , strlen(buffer) , 0 );  
                }
            }
        }
    }
    return 0;  
} 
