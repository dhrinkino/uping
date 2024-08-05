
#include <iostream>
#include <thread>
#include <tins/tins.h>
#include "generator.h"

using namespace std;
using namespace Tins;

IP udp(std::string src_ipv4, std::string dest_ipv4, int src_port, int dst_port, int size, int ttl, bool dnf) {
    // Define SRC and DEST
    std::string source = src_ipv4;
    std::string destination = dest_ipv4;

    std::string data = generate(size);

    // Create IPv4 packet with TCP header
    IP pkt_ip = IP(destination, source) / UDP(dst_port,src_port);
    pkt_ip.ttl(ttl); // Set Time to Live
    if (dnf) {
        pkt_ip.flags(IP::DONT_FRAGMENT);
    }
    RawPDU pkt_data(data.begin(), data.end());
    pkt_ip /= pkt_data;
    return pkt_ip;
}
