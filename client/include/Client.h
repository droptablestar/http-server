#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <iomanip>
#include <sys/socket.h>
#include <sys/select.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <regex.h>
#include <sys/time.h>
#include <fstream>
#include <sstream>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

// procedures
void *run(void *);
double socket_setup(const char *, const char *, int *);

struct thread_params {
    unsigned short id;
    std::string serv_addr;
    std::string port;
    std::string request;
    std::string hn;
};

#endif
