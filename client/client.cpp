#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

int main() {

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::string command;
    std::cout << "Enter command: ";
    std::getline(std::cin, command);

    if (send(sock, command.c_str(), command.size(), 0) == SOCKET_ERROR) {
        std::cerr << "Send failed" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    char buffer[1024] = {0};
    int bytesReceived = recv(sock, buffer, 1024, 0);
    if (bytesReceived > 0) {
        std::cout << "Server response: " << buffer << std::endl;
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}