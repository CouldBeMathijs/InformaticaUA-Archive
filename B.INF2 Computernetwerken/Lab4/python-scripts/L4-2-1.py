from ipmininet.iptopo import IPTopo
from ipmininet.ipnet import IPNet
from ipmininet.cli import IPCLI
from ipmininet.router.config import RouterConfig, STATIC, StaticRoute

class MyTopo(IPTopo):
    def build(self, *args, **kwargs):

        h1 = self.addHost("h1")
        h2 = self.addHost("h2")

        r1 = self.addRouter("r1", config=RouterConfig)
        r2 = self.addRouter("r2", config=RouterConfig)
        r3 = self.addRouter("r3", config=RouterConfig)
        r4 = self.addRouter("r4", config=RouterConfig)

        lr1h1 = self.addLink(r1, h1)
        lr1r2 = self.addLink(r1, r2)
        lr2r3 = self.addLink(r2, r3)
        lr3h2 = self.addLink(r3, h2)
        lr1r4 = self.addLink(r1, r4)
        lr2r4 = self.addLink(r2, r4)

        lr1h1[h1].addParams(ip="fc00:0:0:1::1/64")
        lr3h2[h2].addParams(ip="fc00:0:0:6::2/64")

        lr1h1[r1].addParams(ip="fc00:0:0:1::11/64")
        lr1r2[r1].addParams(ip="fc00:0:0:2::11/64")
        lr1r4[r1].addParams(ip="fc00:0:0:5::11/64")

        lr1r2[r2].addParams(ip="fc00:0:0:2::12/64")
        lr2r3[r2].addParams(ip="fc00:0:0:3::12/64")
        lr2r4[r2].addParams(ip="fc00:0:0:4::12/64")

        lr2r3[r3].addParams(ip="fc00:0:0:3::13/64")
        lr3h2[r3].addParams(ip="fc00:0:0:6::13/64")

        lr1r4[r4].addParams(ip="fc00:0:0:5::14/64")
        lr2r4[r4].addParams(ip="fc00:0:0:4::14/64")

        super().build(*args, **kwargs)

net = IPNet(topo=MyTopo(), allocate_IPs=False)
try:
    net.start()
    IPCLI(net)
finally:
    net.stop()