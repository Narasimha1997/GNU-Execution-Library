#include "Execution.cpp"

#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<stdlib.h>

// A simple wrapper class for functions containing multiple AsyncExeccution threads

class ExecutionType {
    private: void * (*function)(void *);
    public:
    ExecutionType() {return;}
    ExecutionType(void * (*main_function)(void *), void *args) {
        this->function = main_function;
    }

    void* (*getFunction())(void *) {
        return this->function;
    }
};

class TaskExecutor {
    private:
     pid_t proc_type;
     ExecutionType *exe;
     int num_p;
    
    public:
      TaskExecutor(ExecutionType *exec, int num_tasks) {
          this->exe = exec;
          this->num_p = num_tasks;
      }

      void execute() {
          pid_t pid[this->num_p];
          int iterator_;
          for(iterator_ = 0; iterator_ < this->num_p; iterator_++){
              pid[iterator_] = fork();
              if(pid[iterator_] == 0 ){
                  fprintf(stdout, "Spanning child %d\n", iterator_);
                  this->exe[iterator_].getFunction()((void* )iterator_);
              }
          }

          //create wait for threads:
          int processes = this->num_p, *status;
          while(processes > 0){
              wait(NULL);
              fprintf(stdout, "Deleted child: %d");
              --processes;
          } if(processes  == 0) exit(0);
      }
};

void *run(void * args) {
    fprintf(stdout, "Executing instance from %d\n", (int *)args);
    int i = 0;
    for(i = 0; i<10; i++) {
        fprintf(stdout, "Hello, world FROM: %d\n\n", (int *)args);
        sleep(2);          
    }
}

void *taskFunction(void * args) {
    AsyncExecution exe = AsyncExecution(run, args);
    exe.execute(); exe.join();
}
