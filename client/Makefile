CPP=g++
INC=include
FILES = src/Client.cpp
FLAGS=-pthread -std=c++0x -Wall 

all:
	$(CPP) $(FLAGS) -I$(INC) $(FILES) -o client

debug:
	$(CPP) $(FLAGS) -g -ggdb -I$(INC) $(FILES) -o myhttpd
