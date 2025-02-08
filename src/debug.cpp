#include <iostream>
#include "configure.h"

void debug(Config config)
{
        std::cout << "Fast: " << (config.fast ? "true" : "false") << std::endl;
        std::cout << "Faster: " << (config.faster ? "true" : "false") << std::endl;
        std::cout << "Interval: " << config.interval << std::endl;
        std::cout << "Interface: " << config.iface << std::endl;
        std::cout << "TCP: " << (config.is_tcp ? "true" : "false") << std::endl;
        std::cout << "IPv6: " << (config.is_ipv6 ? "true" : "false") << std::endl;
        std::cout << "UDP: " << (config.is_udp ? "true" : "false") << std::endl;
        std::cout << "ICMP: " << (config.is_icmp ? "true" : "false") << std::endl;
        std::cout << "Size: " << config.size << std::endl;
        std::cout << "Do Not Fragment: " << (config.dnf ? "true" : "false") << std::endl;
        std::cout << "Source IP: " << config.src_ip << std::endl;
        std::cout << "Destination IP: " << config.dst_ip << std::endl;
        std::cout << "Source Port: " << config.src_port << std::endl;
        std::cout << "Destination Port: " << config.dst_port << std::endl;
        std::cout << "Random: " << (config.is_random ? "true" : "false") << std::endl;
        std::cout << "Timeout: " << config.timeout << std::endl;
        std::cout << "Count: " << config.count << std::endl;
        std::cout << "Wait: " << config.wait << std::endl;
        std::cout << "Fry: " << (config.fry ? "true" : "false") << std::endl;
        std::cout << "TCP SYN: " << (config.syn ? "true" : "false") << std::endl;
        std::cout << "TCP ACK: " << (config.ack ? "true" : "false") << std::endl;
        std::cout << "TCP URG: " << (config.urg ? "true" : "false") << std::endl;
        std::cout << "TCP FIN: " << (config.fin ? "true" : "false") << std::endl;
        std::cout << "TCP PUSH: " << (config.psh ? "true" : "false") << std::endl;
        std::cout << "TCP RST: " << (config.rst ? "true" : "false") << std::endl;
        std::cout << "Uniq random source IP: " << (config.is_random_uniq ? "true" : "false") << std::endl;

}