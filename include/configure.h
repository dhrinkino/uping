//
// Created by Dominik Hrinkino on 12/08/2024.
//

#ifndef CONFIGURE_H
#define CONFIGURE_H
#include <string>

struct Config {
    bool fast = false;
    bool faster = false;
    int interval = 1000000;
    bool is_tcp = false;
    bool is_ipv6 = false;
    bool is_udp = false;
    bool is_icmp = false;
    int size = 1024;
    std::string src_ip;
    std::string dst_ip;
    int src_port = 1000;
    int dst_port = 1001;
    bool is_random = false;
    bool is_random_uniq = false;
    int timeout = 0;
    int count = 0;
    bool fry = false;
    bool debug = false;
    bool syn = false;
    bool ack = false;
    bool fin = false;
    bool urg = false;
    bool rst = false;
    bool psh = false;
    bool dnf = false;
    int ttl = 64;
    std::string iface;
};

void configure(int argc, char* argv[], Config& config);

#endif //CONFIGURE_H
