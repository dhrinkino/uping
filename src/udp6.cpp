
#include <iostream>
#include <thread>
#include <tins/tins.h>
#include "generator.h"

using namespace std;
using namespace Tins;

IPv6 udp6(std::string src_ipv6, std::string dest_ipv6, int src_port, int dst_port, int size, int ttl) {
    // define SRC and DEST
    std::string source = src_ipv6;
    std::string destination = dest_ipv6;

    std::string data = generate(size);
    IPv6 pkt_ip = IPv6(destination, source) / UDP(dst_port,src_port);
    pkt_ip.hop_limit(ttl);

    RawPDU pkt_data(data.begin(), data.end());
    pkt_ip /= pkt_data;
    return pkt_ip;
}