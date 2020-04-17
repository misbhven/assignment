#AUTHOR Dylan Travers
#ID: 19128043
#PROGRAM: lift_sim

#Flags
CC = gcc
CFLAGS = -Wall -g -ansi -pedantic -Werror
OBJ = main.o fileIO.o
EXEC = lift_sim_A

$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

#files
main.o : main.c main.h fileIO.h
	$(CC) -c main.c $(CFLAGS)

fileIO.o : fileIO.c fileIO.h
	$(CC) -c fileIO.c $(CFLAGS)

#clean
	#to run: OS/assignment$ make clean
clean :
	rm -f $(EXEC) $(OBJ)

#run lift_sim
	#to run: OS/assignment$ make run
run :
	./$(EXEC)

#run with valgrind
	#to run: OS/assignment$ make leaks
leaks :
	valgrind --leak-check=full ./$(EXEC)
