//
// Created by Dominik Hrinkino on 25/07/2024.
//


#include <iostream>
#include <thread>
#include <tins/tins.h>
#include "generator.h"

using namespace std;
using namespace Tins;

    IPv6 icmpv6(std::string src_ipv6, std::string dest_ipv6, int size) {
        // define SRC and DEST
        std::string source = src_ipv6;
        std::string destination = dest_ipv6;

        std::string data = generate(size);

        // compose ICMPv6 packet
        IPv6 pkt_ip = IPv6(destination, source) / ICMPv6();
        ICMPv6& icmpv6 = pkt_ip.rfind_pdu<ICMPv6>();
        icmpv6.type(ICMPv6::ECHO_REQUEST);
        icmpv6.code(0);

        // set TTL to 128 (like OS Windows)
        pkt_ip.hop_limit(128);

        // insert data payload
        RawPDU pkt_data(data);
        pkt_ip /= pkt_data;
        return pkt_ip;
    }
