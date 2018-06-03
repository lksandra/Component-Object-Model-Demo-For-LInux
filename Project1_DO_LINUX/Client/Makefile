# MakeFile for client.cpp
# - creates shared library libFilemanager.so
# Jim Fawcett, CSE775 - Distributed Objects, Spring 2018

# all

#all: client.out

#client.o: main.cpp libFileManager.so
#	g++ -g -std=c++11 -Wall -Werror main.cpp libFileManager.so -ldl -o client.o


#this worked for loading only libFileManager.so 
#client.out: main.cpp 
#	g++ -g -std=c++11 -Wall -Werror main.cpp -ldl  -o client.out

#LD_LIBRARY_PATH=../FileManager

all: client.exe

client.exe: main.cpp libtextSearch.so libFileManager.so
	g++ -g -std=c++11 -Wall -Werror main.cpp -ldl -o client.exe


