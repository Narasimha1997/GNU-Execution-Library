#include "lib/DomainSockets.cpp";


//Create 3 handler functions, we use them to demonstrate 3 non-blocking sockets
void *handler1(void *args) {
    FILE *file_d;
    if((file_d = fopen("data1.txt", "a"))!= NULL){
        fprintf(file_d, "%s from handler1\n", (char *)args);
        fclose(file_d);
    }else exit(0);
}

void *handler2(void *args) {
    FILE *file_d;
    if((file_d = fopen("data2.txt", "a"))!= NULL){
        fprintf(file_d, "%s from handler2\n", (char *)args);
        fclose(file_d);
    }else exit(0);
}

void *handler3(void *args) {
    FILE *file_d;
    if((file_d = fopen("data3.txt", "a"))!= NULL){
        fprintf(file_d, "%s from handler3\n", (char *)args);
        fclose(file_d);
    }else exit(0);
}

int main(int argc, char **argv) {
    //handler for socket1 
    AsyncExecution exe1 = create_socket("V4", "UDP", "127.0.0.1", 6000, handler1, 1);
    exe1.execute();

    //handler for socket2
    AsyncExecution exe2 = create_socket("V4", "UDP", "127.0.0.1", 6001, handler2, 1);
    exe2.execute();

    //handler for socket3
    AsyncExecution exe3 = create_socket("V4", "UDP", "127.0.0.1", 6002, handler3, 1);
    exe3.execute();

    /* you can perform any task here
        DEMO:
    */
    fprintf(stdout, "\nSockets created, main-thread waiting\n");
    //Wait for completion:
    exe1.join(); exe2.join(); exe3.join();
}

