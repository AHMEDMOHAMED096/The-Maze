# The compiler and flags
CC = gcc
CFLAGS = -Iheaders
LDFLAGS = -lSDL2 -lSDL2_image -lm

# The target executable
TARGET = maze

# The source files
SRCS = src/map.c src/maze.c src/input.c src/texture.c

# The object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compile the source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up the build files
clean:
	rm -f $(OBJS) $(TARGET)
