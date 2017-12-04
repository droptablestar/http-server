"""Custom topology 1

"""

from mininet.topo import Topo

class MyTopo(Topo):
    def __init__(self, clients=1, top='6ms'):
        # Initialize topology
        Topo.__init__( self )

        switch = self.addSwitch('s0')
        server = self.addHost('h0')
        self.addLink(server, switch, bw=15, delay=top)
        
        for i in range(1,clients+1):
            #create host
            host = self.addHost('h'+str(i))

            # Add links
            self.addLink(host, switch, bw=15, delay='24ms')

topos = { 'top1': ( lambda: MyTopo() ) }
