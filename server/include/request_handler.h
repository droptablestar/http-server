#ifndef REQ_HANDLER_H
#define REQ_HANDLER_H

#include <string>
#include <Responder.h>

class request_handler {
  public:
    int code, length;

    void set_code(std::string &);
    std::string format_header(float, Responder *);
    int get_file_length(std::string &, Responder *);
    std::string get_header(std::string, float, Responder *);
    std::string get_error_page(Responder *);
};


#endif
