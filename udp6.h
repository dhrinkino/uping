//
// Created by Dominik Hrinkino on 25/07/2024.
//

#ifndef UDP6_H
#define UDP6_H
#include <tins/ipv6.h>
Tins::IPv6 udp6(std::string src_ipv6, std::string dest_ipv6, int src_port, int dst_port, int size);
#endif //UDP6_H
