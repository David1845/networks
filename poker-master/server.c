#include <sys/socket.h>       // socket definitions
#include <sys/types.h>        // socket types
#include <arpa/inet.h>        // inet (3) funtions
#include <unistd.h>           // misc. UNIX functions
#include <signal.h>           // signal handling
#include <stdlib.h>           // standard library
#include <stdio.h>            // input/output library
#include <string.h>           // string library
#include <errno.h>            // error number library
#include <fcntl.h>            // for O_* constants
#include <sys/mman.h>         // mmap library
#include <sys/types.h>        // various type definitions
#include <sys/stat.h>         // more constants
#include "Poker.h"
// global constants
#define PORT 8888           // port to connect on
#define LISTEN 10            // number of connections

int list_s;                   // listening socket



// get a message from the socket until a blank line is recieved


// send a message to a socket file descripter
int sendMessage(int fd, char *msg) {
    return write(fd, msg, strlen(msg));
}

// Extracts the filename needed from a GET request and adds public_html to the front of it
char * getFileName(char* msg)
{
    // Variable to store the filename in
    char * file;
    // Allocate some memory for the filename and check it went OK
    if( (file = malloc(sizeof(char) * strlen(msg))) == NULL)
    {
        fprintf(stderr, "Error allocating memory to file in getFileName()\n");
        exit(EXIT_FAILURE);
    }
    
    // Get the filename from the header
    sscanf(msg, "GET %s HTTP/1.1", file);
    
    // Allocate some memory not in read only space to store "public_html"
    char *base;
    if( (base = malloc(sizeof(char) * (strlen(file) + 18))) == NULL)
    {
        fprintf(stderr, "Error allocating memory to base in getFileName()\n");
        exit(EXIT_FAILURE);
    }
    
    char* ph = "public_html";
    
    // Copy public_html to the non read only memory
    strcpy(base, ph);
    
    // Append the filename after public_html
    strcat(base, file);
    
    // Free file as we now have the file name in base
    free(file);
    
    // Return public_html/filetheywant.html
    return base;
}



// clean up listening socket on ctrl-c
void cleanup(int sig) {
    
    printf("Cleaning up connections and exiting.\n");
    
    // try to close the listening socket
    if (close(list_s) < 0) {
        fprintf(stderr, "Error calling close()\n");
        exit(EXIT_FAILURE);
    }
    
    // Close the shared memory we used
    shm_unlink("/sharedmem");
    
    // exit with success
    exit(EXIT_SUCCESS);
}
void getPokerMessage(char *buff,hands AllHands){
    char temp[16];
    buff[0] = '{';
   

    for(int i =0; i < 2;i++){
        strncat(buff,"\"",1);
        strncat(buff,"Pair",4);
        strncat(buff,"\" : ",4);
        sprintf(temp,"%d",AllHands[i].pair);
        strncat(buff,temp,strlen(temp));
        bzero(temp,sizeof(temp));
    }
    strncat(buff,"}",1);
    printf("\n\n\n%s\n",buff);
}

void JsonMessage(int sockfd)
{ 
    int MAX = 1000;
    char buff[MAX];
    struct hands *AllHands;
    int n; 
    // infinite loop for chat 
        bzero(buff, MAX); 
  
        // read the message from client and copy it in buffer 
        read(sockfd, buff, sizeof(buff)); 
        // print buffer which contains the client contents 
        printf("From client: %s\t To client : ", buff); 
        bzero(buff, MAX); 
        n = 0; 
        // copy server message in the buffer 

        // runs poker simulation with a selected hands
        AllHands = runPokerSimulation();
       // StatsOfAllIterations(AllHands);
        getPokerMessage(buff,AllHands);
        printf("\n\n\n%s\n",buff);
        strcat(buff,"\n");
        // and send that buffer to client 
        write(sockfd, buff, sizeof(buff)); 
  
} 

int main(int argc, char *argv[]) {
    int conn_s;                  //  connection socket
    short int port = PORT;       //  port number
    struct sockaddr_in servaddr; //  socket address structure
    
    // set up signal handler for ctrl-c
    (void) signal(SIGINT, cleanup);
    
    // create the listening socket
    if ((list_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        fprintf(stderr, "Error creating listening socket.\n");
        exit(EXIT_FAILURE);
    }
    /// release socket if binded before
    if (setsockopt(list_s, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
    perror("setsockopt(SO_REUSEADDR) failed");
    
    // set all bytes in socket address structure to zero, and fill in the relevant data members
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(port);
    
    // bind to the socket address
    if (bind(list_s, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
        fprintf(stderr, "Error calling bind()\n");
        exit(EXIT_FAILURE);
    }
    
    
    // Listen on socket list_s
    if( (listen(list_s, 10)) == -1)
    {
        fprintf(stderr, "Error Listening\n");
        exit(EXIT_FAILURE);
    } 
    

    // Size of the address
    int addr_size = sizeof(servaddr);
    // Number of child processes we have spawned
    int children = 0;
    // Variable to store the ID of the process we get when we spawn
    pid_t pid;
    
    // Loop infinitly serving requests
    while(1)
    {
    
        // If we haven't already spawned 10 children fork
      
        if( children <= 10){
            pid = fork();
            children++;
        }
     
        // If the pid is -1 the fork failed so handle that
        if( pid == -1){
            fprintf(stderr,"can't fork, error %d\n" , errno);
            exit (1);
        }
        
        // Have the child process deal with the connection
        if ( pid == 0)
        {	
            // Have the child loop infinetly dealing with a connection then getting the next one in the queue
            while(1)
            {
                // Accept a connection
                conn_s = accept(list_s, (struct sockaddr *)&servaddr, &addr_size);

                // If something went wrong with accepting the connection deal with it
                if(conn_s == -1)
                {
                    fprintf(stderr,"Error accepting connection \n");
                    exit (1);
                }
                
                // Get the message from the file descriptor
              
                JsonMessage(conn_s) ;
                
                

               close(conn_s);
                
            }
        }
    }
    
    return EXIT_SUCCESS;
}
///sudo lsof -nPi :8888
///  kill $(lsof -t -i:8888)
