#include "DomainSockets.cpp"

void *default_log_handler(void *args) {
    fprintf(stdout, "%s\n", (char *)args);
}

int main(int argc, char **argv) {
    if(argc < 5) {
        fprintf(stdout, "Provide arguments\n");
        fprintf(stdout, "Arguments: \n1. Address Type: V4 or V6");
        fprintf(stdout, "\n2. Socket Type: UDP or TCP or UDS");
        fprintf(stdout, "\n3. Adddress: Host address");
        fprintf(stdout, "\n4. PORT: Port number");
        fprintf(stdout, "Mode: 1 -> Blocking execution 2-> Async execution\n");
        fprintf(stdout, "%s\n", argv[0]);
        exit(0);
    }
    AsyncExecution exe = create_socket(argv[1], argv[2], argv[3], atoi(argv[4]), default_log_handler ,argv[5]);
    exe.execute();
    exe.join();
}