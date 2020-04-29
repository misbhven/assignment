# assignment
AUTHOR: DYLAN TRAVERS
PROGRAM: lift_sim_A (pthreads)
SEM 1 2020

An Operating Systems COMP200 assignment to design a Bounded-Buffer/Producer-Consumer
solution for multiple simulated elevators servicing different floors of a building. Written in C using pthreads.

usage:
~/./lift_sim_A <buffer_size> <sleep_time>

make commands:
~/make
    compile using gcc
~/make clean
    remove executable, .o files, output text file
~/make run
    run program with sample inputs of buffer size = 10 and sleep time = 1
~/make valgrind
    run with valgrind
~/make helgrind
    run with valgrind tool helgrind for race condiditons
  
