CC = gcc
DEV = -Wall -Wextra
CFLAGS =  -std=c99
OBJ = main.o parcer/rotate.o parcer/rectangle.o parcer/parcer.o parcer/circle.o parcer/additional.o
EXEC = cw

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

main.o: main.c parcer/parcer.h
	$(CC) $(CFLAGS) -c main.c -o main.o

parcer/parcer.o: parcer/parcer.c parcer/parcer.h parcer/rotate.h parcer/rectangle.h  parcer/circle.h parcer/additional.h
	$(CC) $(CFLAGS) -c parcer/parcer.c -o parcer/parcer.o

parcer/rotate.o: parcer/rotate.c  parcer/rotate.h parcer/parcer.h
	$(CC) $(CFLAGS) -c parcer/rotate.c -o parcer/rotate.o

parcer/rectangle.o: parcer/rectangle.c  parcer/rectangle.h parcer/parcer.h
	$(CC) $(CFLAGS) -c parcer/rectangle.c -o parcer/rectangle.o

parcer/circle.o: parcer/circle.c  parcer/circle.h parcer/parcer.h
	$(CC) $(CFLAGS) -c parcer/circle.c -o parcer/circle.o

parcer/additional.o: parcer/additional.c  parcer/additional.h parcer/parcer.h
	$(CC) $(CFLAGS) -c parcer/additional.c -o parcer/additional.o

clean:
	rm -f $(OBJ) $(EXEC)