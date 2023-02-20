
# pthread_create:
   1. takes 4 arguments
      1. pointer to thread
      2. specifies attributes , if is NULL , default attributes will be used, (attribute means behaviour of thread)
      3. function name to be executed by the thread 
      4. argumnets that will be passed to function above (myThreadFun)
   2. return 0 if thread create succefully, otherwise return error number

# pthread_join:
   1. takes 2 arguments:
      1. thread to wait for
      2. thread to return if the thread above did terminate
   2. equivalent to wait for processes

# pthread_exit: 
   1. takes 1 argument:
      1. address to thread
   2. make this address available for other threads
   3. never use it to return a pointer to local variable

# pthread_self:
   1. create sub thread for thread
   2. return the id of new sub thread

# pthread_mutex_int:
   1. takes 2 argument:
      1. pointer to pthread_mutex_t
      2. pointer to const pthread_mutexattr_t
   2. create a new mutex
   3. return 0 if succefully created mutex