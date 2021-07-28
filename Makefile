#Copyright 2021 Jason Yair Corona Ventura
CC=gcc
CFLAGS= -I ./
TARGET = MultipleSocketHandling
OBJ = sender.o \
      MultipleSocketHandling.o
LIBS = -lcurl -lpthread

all:$(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm *.o
	rm $(TARGET)
