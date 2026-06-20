from ipmininet.iptopo import IPTopo
from ipmininet.ipnet import IPNet
from ipmininet.cli import IPCLI
from ipmininet.router.config import RouterConfig, STATIC, StaticRoute

class MyTopo(IPTopo):
    def build(self, *args, **kwargs):
        h1 = self.addHost("h1")
        h2 = self.addHost("h2")
        h3 = self.addHost("h3")

        s1 = self.addSwitch("s1", stp=False)

        r1 = self.addRouter("r1", config=RouterConfig)
        r2 = self.addRouter("r2", config=RouterConfig)

        lr1h1 = self.addLink(r1, h1)
        lr1s1 = self.addLink(r1, s1)
        lr2s1 = self.addLink(r2, s1)
        ls1h2 = self.addLink(s1, h2)
        lr2h3 = self.addLink(r2, h3)

        lr1h1[h1].addParams(ip="fc00:0:0:1::1/64")
        lr1h1[r1].addParams(ip="fc00:0:0:1::10/64")
        lr1s1[r1].addParams(ip="fc00:0:0:2::10/64")
        lr2s1[r2].addParams(ip="fc00:0:0:2::11/64")
        ls1h2[h2].addParams(ip="fc00:0:0:2::2/64")
        lr2h3[r2].addParams(ip="fc00:0:0:3::10/64")
        lr2h3[h3].addParams(ip="fc00:0:0:3::3/64")

        super().build(*args, **kwargs)

net = IPNet(topo=MyTopo(), allocate_IPs=False)
try:
    net.start()
    IPCLI(net)
finally:
    net.stop()