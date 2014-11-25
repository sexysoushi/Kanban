CC = gcc
CFLAGS = -W -Werror 
LIBS = -lpthread
EXEC = main

all: $(EXEC)

main : main.o Workshop.o Structures.h
	$(CC) $(CFLAGS) -o $@  $^ $(LIBS) 

main.o : main.c Structures.h
	$(CC) $(CFLAGS) -c $<

Workshop.o : Workshop.c Workshop.h
	$(CC) $(CFLAGS) -c $<
	
#Step.o : Step.c Step.h
	#$(CC) $(CFLAGS) -c $<

clean : 
	rm *~
	rm *.o
