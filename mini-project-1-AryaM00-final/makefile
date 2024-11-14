# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Libraries to link against (adjust if necessary)
LIBS = -lm

# Output executable name
OUTPUT = myprogram

# Source files
SRCS = $(wildcard *.c)

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(OUTPUT)

# Rule to link the object files and create the executable
$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Rule to compile each source file into an object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and the executable
clean:
	rm -f $(OBJS) $(OUTPUT)