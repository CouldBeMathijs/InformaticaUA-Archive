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
        h4 = self.addHost('h4')
 
        # Create router
        s1 = self.addSwitch('s1', stp=False)
 
        # Add links between the Router and each host
        ls1h1 = self.addLink(s1, h1, bw = 1)
        ls1h1[h1].addParams(ip=("10.0.0.1/8","fc00::1/48",), delay=("100ms"))
        
        ls1h2 = self.addLink(s1, h2,  bw = 1)
        ls1h2[h2].addParams(ip=("10.0.0.2/24","fc00::2/64"), delay=("100ms"))
        
        ls1h3 = self.addLink(s1, h3,  bw = 1)
        ls1h3[h3].addParams(ip=("10.0.255.3/16","fc00:0:0:1::3/48"), delay=("100ms"))
        
        ls1h4 = self.addLink(s1, h4,  bw = 1)
        ls1h4[h4].addParams(ip=("10.1.0.4/24","fc00:0:0:1::4/64"), delay=("100ms"))
        
        super().build(*args, **kwargs)
        
topos = IPNet(topo=MyTopo(), allocate_IPs=False)

try:
    topos.start()
    IPCLI(topos)
finally:
    topos.stop()
