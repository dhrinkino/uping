//
// Created by Dominik Hrinkino on 25/07/2024.
//

#ifndef TCP_H
#define TCP_H
#include <tins/ip.h>
Tins::IP tcp(std::string src_ipv4, std::string dest_ipv4, int src_port, int dst_port, int size);
#endif //TCP_H