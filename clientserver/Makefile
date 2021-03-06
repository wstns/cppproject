#
# Makefile to make the file libclientserver.a, containing
# connection.o and server.o
#
# Define the compiler. g++ can be
# changed to clang++.
CXX = g++
CC  = g++

# Define preprocessor, compiler, and linker flags. Uncomment the # lines
# if you use clang++ and wish to use libc++ instead of libstdc++.
CPPFLAGS =  -std=c++11
CXXFLAGS =  -O2 -Wall -Wextra -pedantic-errors -Wold-style-cast
CXXFLAGS += -std=c++11
CXXFLAGS += -g
#CXXFLAGS =  -stdlib=libc++
#CPPFLAGS =  -stdlib=libc++
#CXXFLAGS += -stdlib=libc++

TARGET  = testfiledatabase
SOURCES = $(shell echo src/*.cc)
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET) libclientserver.a

# Create the library; ranlib is for Darwin (OS X) and maybe other systems.
# Doesn't seem to do any damage on other systems.

$(TARGET): $(OBJECTS)

libclientserver.a: article.o connection.o database.o filesystemdatabase.o inmemorydatabase.o messagehandler.o newsgroup.o server.o inmemoryserver.o filesystemserver.o
	ar rv libclientserver.a  article.o connection.o database.o filesystemdatabase.o inmemorydatabase.o messagehandler.o newsgroup.o server.o
	ranlib libclientserver.a

# Phony targets
.PHONY: all clean

# Standard clean
clean:
	rm -f *.o libclientserver.a

install:
	cp ./src/filesystemserver ./bin
	cp ./src/inmemoryserver ./bin
	cp ./src/client ./bin

# Generate dependencies in *.d files
%.d: %.cc
	@set -e; rm -f $@; \
         $(CPP) -MM $(CPPFLAGS) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

# Include the *.d files
SRC = $(wildcard *.cc)
include $(SRC:.cc=.d)
