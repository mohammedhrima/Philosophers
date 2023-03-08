Threads

synchronize / unsynchronize
mutex should not be copied / the behaiviour is undefined

threads share all data segement (global and static variables)

POSIX: portable operatinh system interface

we can switch between threads much faster than switching between processes , because threads share the same data addresses

mutex: mutual exclusion
    is like a imaginary box stands / means that thread is usign this variable and no one can access to it  

=================================================================================================================
Process:
    is an address space with one or more threads executing in this space

processes id go from 2 to 32768
1 is reserved to init process wo mange all processes

ps: showes processes related to terminal
ps -e: shows all processes (even those who don't need to comunicate with terminal)
ps -e -f: to get full informations

semaphores:
mutexes:

================================================================================================================
Concurency: cores switch between two threads

never make a copy of thread condition variable

- to create a condition statically:
        var data = {
            PTHREAD_MUTEX_INITIALIZER,
            PTHREAD_COND_INITIALIZER,
            0};

- to create a condition dynamically:
    
- always destory 

- types of behaviour of threads:
    joinable (default): main thread wait for it
    detached : main thread won't wait for it 


- to see adress of all your data: objdump -t

- to run programs one after another:
    ./program1 && ./program2

- to run programs in same time:
    ./program1 & ./program2

-  WL,-no_pie (no position indipendent executable)

- how to use valgrind:
    gcc -g file.c -o file
    valgrind --leak-check=full ./file