# UPing
## Universal IPv4 and IPv6 Packet generator

UPing originated as a product of academic research projects in which I participated. Academics and security researchers have long struggled with the lack of a unified testing tool for IPv4 and IPv6 communication. The most commonly used tool by the community, hping3, does not allow for the generation of IPv6 packets, often necessitating custom solutions for packet generation. UPing enables the generation of ICMP, UDP, and TCP packets for both IPv4 and IPv6 networks.

## Disclaimer
This program is intended solely for educational and academic purposes. The creator of this program does not assume any responsibility for any misuse, damage, or consequences that may arise from its use. By using this program, you agree that you are responsible for ensuring its application complies with all relevant laws, regulations, and policies. The creator disclaims any liability for any actions taken based on the information provided by this program. Use at your own risk.

## Installation

UPing requires libtins 4.5 and Boost

Ubuntu/Debian/WSL2 Example
```bash
apt-get install libtins-dev libboost-all-dev g++ cmake
```

MacOS example
```bash
brew install libtins 
```


### Building from source
#### Download repository
```bash
git clone https://github.com/dhrinkino/uping.git
```
#### Building
```bash
cd uping; mkdir build; cd build ; cmake .. && make 
```
#### Copy to bin folder (Optional)
```bash
cp uping /usr/local/bin
```

## Examples 

IPv4 ICMP Flood with interval 10ms 
```bash
uping --src_ip=192.168.0.2 --dst_ip=192.168.0.1 --interval=10 --icmp -v 
```

IPv6 ICMPv6 Flood with interval 10ms
```bash
uping --src_ip=2001:db8::1:0:0:1 --dst_ip=2001:db8::1:0:0:2 --interval=10 --icmp --ipv6 -v 
```

IPv4 UDP Flood from source port 100 and destination Port 500 with interval 10ms
```bash
uping --src_ip=192.168.0.2 --dst_ip=192.168.0.1 --interval=10 --udp --src_port=100 --dst_port=500 -v
```

IPv6 UDP Flood from source port 100 and destination Port 500 with interval 10ms
```bash
uping --src_ip=2001:db8::1:0:0:1 --dst_ip=2001:db8::1:0:0:2 --interval=10 --udp --src_port=100 --dst_port=500 --ipv6 -v

```
IPv4 TCP Flood from source port 100 and destination Port 500 with interval 10ms
```bash
uping --src_ip=192.168.0.2 --dst_ip=192.168.0.1 --interval=10 --tcp --src_port=100 --dst_port=500 --ipv6 -v 

```
IPv6 TCP Flood from source port 100 and destination Port 500 with interval 10ms
```bash
uping --src_ip=2001:db8::1:0:0:1 --dst_ip=2001:db8::1:0:0:2 --interval=10 --tcp --src_port=100 --dst_port=500 --ipv6 -v 
```

IPv6 TCP Flood from source port 100 and destination Port 500 with interval 10ms and random source
```bash
uping --src_ip=2001:db8::1:0:0:1 --dst_ip=2001:db8::1:0:0:2 --interval=10 --tcp --src_port=100 --dst_port=500 --ipv6 -v --random
```
15 TCP sessions connected to 192.168.2.1:22 with keepalive interval 5 seconds
```bash
uping --session --dst-ip=192.168.2.1 --dst-port=22 --keepalive-interval=5 --num-connection=15
```

# Options and Arguments

| Argument                        | Description                                                                                   | Value Type          |
|---------------------------------|-----------------------------------------------------------------------------------------------|---------------------|
| `--fast`                        | Enables fast mode, bypassing the interval between packets.                                    | Flag                |
| `--faster`                      | Enables an even faster mode, bypassing the interval and not printing anything to the console. | Flag                |
| `--interval=`                   | Sets the interval between packets (in ms).                                                    | Time in ms          |
| `--iface=`                      | Set output interface (optional, but required when using link-local address).                  | Name of Interface   |
| `--ttl=`                        | Set custom TTL value (default: 64).                                                           | Integer             |
| `--tcp`                         | Sends TCP packets.                                                                            | Flag                |
| `--syn`                         | Set SYN Flag to TCP packet (works only with --tcp)                                            | Flag                |
| `--ack`                         | Set ACK Flag to TCP packet (works only with --tcp)                                            | Flag                |
| `--fin`                         | Set FIN Flag to TCP packet (works only with --tcp)                                            | Flag                |
| `--urg`                         | Set URG Flag to TCP packet (works only with --tcp)                                            | Flag                |
| `--rst`                         | Set RST Flag to TCP packet (works only with --tcp)                                            | Flag                |
| `--psh`                         | Set PUSH Flag to TCP packet (works only with --tcp)                                           | Flag                |
| `--ipv6`                        | Uses IPv6 (default is IPv4).                                                                  | Flag                |
| `--udp`                         | Sends UDP packets.                                                                            | Flag                |
| `--icmp`                        | Sends ICMP packets.                                                                           | Flag                |
| `--size=`                       | Sets the size of the payload.                                                                 | Size in bytes       |
| `--do-not-fragment`             | Set Do Not Fragment flag (IPv4 only)                                                          | Flag                |
| `--src_ip=` `--src-ip=`         | Sets the source IP address (IPv4 or IPv6 if `--ipv6` is used).                                | IPv4/IPv6 Address   |
| `--dst_ip=`  `--dst-ip=`        | Sets the destination IP address (IPv4 or IPv6 if `--ipv6` is used).                           | IPv4/IPv6 Address   |
| `--src_port=` `--src-port=`     | Sets the source port.                                                                         | Port Number 1-65535 |
| `--dst_port=`  `--dst-port=`    | Sets the destination port.                                                                    | Port Number 1-65535 |
| `--random`                      | Generates a random IPv4 or IPv6 address for source.                                           | Flag                |
| `--uniq_random` `--uniq-random` | Generates a random source IPv4 or IPv6 address for each packet.                               | Flag                |
| `--timeout=`                    | Sets the timeout period in seconds, after which the program will end.                         | Time in seconds     |
| `--count=`                      | Sets the number of packets to be sent.                                                        | Integer             |
| `--wait=`                       | Sets the time (in seconds) after which packet generation will start.                          | Integer             |
| `--fry`                         | Runs 5 senders in parallel mode without any waiting.                                          | Flag                |
| `--session`                     | Make X number of TCP sessions with defined keepalive                                          | Flag                |
| `--keepalive_interval=`         | Keepalive interval in seconds                                                                 | Integer             |
| `--num_connection=`             | Number of parallel sessions                                                                   | Integer             |
| `-v`                            | Verbose mode, prints all set arguments.                                                       | Flag                |
| `--help`                        | Display help message                                                                          | Flag                |
