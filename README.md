# UPing
## Universal IPv4 and IPv6 Packet generator

UPing originated as a product of academic research projects in which I participated. Academics and security researchers have long struggled with the lack of a unified testing tool for IPv4 and IPv6 communication. The most commonly used tool by the community, hping3, does not allow for the generation of IPv6 packets, often necessitating custom solutions for packet generation. UPing enables the generation of ICMP, UDP, and TCP packets for both IPv4 and IPv6 networks.

## Installation

UPing requires libtins 4.5

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

# Options and Arguments

| Argument        | Description                                                                                   | Value Type          |
|-----------------|-----------------------------------------------------------------------------------------------|---------------------|
| `--fast`        | Enables fast mode, bypassing the interval between packets.                                    | Flag                |
| `--faster`      | Enables an even faster mode, bypassing the interval and not printing anything to the console. | Flag                |
| `--interval=`   | Sets the interval between packets (in ms).                                                    | Time in ms          |
| `--tcp`         | Sends TCP packets.                                                                            | Flag                |
| `--syn`         | Set SYN Flag to TCP packet (works only with --tcp)                                            | Flag                |
| `--ipv6`        | Uses IPv6 (default is IPv4).                                                                  | Flag                |
| `--udp`         | Sends UDP packets.                                                                            | Flag                |
| `--icmp`        | Sends ICMP packets.                                                                           | Flag                |
| `--size=`       | Sets the size of the payload.                                                                 | Size in bytes       |
| `--src_ip=`     | Sets the source IP address (IPv4 or IPv6 if `--ipv6` is used).                                | IPv4/IPv6 Address   |
| `--dst_ip=`     | Sets the destination IP address (IPv4 or IPv6 if `--ipv6` is used).                           | IPv4/IPv6 Address   |
| `--src_port=`   | Sets the source port.                                                                         | Port Number 1-65535 |
| `--dst_port=`   | Sets the destination port.                                                                    | Port Number 1-65535 |
| `--random`      | Generates a random IPv4 or IPv6 address for source.                                           | Flag                |
| `--uniq_random` | Generates a random source IPv4 or IPv6 address for each packet.                               | Flag                |
| `--timeout=`    | Sets the timeout period in seconds, after which the program will end.                         | Time in seconds     |
| `--count=`      | Sets the number of packets to be sent.                                                        | Integer             |
| `--fry`         | Runs 5 senders in parallel mode without any waiting.                                          | Flag                |
| `-v`            | Verbose mode, prints all set arguments.                                                       | Flag                |
| `--help`        | Display help message                                                                          | Flag                |
