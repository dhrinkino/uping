//
// Created by Dominik Hrinkino on 25/07/2024.
//

#ifndef ICMP_H
#define ICMP_H
#include <tins/ip.h>
Tins::IP icmp(std::string src_ipv4, std::string dest_ipv4, int size, int ttl);

#endif //ICMP_H
