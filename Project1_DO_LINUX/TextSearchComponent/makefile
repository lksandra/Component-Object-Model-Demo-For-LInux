#this make file is for textSearchComponent and builds a .so

all: libtextSearch.so

FileSystem.o: FileSystem.cpp FileSystem.h 
	g++ -c -std=c++11  -fpic FileSystem.cpp 

textSearch.o : textSearch.cpp TextSearch.h ITextSearch.h IUnknown.h FileSystem.h FileSystem.cpp 
	g++ -std=c++11 -c -fpic textSearch.cpp

libtextSearch.so:  textSearch.o FileSystem.o
	g++ -std=c++11 -shared textSearch.o FileSystem.o -o libtextSearch.so
