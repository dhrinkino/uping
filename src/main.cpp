#include <iostream>
#include <tins/tins.h>
#include <thread>
#include <cstdlib>
#include <limits>

#include "configure.h"
#include "debug.h"
#include "icmpv6.h"
#include "tcp6.h"
#include "udp6.h"
#include "tcp.h"
#include "icmp.h"
#include "udp.h"
#include "randomIPv4.h"
#include "randomIPv6.h"
#include "help.h"

using namespace Tins;


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
    Config config;
    std::vector<std::thread> threads;
    PacketSender sender;
    IP packet;
    IPv6 packet6;
    int counter = 0;
    long long end_time = std::numeric_limits<long long>::max();
    configure(argc,argv,config);

    if (config.src_ip.empty()) {
        if (config.is_ipv6) {
            config.src_ip = randomIPv6();
        } else {
            config.src_ip = randomIPv4();
        }
    }

    if (config.dst_ip.empty()) {
        help();
        return 1;
    }

    if (config.debug)
    {
        debug(config);
    }

    // packet building

    if (config.is_ipv6) {
        if (config.is_random) {
            config.src_ip = randomIPv6();
        }
        if (config.is_udp) {
            packet6 = udp6(config.src_ip,config.dst_ip,config.src_port,config.dst_port,config.size, config.ttl);
        } else if (config.is_tcp) {
            packet6 = tcp6(config.src_ip,config.dst_ip,config.src_port,config.dst_port,config.size,config.ttl,config.syn,config.ack,config.fin,config.urg,config.rst,config.psh);
        } else {
            // nothing selected fallback to ICMP
            packet6 = icmpv6(config.src_ip,config.dst_ip,config.size,config.ttl);
        }
    } else {
        if (config.is_random) {
            config.src_ip = randomIPv4();
        }
        if (config.is_udp) {
            packet = udp(config.src_ip,config.dst_ip,config.src_port,config.dst_port,config.size,config.ttl,config.dnf);
        } else if (config.is_tcp) {
            packet = tcp(config.src_ip,config.dst_ip,config.src_port,config.dst_port,config.size,config.ttl,config.syn,config.ack,config.fin,config.urg,config.rst,config.psh,config.dnf);
        } else {
            // nothing selected fallback to ICMP
            packet = icmp(config.src_ip,config.dst_ip,config.size,config.ttl,config.dnf);
        }
    }

    // packet generator
    if (config.fry) {
        if (config.is_ipv6){
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

        if (config.timeout > 0) {
            // calculate end time
            end_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() + config.timeout;
        }

        if (config.is_ipv6) {
            while (true) {
                if (config.is_random_uniq) {
                    config.src_ip = randomIPv6();
                    if (config.is_udp) {
                        packet6 = udp6(config.src_ip,config.dst_ip,config.src_port,config.dst_port,config.size,config.ttl);
                    } else if (config.is_tcp) {
                        packet6 = tcp6(config.src_ip,config.dst_ip,config.src_port,config.dst_port,config.size,config.ttl,config.syn,config.ack,config.fin,config.urg,config.rst,config.psh);
                    } else {
                        // nothing selected fallback to ICMP
                        packet6 = icmpv6(config.src_ip,config.dst_ip,config.size,config.ttl);
                    }
                }
                if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() > end_time) {
                    exit(0);
                }
                if (counter > config.count && config.count != 0) {
                    exit(0);
                }
                if (config.fast) {
                    printf(".");
                    fflush(stdout);
                }else if (config.faster) {
                    //
                } else {
                    std::this_thread::sleep_for(std::chrono::microseconds(config.interval));
                    printf(".");
                    fflush(stdout);
                }
                sender.send(packet6);
            }
        } else {
            while (true) {

                if (config.is_random_uniq) {
                        config.src_ip = randomIPv4();
                    if (config.is_udp) {
                        packet = udp(config.src_ip,config.dst_ip,config.src_port,config.dst_port,config.size,config.ttl,config.dnf);
                    } else if (config.is_tcp) {
                        packet = tcp(config.src_ip,config.dst_ip,config.src_port,config.dst_port,config.size,config.ttl,config.syn,config.ack,config.fin,config.urg,config.rst,config.psh,config.dnf);
                    } else {
                        // nothing selected fallback to ICMP
                        packet = icmp(config.src_ip,config.dst_ip,config.size,config.ttl,config.dnf);
                    }
                }

                if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() > end_time) {
                    exit(0);
                }
                if (counter > config.count && config.count != 0) {
                    exit(0);
                }
                if (config.fast) {
                    printf(".");
                    fflush(stdout);
                }else if (config.faster) {
                    //
                } else {
                    std::this_thread::sleep_for(std::chrono::microseconds(config.interval));
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
