#ifndef RESPONDER_H
#define RESPONDER_H

#include <string>
#include <file_handler.h>
#include <time.h>

const short SENDSIZE = 512;

struct Responder {
    unsigned long offset;
    int id;
    bool is_sending;
    file_handler *fh;
    time_t last_event;
    
    std::string get_chunk();
    
  Responder():offset(0),id(0),is_sending(false),last_event(0) {}
};

#endif
