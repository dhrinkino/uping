#include <iostream>
#include <tins/tins.h>
#include <thread>
#include <cstring>
#include <cstdlib>
#include <limits>
#include <thread>


#include "icmpv6.h"
#include "tcp6.h"
#include "udp6.h"
#include "tcp.h"
#include "icmp.h"
#include "udp.h"
#include "randomIPv4.h"
#include "randomIPv6.h"


using namespace Tins;



void help() {
    std::cout << "Usage: uping [options]\n"
          << "Options:\n"
          << "  --interval=INT      Set interval in microseconds (default: 1000000us = 1 second )\n"
          << "  --ttl=INT           Set custom TTL value (default: 64)\n"
          << "  --tcp               Use TCP protocol\n"
          << "  --syn               Add SYN Flag to TCP packet\n"
          << "  --ack               Add ACK Flag to TCP packet\n"
          << "  --fin               Add FIN Flag to TCP packet\n"
          << "  --urg               Add URG Flag to TCP packet\n"
          << "  --rst               Add RST Flag to TCP packet\n"
          << "  --psh               Add PSH Flag to TCP packet\n"
          << "  --ipv6              Use IPv6\n"
          << "  --udp               Use UDP protocol\n"
          << "  --icmp              Use ICMP protocol (default if no mode selected)\n"
          << "  --size=INT          Set size (default 1024)\n"
          << "  --do-not-fragment   Set Do Not Fragment flag (IPv4 only)\n"
          << "  --src_ip=IP         Set source IP address, if empty, src will be generated same as --random\n"
          << "  --dst_ip=IP         Set destination IP address (required)\n"
          << "  --src_port=PORT     Set source port (ignored when icmp is used) (default: 1000) \n"
          << "  --dst_port=PORT     Set destination port (default: 1001)\n"
          << "  --random            Generates a random IPv4 or IPv6 address for source.\n"
          << "  --uniq_random       Generates a random source IPv4 or IPv6 address for each packet.\n"
          << "  --timeout=INT       Sets the timeout period in seconds, after which the program will end.\n"
          << "  --count=INT         Sets the number of packets to be sent.\n"
          << "  --fast              Enables fast mode, bypassing the interval between packets.\n"
          << "  --faster            Enables an even faster mode, bypassing the interval and not printing anything to the console.\n"
          << "  --fry               Runs 5 senders in parallel mode without any waiting\n"
          << "  -v                  Enable verbose mode\n"
          << "  --help              Display this help message\n";
}


void sendPara4(IP packet)
{
    PacketSender sender;
    while (true){
        sender.send(packet);
    }
}
void sendPara6(IPv6 packet)
{
    PacketSender sender;
    while (true){
        sender.send(packet);
    }
}


