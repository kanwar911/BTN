# Smart Home Network Design

## Network Architecture Overview

The smart home system is designed with a segmented network architecture that separates different types of devices into distinct subnets for enhanced security, management, and performance. This design enables proper isolation between device categories while maintaining connectivity through strategic routing.

## Subnet Design

### Subnet Allocation (VLSM Implementation)

We've implemented Variable Length Subnet Masking (VLSM) to efficiently allocate IP addresses based on the number of expected devices in each category:

| Subnet Name   | Network Address | Subnet Mask     | CIDR Notation | Available Hosts | Device Types                   |
|---------------|-----------------|-----------------|---------------|-----------------|--------------------------------|
| Management    | 192.168.0.0     | 255.255.255.224 |       /27     |       30        | Servers, Controllers           |
| Lighting      | 192.168.0.32    | 255.255.255.224 |       /27     |       30        | Smart Lights, Light Sensors    |
| Climate       | 192.168.0.64    | 255.255.255.224 |       /27     |       30        | Thermostats, HVAC Systems      |
| Security      | 192.168.0.96    | 255.255.255.192 |       /26     |       62        | Cameras, Motion Sensors, Alarms|
| Entertainment | 192.168.0.160   | 255.255.255.224 |       /27     |       30        | Smart TVs, Audio Systems       |
| User Devices  | 192.168.0.192   | 255.255.255.192 |       /26     |       62        | Computers, Phones, Tablets     |

### IP Address Assignment Strategy

- Static IP Addresses: Critical infrastructure devices (server, gateway, primary controllers)
- DHCP with Reservations: Smart home devices with fixed functionality (lights, thermostats, cameras)
- Dynamic DHCP: User devices (phones, laptops, tablets)

## Routing Configuration

### Static Routing Tables

The smart home network utilizes static routing tables for predictable and efficient routing between subnets. Below is the routing table for the main gateway:

| Destination Network | Subnet Mask     | Next Hop | Interface |
|---------------------|-----------------|----------|-----------|
| 192.168.0.0         | 255.255.255.224 | -        | eth0      |
| 192.168.0.32        | 255.255.255.224 | -        | eth1      |
| 192.168.0.64        | 255.255.255.224 | -        | eth2      |
| 192.168.0.96        | 255.255.255.192 | -        | eth3      |
| 192.168.0.160       | 255.255.255.224 | -        | eth4      |
| 192.168.0.192       | 255.255.255.192 | -        | eth5      |
| 0.0.0.0 | 0.0.0.0   | 192.168.1.1     | wan0     |

###Default Routing

All devices within each subnet are configured with a default route pointing to the local gateway interface for that subnet. This simplifies device configuration while ensuring proper inter-subnet communication.

## ARP Implementation

The Address Resolution Protocol (ARP) is implemented to resolve IP addresses to MAC addresses within the local network:

### ARP Process Flow

1. When a device needs to communicate with another device, it first checks its ARP cache
2. If the MAC address isn't in the cache, the device broadcasts an ARP request
3. The device with the matching IP address responds with its MAC address
4. The requesting device updates its ARP cache and proceeds with communication

### ARP Cache Management

- Timeout: ARP cache entries expire after 300 seconds (5 minutes)
- Refresh: Cache entries are refreshed upon successful communication
- Dynamic Learning: New devices are automatically added to the ARP cache upon discovery

## Network Security Considerations

- Subnet Isolation: Prevents unauthorized access between device categories
- MAC Filtering: Additional layer of security for critical devices
- ARP Inspection: Prevents ARP spoofing attacks
- Firewall Rules: Inter-subnet communication is restricted based on need-to-communicate basis

This network design enables efficient and secure communication between all smart home devices while maintaining proper segmentation and control over network traffic. 