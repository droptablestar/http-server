#include <request_handler.h>
#include <sstream>
#include <responses.hpp>
#include <string.h>
#include <header_data.hpp>

#include <stdio.h>
#include <iostream>

using namespace std;

// main method call. does all the things required to produce an HTTP header
string request_handler::get_header(string request, float http, Responder *r) {
    set_code(request);
    length = get_file_length(request, r);

    string resp;
    if (code != 200) {   // this request failed
        r->is_sending = false;
        string page = get_error_page(r);
        length = page.length();
        resp = format_header(http, r) + page;
    }
    else {
        r->is_sending = true;
        resp = format_header(http, r);
    }

    return resp;
}

// this finds the error page associated with whatever went wrong
string request_handler::get_error_page(Responder *r) {
    unsigned long offset = 0;
    string page="";

    short error = r->fh->error_code;
    if (error == FNOTOPEN || error == FCRASH || error == UNSUP) {
        return "";
    }
    switch (code) {
        case 400:
            r->fh->get_length("www/errors/400.html");
            break;
        case 404:
            r->fh->get_length("www/errors/404.html");
            break;
    }
    page = r->fh->read_file(&offset, r->fh->length);

    return page.c_str();
}

// checks to make sure the request is a GET
void request_handler::set_code(string &request) {
    size_t start = request.find(" ");
    string command = request.substr(0, start);
    // cout << "{" << command << "}" << endl;
    if (!command.compare("GET"))
        code = 200;
    else {
        if (!command.compare("HEAD") || !command.compare("POST") ||
            !command.compare("PUT") || !command.compare("DELETE") ||
            !command.compare("TRACE") || !command.compare("CONNECT"))
            code = 501;
        else code = 400;
    }
}

// gets the length of the file
int request_handler::get_file_length(string &request, Responder *r) {
    if (code == 200)  {
        size_t start = request.find(" ");
        size_t end = request.find(" ", start+1);
        string fname = request.substr(start+1, end-(start+1));
        fname = "./www" + fname;
        if (!fname.compare("./www/")) fname = "./www/index.html";

        // response = File_Handler.open_file(fname.c_str());
        r->fh->get_length(fname.c_str());
        int err = r->fh->get_error_code();

        switch (err) {
            case UNSUP:
                code = 400;
                break;
            case FNOTOPEN:
                code = 404;
                break;
            case FCRASH:
                code = 500;
                break;
        }
    }
    return r->fh->length;
}

// creates the HTTP header to be sent
string request_handler::format_header(float http, Responder *r) {
    string resp = header;
    if (http == 1.0)
        resp.replace(resp.find("{0}"), 3, "1.0");
    else
        resp.replace(resp.find("{0}"), 3, "1.1");
        
    stringstream tmp;
    tmp << code;
    resp.replace(resp.find("{1}"), 3, tmp.str());
    tmp.str("");

    switch (code) {
        case 200:
            resp.replace(resp.find("{5}"), 3, "OK");
            break;
        case 400:
            resp.replace(resp.find("{5}"), 3, "Bad Request");
            break;
        case 404:
            resp.replace(resp.find("{5}"), 3, "Not Found");
            break;
        case 500:
            resp.replace(resp.find("{5}"), 3, "Internal Server Error");
            break;
        case 501:
            resp.replace(resp.find("{5}"), 3, "Not implemented");
            break;
    }
    time_t t = time(0);
    struct tm *now = localtime(&t);

    resp.replace(resp.find("{2}"), 3, asctime(now));

    string server = "Cherokee/1.0.0 (";
    server += OS;
    server += ")";
    resp.replace(resp.find("{3}"), 3, server);

    tmp << length;
    resp.replace(resp.find("{4}"), 3, tmp.str());

    short type = r->fh->type;
    switch (type) {
        case TEXT:
            resp.replace(resp.find("{6}"), 3, "text/html");
            break;
        case JPG:
            resp.replace(resp.find("{6}"), 3, "image/jpeg");
            break;
        case GIF:
            resp.replace(resp.find("{6}"), 3, "image/gif");
            break;
        case UNSUP:
            resp.replace(resp.find("{6}"), 3, "Unknown");
            break;
            
    }

    return resp;
}
