CC = gcc
CFLAGS = $(foreach dir, $(wildcard inc/*), -I$(dir))
LDFLAGS = $(foreach dir, $(wildcard lib/*), -L$(dir)) -lm -lnsl -lz

SRCDIR = src
OBJDIR = obj
LIBDIRS = $(wildcard lib/*)
LIBSA = $(foreach dir, $(LIBDIRS), $(dir)/*.a)

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

PROGRAM = Asteria

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS) 
	$(CC) $< -o $@ $(LIBSA) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm $(PROGRAM)
	rm $(OBJECTS)

.PHONY: all clean
