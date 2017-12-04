from mininet.net import Mininet
from mininet.link import TCLink
from top1 import MyTopo
import sys, traceback
import threading
import time

if len(sys.argv) < 6 or len(sys.argv) > 8:
    print 'Usage: python test.py <http> <port> <timeout> ' \
        '<num_client_threads> <number_clients> <link_speed> [file_request]'
    exit()
top = MyTopo(int(sys.argv[5]), sys.argv[6])
net = Mininet(topo=top, link=TCLink)
net.start()

def main():
    try:
        req = ''
        if len(sys.argv) == 8:
            req = sys.argv[7]
        server = net.hosts[0]
        IP = server.IP()
        num = sys.argv[4]
        http = sys.argv[1]
        port = sys.argv[2]
        timeout = sys.argv[3]
        for i in range(5):
            threads = []
            threads.append(threading.Thread(target=server_s,
                                            args=[http,port,timeout]))
            threads[-1].start()

            for c in net.hosts[1:]:
                threads.append(threading.Thread(target=client,
                                            args=[c,num,IP,port,req]))
                threads[-1].start()

            for t in threads[1:]:
                t.join()

            t = threading.Thread(target=client,
                                 args=[net.hosts[1],1,IP,port,"STOP"])
            t.start()
            t.join()

        net.hosts[0].cmd('cd ../mininet')
        net.hosts[0].cmd('chown mininet:mininet *.txt')
        net.stop()
    except:
        crash()

def server_s(http, port, timeout):
    try:
        print 'h1 starting server'
        h1 = net.hosts[0]
        h1.cmd('cd ../server')
        print './myhttpd %s %s %s' % (http, port, timeout)
        h1.cmd('./myhttpd %s %s %s' % (http, port, timeout))
    except:
        crash()
        
def client(c, num, host, port, request):
    try:
        print 'client',c.IP()
        c.cmd('cd ../client')
        # print './client %s %s %s %s %s' % (num, host, port, str(c), request)
        print c.cmd('./client %s %s %s %s %s' % (num,host,port,str(c),request))
    except:
        crash()
        
def crash():
    exc_tb = sys.exc_info()
    print 'Uh...:', exc_tb[0]
    print traceback.print_exc()
    net.stop()
    exit()

if __name__ == '__main__':
    main()
