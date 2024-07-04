CXX = gcc
CXXFLAGS = -Iinc $(foreach dir, $(wildcard lib/*), -I$(dir))
LDFLAGS = $(foreach dir, $(wildcard lib/*), -L$(dir)) -lm -lnsl -lz

SRCDIR = src
OBJDIR = obj
LIBDIRS = $(wildcard lib/*)
LIBS = $(foreach dir, $(LIBDIRS), $(dir)/*.a)

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

PROGRAM = Asteria

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS) $(LIBS)
	$(CC) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) -c -o $@ $< $(CXXFLAGS)

clean:
	rm -rf $(OBJDIR) $(PROGRAM)

.PHONY: all clean

