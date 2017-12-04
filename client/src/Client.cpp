/*
 * output file: connect_time,transfer_time,file_size\n
 */

#include <Client.h>

using namespace std;

// size of the receive buffer. should match or exceed that of the server's
// send buffer size
const unsigned short BUFSIZE = 1024;

int portnum;
string server_address;
string send_data;
string header = "GET /{0}";

pthread_mutex_t file_mutex;

int main(int argc, char *argv[]) {
    pthread_mutex_init(&file_mutex, NULL);
    
    string req = "";
    if (argc < 5 || argc > 6) {
        printf("Usage: ./client <number_of_threads> <host> <port> "
               "<host_name> [file_request]\n");
        exit(-1);
    }
    if (argc == 6)
        req = argv[5];
    req = header.replace(header.find("{0}"), 3, req);

    int numthreads = atoi(argv[1]);
    pthread_t threads[numthreads];
    struct thread_params *params[numthreads];

    for (int i=0; i<numthreads; i++) {
        struct thread_params *th_p = new thread_params;
        th_p->id = i;
        th_p->serv_addr = argv[2];
        th_p->port = argv[3];
        th_p->request = req;
        th_p->hn = argv[4];
        th_p->hn += ".txt";
        
        params[i] = th_p;
        pthread_create(&threads[i], NULL, run, (void *)th_p);
        sleep(.01);
    }

    for (int i=0; i<numthreads; i++)
        pthread_join(threads[i], NULL);

    for (int i=0; i<numthreads; i++)
        delete params[i];
}

void *run(void *arg) {
    fd_set master, read_fds;
    char recvBuf[BUFSIZE];
    int send_sd, fdmax;

    stringstream s;

    struct thread_params *th_p = (struct thread_params *)arg;

    string tosend = th_p->request;
    double connect_t = socket_setup((th_p->serv_addr).c_str(),
                                    (th_p->port).c_str(), &send_sd);
    if (connect_t < 0) {
        cout << "Connect fail\n";
        close(send_sd);
        return (void *)th_p;
    }

    ofstream out("../mininet/"+th_p->hn, ios::out|ios::app);
    if (!out.is_open()) {
        cout << "FILE ERROR " << th_p->hn << "_" << th_p->id << endl;
        close(send_sd);
        return (void *)th_p;
    }
    s.unsetf(ios::floatfield);
    s.setf(ios::fixed);
    s.precision(3);
    s.setf(ios::showpoint);
    s << connect_t << ","; 

    FD_ZERO(&master);
    FD_SET(send_sd, &master);
    fdmax = send_sd;

    send(send_sd, tosend.c_str(), tosend.length()+1, 0);
    string reply = "";

    double resp_t = 0.0;
    struct timeval start, end;
    int nbytes;
    do {
        nbytes = 1;
        memcpy(&read_fds, &master, sizeof(master));

        gettimeofday(&start, NULL);
        select(fdmax+1, &read_fds, NULL, NULL, 0);
        if (FD_ISSET(send_sd, &read_fds)) {
            nbytes = recv(send_sd, recvBuf, sizeof(recvBuf), MSG_DONTWAIT);

            if (nbytes < 0) {
                if (nbytes != EWOULDBLOCK || nbytes == 0)
                    break;
            }
            else {
                gettimeofday(&end, NULL);
                double t1=(start.tv_sec*1000)+(start.tv_usec/1000.0);
                double t2=(end.tv_sec*1000)+(end.tv_usec/1000.0);
                resp_t += (t2-t1);
                if (nbytes != 0) {
                    reply += recvBuf;
                    bzero(recvBuf, sizeof(recvBuf));
                }
            }
        }
    } while (nbytes > 0);
    s << resp_t << "," << reply.length() << endl;
    if (reply.compare("STOP")) {
        pthread_mutex_lock(&file_mutex);
        
        out << s.str();
        out.flush();
        pthread_mutex_unlock(&file_mutex);
    }
    out.close();
    close(send_sd);

    return (void *)th_p;
}

double socket_setup(const char *addr, const char *port, int *send_sd) {
    struct addrinfo hints;
    struct addrinfo *results, *rp;
    struct timeval start, end;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;
    int s = getaddrinfo(addr, port, &hints, &results);
    if (s != 0) {
        cout << "Error getting address\n";
        exit(-1);
    }
    for (rp = results; rp != NULL; rp = rp->ai_next) {
        (*send_sd) = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if ((*send_sd) == -1) continue;
        gettimeofday(&start, NULL);
        if (connect((*send_sd), rp->ai_addr, rp->ai_addrlen) != -1) break;
        
        close((*send_sd));
    }
    if (rp != NULL) {
        gettimeofday(&end, NULL);
        double t1=(start.tv_sec*1000)+(start.tv_usec/1000.0);
        double t2=(end.tv_sec*1000)+(end.tv_usec/1000.0);
        return t2-t1;
    }
    else {printf("No connection made.\n"); return -1;}

}