int main(int argc, char* argv[]) {
    std::vector<std::thread> threads;
    PacketSender sender;
    IP packet;
    IPv6 packet6;
    int counter = 0;

    long long end_time = std::numeric_limits<long long>::max();

    // argument processing
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
        } else if (strcmp(argv[i], "--uniq_random") == 0) {
            is_random_uniq = true;
        } else if (strncmp(argv[i], "--timeout=", 10) == 0) {
            timeout = atoi(argv[i] + 10);
        } else if (strncmp(argv[i], "--count=", 8) == 0) {
            count = atoi(argv[i] + 8);
        } else if (strcmp(argv[i], "--fry") == 0) {
            fry = true;
        } else if (strcmp(argv[i], "-v") == 0) {
            debug = true;
        } else if (strcmp(argv[i], "--syn") == 0) {
            syn = true;
        } else if (strcmp(argv[i], "--ack") == 0) {
            ack = true;
        } else if (strcmp(argv[i], "--fin") == 0) {
            fin = true;
        } else if (strcmp(argv[i], "--urg") == 0) {
            urg = true;
        } else if (strcmp(argv[i], "--rst") == 0) {
            rst = true;
        } else if (strcmp(argv[i], "--psh") == 0) {
            psh = true;
        } else if (strcmp(argv[i], "--do-not-fragment") == 0) {
            dnf = true;
        } else if (strncmp(argv[i], "--ttl=", 6) == 0) {
            ttl = atoi(argv[i] + 6);
        } else if (strcmp(argv[i], "--help") == 0) {
            help();
            return 0;
        }
    }

    if (src_ip.empty()) {
        if (is_ipv6) {
            src_ip = randomIPv6();
        } else {
            src_ip = randomIPv4();
        }
    }

    if (dst_ip.empty()) {
        help();
        return 1;
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
        std::cout << "Do Not Fragment: " << (dnf ? "true" : "false") << std::endl;
        std::cout << "Source IP: " << src_ip << std::endl;
        std::cout << "Destination IP: " << dst_ip << std::endl;
        std::cout << "Source Port: " << src_port << std::endl;
        std::cout << "Destination Port: " << dst_port << std::endl;
        std::cout << "Random: " << (is_random ? "true" : "false") << std::endl;
        std::cout << "Timeout: " << timeout << std::endl;
        std::cout << "Count: " << count << std::endl;
        std::cout << "Fry: " << (fry ? "true" : "false") << std::endl;
        std::cout << "TCP SYN: " << (syn ? "true" : "false") << std::endl;
        std::cout << "TCP ACK: " << (ack ? "true" : "false") << std::endl;
        std::cout << "TCP URG: " << (urg ? "true" : "false") << std::endl;
        std::cout << "TCP FIN: " << (fin ? "true" : "false") << std::endl;
        std::cout << "TCP PUSH: " << (psh ? "true" : "false") << std::endl;
        std::cout << "TCP RST: " << (rst ? "true" : "false") << std::endl;
        std::cout << "Uniq random source IP: " << (is_random_uniq ? "true" : "false") << std::endl;

    }


    // packet building

    if (is_ipv6) {
        if (is_random) {
            src_ip = randomIPv6();
        }
        if (is_udp) {
            packet6 = udp6(src_ip,dst_ip,src_port,dst_port,size, ttl);
        } else if (is_tcp) {
            packet6 = tcp6(src_ip,dst_ip,src_port,dst_port,size,ttl,syn,ack,fin,urg,rst,psh);
        } else {
            // nothing selected fallback to ICMP
            packet6 = icmpv6(src_ip,dst_ip,size,ttl);
        }
    } else {
        if (is_random) {
            src_ip = randomIPv4();
        }
        if (is_udp) {
            packet = udp(src_ip,dst_ip,src_port,dst_port,size,ttl,dnf);
        } else if (is_tcp) {
            packet = tcp(src_ip,dst_ip,src_port,dst_port,size,ttl,syn,ack,fin,urg,rst,psh,dnf);
        } else {
            // nothing selected fallback to ICMP
            packet = icmp(src_ip,dst_ip,size,ttl,dnf);
        }
    }

    // packet generator
    if (fry) {
        if (is_ipv6){
            std::thread it1(sendPara6, packet6);
            std::thread it2(sendPara6, packet6);
            std::thread it3(sendPara6, packet6);
            std::thread it4(sendPara6, packet6);
            std::thread it5(sendPara6, packet6);

            it1.join();
            it2.join();
            it3.join();
            it4.join();
            it5.join();
        } else {
            std::thread t1(sendPara4, packet);
            std::thread t2(sendPara4, packet);
            std::thread t3(sendPara4, packet);
            std::thread t4(sendPara4, packet);
            std::thread t5(sendPara4, packet);

            t1.join();
            t2.join();
            t3.join();
            t4.join();
            t5.join();
        }

    } else {

        if (timeout > 0) {
            // calculate end time
            end_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() + timeout;
        }

        if (is_ipv6) {
            while (true) {
                if (is_random_uniq) {
                    src_ip = randomIPv6();
                    if (is_udp) {
                        packet6 = udp6(src_ip,dst_ip,src_port,dst_port,size,ttl);
                    } else if (is_tcp) {
                        packet6 = tcp6(src_ip,dst_ip,src_port,dst_port,size,ttl,syn,ack,fin,urg,rst,psh);
                    } else {
                        // nothing selected fallback to ICMP
                        packet6 = icmpv6(src_ip,dst_ip,size,ttl);
                    }
                }
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

                if (is_random_uniq) {
                        src_ip = randomIPv4();
                    if (is_udp) {
                        packet = udp(src_ip,dst_ip,src_port,dst_port,size,ttl,dnf);
                    } else if (is_tcp) {
                        packet = tcp(src_ip,dst_ip,src_port,dst_port,size,ttl,syn,ack,fin,urg,rst,psh,dnf);
                    } else {
                        // nothing selected fallback to ICMP
                        packet = icmp(src_ip,dst_ip,size,ttl,dnf);
                    }
                }

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
