//
// Created by Dominik Hrinkino on 25/07/2024.
//

#ifndef TCP6_H
#define TCP6_H
#include <tins/ipv6.h>
Tins::IPv6 tcp6(std::string src_ipv6, std::string dest_ipv6, int src_port, int dst_port, int size, bool syn);

#endif //TCP6_H
