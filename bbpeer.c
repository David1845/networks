
//
//  main.c
//
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
#include <pthread.h>
#define MAXNAME 1024
#define SERV_PORT 6000
#define has_token 2
typedef struct info{
    char *ip;
    char *port;
    long len;
}info;
char token[256];
int token_pass =0; //
int fd;
int existing_flag =0;
int see_token_travel =0;
const char * messageTitle;
//struct sockaddr_in servaddr;

void create_socket(){
    if((fd=socket(AF_INET, SOCK_DGRAM, 0))<0)
    {
        perror("socket failed");
        exit(1);
    }
}
/// Takes the Ip and port number and
void combined_ip_port(char *message, struct sockaddr_in client){
    
    int temp_port;
    temp_port = ntohs(client.sin_port);
    
    char *ip = inet_ntoa(client.sin_addr); // inet_ntoa RETURNS STATIC MEMORY CANT ADD MORE TO IT.
    
    // combines message and ip
    strcpy (message, ip);
    
    // inserted a space, its easy to sperate the message using strtok
    strcat (message, " ");
    
    char port[16];
    
    sprintf(port,"%d",temp_port);
    message = strcat(message, port);
}

struct hostent* get_host_name(const char *hostname){
    struct hostent *server;
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(1);
    }
    return server;
}



info get_port_ip(char recv[],info stuff){
    stuff.ip = strtok(recv," ");
    stuff.port = strtok( NULL," ");
    return stuff;
}


struct sockaddr_in set_server_info( struct sockaddr_in servaddr, u_short port,struct hostent *server){
    memset((char *)&servaddr,0, sizeof(servaddr));
    servaddr.sin_family = (short) (AF_INET);
    memcpy((void *)&servaddr.sin_addr, server->h_addr_list[0], server->h_length);
    servaddr.sin_port=htons(port);
    return servaddr;
}


void print_server_info( struct sockaddr_in servaddr){
    fprintf(stdout, "information of server %s at port %d\n",inet_ntoa(servaddr.sin_addr),
            ntohs(servaddr.sin_port));
}


void send_to_server(struct sockaddr_in servaddr, char buf[],socklen_t serverlen){
    long len;
    serverlen = sizeof(servaddr);
    len = sendto(fd, buf, 256, 0, (struct sockaddr *)&servaddr,serverlen);
    if (len < 0)
        perror("ERROR in sendto");
}


void recv_from_server(char recv[]){
    
    printf("waitng for server\n");
    recvfrom(fd, recv, 256, 0, NULL , NULL);
}


struct sockaddr_in recv_from(int socket_test,char buffer[],struct sockaddr_in client,socklen_t *address_length){
    
    long length = recvfrom(socket_test,
                           buffer,
                           256,
                           0,
                           (struct sockaddr *)&client,
                           address_length);
    if(length < 0)
        perror("error in recvfrom");
    return client;
}


struct sockaddr_in set_client( struct sockaddr_in client,info neighbor){
    memset((char *)&client, 0, sizeof(client));
    client.sin_family = (short)(AF_INET);
    inet_aton(neighbor.ip,&client.sin_addr);
    client.sin_port = htons(atoi(neighbor.port));
    return client;
}


void print_client_info( struct sockaddr_in client){
    fprintf(stdout, "information of client %s at port %d\n",inet_ntoa(client.sin_addr),
            ntohs(client.sin_port));
}


void send_to_neighborClient(struct sockaddr_in client, char buf[],socklen_t serverlen){
    long len;
    serverlen = sizeof(client);
    len = sendto(fd, buf, 256, 0, (struct sockaddr *)&client,serverlen);
    if (len < 0)
        perror("ERROR in sendto");
}
void show_token(){
    if(see_token_travel == 1){
        printf("token\n");
    }
}

