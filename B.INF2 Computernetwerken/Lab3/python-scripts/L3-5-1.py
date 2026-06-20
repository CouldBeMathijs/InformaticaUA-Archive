from ipmininet.iptopo import IPTopo
from ipmininet.router.config import RouterConfig
from ipmininet.ipnet import IPNet
from ipmininet.cli import IPCLI

class MyTopo(IPTopo):
    
    def build(self, *args, **kwargs):
        h1 = self.addHost('h1')
        h2 = self.addHost('h2')
        h3 = self.addHost('h3')
        s1 = self.addSwitch('s1', stp=False)
        r1 = self.addRouter('r1')

        ls1h1 = self.addLink(s1, h1, bw=1)
        ls1h1[h1].addParams(ip="10.0.1.101/24")

        ls1h2 = self.addLink(s1, h2, bw=1)
        ls1h2[h2].addParams(ip="10.0.1.102/24")

        eth0 = self.addLink(r1, s1, bw=1)
        eth0[r1].addParams(ip="10.0.1.1/24", delay="100ms")

        eth1 = self.addLink(r1, h3, bw=1)
        eth1[r1].addParams(ip="128.66.0.1/24", delay="100ms")
        eth1[h3].addParams(ip="128.66.0.103/24") 

        super().build(*args, **kwargs)

topos = IPNet(topo=MyTopo(), allocate_IPs=False)
try:
    topos.start()
    IPCLI(topos)
finally:
    topos.stop()