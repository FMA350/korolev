CC        := gcc
LL        := gcc
CFLAGS    := -c
# -include 'signal.h' -include 'stdlib.h' -include 'pthread.h' -include 'limits.h' -include 'stdio.h' -include 'unistd.h'
LFLAGS    := -lm -lpthread

EXECPATH  := builds
SRCPATH   := src
STRUCTURE := $(shell find $(SRCPATH) -type d)

SRCFILES  := $(addsuffix /* , $(STRUCTURE))
SRCFILES  := $(wildcard $(SRCFILES))

CFILES 	  := $(filter %.c,$(SRCFILES))
HDRFILES  := $(filter %.h,$(SRCFILES))
OBJFILES  := $(CFILES:%.c=%.o)
OBJFILES  := $(filter-out src/init.o, $(OBJFILES))

# SOURCES  = $(addprefix $(SRCPATH), $(CFILES))
# OBJECTS  = $(SOURCES:.c=.o)

EXECUTABLE = $(EXECPATH)/korolev

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJFILES)
	$(LL) $(LFLAGS) $(OBJFILES) -o $@

%.o:%.c
	$(CC) $(CFLAGS) $< -o $@
# .c.o:
# 	$(CC) $(CFLAGS) $< -o $@

.PHONY : clean
clean:
	rm $(OBJFILES)
