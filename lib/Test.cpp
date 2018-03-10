#include "DomainSockets.cpp"

void *handler(void *args){
    fprintf(stdout, "%s\n", (char *)args);
}

void *handler2(void *args) {
    fprintf(stdout, "%s\n", (char *)args);
}

int main(int argc, char **argv) {
    AsyncExecution exe1 = create_socket("V4", "UDP", "127.0.0.1", 6000, handler, 1);
    exe1.execute();
    AsyncExecution exe2 = create_socket("V4", "UDP", "127.0.0.1", 6002, handler2, 1);
    exe2.execute();
    exe1.join(); exe2.join();
}