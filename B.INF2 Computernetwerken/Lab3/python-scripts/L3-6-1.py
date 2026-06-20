from ipmininet.iptopo import IPTopo
from ipmininet.router.config import RouterConfig
from ipmininet.ipnet import IPNet
from ipmininet.cli import IPCLI


class MyTopo( IPTopo ):
    
    def build(self, *args, **kwargs):
        # Create hosts
        h1 = self.addHost('h1')
        h2 = self.addHost('h2')
        h3 = self.addHost('h3')
        # Create switch
        s1 = self.addSwitch('s1', stp=False)
        # Create router
        r1 = self.addRouter('r1')
 
        
        #links between switch and each host
        
        ls1h1 = self.addLink(s1, h1, bw = 1)
        ls1h2 = self.addLink(s1, h2, bw = 1)

        
 
        #links between the Router and each host
        eth0 = self.addLink(r1, s1, bw = 1)
        eth0[r1].addParams(ip=("fc00:0:0:1::1/64"), delay=("100ms"))
 
        
        
        
        eth1 = self.addLink(r1, h3,  bw = 1)
        eth1[r1].addParams(ip=("fc00:0:0:2::1/64"), delay=("100ms"))
        super().build(*args, **kwargs)
        
topos = IPNet(topo=MyTopo(), allocate_IPs=False)

try:
    topos.start()
    IPCLI(topos)
finally:
    topos.stop()
