#AUTHOR Dylan Travers
#ID: 19128043
#PROGRAM: lift_sim

#Flags
CC = gcc
CFLAGS = --std=c99 -Wall -Wextra -Wformat=2 -Wswitch-default -Wswitch-enum -Wpointer-arith -Wbad-function-cast -Wstrict-overflow=5 -Wstrict-prototypes -Winline -Wundef -Wnested-externs -Wcast-qual -Wshadow -Wunreachable-code -Wfloat-equal -Wstrict-aliasing -Wredundant-decls -Wold-style-definition -g -O0 -fno-omit-frame-pointer -fno-common -Wdouble-promotion -Wcast-align -Winit-self
TARGET = lift_sim_A
OBJECTS = main.o fileIO.o queue.o threads.o

#files

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) -lpthread

main.o : main.c main.h
	$(CC) $(CFLAGS) -c main.c

fileIO.o : fileIO.c fileIO.h
	$(CC) $(CFLAGS) -c fileIO.c

queue.o : queue.c queue.h
	$(CC) $(CFLAGS) -c queue.c

threads.o : threads.c threads.h
	$(CC) $(CFLAGS) -c threads.c

#CLEAN
clean :
	rm -rf lift_sim_A
	rm -rf *.o
	rm -rf sim_out.txt

#RUN
run :
	./lift_sim_A 10 1

valgrind :
	valgrind ./lift_sim_A 10 1

helgrind :
	valgrind --tool=helgrind ./lift_sim_A 10 1
