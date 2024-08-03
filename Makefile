CC = gcc
CFLAGS = $(foreach dir, $(wildcard inc/*), -I$(dir)) `pkg-config --cflags --libs gtk+-3.0`
LDFLAGS = $(foreach dir, $(wildcard lib/*), -L$(dir)) `pkg-config --cflags --libs gtk+-3.0` -lm

SRCDIR = src
OBJDIR = obj

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

LIBDIRS = $(wildcard lib/*)
LIBSA = $(foreach dir, $(LIBDIRS), $(dir)/*.a)

PROGRAM = Asteria

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS) 
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJECTS)
	rm $(PROGRAM)

.PHONY: all clean
