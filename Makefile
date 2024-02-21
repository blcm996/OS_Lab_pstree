# Makefile for OS_LAB_PSTREE project

# Compiler and flags
CC = g++
CFLAGS = -O2 -Wall -I.

# Source files
SRCS = getpid.cpp pidinfo.cpp prase.cpp treePrint.cpp pstree.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Target binary
TARGET = Pstree

# Default target
all: $(TARGET)

# Linking rule
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compilation rules
%.o: %.cpp
	$(CC) $(CFLAGS) -g -c $<

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)
