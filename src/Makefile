SRCS = \
main.cpp gameState.cpp window.cpp \
object.cpp 

GXX = g++
GCC = gcc
CFLAGS = -c -Wall -g
LDFLAGS = -ggdb `allegro-config --libs`
			
TARGET = ninjas
			
# Dont _really_ need to mess with anything below this

OBJS = $(SRCS:.cpp=.o)

.cpp.o:
	$(GXX) $(CFLAGS) $< -o $@

all: target

clean:
	rm -rf *.o core $(TARGET) ../$(TARGET)

target: $(OBJS)
	$(GXX) $(OBJS) -o $(TARGET) $(LDFLAGS)
	cp $(TARGET) ..
