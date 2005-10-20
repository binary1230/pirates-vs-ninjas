# makefile for linux/*nix
include Makefile.common

GXX = g++
GCC = gcc
CFLAGS = -c -Wall -g
LDFLAGS = -ggdb `allegro-config --libs` -lm
			
TARGET = ninjas
			
# Dont _really_ need to mess with anything below this

OBJS = $(SRCS:.cpp=.o)

.cpp.o:
	$(GXX) $(CFLAGS) $< -o $@


all: target

target: $(OBJS)
	$(GXX) $(OBJS) -o $(TARGET) $(LDFLAGS)
	cp $(TARGET) ..
	
doc: 
	doxygen
	
test: target
	cd .. && ./$(TARGET)

clean:
	rm -rf *.o core $(TARGET) ../$(TARGET)
