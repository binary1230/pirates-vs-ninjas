SRCS = \
main.cpp gameState.cpp window.cpp \
objectFactory.cpp \
object.cpp objectRadiusBlock.cpp objectMouseBlock.cpp

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

test: target
	./$(TARGET)

clean:
	rm -rf *.o core $(TARGET) ../$(TARGET)

target: $(OBJS)
	$(GXX) $(OBJS) -o $(TARGET) $(LDFLAGS)
	cp $(TARGET) ..
