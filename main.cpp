#include <iostream>
#include <tins/tins.h>
#include <thread>
#include <cstring>
#include <cstdlib>
#include <limits>


// includes of internal files
#include "icmpv6.h"
#include "tcp6.h"
#include "udp6.h"
#include "tcp.h"
#include "icmp.h"
#include "udp.h"
#include "randomIPv4.h"
#include "randomIPv6.h"


using namespace Tins;

int main(int argc, char* argv[]) {

    IP packet;
    IPv6 packet6;
    PacketSender sender;
    int counter = 0;

    long long end_time = std::numeric_limits<long long>::max();

    // argument processing
    bool fast = false;
    bool faster = false;
    int interval = 10000000;
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
    int timeout = 0;
    int count = 0;
    bool fry = false;
    bool debug = false;


    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--fast") == 0) {
            fast = true;
        } else if (strcmp(argv[i], "--faster") == 0) {
            faster = true;
        } else if (strncmp(argv[i], "--interval=", 11) == 0) {
            interval = (int)(atof(argv[i] + 11)*1000); // convert to microseconds and then to int

        } else if (strcmp(argv[i], "--tcp") == 0) {
            is_tcp = true;
        } else if (strcmp(argv[i], "--ipv6") == 0) {
            is_ipv6 = true;
        } else if (strcmp(argv[i], "--udp") == 0) {
            is_udp = true;
        } else if (strcmp(argv[i], "--icmp") == 0) {
            is_icmp = true;
        } else if (strncmp(argv[i], "--size=", 7) == 0) {
            size = atoi(argv[i] + 7);
        } else if (strncmp(argv[i], "--src_ip=", 9) == 0) {
            src_ip = argv[i] + 9;
        } else if (strncmp(argv[i], "--dst_ip=", 9) == 0) {
            dst_ip = argv[i] + 9;
        } else if (strncmp(argv[i], "--src_port=", 11) == 0) {
            src_port = atoi(argv[i] + 11);
        } else if (strncmp(argv[i], "--dst_port=", 11) == 0) {
            dst_port = atoi(argv[i] + 11);
        } else if (strcmp(argv[i], "--random") == 0) {
            is_random = true;
        } else if (strncmp(argv[i], "--timeout=", 10) == 0) {
            timeout = atoi(argv[i] + 10);
        } else if (strncmp(argv[i], "--count=", 8) == 0) {
            count = atoi(argv[i] + 8);
        } else if (strcmp(argv[i], "--fry") == 0) {
            fry = true;
        } else if (strcmp(argv[i], "-v") == 0) {
            debug = true;
        }
    }

    if (debug){
        // Output the parsed values
        std::cout << "Fast: " << (fast ? "true" : "false") << std::endl;
        std::cout << "Faster: " << (faster ? "true" : "false") << std::endl;
        std::cout << "Interval: " << interval << std::endl;
        std::cout << "TCP: " << (is_tcp ? "true" : "false") << std::endl;
        std::cout << "IPv6: " << (is_ipv6 ? "true" : "false") << std::endl;
        std::cout << "UDP: " << (is_udp ? "true" : "false") << std::endl;
        std::cout << "ICMP: " << (is_icmp ? "true" : "false") << std::endl;
        std::cout << "Size: " << size << std::endl;
        std::cout << "Source IP: " << src_ip << std::endl;
        std::cout << "Destination IP: " << dst_ip << std::endl;
        std::cout << "Source Port: " << src_port << std::endl;
        std::cout << "Destination Port: " << dst_port << std::endl;
        std::cout << "Random: " << (is_random ? "true" : "false") << std::endl;
        std::cout << "Timeout: " << timeout << std::endl;
        std::cout << "Count: " << count << std::endl;
        std::cout << "Fry: " << (fry ? "true" : "false") << std::endl;
    }


    // packet building

    if (is_ipv6) {
        if (is_random) {
            src_ip = randomIPv6();
        }
        if (is_udp) {
            packet6 = udp6(src_ip,dst_ip,src_port,dst_port,size);
        } else if (is_tcp) {
            packet6 = tcp6(src_ip,dst_ip,src_port,dst_port,size);
        } else {
            // nothing selected fallback to ICMP
            packet6 = icmpv6(src_ip,dst_ip,size);
        }
    } else {
        if (is_random) {
            src_ip = randomIPv4();
        }
        if (is_udp) {
            packet = udp(src_ip,dst_ip,src_port,dst_port,size);
        } else if (is_tcp) {
            packet = tcp(src_ip,dst_ip,src_port,dst_port,size);
        } else {
            // nothing selected fallback to ICMP
            packet = icmp(src_ip,dst_ip,size);
        }
    }

    // packet generator
    if (fry) {
        //TODO: run POSIX threads and send it
    } else {

        if (timeout > 0) {
            // calculate end time
            end_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() + timeout;
        }

        if (is_ipv6) {
            while (true) {
                if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() > end_time) {
                    exit(0);
                }
                if (counter > count && count != 0) {
                    exit(0);
                }
                if (fast) {
                    printf(".");
                    fflush(stdout);
                }else if (faster) {
                    //
                } else {
                    std::this_thread::sleep_for(std::chrono::microseconds(interval));
                    printf(".");
                    fflush(stdout);
                }
                sender.send(packet6);
            }
        } else {
            while (true) {

                if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() > end_time) {
                    exit(0);
                }
                if (counter > count && count != 0) {
                    exit(0);
                }
                if (fast) {
                    printf(".");
                    fflush(stdout);
                }else if (faster) {
                    //
                } else {
                    std::this_thread::sleep_for(std::chrono::microseconds(interval));
                    printf(".");
                    fflush(stdout);
                }
                sender.send(packet);
                counter++;
            }
        }

    }

    return 0;
}
