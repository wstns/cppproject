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
CPPFLAGS =  -std=c++11 -I..
CXXFLAGS =  -O2 -Wall -Wextra -pedantic-errors -Wold-style-cast
CXXFLAGS += -std=c++11
CXXFLAGS += -g
#LDFLAGS   = -g -L..
#CXXFLAGS =  -stdlib=libc++
#CPPFLAGS =  -stdlib=libc++
#CXXFLAGS += -stdlib=libc++

VPATH = src
#SOURCES = $(shell echo src/*.cc)
#OBJECTS = $(SOURCES:.cc=.o)
#Libraries
#LDLIBS = -lclientserver

# Targets
PROGS = inmemoryserver filesystemserver client

all:  $(PROGS)
#libclientserver.a
# Create the library; ranlib is for Darwin (OS X) and maybe other systems.
# Doesn't seem to do any damage on other systems.

# Targets rely on implicit rules for compiling and linking
# The dependency on libclientserver.a is not defined.

#libclientserver.a: $(OBJECTS)	
#	ar rv libclientserver.a  $(OBJECTS)
#	ranlib libclientserver.a

inmemoryserver: inmemoryserver.o article.o messagehandler.o connection.o server.o inmemorydatabase.o newsgroup.o filesystemdatabase.o
filesystemserver: filesystemserver.o article.o server.o connection.o database.o filesystemdatabase.o messagehandler.o newsgroup.o inmemorydatabase.o
client: client.o article.o server.o connection.o database.o messagehandler.o filesystemdatabase.o inmemorydatabase.o newsgroup.o



# Phony targets
.PHONY: all clean

# Standard clean
clean:
	rm -f *.o

install:
	cp ./filesystemserver ./bin
	cp ./inmemoryserver ./bin
	cp ./client ./bin
	rm -f filesystemserver inmemoryserver client

# Generate dependencies in *.d files
%.d: %.cc
	@set -e; rm -f $@; \
         $(CPP) -MM $(CPPFLAGS) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

# Include the *.d files
SRC = $(wildcard src/*.cc)
include $(SRC:.cc=.d)
