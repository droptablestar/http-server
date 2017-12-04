#ifndef RESPONSES_HPP
#define RESPONSES_HPP

#include <string>

std::string ERR400 = "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">"
    "<html><head>"
    "<title>400 Bad Request</title>"
    "</head><body>"
    "<h1>Bad Request</h1>"
    "<p>"
    "Your browser sent a request that this server could not understand.<br />"
    "</p>"
    "<hr>"
    "<address>Apache/2.2.11 (Unix) mod_ssl/2.2.11 OpenSSL/0.9.8r Server at www.cs.purdue.edu Port 80</address>"
    "</body></html>";

std::string header = "HTTP/{0} {1} {5}\r\n"
    "Date: {2}"
    "Server: {3}\r\n"
    "Last-Modified: Tue, 30 Oct 2007 17:00:02 GMT\r\n"
    "Accept-Ranges: bytes\r\n"
    "Content-Length: {4}\r\n"
    "Keep-Alive: timeout=10, max=100\r\n"
    "Connection: Keep-Alive\r\n"
    "Content-Type: {6}\r\n\r\n";

std::string PG0 = "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\r\n"
    "<html><head>\r\n"
    "<title>Testing response number 1</title>\r\n"
    "</head><body>\r\n"
    "<h1>Successful connection</h1>\r\n"
    "<p>\r\n"
    "Your browser is fine..<br />\r\n"
    "</p>\r\n"
    "<hr>\r\n"
    "<address>Cherokee/1.0.0 (Unix) mod_ssl/2.2.11 OpenSSL/0.9.8r Server at localhost Port Something</address>\r\n"
    "</body></html>\r\n";



#endif
