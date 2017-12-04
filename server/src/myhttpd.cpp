#include <myhttpd.hpp>

using namespace std;

bool serv_run = true;
    
int main(int argc, char **argv) {
    parse_args(argv, argc);
    socket_setup();
    printf("HTTP/%.1f port: %d timeout: %d\n",http, portnum, timeout);

    // main server loop!
    while (serv_run) {
        // since select may modify the sets we need to copy the master
        // into the set each iteration
        memcpy(&read_fds, &master_r, sizeof(master_r));
        memcpy(&write_fds, &master_w, sizeof(master_w));
        tv.tv_sec = 5; tv.tv_usec = 0;

        // select (timeout 5s)
        if (select(fdmax+1, &read_fds, &write_fds, NULL, &tv) < 0) {
            printf("Error selecting\n");
            exit(-1);
        }

        // something is ready to read / write (or select timed out)
        for (int i=0; i<= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) {    // got a connection 
                if (i == listen_sd) {        // connection is a new client
                    do {
                        new_sd = accept(listen_sd, NULL, NULL); 
                        if (new_sd < 0) break;

                        // this will hold all the info the server needs to
                        // continue sending data to the client
                        Responder *r = new Responder;   
                        file_handler *fh = new file_handler;
                        r->fh = fh;
                        r->id = new_sd;
                        if (http > 1.0) {
                            time(&(r->last_event));
                            pheap.push(pair<time_t,int>(r->last_event,new_sd));
                        }
                        pair<int, Responder *> p (new_sd, r);
                        Responders.insert(p);

                        FD_SET(new_sd, &master_r);
                        if (new_sd > fdmax) fdmax = new_sd;
                    } while (new_sd != -1);
                }
                else {
                    // someone is ready to read that isnt the main socket
                    close_conn = FALSE;
                    nbytes = recv(i, reqBuff, sizeof(reqBuff), MSG_DONTWAIT);
                    if (nbytes < 0) {      // unsuccessful read
                        if (errno != EWOULDBLOCK || nbytes == 0) {
                            if (FD_ISSET(i, &write_fds)) FD_CLR(i, &write_fds);
                            close_conn = TRUE;
                        }
                    }
                    else { // successful read
                        // a little hack to terminate the server from a client
                        if (!strcmp(reqBuff, "GET /STOP")) {
                            serv_run = false;
                            send(i, "STOP", 4, 0);
                            break;
                        }
                        // get the responder and send him the header
                        got = Responders.find(i);
                        if (!got->second->is_sending) {
                            string header = Handler.get_header(
                                reqBuff, http, got->second);
                            memset(reqBuff, '0', sizeof(reqBuff));

                            if (got->second->is_sending)
                                FD_SET(i, &master_w);
                            else
                                close_conn = TRUE;
                            send(i, header.c_str(), header.length(), 0);
                        }
                    }
                }
            }
            if (FD_ISSET(i, &write_fds)) {  // someone is ready to write
                // get the responder and the chunk of data to send
                got = Responders.find(i);
                string tosend = got->second->get_chunk();
                int length = got->second->fh->send_length;
                
                int rc;
                // send him a chunk of data
                rc = send(i, tosend.c_str(), length, 0);

                if (rc != length) {cout << "SEND ERROR!\n";}

                if (got->second->fh->eof) {     // finished sending
                    FD_CLR(i, &master_w);
                    if (!(http > 1.0)) close_conn = TRUE;
                }
                if (http > 1.0) {       // http1.1 - keep timeout information
                    time(&(got->second->last_event));
                    update_heap(got->second, i);
                }
            }
            if (close_conn && FD_ISSET(i, &master_r)) {
                close_socket(i);
                close_conn = FALSE;
            }
        }
        if (http > 1.0) {Responder dummy;  update_heap(&dummy, 0);}
    }
}

// closes a socket and frees some structures
void close_socket(int i) {
    got = Responders.find(i);
    if (got != Responders.end()) {
        close(i);
        FD_CLR(i, &master_r);
        if (FD_ISSET(i, &master_w)) FD_CLR(i, &master_w);
        if (i == fdmax) {
            while (FD_ISSET(fdmax, &master_r) == FALSE)
                fdmax -= 1;
        }                

        delete got->second->fh;
        delete got->second;
        Responders.erase(i);
    }
}

// this is used in HTTP1.1 to manage the timeout heap
void update_heap(Responder *r, int id) {
     if (pheap.empty()) return;

     got = Responders.find(id);
     time_t now;
     time(&now);

    pair<time_t, int> up;
    up = pheap.top();
    pheap.pop();

    // clear house --- this guy timed out
    if ((now - up.first) > timeout && pheap.empty())
        close_socket(up.second);

    // loop through, pop elements until we find someone who hasnt timedout
    while ((now - up.first) > timeout && !pheap.empty()) {
        if (up.second == id && up.first != got->second->last_event) {
            // update the heap element if its a Responder with a new time
            up.first = got->second->last_event;
            pheap.push(up);
        }
        else  
            close_socket(up.second);
        up = pheap.top();
        pheap.pop();
    }
    if ((now - up.first) <= timeout)
        pheap.push(up);
    else
        close_socket(up.second);
}

// does all the initial socket creation
void socket_setup() {
    int on = 1;
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(reqBuff, '0', sizeof(reqBuff));

    if ((listen_sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        printf("Error creating socket\n");
    if (setsockopt(listen_sd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on,
                   sizeof(on)) < 0)
        printf("Error creating reusable socket\n");
    if (fcntl(listen_sd, F_SETFL, on | O_NONBLOCK) < 0)
        printf("Error creating 'non-blocking' socket.");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(portnum);
    
    if (::bind(listen_sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
        printf("Error binding to socket\n");
    if (listen(listen_sd, portnum) < 0)
        printf("Error listening\n");

    // fd_sets for select
    FD_ZERO(&master_r);
    FD_ZERO(&master_w);
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_SET(listen_sd, &master_r);
    fdmax = listen_sd;
}

/* takes the arguments passed to the server and sets the http type (1.0/1.1),
 * the portnumber (1025-65535), and the timout value (>0)
 */
void parse_args(char **argv, int argc) {
    short stop = 0;
    // default setting
    http=1.0; portnum=1025; timeout=300;

    // parse the arguments
    // http can only be the first argument
    if (argc > 1) {
        if (isHTTP(argv[1])) {
            http = atof(argv[1]);
            stop++;
        }
        // check for the port. could be first or second
        for (int i=argc-2; i>stop; i--) {
            if (isPort(argv[i])) {
                portnum=atoi(argv[i]);
                stop++;
                break;
            }
        }
        // check for the timeout. could be first, second, or third
        for (int i=argc-1; i>stop; i--) {
            if (isTime(argv[i])) {
                timeout=atoi(argv[i]);
                break;
            }
        }
    }
}

bool isHTTP (char *http) {
    return atof(http) == 1.0 or atof(http) == 1.1;
}

bool isPort (char *port) {
    return (1024 < atoi(port)) && (atoi(port) < 65536);
}

bool isTime (char *time) {
    return atoi(time) > 0;
}
