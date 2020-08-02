# A simple Makefile for compiling small SDL projects

# set the compiler
CC := clang++-9
# set the compiler flags
CFLAGS := `sdl2-config --libs --cflags`

# add header files here
HDRS :=

# add source files here
SRCS := main.cpp

# generate names of object files
OBJS := $(SRCS:.c=.o)

# name of executable
EXEC := gol

# default recipe
all: $(EXEC)

showfont: showfont.c Makefile
	$(CC) -o $@ $@.c $(CFLAGS) $(LIBS)

glfont: glfont.c Makefile
	$(CC) -o $@ $@.c $(CFLAGS) $(LIBS)

# recipe for building the final executable
$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS)

# recipe for building object files
#$(OBJS): $(@:.o=.c) $(HDRS) Makefile
#    $(CC) -o $@ $(@:.o=.c) -c $(CFLAGS)

.PHONY: all 
