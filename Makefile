
TARGET = mcinit

OBJS = mcinit.o

CC = gcc

CFLAGS = -c -g

LDFLAGS = -lcurl -ljson-c

$(TARGET) : $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LIBS)

mcinit.o : mcinit.c
	$(CC) $(CFLAGS) $<
