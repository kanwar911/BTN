# Smart Home Automation System

A comprehensive network-based smart home control system implementing a client-server architecture with multithreading, custom application layer protocol, and network design principles.

## Project Overview

This project implements a smart home automation system allowing clients to control various home devices (lights, thermostats, cameras) over a network. The system features:

- Client-Server Architecture: TCP/IP-based communication
- Multithreaded Server: Handles multiple client connections simultaneously
- Custom Application Protocol: HTTP-like protocol for device control
- Network Design: Subnetting, routing, and ARP implementation
- Device Control: Lights, thermostats, security cameras
- Web-Based Control Panel: Modern UI for browser-based device control

## Repository Structure


/
├── client/             # Client application
│   └── client.cpp      # Client implementation
│
├── server/             # Server application 
│   ├── main.cpp        # Server implementation
│   ├── light.h         # Light device implementation
│   ├── thermostat.h    # Thermostat device implementation
│   ├── camera.h        # Camera device implementation
│   └── web_server.h    # Web server implementation
│
├── diagram/            # Network and architecture diagrams
│   └── network_diagram.txt  # ASCII network topology diagram
│
├── report/             # Project documentation
│   ├── network_design.md    # Network design documentation
│   ├── threading_design.md  # Multithreading implementation details
│   ├── protocol_design.md   # Application protocol specification
│   └── implementation_status.md # Status report of requirements
│
├── client.exe          # Compiled client application
├── server.exe          # Compiled server application
└── README.md           # This file


## Network Design

The system implements a segmented network architecture with:

- Subnet Allocation: Uses Variable Length Subnet Masking (VLSM)
- Device Categorization: Separates devices into logical subnets
- Static Routing: Routes traffic between device subnets
- ARP Implementation: Resolves IP addresses to MAC addresses

See the detailed [Network Design Documentation](report/network_design.md) for more information.

## Multithreading & Concurrency

The server implements a multithreaded architecture for:

- Handling multiple concurrent client connections
- Managing device operations
- Ensuring thread safety for shared resources

See the [Threading Design Documentation](report/threading_design.md) for implementation details.

## Application Protocol

The system uses a custom HTTP-like protocol for client-server communication:

- Request Format: METHOD /resource/action[/parameter]
- Response Format: Text-based status/confirmation messages
- Example: GET /light/on → Light turned ON

See the [Protocol Design Documentation](report/protocol_design.md) for the complete specification.

## Web-Based Control Panel

The system includes a modern web interface for controlling smart home devices:

- Responsive Design: Works on both desktop and mobile browsers
- Intuitive UI: Card-based layout with visual status indicators
- Real-Time Controls: Dynamic updates with loading animations
- Interactive Elements: Slider control for thermostat temperature
- Notification System: Visual feedback for user actions

The web interface runs on port 8081 and provides the same functionality as the command-line client but with a user-friendly graphical interface.

## Setup & Compilation

### Prerequisites

- Windows operating system
- MinGW-w64 with GCC 6.3.0 or later
- WinSock2 networking libraries

### Compilation

Compile the server:

g++ server/main.cpp -o server.exe -lws2_32


Compile the client:

g++ client/client.cpp -o client.exe -lws2_32


## Usage

### Command-Line Interface

1. Start the server:
   
   .\server.exe
   
   The server will display: "Device server is running on port 8080..." and "Web server is running on port 8081..."

2. Run the client:
   
   .\client.exe
   

3. Enter commands when prompted. Available commands:

   - Lighting:
     - GET /light/on - Turn on lights
     - GET /light/off - Turn off lights
     - GET /light/status - Check light status

   - Thermostat:
     - GET /thermostat/set/25 - Set temperature to 25°C
     - GET /thermostat/status - Check current temperature

   - Security:
     - GET /camera/status - Check camera status

4. The server will respond with the current status or confirmation of the action.

### Web Interface

1. Start the server (if not already running):
   
   .\server.exe
   

2. Open a web browser and navigate to:
   
   http://localhost:8081
   

3. Use the graphical interface to control your smart home devices:
   - Click buttons to control lights
   - Use the slider to set the thermostat temperature
   - View device status in real-time

## Implementation Details

### Client

The client application establishes a TCP connection to the server, sends command requests, and displays the server's responses.

### Server

The server accepts client connections, processes commands using a multithreaded approach, and manages the state of all connected smart home devices.

### Web Server

The web server provides an HTTP interface allowing browsers to connect and control devices through a graphical user interface.

### Device Classes

- Light: Controls lighting (on/off functionality)
- Thermostat: Controls temperature settings
- Camera: Monitors security camera status


## Author
Kanwar Singh Sandhu & Shashini Amasha Ranasinghe
=======
