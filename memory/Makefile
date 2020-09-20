CC = gcc								# default is CC = cc
CFLAGS = -I -Wall -g -lvirt -lm -ldl	# default is CFLAGS = [blank]
CPPFLAGS =								# default is CPPFLAGS = [blank]
OBJ = memory_coordinator.o

memory_coordinator: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) 

clean:
	$(RM) -f *.o memory_coordinator	