SRCS = network.cpp main.cpp client.cpp server.cpp

OBJS = $(SRCS:.cpp=.o)
GXX = g++
GCC = gcc
CFLAGS = -c -Wall
LDFLAGS =

TARGET = net

.cpp.o:
	$(GXX) $(CFLAGS) $< -o $@

all: target

clean:
	rm -rf *.o $(TARGET)

target: $(OBJS)
	$(GXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

test: target
	./$(TARGET)
