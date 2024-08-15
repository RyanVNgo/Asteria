CC = gcc
CFLAGS = $(foreach dir, $(wildcard inc/*), -I$(dir)) `pkg-config --cflags --libs gtk+-3.0`

#local path from makefile to cfitsio library
CFITSIO_LIBPATH = lib/cfitsio-4.4.1
LDFLAGS = -Wl,-rpath,$(CFITSIO_LIBPATH)
LDFLAGS += $(foreach dir, $(wildcard lib/*), -L$(dir)) `pkg-config --cflags --libs gtk+-3.0` -lcfitsio -lm

SRC_DIR = src
UI_DIR = $(SRC_DIR)/ui
HELPERS_DIR = $(SRC_DIR)/helpers

UI_SRCS = $(wildcard $(UI_DIR)/*.c)
HELPERS_SRCS = $(wildcard $(HELPERS_DIR)/*.c)
SOURCES = $(wildcard $(SRC_DIR)/*.c) $(HELPERS_SRCS) $(UI_SRCS)

OBJ_DIR = obj
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

LIB_DIRS = $(wildcard lib/*)
LIBS_A = $(foreach dir, $(LIB_DIRS), $(dir)/*.a)

PROGRAM = Asteria

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS) 
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJECTS)
	rm $(PROGRAM)

.PHONY: all clean
