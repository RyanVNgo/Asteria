CXX = gcc
CXXFLAGS = -Iinc $(foreach dir, $(wildcard lib/*), -I$(dir)/include)
LDFLAGS = $(foreach dir, $(wildcard lib/*), -L$(dir)) -lm -lnsl -lz

SRCDIR = src
OBJDIR = obj
LIBDIRS = $(wildcard lib/*)
LIBS = $(foreach dir, $(LIBDIRS), $(dir)/*.a)

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(wildcard $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

PROGRAM = Asteria

# CFITSIO = -L./lib -4.4.1 -Wl,-rpath,./inc -4.4.1 -lcfitsio -lm
all: $(PROGRAM)

$(PROGRAM): $(OBJECTS) $(LIBS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf $(OBJDIR) $(PROGRAM)

.PHONY: all clean

