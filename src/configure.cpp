#include <cstring>
#include <cstdlib>
#include <iostream>
#include <help.h>

#include "configure.h"

void configure(int argc, char* argv[], Config& config) {
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--fast") == 0) {
            config.fast = true;
        } else if (strcmp(argv[i], "--faster") == 0) {
            config.faster = true;
        } else if (strncmp(argv[i], "--interval=", 11) == 0) {
            config.interval = static_cast<int>(atof(argv[i] + 11) * 1000);
        } else if (strcmp(argv[i], "--tcp") == 0) {
            config.is_tcp = true;
        } else if (strcmp(argv[i], "--ipv6") == 0) {
            config.is_ipv6 = true;
        } else if (strcmp(argv[i], "--udp") == 0) {
            config.is_udp = true;
        } else if (strcmp(argv[i], "--icmp") == 0) {
            config.is_icmp = true;
        } else if (strncmp(argv[i], "--size=", 7) == 0) {
            config.size = atoi(argv[i] + 7);
        } else if (strncmp(argv[i], "--src_ip=", 9) == 0 || strncmp(argv[i], "--src-ip=", 9) == 0) {
            config.src_ip = argv[i] + 9;
        } else if (strncmp(argv[i], "--dst_ip=", 9) == 0 || strncmp(argv[i], "--dst-ip=", 9) == 0) {
            config.dst_ip = argv[i] + 9;
        } else if (strncmp(argv[i], "--src_port=", 11) == 0 || strncmp(argv[i], "--src-port=", 11) == 0) {
            config.src_port = atoi(argv[i] + 11);
        } else if (strncmp(argv[i], "--dst_port=", 11) == 0 || strncmp(argv[i], "--dst-port=", 11) == 0) {
            config.dst_port = atoi(argv[i] + 11);
        } else if (strcmp(argv[i], "--random") == 0) {
            config.is_random = true;
        } else if (strcmp(argv[i], "--uniq_random") == 0 || strcmp(argv[i], "--uniq-random") == 0) {
            config.is_random_uniq = true;
        } else if (strncmp(argv[i], "--timeout=", 10) == 0) {
            config.timeout = atoi(argv[i] + 10);
        } else if (strncmp(argv[i], "--count=", 8) == 0) {
            config.count = atoi(argv[i] + 8);
        } else if (strcmp(argv[i], "--fry") == 0) {
            config.fry = true;
        } else if (strcmp(argv[i], "-v") == 0) {
            config.debug = true;
        } else if (strcmp(argv[i], "--syn") == 0) {
            config.syn = true;
        } else if (strcmp(argv[i], "--ack") == 0) {
            config.ack = true;
        } else if (strcmp(argv[i], "--fin") == 0) {
            config.fin = true;
        } else if (strcmp(argv[i], "--urg") == 0) {
            config.urg = true;
        } else if (strcmp(argv[i], "--rst") == 0) {
            config.rst = true;
        } else if (strcmp(argv[i], "--psh") == 0) {
            config.psh = true;
        } else if (strcmp(argv[i], "--do-not-fragment") == 0) {
            config.dnf = true;
        } else if (strncmp(argv[i], "--ttl=", 6) == 0) {
            config.ttl = atoi(argv[i] + 6);
        } else if (strcmp(argv[i], "--help") == 0) {
            // Assuming you have a help function defined
            help();
            exit(0);
        }
    }
}
