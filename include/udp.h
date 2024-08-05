//
// Created by Dominik Hrinkino on 25/07/2024.
//

#ifndef UDP_H
#define UDP_H
#include <tins/ip.h>
Tins::IP udp(std::string src_ipv4, std::string dest_ipv4, int src_port, int dst_port, int size, int ttl, bool dnf);

#endif //UDP_H
