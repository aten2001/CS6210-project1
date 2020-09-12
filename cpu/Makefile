CC = gcc				# default is CC = cc
CFLAGS = -I -Wall -g -lvirt	# default is CFLAGS = [blank]
CPPFLAGS =				# default is CPPFLAGS = [blank]
OBJ = vcpu_scheduler.o

vcpu_scheduler: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) 

clean:
	$(RM) -f *.o vcpu_scheduler	