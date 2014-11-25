CC = gcc
CFLAGS = -W -Werror 
LIBS = -lpthread
EXEC = main

all: $(EXEC)

main : main.o Supplier.o Step.o Structures.h
	$(CC) $(CFLAGS) -o $@  $^ $(LIBS) 

main.o : main.c Supplier.h
	$(CC) $(CFLAGS) -c $<

Supplier.o : Supplier.c Supplier.h
	$(CC) $(CFLAGS) -c $<
	
Step.o : Step.c Step.h
	$(CC) $(CFLAGS) -c $<

clean : 
	rm *~
	rm *.o
