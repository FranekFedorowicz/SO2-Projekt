#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#include <thread>
#include <string>

#pragma comment(lib, "ws2_32.lib") 

void receive_messages(SOCKET sock) {
    char buffer[1024];
    int bytesReceived;
    while ((bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytesReceived] = '\0'; 
        std::cout << "\n[Wiadomosc] " << buffer << "\nTy: "; 
        std::cout.flush();
    }
    std::cout << "\nRozlaczono z serwerem.\n"; 
}

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa); 

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);  
    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(8888); 
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    connect(sock, (sockaddr*)&server, sizeof(server)); 
    std::cout << "Polaczono z serwerem!\n";

    std::thread receiver(receive_messages, sock); 

    std::string input;
    while (true) {
        std::cout << "Ty: ";
        std::getline(std::cin, input); 
        if (input == "/exit") break;
        send(sock, input.c_str(), input.length(), 0); 
    }

    closesocket(sock);
    receiver.detach();  
    WSACleanup(); 
    return 0; 
}
