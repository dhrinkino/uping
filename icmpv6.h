//
// Created by Dominik Hrinkino on 25/07/2024.
//
#ifndef ICMPV6_H
#define ICMPV6_H
#include <tins/ipv6.h>

Tins::IPv6 icmpv6(std::string src_ipv6, std::string dest_ipv6, int size);
#endif //ICMPV6_H
