# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
LDFLAGS = 

# Target executable name
TARGET = elevate

# Source files and Object files
SRCS = elevate.c brute.c dp.c memoize.c recurse.c
OBJS = $(SRCS:.c=.o)
DEPS = elevate.h

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
