CC=gcc
CFLAGS=-g -wall
LDFLAGS=-pthread
OBJS=ECU_main.o FileManager.o Engine.o
TARGET=makefileapp.out

$(TARGET):$(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

ECU_main.o:ECU_main.c
	@echo ECU_main.o
	gcc -c -o ECU_main.o ECU_main.c
Engine.o: Engine.h Engine.c
	gcc -c -o Engine.o Engine.c
FileManager.o: FileManager.h FileManager.c
	gcc -c -o FileManager.o FileManager.c

clean: 
	rm -f *.o
	rm -f $(TARGET)

