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
        r1 = self.addRouter('r1')
 
        # Add links between the Router and each host
        lr1h1 = self.addLink(r1, h1, bw = 1, enable_red=True)
        lr1h1[h1].addParams(ip=("fc00:0:0:1::1/64"), delay=("100ms"))
        lr1h1[r1].addParams(ip=("fc00:0:0:1::10/64"), delay=("100ms"))
        
        lr1h2 = self.addLink(r1, h2,  bw = 1, enable_red=True)
        lr1h2[h2].addParams(ip=("fc00:0:0:2::2/64"), delay=("100ms"))
        lr1h2[r1].addParams(ip=("fc00:0:0:2::10/64"), delay=("100ms"))
        
        lr1h3 = self.addLink(r1, h3,  bw = 1, enable_red=True)
        lr1h3[h3].addParams(ip=("fc00:0:0:3::3/64"), delay=("100ms"))
        lr1h3[r1].addParams(ip=("fc00:0:0:3::10/64"), delay=("100ms"))
        
        lr1h4 = self.addLink(r1, h4,  bw = 1, enable_red=True)
        lr1h4[h4].addParams(ip=("fc00:0:0:4::4/64"), delay=("100ms"))
        lr1h4[r1].addParams(ip=("fc00:0:0:4::10/64"), delay=("100ms"))
        
        super().build(*args, **kwargs)
        
topos = IPNet(topo=MyTopo(), allocate_IPs=False)

try:
    topos.start()
    topos["h1"].cmd("ethtool -K h1-eth0 tso off")
    IPCLI(topos)
finally:
    topos.stop()
