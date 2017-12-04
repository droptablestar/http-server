#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <iostream>
#include <fstream>
#include <string>

const short FNOTOPEN = -1;
const short FCRASH = -2;
const short UNSUP = -3;

const short JPG = 1;
const short GIF = 2;
const short TEXT = 3;

struct file_handler {
    short error_code, type;
    std::ifstream fstream;
    unsigned int length;
    unsigned short send_length;
    bool eof;
    std::string name;
    
    void get_ftype(std::string &);
    void open_file(const char *);
    std::string read_file(unsigned long *, short);
    short get_error_code();
    void get_length(const char *);
    
  file_handler():error_code(0), eof(false) {}
};


#endif
