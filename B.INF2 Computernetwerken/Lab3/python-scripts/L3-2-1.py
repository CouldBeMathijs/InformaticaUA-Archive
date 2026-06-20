from ipmininet.iptopo import IPTopo
from ipmininet.router.config import RouterConfig
from ipmininet.ipnet import IPNet
from ipmininet.cli import IPCLI


class MyTopo( IPTopo ):
    
    def build(self, *args, **kwargs):
        # Create hosts
        h1 = self.addHost('h1')
        h2 = self.addHost('h2')
 
        # Create router
        r1 = self.addRouter('r1')
 
        # Add links between the Router and each host
        eth0 = self.addLink(r1, h1, bw = 1)
        eth0[h1].addParams(ip=("10.0.1.1/24","fc00:0:0:1::1/64",), delay=("100ms"))
        eth0[r1].addParams(ip=("10.0.1.10/24","fc00:0:0:1::10/64"), delay=("100ms"))
 
        
        
        eth1 = self.addLink(r1, h2,  bw = 1)
        eth1[r1].addParams(ip=("10.0.2.10/24","fc00:0:0:2::10/64"), delay=("100ms"))
        eth1[h2].addParams(ip=("10.0.2.2/24","fc00:0:0:2::2/64"), delay=("100ms"))
        super().build(*args, **kwargs)
        
topos = IPNet(topo=MyTopo(), allocate_IPs=False)

try:
    topos.start()
    IPCLI(topos)
finally:
    topos.stop()
