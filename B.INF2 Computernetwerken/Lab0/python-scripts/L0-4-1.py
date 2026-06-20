from ipmininet.iptopo import IPTopo
from ipmininet.ipnet import IPNet
from ipmininet.cli import IPCLI

class MyTopo( IPTopo ):
    
    def build(self, *args, **kwargs):
        h1 = self.addHost("h1")
        h2 = self.addHost("h2")
        h3 = self.addHost("h3")
        h4 = self.addHost("h4")
        h5 = self.addHost("h5")
        h6 = self.addHost("h6")
        s1 = self.addSwitch("s1", stp=False)
        s2 = self.addSwitch("s2", stp=False)
        s3 = self.addSwitch("s3", stp=False)
        r1 = self.addRouter('r1')

        
        ls1h1 = self.addLink(s1, h1)
        # Set the IP addresses of host h1
        ls1h1[h1].addParams(ip=("fc00:0:0:1::1/64"))
        
        ls1h2 = self.addLink(s1, h2)
        # Set the IP addresses of host h2
        ls1h2[h2].addParams(ip=("fc00:0:0:1::2/64"))
        
        ls2h3 = self.addLink(s2, h3)
        ls2h3[h3].addParams(ip=("fc00:0:0:2::1/64"))
        
        ls2h4 = self.addLink(s2, h4)
        ls2h4[h4].addParams(ip=("fc00:0:0:2::2/64"))
        
        ls2h5 = self.addLink(s2, h5)
        ls2h5[h5].addParams(ip=("fc00:0:0:2::3/64"))

        
        ls3h6 = self.addLink(s3, h6)
        ls3h6[h6].addParams(ip=("fc00:0:0:3::1/64"))

        lr1s1 = self.addLink(s1, r1)
        lr1s1[r1].addParams(ip=("fc00:0:0:1::10/64"))
        
        lr1s2 = self.addLink(s2, r1)
        lr1s2[r1].addParams(ip=("fc00:0:0:2::10/64"))
        
        lr1s3 = self.addLink(s3, r1)
        lr1s3[r1].addParams(ip=("fc00:0:0:3::10/64"))
        
        
        super().build(*args, **kwargs)
        
# Create a network using the topology you just created and run IPMininet        
net = IPNet(topo=MyTopo(), allocate_IPs=False)

try:
    net.start()
    IPCLI(net)
finally:
    net.stop()
