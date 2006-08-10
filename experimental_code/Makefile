SRCS = dom-test-udp.cpp  ezSockets.cpp

OBJS = $(SRCS:.cpp=.o)
GXX = g++
GCC = gcc
CFLAGS = -c -Wall
LDFLAGS =

TARGET = server
TARGET_COPY = client

.cpp.o:
	$(GXX) $(CFLAGS) $< -o $@

all: target
	/bin/cp -f $(TARGET) $(TARGET_COPY)

clean:
	rm -rf *.o $(TARGET) $(TARGET_COPY)

target: $(OBJS)
	$(GXX) $(OBJS) -o $(TARGET) $(LDFLAGS)
