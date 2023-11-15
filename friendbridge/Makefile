CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS =

# Set DEBUG=1 for debugging, DEBUG=0 for release
DEBUG = 1
ifeq ($(DEBUG), 1)
	CFLAGS += -DDEBUG
else
	CFLAGS += -DNDEBUG
endif

# List all .c files
SRCS = $(wildcard *.c)

# Generate corresponding .o filenames
OBJS = $(SRCS:.c=.o)

# Output binary name
TARGET = friendbridge

all: compile

compile: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

release: DEBUG = 0
release: clean compile

.PHONY: all clean compile release

