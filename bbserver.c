
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define MAX_BUFFER 256 /*maximum payload size*/
u_short myport;

void combined_ip_port(char *message, struct sockaddr_in client[],int socket_test,socklen_t address_length,int counter){
    
    int temp_port;
    temp_port = ntohs(client[counter].sin_port);
    
    char *ip = inet_ntoa(client[counter].sin_addr); /// inet_ntoa RETURNS STATIC MEMORY CANT ADD MORE TO IT.
    
    /// combines message and ip
    strcpy (message, ip);
    
    /// insertes a space so i can easly sperate the message using strtok
    strcat (message, " ");
    
    char port[16];
    
    sprintf(port,"%d",temp_port);
    message = strcat(message, port);
    printf("the message %s\n",message);
}


int create_socket(){
    int socket_test=socket(PF_INET, SOCK_DGRAM, 0);
    if(socket_test<0)
    {
        perror("error at Socket");
        return 0;
    }
    return socket_test;
}

/// sets the server ip and port
struct sockaddr_in set_server(struct sockaddr_in myaddr){
    memset((char *)&myaddr,0, sizeof(myaddr));
    myaddr.sin_family=AF_INET;
    myaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    myaddr.sin_port=htons(myport);
    return myaddr;
    
}

/// combindes the ip and the port to the socket so the client knows who to send to.
void bind_server(int socket_test, struct sockaddr_in myaddr){
    if(bind(socket_test,(struct sockaddr *)&myaddr,sizeof(myaddr)) < 0)
    {
        perror("bind error");
        exit(1);
    }
}


void recv_from_client(int socket_test,char buffer[],struct sockaddr_in client[],socklen_t *address_length,int counter){
    
    long length = recvfrom(socket_test,
                           buffer,
                           MAX_BUFFER,
                           0,
                           (struct sockaddr *)&client[counter],
                           address_length);
    if(length < 0)
        perror("error in recvfrom");
}


int main(int argc, const char * argv[]) {
    int socket_test;
    int test;
    long length;
    char message[256];
    char *ip;
    char port[16];
    char buffer[MAX_BUFFER];
    int ip_port[16];
    int counter = 0;
    struct sockaddr_in myaddr;
    struct sockaddr_in client[16];
    int maxClients;
    
    
    if(argc != 3){
        printf("need 3 arguments not %d\n",argc);
        exit(1);
    }
    myport = atoi(argv[1]);
    maxClients = atoi(argv[2]);
    
    // initialize length address
    socklen_t address_length = sizeof(struct sockaddr_in);
    
    
    socket_test = create_socket();
    
    
    myaddr = set_server(myaddr);
    
    
    bind_server(socket_test,myaddr);
    
    
    
    while (1)
    {
        printf("wating for client\n");
        // recives a request stores the recpetent ip and port in an arry called client
        recv_from_client(socket_test,buffer,client, &address_length,counter);
        
        fprintf(stdout, "received message from %s at port %d\n",inet_ntoa(client[counter].sin_addr),
                ntohs(client[counter].sin_port));
        
        printf("client %d\n",counter);
       if (counter == maxClients-1){
            
            
        // sends a message to n+1 postion in the client arry for every client expect the last one (1 ... n-1) ////////////////
           int client_num =0;
            for(client_num=0;client_num<counter;client_num++){
                combined_ip_port(message, client, socket_test, address_length, client_num+1);
                long len = sendto(socket_test, message, 256, 0, (struct sockaddr*)&client[client_num], address_length);
                if (len<0) {
                    perror("sendto");
                }
            }
            
            
        //  the server sends client K+1 info to client K; where K is your iterator that iterates to N-1 (K max is n-1)
        //
        //
        //                                         C1<- (C2 IP and port)--S-- (C3 IP and port) ->C2
        //                                                                |
        //                                                          (CK+1 IP and port)
        //                                                                |
        //                                                                v
        //                                                           CK IP and port
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            
        //                       c1 can send to c2, c2 can send to c3 ... c(n-1) can send to cN
        //                                        c1
        //                                        |
        //                                        V
        //                                       c2 --> c3 .... c(n-1) -->c(n)
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            
            
            //// the nth postion sends to the first postion forming the ring ////////////////
            combined_ip_port(message, client, socket_test, address_length, 0);
            long len = sendto(socket_test, message, 256, 0, (struct sockaddr*)&client[counter], address_length);
            if (len<0) {
                perror("sendto");
            }
            break;
            //         N is how many clients. K is the iterator that iterates to N-1 (K max is n-1)
            //
            //    The picture below is the same as the one above; except that the server is sending C1 IP and port to the last client CN. Resulting an establish ring.
            
            //                                                               CN
            //                                                                ^
            //                                                                |
            //                                                          (C1 IP and port)
            //                                                                |
            //                                        C1<- (C2 IP and port) --S-- (C3 IP and port) ->C2
            //                                                                |
            //                                                          (CK+1 IP and port)
            //                                                                |
            //                                                                v
            //                                                           CK IP and port
            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            ///                          ring is establsihed the last client C(n), sends to the first C(1).
            
            //                                        c1  <---------------\
            //                                        |                    \
            //                                        V                     \
            //                                       c2 --> c3 .... c(n-1) -->c(n)
            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            

        }
        counter++;
    }
    //sends token to start the token ring
    
    long len = sendto(socket_test, "token", 256, 0, (struct sockaddr*)&client[0], address_length);
    if (len<0) {
        perror("sendto");
    }
    
    
    return 0;
}
