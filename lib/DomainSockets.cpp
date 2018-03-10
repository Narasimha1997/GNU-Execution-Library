#include "Execution.cpp"
#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>

#define RECV_BUFFER 4096;

class socket_struct {
    public:
    *socket_struct() {
        return;
    }
    char *addr_type;
    char *sock_type;
    char *addr;
    uint16_t PORT;
    void * (*handler)(void *args);
};

class DatagramSocketDomain {
    private:
        int sock_fd;
        struct sockaddr_in address;
        int TYPE;
        void* (*event_handler)(void *);
        const void *optval = (void *)1;

    public:
        DatagramSocketDomain(struct sockaddr_in address, char* type, void* (*event_handler)(void *)) {
            if(strcmp(type, "V6") == 0) TYPE = AF_INET6;
            if(strcmp(type, "V4") == 0) TYPE = AF_INET;
            this->address = address;
            printf("Type: %d", TYPE);
            this->event_handler = event_handler;
        }

        void create() {
            if((this->sock_fd = socket(this->TYPE, SOCK_DGRAM, 0)) < 0){
                fprintf(stderr,"Socket creation failed");
                exit(0);
            }
            fprintf(stdout, "\nSocket created successfully, file descriptor : %d\n", this->sock_fd);
            char buffer[4096];
            setsockopt(this->sock_fd, SOL_SOCKET, SO_REUSEADDR, optval , sizeof(int));
            //create a bind:
            if(bind(this->sock_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
                fprintf(stderr, "\nBind failed\n");
                exit(0);
            }
            //waiting for event:
            while(1) {
                int len = recvfrom(this->sock_fd, buffer, sizeof(buffer)-1, 0, NULL, 0);
                if(len < 0) {fprintf(stderr, "\nReceive Error\n"); exit(0);}
                this->event_handler((void *)buffer);
            }
            return;
        }
};

class TransmissionSocketDomain {
    private:
        int sock_fd;
        struct sockaddr_in address;
        int TYPE;
    public:
        TransmissionSocketDomain(struct sockaddr_in address, char* type) {
            if(strcmp(type, "V6") == 0) TYPE = AF_INET6;
            if(strcmp(type, "V4") == 0) TYPE = AF_INET;
            this->address = address;
        }

        void create() {
            if(sock_fd = socket(this->TYPE, SOCK_STREAM, 0) < 0){
                fprintf(stderr,"Socket creation failed");
                exit(0);
            }
            fprintf(stdout, "\nSocket created successfully\n");
            fprintf(stdout, "\nNot yet implemented\n");
            return;
        }
};

class UnixDomainSocket {
    void create() {
        return;
    }
};


class SocketDomain {

    private:
    //Datatypes
      char *sock_type;
      struct sockaddr_in ip_address;
      int addrlen;
      int sock_descriptor;
      char *addr_type;
      void* (*event_handler)(void *);

    //private routines:
      struct sockaddr_in addressResolve(char *addr_family, char * addr, uint16_t PORT) {
          if(strcmp(addr_family, "v6") == 0) this->ip_address.sin_family = AF_INET6;
          if(strcmp(addr_family, "unix") == 0) this->ip_address.sin_family = AF_UNIX;
          else this->ip_address.sin_family = AF_INET;
          this->ip_address.sin_port = htons(PORT);
          this->ip_address.sin_addr.s_addr = inet_addr(addr);
          memset(this->ip_address.sin_zero, '\0', sizeof(this->ip_address.sin_zero));  

        return ip_address;
      }

      char *__socket_probe(char* sock_type) {
          if(strcmp(sock_type, "UDS") == 0) return "SOCK_RAW";
          if(strcmp(sock_type, "TCP")==0) return "SOCK_STREAM";
          if(strcmp(sock_type, "UDP")==0) return "SOCK_DGRAM";
          else return "SOCK_NO_OP";
      }

      void assign_eventHandler(void* (*event_handler)(void *)){
          this->event_handler = event_handler;
      }

      void invokeTCP(){
          printf("\nCreating a TCP socket\n");
          TransmissionSocketDomain tcp = TransmissionSocketDomain(this->ip_address, this->addr_type);
          tcp.create();
          return;
      }

      void invokeUDP(struct sockaddr_in ip_address){
          fprintf(stdout, "\nCreating a UDP socket\n");
          DatagramSocketDomain udp = DatagramSocketDomain(ip_address, this->addr_type, event_handler);
          udp.create();
          return;
      }

      void invokeUDS(){
          fprintf(stdout,"\nCreating a Unix Domain Socket\n");
      }

      //allocate memory for IP address resolution
      void alloc_addr(){
          memset(&ip_address, 0, sizeof(ip_address));
      }

    public:
    SocketDomain(char *addr_type, char *socket_type, char *addr, uint16_t PORT, void* (*event_handler)(void *)) {
        this->addr_type = addr_type;
        char * sock = this->__socket_probe(socket_type);
        struct sockaddr_in ip_address = this->addressResolve(addr_type, addr, PORT);
        alloc_addr();
        this->assign_eventHandler(event_handler);
        //Probe to get socket type:
        if(strcmp(sock, "SOCK_STREAM") == 0) invokeTCP();
        else if(strcmp(sock, "SOCK_DGRAM")== 0) invokeUDP(ip_address);
        else if(strcmp(sock, "SOCK_RAW") == 0 && strcmp(addr_type, "unix") ==0) invokeUDS();
        else fprintf(stdout,"\nsocket type not supported, (%s) is a no-op\n", sock);
    }
};

void *event_handler(void *args){
    FILE *fp;
    if((fp = fopen("socketData.txt", "a"))!= NULL){
        fprintf(fp, "%s\n", (char *)args);
        fclose(fp);
    }else{fprintf(stderr, "Bad file descriptor"); exit(0); }
    printf("%s\n", (char *)args);
}

void *threaded_socket(void* sock_data){
    socket_struct* NODE = (socket_struct* )sock_data;
    fprintf(stdout, "Creating socket in thread");
    SocketDomain sock = SocketDomain(
        NODE->addr_type,
        NODE->sock_type,
        NODE->addr,
        NODE->PORT,
        NODE->handler
    );
    return NULL;
}

extern AsyncExecution create_socket(char *addr_type, char *sock_type, char* addr, uint16_t PORT, void* (*handler)(void *), int threaded) {
    if(threaded == 0){
        fprintf(stdout, "Started socket in blocking mode, ctrl+C to quit\n");
        SocketDomain socket = SocketDomain(
            addr_type,
            sock_type,
            addr,
            PORT,
            handler
        );
    }else{
       socket_struct *domain_socket = new socket_struct();
       domain_socket->addr_type = addr_type;      
       domain_socket->sock_type = sock_type;
       domain_socket->PORT = PORT;
       domain_socket->handler = handler;
       domain_socket->addr = addr;
       AsyncExecution execute_socket = AsyncExecution(
           threaded_socket,
           (void *)domain_socket 
        );
       return execute_socket;
    }
}

class CreateSockets {
    private:
      int start_no;
      int number_sockets;
      char *IP_ADDR;
      void* (*handler)(void *);
    public:
    CreateSockets(int start_no, int number_sockets, uint16_t sock_number, char *IP, void * (*handler)(void *)){
        this->start_no; this->number_sockets;
        this->start_no = sock_number;
        this->IP_ADDR = IP;
        this->handler = handler;
    }

    void create() {
        return ;
    }
};