CC = gcc
CFLAGS = -W -Werror -g
LIBS = -lpthread
EXEC = main

all: $(EXEC)

main : main.o Workshop.o Initialize.o List.o Structures.h
	$(CC) $(CFLAGS) -o $@  $^ $(LIBS) 

main.o : main.c List.h Structures.h
	$(CC) $(CFLAGS) -c $<

Initialize.o : Initialize.c Initialize.h
	$(CC) $(CFLAGS) -c $<

Workshop.o : Workshop.c Workshop.h
	$(CC) $(CFLAGS) -c $<
	
List.o : List.c List.h
	$(CC) $(CFLAGS) -c $<

clean : 
	rm *.o
	rm *~
