# MakeFile for Filemanager.cpp
# - creates shared library libFilemanager.so
# Jim Fawcett, CSE775 - Distributed Objects, Spring 2018

# all





all: libFileManager.so FileSystem.o FileManager.o



FileSystem.o: FileSystem.cpp FileSystem.h 
	g++ -c -std=c++11  -fpic FileSystem.cpp 


FileManager.o: FileManager.cpp FileManager.h FileSystem.h IFileManager.h IUnknown.h
	g++ -c -std=c++11  -fpic FileManager.cpp 

libFileManager.so: FileManager.o FileSystem.o
	g++ -shared FileManager.o FileSystem.o -o libFileManager.so




