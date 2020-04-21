#AUTHOR Dylan Travers
#ID: 19128043
#PROGRAM: lift_sim

#Flags
CC = gcc
CFLAGS = -Wall -g -ansi -pedantic -Werror -pthread
OBJ = main.o fileIO.o queue.o threads.o
EXEC = lift_sim_A
PARAMS = 10 10

$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

#files
main.o : main.c main.h fileIO.h queue.h threads.h
	$(CC) -c main.c $(CFLAGS)

fileIO.o : fileIO.c fileIO.h queue.h
	$(CC) -c fileIO.c $(CFLAGS)

queue.o : queue.c queue.h
	$(CC) -c queue.c $(CFLAGS)

threads.o : threads.c threads.h queue.h
	$(CC) -c threads.c $(CFLAGS)



#CLEAN
clean :
	rm -f $(EXEC) $(OBJ)

#RUN
run :
	./$(EXEC) $(PARAMS)

#LEAKS
leaks :
	valgrind --leak-check=full ./$(EXEC)
