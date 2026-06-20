from ipmininet.cli import IPCLI
from ipmininet.ipnet import IPNet
from ipmininet.iptopo import IPTopo

from ipmininet.router.config import RouterConfig, BGP, ebgp_session, OSPF6
import ipmininet.router.config.bgp as _bgp


"""This file contains a simple network using BGP"""

class BGPConfig(RouterConfig):
    """A simple config with only a BGP daemon"""
    def __init__(self, node, *args, **kwargs):
        super(BGPConfig, self).__init__(node,
                                        daemons=((BGP, defaults),),
                                        *args, **kwargs)


class BGPOSPF(IPTopo):

    def build(self, *args, **kwargs):
        # BGP routers
        as1rtr = self.bgp('as1rtr', ['fc00:0:1::/48'])
        as2rtr = self.bgp('as2rtr', ['fc00:0:2::/48'])
        as3rtr = self.bgp('as3rtr', ['fc00:0:3::/48'])

        # OSPF Routers in AS 1
        as1ra = self.addRouter('as1ra', config=RouterConfig)
        as1rb = self.addRouter('as1rb', config=RouterConfig)
        as1rc = self.addRouter('as1rc', config=RouterConfig)

        # OSPF Routers in AS 2
        as2ra = self.addRouter('as2ra', config=RouterConfig)
        as2rb = self.addRouter('as2rb', config=RouterConfig)
        as2rc = self.addRouter('as2rc', config=RouterConfig)

        # OSPF Routers in AS 3
        as3ra = self.addRouter('as3ra', config=RouterConfig)
        as3rb = self.addRouter('as3rb', config=RouterConfig)
        as3rc = self.addRouter('as3rc', config=RouterConfig)

        # Intra-AS links
        # AS 1
        self.addLink(as1rtr, as1ra, igp_metric=1,
                     params1={"ip":"fc00:0:1:abc::a1/64"},
                     params2={"ip":"fc00:0:1:abc::1a/64"})
        self.addLink(as1ra, as1rb, igp_metric=4,
                     params1={"ip":"fc00:0:1:abc::ba/64"},
                     params2={"ip":"fc00:0:1:abc::ab/64"})
        self.addLink(as1rb, as1rc, igp_metric=3,
                     params1={"ip":"fc00:0:1:abc::cb/64"},
                     params2={"ip":"fc00:0:1:abc::bc/64"})
        self.addLink(as1rc, as1rtr, igp_metric=1,
                     params1={"ip":"fc00:0:1:abc::1c/64"},
                     params2={"ip":"fc00:0:1:abc::c1/64"})
        self.addLink(as1ra, self.addHost('h1a'),
                     params1={"ip": "fc00:0:1:a::a/64"},
                     params2={"ip": "fc00:0:1:a::1/64"})
        self.addLink(as1rb, self.addHost('h1b'),
                     params1={"ip": "fc00:0:1:b::b/64"},
                     params2={"ip": "fc00:0:1:b::1/64"})
        self.addLink(as1rc, self.addHost('h1c'),
                     params1={"ip": "fc00:0:1:c::c/64"},
                     params2={"ip": "fc00:0:1:c::1/64"})
        
        # AS 2
        self.addLink(as2rtr, as2ra, igp_metric=1,
                     params1={"ip":"fc00:0:2:abc::a1/64"},
                     params2={"ip":"fc00:0:2:abc::1a/64"})
        self.addLink(as2ra, as2rb, igp_metric=4,
                     params1={"ip":"fc00:0:2:abc::ba/64"},
                     params2={"ip":"fc00:0:2:abc::ab/64"})
        self.addLink(as2rb, as2rc, igp_metric=3,
                     params1={"ip":"fc00:0:2:abc::cb/64"},
                     params2={"ip":"fc00:0:2:abc::bc/64"})
        self.addLink(as2rc, as2rtr, igp_metric=1,
                     params1={"ip":"fc00:0:2:abc::1c/64"},
                     params2={"ip":"fc00:0:2:abc::c1/64"})
        self.addLink(as2ra, self.addHost('h2a'),
                     params1={"ip": "fc00:0:2:a::a/64"},
                     params2={"ip": "fc00:0:2:a::1/64"})
        self.addLink(as2rb, self.addHost('h2b'),
                     params1={"ip": "fc00:0:2:b::b/64"},
                     params2={"ip": "fc00:0:2:b::1/64"})
        self.addLink(as2rc, self.addHost('h2c'),
                     params1={"ip": "fc00:0:2:c::c/64"},
                     params2={"ip": "fc00:0:2:c::1/64"})
        
        # AS 3
        self.addLink(as3rtr, as3ra, igp_metric=1,
                     params1={"ip":"fc00:0:3:abc::a1/64"},
                     params2={"ip":"fc00:0:3:abc::1a/64"})
        self.addLink(as3ra, as3rb, igp_metric=4,
                     params1={"ip":"fc00:0:3:abc::ba/64"},
                     params2={"ip":"fc00:0:3:abc::ab/64"})
        self.addLink(as3rb, as3rc, igp_metric=3,
                     params1={"ip":"fc00:0:3:abc::cb/64"},
                     params2={"ip":"fc00:0:3:abc::bc/64"})
        self.addLink(as3rc, as3rtr, igp_metric=1,
                     params1={"ip":"fc00:0:3:abc::1c/64"},
                     params2={"ip":"fc00:0:3:abc::c1/64"})
        self.addLink(as3ra, self.addHost('h3a'),
                     params1={"ip": "fc00:0:3:a::a/64"},
                     params2={"ip": "fc00:0:3:a::1/64"})
        self.addLink(as3rb, self.addHost('h3b'),
                     params1={"ip": "fc00:0:3:b::b/64"},
                     params2={"ip": "fc00:0:3:b::1/64"})
        self.addLink(as3rc, self.addHost('h3c'),
                     params1={"ip": "fc00:0:3:c::c/64"},
                     params2={"ip": "fc00:0:3:c::1/64"})

        # Inter-AS links
        self.addLink(as1rtr, as2rtr, igp_passive=True,                  
                     params1={"ip": "fc00:12::1/64"},
                     params2={"ip": "fc00:12::2/64"})
        self.addLink(as2rtr, as3rtr, igp_passive=True,                    
                     params1={"ip": "fc00:23::2/64"},
                     params2={"ip": "fc00:23::3/64"})
        self.addLink(as3rtr, as1rtr, igp_passive=True,                    
                     params1={"ip": "fc00:13::3/64"},
                     params2={"ip": "fc00:13::1/64"})
        
        # Set AS-ownerships
        self.addAS(1, routers=[as1rtr,as1ra,as1rb,as1rc])
        self.addAS(2, routers=[as2rtr,as2ra,as2rb,as2rc])
        self.addAS(3, routers=[as3rtr,as3ra,as3rb,as3rc])

        # Add eBGP peering
        ebgp_session(self, as1rtr, as2rtr)
        ebgp_session(self, as2rtr, as3rtr)
        ebgp_session(self, as3rtr, as1rtr)

        # Add OSPF daemons
        # AS 1
        as1rtr.addDaemon(OSPF6, hello_int=5, dead_int=20)
        as1ra.addDaemon(OSPF6, hello_int=5, dead_int=20)
        as1rb.addDaemon(OSPF6, hello_int=5, dead_int=20)
        as1rc.addDaemon(OSPF6, hello_int=5, dead_int=20)

        # AS 2
        as2rtr.addDaemon(OSPF6, hello_int=5, dead_int=20)
        as2ra.addDaemon(OSPF6, hello_int=5, dead_int=20)
        as2rb.addDaemon(OSPF6, hello_int=5, dead_int=20)
        as2rc.addDaemon(OSPF6, hello_int=5, dead_int=20)

        # AS 3
        as3rtr.addDaemon(OSPF6, hello_int=5, dead_int=20)
        as3ra.addDaemon(OSPF6, hello_int=5, dead_int=20)
        as3rb.addDaemon(OSPF6, hello_int=5, dead_int=20)
        as3rc.addDaemon(OSPF6, hello_int=5, dead_int=20)

        self.addNetworkCapture(nodes=[as1rtr],
                                base_filename="bgpospf",
                                extra_arguments="-i any")

        super(BGPOSPF, self).build(*args, **kwargs)

    def bgp(self, name, net=None):
        if net is None:
            net=[]
        return self.addRouter(name, use_v4=True, 
                              use_v6=True,
                              lo_addresses=net,
                              config=(RouterConfig, 
                                      { 'daemons': [(BGP, 
                                                   { 'address_families': ( _bgp.AF_INET6(networks=net),)} 
                                                   )]
                                       }
                                      )
                              )


# Start network
net = IPNet(topo=BGPOSPF(), use_v4=False, use_v6=True, allocate_IPs=False)
net.start()
net["as1rtr"].cmd("(echo 'zebra'; echo 'enable'; echo 'conf t'; echo 'router ospf6'; echo 'redistribute bgp') | telnet localhost 2606")
net["as2rtr"].cmd("(echo 'zebra'; echo 'enable'; echo 'conf t'; echo 'router ospf6'; echo 'redistribute bgp') | telnet localhost 2606")
net["as3rtr"].cmd("(echo 'zebra'; echo 'enable'; echo 'conf t'; echo 'router ospf6'; echo 'redistribute bgp') | telnet localhost 2606")
IPCLI(net)
net.stop()