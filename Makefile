#Copyright 2021 Shadai Rafael Lopez Garcia
CC=gcc
CFLAGS= -Wall -Werror -I ./
TARGET = multithreadedworkqueue
OBJ = sender.o \
      main.o
LIBS = -lcurl

all:$(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm *.o
	rm $(TARGET)

