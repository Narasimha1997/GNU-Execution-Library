#include<pthread.h>
#include<stdio.h>
#include<unistd.h>


class AsyncExecution {
    private:
      pthread_t thread_id;
      void * (*__start_routine) (void *);
      void *args;

    public:
      AsyncExecution(void * (*routine) (void *), void *args){
          this->__start_routine = routine;
          this->args = args;
      }

      void execute() {
          pthread_create(&this->thread_id, NULL, __start_routine, this->args);
      }
      
      //Co-routines for obtaining functions and arguments
      void*  (*getSubroutine())(void *);
      void* getArgs();

      //Co-routines for creation and obtaining pthread_t
      static void createJoin(pthread_t t_id) {
          pthread_join(t_id, NULL);
      }

      pthread_t getThread_t(){
          return this->thread_id;
      }

      void join(){
          pthread_join(this->thread_id, NULL);
      }

    static joinAll(AsyncExecution *objects) {
        int __iterator = 0;
        for(__iterator = 0; __iterator < (sizeof(objects)/sizeof(objects[0])); __iterator++)
           objects[__iterator].join();
    }
};

void *AsyncExecution::getArgs(){
    return this->args;
}

void* (*AsyncExecution::getSubroutine())(void *) {
    return this->__start_routine;
}

// Example function, use this to test:
void *runner(void *args) {
    int i;
    for(i = 0; i <= (int *)args; i++){printf("%d\n", i);};
}

extern void* runner(void*);

extern AsyncExecution executor;