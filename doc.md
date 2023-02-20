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

