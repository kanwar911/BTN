#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "light.h"
#include "thermostat.h"
#include "camera.h"
#include "web_server.h"

#pragma comment(lib, "ws2_32.lib")

CRITICAL_SECTION mtx;
Light light;
Thermostat thermostat;
Camera camera;

std::string processCommand(const std::string& command) {
    EnterCriticalSection(&mtx);
    std::string result;
    
    std::string normalizedCmd = command;
    size_t pos = normalizedCmd.find("GET/");
    if (pos != std::string::npos) {
        normalizedCmd.insert(pos + 3, " ");
    }
    
    if (normalizedCmd == "GET /light/on") result = light.turnOn();
    else if (normalizedCmd == "GET /light/off") result = light.turnOff();
    else if (normalizedCmd == "GET /light/status") result = light.status();
    else if (normalizedCmd.find("GET /thermostat/set/") == 0) {
        int temp = std::stoi(normalizedCmd.substr(20));
        result = thermostat.setTemperature(temp);
    }
    else if (normalizedCmd == "GET /thermostat/status") result = thermostat.status();
    else if (normalizedCmd == "GET /camera/status") result = camera.status();
    else result = "400 Bad Request";
    
    LeaveCriticalSection(&mtx);
    return result;
}

DWORD WINAPI handleClient(LPVOID lpParam) {
    SOCKET client_socket = (SOCKET)lpParam;
    char buffer[1024] = {0};
    int bytesReceived = recv(client_socket, buffer, 1024, 0);
    if (bytesReceived > 0) {
        std::string response = processCommand(std::string(buffer));
        send(client_socket, response.c_str(), response.length(), 0);
    }
    closesocket(client_socket);
    return 0;
}

int main() {

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }

    InitializeCriticalSection(&mtx);

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        DeleteCriticalSection(&mtx);
        return 1;
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) == SOCKET_ERROR) {
        std::cerr << "setsockopt failed" << std::endl;
        closesocket(server_fd);
        WSACleanup();
        DeleteCriticalSection(&mtx);
        return 1;
    }

    sockaddr_in address{};
    int addrlen = sizeof(address);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Bind failed" << std::endl;
        closesocket(server_fd);
        WSACleanup();
        DeleteCriticalSection(&mtx);
        return 1;
    }

    if (listen(server_fd, 10) == SOCKET_ERROR) {
        std::cerr << "Listen failed" << std::endl;
        closesocket(server_fd);
        WSACleanup();
        DeleteCriticalSection(&mtx);
        return 1;
    }

    std::cout << "Device server is running on port 8080...\n";
    
    SOCKET web_server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (web_server_fd == INVALID_SOCKET) {
        std::cerr << "Web socket creation failed" << std::endl;
        closesocket(server_fd);
        WSACleanup();
        DeleteCriticalSection(&mtx);
        return 1;
    }
    
    if (setsockopt(web_server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) == SOCKET_ERROR) {
        std::cerr << "Web setsockopt failed" << std::endl;
        closesocket(server_fd);
        closesocket(web_server_fd);
        WSACleanup();
        DeleteCriticalSection(&mtx);
        return 1;
    }
    
    sockaddr_in web_address{};
    web_address.sin_family = AF_INET;
    web_address.sin_addr.s_addr = INADDR_ANY;
    web_address.sin_port = htons(8081);  
    
    if (bind(web_server_fd, (sockaddr*)&web_address, sizeof(web_address)) == SOCKET_ERROR) {
        std::cerr << "Web bind failed" << std::endl;
        closesocket(server_fd);
        closesocket(web_server_fd);
        WSACleanup();
        DeleteCriticalSection(&mtx);
        return 1;
    }
    
    if (listen(web_server_fd, 10) == SOCKET_ERROR) {
        std::cerr << "Web listen failed" << std::endl;
        closesocket(server_fd);
        closesocket(web_server_fd);
        WSACleanup();
        DeleteCriticalSection(&mtx);
        return 1;
    }
    
    std::cout << "Web server is running on port 8081...\n";
    std::cout << "Access the web control panel at http://localhost:8081\n";
    
    HANDLE webThread = CreateThread(NULL, 0, webServerThread, (LPVOID)web_server_fd, 0, NULL);
    if (webThread == NULL) {
        std::cerr << "Failed to create web server thread" << std::endl;
        closesocket(server_fd);
        closesocket(web_server_fd);
        WSACleanup();
        DeleteCriticalSection(&mtx);
        return 1;
    }

    while (true) {
        SOCKET client_socket = accept(server_fd, (sockaddr*)&address, &addrlen);
        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }
        CreateThread(NULL, 0, handleClient, (LPVOID)client_socket, 0, NULL);
    }

    closesocket(server_fd);
    closesocket(web_server_fd);
    DeleteCriticalSection(&mtx);
    WSACleanup();
    return 0;
}
