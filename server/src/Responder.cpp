#include <Responder.h>

// this was a poor design decision
std::string Responder::get_chunk() {
    return fh->read_file(&offset, SENDSIZE);
}
