
#include <iostream>
#include <thread>
#include <tins/tins.h>
#include "generator.h"

using namespace std;
using namespace Tins;

IP tcp(std::string src_ipv4, std::string dest_ipv4, int src_port, int dst_port, int size, int ttl, bool syn, bool ack, bool fin, bool urg, bool rst, bool psh, bool dnf) {
    // Define SRC and DEST
    std::string source = src_ipv4;
    std::string destination = dest_ipv4;

    std::string data = generate(size);

    // Create IPv4 packet with TCP header
    IP pkt_ip = IP(destination, source) / TCP(dst_port,src_port);
    pkt_ip.ttl(ttl);
    if (dnf) {
        pkt_ip.flags(IP::DONT_FRAGMENT);
    }
    if (syn) {
        pkt_ip.rfind_pdu<TCP>().flags(TCP::SYN);
    }
    if (ack) {
        pkt_ip.rfind_pdu<TCP>().flags(TCP::ACK);
    }
    if (fin) {
        pkt_ip.rfind_pdu<TCP>().flags(TCP::FIN);
    }
    if (urg) {
        pkt_ip.rfind_pdu<TCP>().flags(TCP::URG);
    }
    if (rst) {
        pkt_ip.rfind_pdu<TCP>().flags(TCP::RST);
    }
    if (psh) {
        pkt_ip.rfind_pdu<TCP>().flags(TCP::PSH);
    }
    RawPDU pkt_data(data.begin(), data.end());
    pkt_ip /= pkt_data;
    return pkt_ip;
}