void *Pass_token(void * param){
    info *neighbor = (info *) param;
    struct sockaddr_in clientNext;
    struct sockaddr_in recived;
    clientNext = set_client(clientNext, *neighbor);
    
    socklen_t serverlen = sizeof(struct sockaddr_in);
    while (1) {
        token_pass =0;
        recived = recv_from(fd,token,recived,&serverlen);
        show_token();
        if (strcmp(token,"token")==0){
            token_pass = 2;
            if(existing_flag == 1){
                /// exiting the ring
                char existing_message[256];
                char temp[256];
                memset(existing_message, '\0', sizeof(existing_message));
                strcat(existing_message,"exit ");
                combined_ip_port(temp,clientNext);
                strcat(existing_message,temp);
                send_to_neighborClient(recived,existing_message, serverlen);
                printf("No longer reciveing the token goodbye\n");
                exit(1);
            }else{
                sleep(1);
            send_to_neighborClient(clientNext, token, serverlen);
            }
            //restablishing the ring when an exit command happens
        }else if (strncmp("exit",token, 4)==0){
            info next;
            strtok(token, " ");
            next.ip  = strtok(NULL, " ");
            printf("%s\n",next.ip);
            next.port = strtok(NULL, " ");
            printf("%s\n",next.port);
            clientNext = set_client(clientNext,next);
            send_to_neighborClient(clientNext,"token", serverlen);
        }
        else{ // accpeting other client trying to connect
            char message[256];
            memset(message, '\0', sizeof(message));
            combined_ip_port(message,clientNext);
            send_to_neighborClient(recived, message, serverlen);
            clientNext = recived;
        }
        
    }
    return NULL;
}


//////////////////////////////////                                       ////////////////////////////////////////////////////////
//////////////////////////////////             message board             ////////////////////////////////////////////////////////
//////////////////////////////////                                       ////////////////////////////////////////////////////////


FILE * read_file(){
    FILE * fp;
    fp = fopen(messageTitle, "r");
    if(fp==NULL){
        perror("file");
    }
    return fp;
    
}


int find_message_next_number(){
    FILE * fp = read_file();
    int how_many_messages=0;
    char buf[256];
    while (!feof(fp))
    {
        if(strstr(buf, "<message n=")){
            how_many_messages++;
        }
        fgets(buf, 256, fp);
        
    }
    how_many_messages++;
    return how_many_messages;
}


void printFile(){
    FILE * fp = read_file();
    char buf[256];
    while (fgets(buf, 256, fp)){
        printf("%s",buf);
    }
    fclose(fp);
}


void create_header(char *header){
    strcpy(header, "<message n=");
    int messageNumeber =find_message_next_number();
    char temp[8];
    sprintf(temp,"%d",messageNumeber);
    strcat(header, temp);
    strcat(header, ">\n");
    
}


unsigned long body_size(char *message, char *body){
    strcat(body, message);
    unsigned long size = strlen(body);
    return size+1;
}


void combined(char * body, char* header){
    strcat(header,body);
    strcat(header, "</message>\n");
}


void append(char header[]){
    FILE * fp;
    fp = fopen(messageTitle, "a");
    if(fp==NULL){
        perror("file");
    }
    fputs(header, fp);
    
    fclose(fp);
}


void sperate_message(char total[], char *command, char *message){
    char * token = strtok(total," ");
    //message = strtok(NULL, " ");
    int counter=0;
    while(token!=NULL){
        if(counter == 0){
            strcat(command, token);}
        else{
            strcat(message, token);
            strcat(message, " ");}
        token = strtok(NULL, " ");
        counter++;
    }
}


void read_certain_message(int messagenumber){
    FILE * fp = read_file();
    int how_many_messages =0;
    char buf[256];
    while (!feof(fp)) {
        if(strstr(buf, "<message n=")){
            how_many_messages++;
            if (how_many_messages == messagenumber){
                printf("%s",buf);
                while (1) {
                    fgets(buf, 256, fp);
                    printf("%s",buf);
                    
                    if(strstr(buf, "</message>")){
                        break;
                    }
                }
            }
        }
        fgets(buf, 256, fp);
    }
    if (messagenumber > how_many_messages){
        printf("message not found");
    }
}


