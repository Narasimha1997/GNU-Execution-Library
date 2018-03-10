#include<stdio.h>
#include "lib/TaskExecutor.cpp"

void *thread1(void *) {
    fprintf(stdout, "Thread 1 -task1\n");
    sleep(3);
}

void *thread2(void *) {
    while(1) {
        fprintf(stdout, "\nThread 2 -task1\n");
        sleep(3);
    }
}

void *thread3(void *) {
    fprintf(stdout, "Thread 1 -task2\n");
    sleep(3);
}


void *thread4(void *) {
        fprintf(stdout, "Thread 2 -task2\n");
        sleep(3);
}


void *task1(void *){
    AsyncExecution exe = AsyncExecution(thread1, NULL);
    exe.execute();
    AsyncExecution exe2 = AsyncExecution(thread2, NULL);
    exe2.execute();

    exe.join(); exe2.join();
}

void *task2(void *) {
    AsyncExecution exe = AsyncExecution(thread3, NULL);
    exe.execute();
    AsyncExecution exe2 = AsyncExecution(thread4, NULL);
    exe2.execute();

    exe.join(); exe2.join();
}

int main(int argc, char **argv) {
    int tasks = 2;
    ExecutionType types[tasks];
    types[0] = ExecutionType(task1, NULL);
    types[1] = ExecutionType(task2, NULL);

    TaskExecutor exe = TaskExecutor(types, tasks);
    exe.execute();
}