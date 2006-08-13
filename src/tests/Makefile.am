# Rules for the test code (use `make check` to execute)

# all test programs to run
TESTS = vector2D assetManager rect
AM_CXXFLAGS = -I../external -I../objects -I../ -Wall $(CPPUNIT_FLAGS)
check_PROGRAMS = $(TESTS)

# rect unit tests
rect_SOURCES = 	runner.cpp ../rect.h \
										test_rect.cpp test_rect.h
rect_LDFLAGS = $(CPPUNIT_LIBS)
rect_LDADD = ../rect.o ../vector2D.o


# vector2D unit tests
vector2D_SOURCES = 	runner.cpp ../vector2D.h \
										test_vector2D.cpp test_vector2D.h
vector2D_LDFLAGS = $(CPPUNIT_LIBS)
vector2D_LDADD = ../vector2D.o

# assetManager unit tests
assetManager_SOURCES = 	runner.cpp ../assetManager.h \
										test_assetManager.cpp test_assetManager.h
assetManager_LDFLAGS = $(CPPUNIT_LIBS)
assetManager_LDADD = ../assetManager.o ../gameBase.o
