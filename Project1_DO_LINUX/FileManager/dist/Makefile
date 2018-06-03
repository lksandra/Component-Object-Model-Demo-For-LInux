# MakeFile for Component.cpp
# - creates shared library libComponent.so
# Jim Fawcett, CSE775 - Distributed Objects, Spring 2018

# all
all: libComponent.so

libComponent.so: libComponent.o
	g++ -shared libComponent.o -o libComponent.so

libComponent.o: Component.cpp
	g++ -c -std=c++11 -Wall -Werror -fpic Component.cpp -o libComponent.o 


