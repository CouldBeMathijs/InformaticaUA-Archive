from ipmininet.iptopo import IPTopo
from ipmininet.router.config import RouterConfig
from ipmininet.ipnet import IPNet
from ipmininet.cli import IPCLI
import time
import builtins
import subprocess

class MyTopo(IPTopo):
    def build(self, *args, **kwargs):
        h1 = self.addHost('h1')
        h2 = self.addHost('h2')
        h3 = self.addHost('h3')
        h4 = self.addHost('h4')

        r1 = self.addRouter('r1')

        lr1h1 = self.addLink(r1, h1, bw=0.2, delay='100ms', enable_red=True)
        lr1h1[h1].addParams(ip="fc00:0:0:1::1/64")
        lr1h1[r1].addParams(ip="fc00:0:0:1::10/64")

        lr1h2 = self.addLink(r1, h2, bw=1, delay='100ms')
        lr1h2[h2].addParams(ip="fc00:0:0:2::2/64")
        lr1h2[r1].addParams(ip="fc00:0:0:2::10/64")

        lr1h3 = self.addLink(r1, h3, bw=1, delay='100ms')
        lr1h3[h3].addParams(ip="fc00:0:0:3::3/64")
        lr1h3[r1].addParams(ip="fc00:0:0:3::10/64")

        lr1h4 = self.addLink(r1, h4, bw=1, delay='100ms')
        lr1h4[h4].addParams(ip="fc00:0:0:4::4/64")
        lr1h4[r1].addParams(ip="fc00:0:0:4::10/64")

        super().build(*args, **kwargs)


def run_tests(net):
    map = "/home/computernetwerken/Desktop/L2-10-test/"
    tmp_capture  = "/tmp/capture.pcapng"
    destination_capture = f"{map}capture.pcapng"

    subprocess.call(["mkdir", "-p", map])
    server_ip = "fc00:0:0:1::1"

    print("\n" + "="*60)
    print("EXPERIMENT: TCP Fairness + UDP competitie")
    print("="*60)

    print("[t= 0s]  dumpcap starten op h1-eth0...")
    p_dumpcap = net["h1"].popen(
        ["dumpcap", "-i", "h1-eth0", "-n", "-w", tmp_capture]
    )
    time.sleep(1)

    print("[t= 0s]  3x iperf3-server starten op h1")
    net["h1"].cmd("pkill iperf3; sleep 1")
    net["h1"].cmd("iperf3 -s -p 5201 -D")
    net["h1"].cmd("iperf3 -s -p 5202 -D")
    net["h1"].cmd("iperf3 -s -p 5203 -D")
    time.sleep(2)

    print("[t= 0s]  h2 TCP start")
    f_h2 = open(f"{map}h2-tcp.txt", "w")
    p_h2 = net["h2"].popen(
        ["iperf3", "-c", server_ip, "-p", "5201", "-t", "30"],
        stdout=f_h2, stderr=f_h2
    )

    time.sleep(10)
    print("[t=10s]  h3 TCP start")
    f_h3 = open(f"{map}h3-tcp.txt", "w")
    p_h3 = net["h3"].popen(
        ["iperf3", "-c", server_ip, "-p", "5202", "-t", "25"],
        stdout=f_h3, stderr=f_h3
    )

    time.sleep(10)
    print("[t=20s]  h4 UDP start")
    f_h4 = open(f"{map}h4-udp.txt", "w")
    p_h4 = net["h4"].popen(
        ["iperf3", "-u", "-c", server_ip, "-p", "5203", "-b", "0.2M", "-t", "10"],
        stdout=f_h4, stderr=f_h4
    )

    time.sleep(10)
    print("[t=30s]  h4 UDP gestopt, TCP herstelt zich via congestion control...")

    time.sleep(5)
    print("[t=35s]  h2 en h3 TCP klaar.")

    p_h2.wait(); f_h2.close()
    p_h3.wait(); f_h3.close()
    p_h4.wait(); f_h4.close()

    print("Dumpcap stoppen en capture opslaan...")
    p_dumpcap.terminate()
    p_dumpcap.wait()
    time.sleep(1)
    subprocess.call(["cp", tmp_capture, destination_capture])

    net["h1"].cmd("pkill iperf3")

    import os
    real_user = os.environ.get("SUDO_USER", "computernetwerken")
    subprocess.call(["chown", "-R", f"{real_user}:{real_user}", map])

    print()
    print("Alle testen voltooid! Bestanden staan in:")
    print(f"  {map}")
    print(f"  capture.pcapng  — Wireshark trace van h1-eth0")
    print(f"  h2-tcp.txt      — TCP throughput van h2")
    print(f"  h3-tcp.txt      — TCP throughput van h3")
    print(f"  h4-udp.txt      — UDP throughput van h4")
    print("="*60 + "\n")


net = IPNet(topo=MyTopo(), allocate_IPs=False)

try:
    net.start()

    net["h1"].cmd("ethtool -K h1-eth0 tso off")

    print("\n" + "="*60)
    print("Netwerk gestart!")
    print()
    print("Voer het experiment uit (~35 seconden):")
    print("  mininet> py run_tests(net)")
    print()
    print("Daarna staan alle bestanden automatisch op de Desktop.")
    print("Typ 'exit' om mininet af te sluiten.")
    print("="*60 + "\n")

    builtins.run_tests = run_tests
    builtins.time = time

    IPCLI(net)

finally:
    print("Netwerk wordt afgesloten...")
    net.stop()