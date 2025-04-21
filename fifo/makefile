CC = gcc
CFLAGS = -Wall -Wextra -g -Wno-unused-parameter -Wno-unused-variable -Wno-parentheses
LDFLAGS = -lncurses

SRCS = modulo2.c lista.c swap.c
OBJS = $(SRCS:.c=.o)
TARGET = prueba

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
