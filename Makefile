CC = gcc
LL = gcc
CFLAGS = -c -include 'signal.h' -include 'stdlib.h' -include 'pthread.h' -include 'limits.h' -include 'stdio.h' -include 'unistd.h' -I $(SRCPATH) -I .
LFLAGS = -lm -lpthread

EXECPATH = builds/
SRCPATH  = src/

CFILES   = main.c error.c commands.c orbitalFunctions.c various.c data.c \
 		   simulation.c monitor.c tlist.c suspend.c matmath.c
SOURCES  = $(addprefix $(SRCPATH), $(CFILES))
OBJECTS  = $(SOURCES:.c=.o)

EXECUTABLE = $(EXECPATH)korolev




all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(LL) $(LFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(SRCPATH)*.o
