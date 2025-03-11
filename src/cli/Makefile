CC = gcc
DEV = -Wall -Wextra
CFLAGS =  -std=c99
OBJ = main.o parcer/rotate.o parcer/rectangle.o parcer/parcer.o parcer/circle.o parcer/ornament.o parcer/additional.o parcer/help.o checker/base_checker.o checker/check_functions.o checker/help_functions.o
EXEC = cw

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

main.o: main.c parcer/parcer.h structures.h checker/base_checker.h
	$(CC) $(CFLAGS) -c main.c -o main.o

parcer/parcer.o: parcer/parcer.c parcer/parcer.h parcer/rotate.h parcer/rectangle.h  parcer/circle.h parcer/additional.h parcer/help.h structures.h
	$(CC) $(CFLAGS) -c parcer/parcer.c -o parcer/parcer.o

parcer/rotate.o: parcer/rotate.c  parcer/rotate.h parcer/parcer.h
	$(CC) $(CFLAGS) -c parcer/rotate.c -o parcer/rotate.o

parcer/rectangle.o: parcer/rectangle.c  parcer/rectangle.h parcer/parcer.h
	$(CC) $(CFLAGS) -c parcer/rectangle.c -o parcer/rectangle.o

parcer/circle.o: parcer/circle.c  parcer/circle.h parcer/parcer.h
	$(CC) $(CFLAGS) -c parcer/circle.c -o parcer/circle.o

parcer/ornament.o: parcer/ornament.c  parcer/ornament.h parcer/parcer.h
	$(CC) $(CFLAGS) -c parcer/ornament.c -o parcer/ornament.o

parcer/additional.o: parcer/additional.c  parcer/additional.h parcer/parcer.h
	$(CC) $(CFLAGS) -c parcer/additional.c -o parcer/additional.o

parcer/help.o: parcer/help.c  parcer/help.h parcer/parcer.h
	$(CC) $(CFLAGS) -c parcer/help.c -o parcer/help.o

checker/base_checker.o: checker/base_checker.c  checker/base_checker.h structures.h checker/check_functions.h
	$(CC) $(CFLAGS) -c checker/base_checker.c -o checker/base_checker.o

checker/check_functions.o: checker/check_functions.c  structures.h checker/check_functions.h checker/help_functions.h
	$(CC) $(CFLAGS) -c checker/check_functions.c -o checker/check_functions.o

checker/help_functions.o: checker/help_functions.c  structures.h checker/check_functions.h checker/help_functions.h
	$(CC) $(CFLAGS) -c checker/help_functions.c -o checker/help_functions.o

test:
	../tests.sh

clean:
	rm -f $(OBJ) $(EXEC)