void *message_Board(){
    int number=0;
    char total[256];
    char command[256];
    char message[256];
    printf("----------------Message board Commands----------------------\n");
    printf("type 'write' and the message you wish to send\n");
    printf("type 'read' and any number to read a particular message \n");
    printf("type 'list' to list all messages\n");
    printf("type 'exit' to leave the message board\n");
    printf("type 'see' to see when the token arives type 'see' again to turn it off\n");
    while(1){
        if(number ==0){
            memset(total, '\0', sizeof(total));
            memset(command, '\0', sizeof(command));
            memset(message, '\0', sizeof(message));
            printf("enter message \n");
            fgets(total,sizeof(total), stdin);
            sperate_message(total ,command, message);
            number++;
        }
        if(strcmp(command, "write")==0 && token_pass == has_token){
            
            char header [1024];
            char body[1024];
            memset(body, '\0', sizeof(body));
            memset(header, '\0', sizeof(header));
            create_header(header);
            unsigned long size = body_size(message,body);
            message[size]='\n';
            combined(message, header);
            append(header);
            number = 0;
        } else if (strcmp(command, "read")==0){
            int messagenumber = atoi(&message[0]);
            read_certain_message(messagenumber);
            number =0;
        }
        else if (strcmp(command, "list\n" )==0){
            printFile();
            number =0;
        }
        else if (strcmp(command, "exit\n" )==0){
            existing_flag = 1;
            number = 0;
        }
        else if(strcmp(command,"see\n")==0){
            // turns token visability on and off
            if (see_token_travel==1) {
                see_token_travel = 0;
            }else{
            see_token_travel = 1;
            }
            number =0;
        }
        else if(strcmp(command, "write")!=0) {
            printf("invaild command\n");
            number = 0;
        }
        
    }
    return NULL;
}
///////////////////////////////// ////////////////////////////////// //////////////////////////////////
//                                      message board
////////////////////////////////// ////////////////////////////////// //////////////////////////////////
void bind_client(int socket_test, struct sockaddr_in myaddr){
    if(bind(socket_test,(struct sockaddr *)&myaddr,sizeof(myaddr)) < 0)
    {
        perror("bind error");
        exit(1);
    }
}


struct sockaddr_in set_Me(struct sockaddr_in myaddr,ushort myport){
    memset((char *)&myaddr,0, sizeof(myaddr));
    myaddr.sin_family=AF_INET;
    myaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    myaddr.sin_port=htons(myport);
    return myaddr;
}


int main(int argc, const char * argv[]) {
    
 ////////////////////////////////// ////////////////////////////////// //////////////////////////////////
    
 ////////////////////////////////// ////////////////////////////////// //////////////////////////////////
    info neighbor;
    struct sockaddr_in servaddr;
    struct sockaddr_in client;
    struct hostent *server;
    socklen_t serverlen;
    char buf[256];
    char recv[256];
   ////////////////////////////////// ////////////////////////////////// //////////////////////////////////
    
   ////////////////////////////////// ////////////////////////////////// //////////////////////////////////
    if(argc != 5){
        printf("need 5 arguments not %d",argc);
        exit(1);
    }
    const char * host = argv[1];
    const u_short port = atoi(argv[2]);
    const u_short myport = atoi(argv[3]);
    messageTitle = argv[4];
    
    
    strcpy(buf, "this is a message");
    
    
    create_socket();
    server = get_host_name(host);
    
    servaddr = set_server_info(servaddr, port,server);
    print_server_info(servaddr);
    
    
    /////////////sending to server///////////////////////////////
    
    client = set_Me(client,myport);
    bind_client(fd,client);
    print_client_info(client);
    send_to_server(servaddr,buf,serverlen);
    recv_from_server(recv);
    
    
    neighbor = get_port_ip(recv,neighbor);

    pthread_t tid;
    pthread_t tid2;
    pthread_create(&tid2, NULL, message_Board, NULL);
    pthread_create(&tid, NULL,(void*)&Pass_token, (void *)&neighbor);
    pthread_join(tid2, NULL);
    pthread_join(tid,NULL);
    close(fd);
    return 0;
}

