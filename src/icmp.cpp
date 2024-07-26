//
// Created by Dominik Hrinkino on 25/07/2024.
//


#include <iostream>
#include <thread>
#include <tins/tins.h>
#include "generator.h"

using namespace std;
using namespace Tins;

IP icmp(std::string src_ipv4, std::string dest_ipv4, int size) {
    // define SRC and DEST
    std::string source = src_ipv4;
    std::string destination = dest_ipv4;

    std::string data = generate(size);

    // Create IPv4 packet with ICMP header
    IP pkt_ip = IP(destination, source) / ICMP();
    ICMP& icmp = pkt_ip.rfind_pdu<ICMP>();
    icmp.type(ICMP::ECHO_REQUEST);
    icmp.code(0);

    // set TTL to 128 (like OS Windows)
    pkt_ip.ttl(128);

    // insert data payload
    RawPDU pkt_data(data);
    pkt_ip /= pkt_data;
    return pkt_ip;
}
