#AUTHOR Dylan Travers
#ID: 19128043
#PROGRAM: lift_sim

#Flags
CC = gcc
CFLAGS = -Wall -g -ansi -pedantic -Werror
OBJ = main.o fileIO.o
EXEC = lift_sim_A
PARAMS = 10 10

$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

#files
main.o : main.c main.h fileIO.h
	$(CC) -c main.c $(CFLAGS)

fileIO.o : fileIO.c fileIO.h
	$(CC) -c fileIO.c $(CFLAGS)

#CLEAN
clean :
	rm -f $(EXEC) $(OBJ)

#RUN
run :
	./$(EXEC) $(PARAMS)

#LEAKS
leaks :
	valgrind --leak-check=full ./$(EXEC)
