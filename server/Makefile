CPP=g++
INC=include
FILES = src/myhttpd.cpp src/request_handler.cpp src/file_handler.cpp \
	src/Responder.cpp
FLAGS=-std=c++0x -Wall

all:
	$(CPP) $(FLAGS) -I$(INC) $(FILES) -o myhttpd

debug:
	$(CPP) $(FLAGS) -g -ggdb -I$(INC) $(FILES) -o myhttpd
