# Application Layer Protocol Design

## Overview

The smart home system implements a simplified HTTP-like application layer protocol for communication between clients and the server. This protocol is designed to be lightweight, human-readable, and follow RESTful principles for intuitive device control.

## Protocol Structure

### Request Format

All client requests follow this general format:


METHOD /resource/action[/parameter]


Where:
- METHOD: The operation to perform (currently only GET is implemented)
- /resource: The device or subsystem to interact with (e.g., light, thermostat, camera)
- /action: The specific action to take on the resource (e.g., on, off, status, set)
- /parameter: Optional parameters for the action (e.g., temperature value)

### Response Format

Server responses are simple string messages indicating the result of the operation:


[Status/Confirmation Message]


Examples:
- Light turned ON
- Current temperature: 22°C
- Camera is ACTIVE
- 400 Bad Request

## Command Examples

### Lighting Control

| Command           | Description              | Response Example |
|-------------------|--------------------------|------------------|
| GET /light/on     | Turn on the lights       | Light turned ON  |
| GET /light/off    | Turn off the lights      | Light turned OFF |
| GET /light/status | Get current light status | Light is ON or Light is OFF |

### Thermostat Control

| Command                | Description             | Response Example          |
|------------------------|-------------------------|---------------------------|
| GET /thermostat/status | Get current temperature | Current temperature: 22°C |
| GET /thermostat/set/25 | Set temperature to 25°C | Thermostat set to 25°C    |

### Security System

| Command            | Description       | Response Example                      |
|--------------------|-------------------|---------------------------------------|
| GET /camera/status | Get camera status | Camera is ACTIVE or Camera is OFFLINE |

## Protocol Implementation

The protocol is implemented in the server's processCommand function which parses incoming commands and routes them to the appropriate device handlers:

cpp

std::string processCommand(const std::string& command) {
    // Normalize command formatting
    std::string normalizedCmd = command;
    size_t pos = normalizedCmd.find("GET/");
    if (pos != std::string::npos) {
        normalizedCmd.insert(pos + 3, " ");
    }
    
    // Route command to appropriate handler
    if (normalizedCmd == "GET /light/on") 
        return light.turnOn();
    else if (normalizedCmd == "GET /light/off") 
        return light.turnOff();
    else if (normalizedCmd == "GET /light/status") 
        return light.status();
    else if (normalizedCmd.find("GET /thermostat/set/") == 0) {
        int temp = std::stoi(normalizedCmd.substr(20));
        return thermostat.setTemperature(temp);
    }
    else if (normalizedCmd == "GET /thermostat/status") 
        return thermostat.status();
    else if (normalizedCmd == "GET /camera/status") 
        return camera.status();
    else 
        return "400 Bad Request";
}


## Protocol Extensions

The current protocol can be extended in several ways:

### Additional HTTP Methods

Introduce other HTTP methods for a more complete RESTful API:

| Method | Description             | Example                  |
|--------|-------------------------|--------------------------|
| GET    | Retrieve resource state | GET /light/status        |
| POST   | Create new resource     | POST /schedule/new       |
| PUT    | Update resource         | PUT /thermostat/schedule |
| DELETE | Remove resource         | DELETE /schedule/1       |

### Response Status Codes

Implement formal HTTP-like status codes:

| Code | Description        | Example                   |
|------|--------------------|---------------------------|
| 200  | Success            | 200 OK                    |
| 400  | Bad Request        | 400 Bad Request           |
| 404  | Resource Not Found | 404 Device Not Found      |
| 500  | Server Error       | 500 Internal Server Error |

### Authentication

Add authentication headers for secure access:


GET /light/on
Authorization: Bearer [token]

### JSON Response Format

Implement structured JSON responses for richer data exchange:

json
{
  "status": "success",
  "code": 200,
  "data": {
    "device": "thermostat",
    "temperature": 22,
    "mode": "heating",
    "humidity": 45
  }
}


## Conclusion

The implemented HTTP-like protocol provides a simple yet effective way for clients to communicate with the smart home server. The RESTful design makes it intuitive for users to control devices through clear resource/action patterns. While the current implementation is basic, it establishes a solid foundation that can be extended with additional features as the system evolves. 