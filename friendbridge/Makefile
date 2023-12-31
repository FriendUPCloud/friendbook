CC = gcc

CFLAGS = -Wall -Wextra -g $(shell pkg-config --cflags x11)
LDFLAGS = $(shell pkg-config --libs x11)

# Set DEBUG=1 for debugging, DEBUG=0 for release
DEBUG = 1
ifeq ($(DEBUG), 1)
	CFLAGS += -DDEBUG
else
	CFLAGS += -DNDEBUG
endif

# List all .c files
SRCS = $(wildcard *.c)

# List all .c files in the communication/ directory
SRCS += $(wildcard communication/*.c)

# List all .c files in the windowmanagement/ directory
SRCS += $(wildcard windows/*.c)

# Generate corresponding .o filenames
OBJS = $(SRCS:.c=.o)

# Output binary name
TARGET = friendbridge

all: compile

compile: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)  # Include $(LDFLAGS) here

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

release: DEBUG = 0
release: clean compile

.PHONY: all clean compile release
