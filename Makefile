CC = gcc
CFLAGS = -Wall -Wextra 
TARGET = task8
SRCS = main.c
OBJS = $(SRCS:.c=.o)
LIB_NAME = s21_matrix.a
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIN_NAME)
.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET) 
