#ifndef MYHTTP_HPP
#define MYHTTP_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <ctime>
#include <fcntl.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>

#include <Responder.h>
#include <request_handler.h>
#include <file_handler.h>

#include <unordered_map>

int listen_sd, new_sd, portnum, timeout, fdmax, nbytes, close_conn;
float http;
struct sockaddr_in serv_addr;
char reqBuff[1024];
fd_set master_r, master_w, read_fds, write_fds;
struct timeval tv;
request_handler Handler;
std::unordered_map<int,Responder *> Responders;
std::unordered_map<int,Responder *>::const_iterator got;

void parse_args(char **, int);
bool isHTTP(char *);
bool isPort(char *);
bool isTime (char *);
void socket_setup();
int parse_request(std::string);
void update_heap(Responder *, int);
void close_socket(int);

struct compare {
    bool operator() (const std::pair<time_t, int> &p1,
                     const std::pair<time_t, int> &p2 ) const {
        if(p1.first > p2.first) return true;
        else return false;
        
    }
};

std::priority_queue<std::pair<time_t, int>,
                    std::vector<std::pair<time_t, int> >, compare > pheap;


#define TRUE  1
#define FALSE 0

#endif
