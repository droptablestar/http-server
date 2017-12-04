#include <file_handler.h>
#include <string.h>

using namespace std;

// gets the type of the file (jpf, gif, html, txt)
void file_handler::get_ftype(string &fname) {
    string ext = fname.substr(fname.find_last_of(".")+1);

    if (!ext.compare("jpeg") || !ext.compare("jpg") ||
        !ext.compare("JPEG") || !ext.compare("JPG"))
        type = JPG;
    else if (!ext.compare("gif") || !ext.compare("GIF"))
        type = GIF;
    else if (!ext.compare("html") || !ext.compare("htm") ||
             !ext.compare("txt"))
        type = TEXT;
    else
        type = UNSUP;
}

// sets the length in the file_handler struct
void file_handler::get_length(const char *fname) {
    open_file(fname);
    length = fstream.tellg();
    fstream.seekg(0, ios::beg);
}

// opens the file and sets an error code on failure
void file_handler::open_file(const char *fname) {
    // cout << "fname: [" << fname << "]" << endl;
    name = fname;
    get_ftype(name);
    
    if (type == UNSUP)
        error_code = UNSUP;

    if (type == TEXT) 
        fstream.open(fname, ios::in|ios::ate);
    else 
        fstream.open(fname, ios::in|ios::binary|ios::ate);

    if (!fstream.is_open()) 
        error_code = FNOTOPEN;
    
    eof = false;
}

// reads a chunk of a file performing checks to make sure too much isnt read
string file_handler::read_file(unsigned long *offset, short amount) {
    
    if ((*offset)+amount > length) {  // this read will overflow the file
        send_length = length - (*offset);
        eof = true;
    }
    else 
        send_length = amount;

    string str; str.reserve(send_length);
    
    fstream.read(&str[0], send_length);

    if (fstream.gcount() != send_length) {cout << "READ ERROR!\n"; exit(-1);}
    if ((*offset)+amount > length) {(*offset) = 0; fstream.close();}
    else (*offset) += amount;

    return str;
}

short file_handler::get_error_code() {
    short tmp = error_code;
    error_code = 0;
    return tmp;
}
