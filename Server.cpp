#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <string>

#pragma comment(lib, "ws2_32.lib") 

std::vector<SOCKET> clients; 
std::mutex clients_mutex; 


void broadcast(const std::string& message, SOCKET sender = INVALID_SOCKET) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (SOCKET client : clients) {
        if (client != sender) {
            send(client, message.c_str(), message.length(), 0);
        }
    }
}


void handle_client(SOCKET clientSocket) {
    char buffer[1024];
    int bytesReceived;
   
    while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytesReceived] = '\0'; 
        std::string msg = buffer;

        std::cout << "[Klient] " << msg << std::endl;
        broadcast("[Klient] " + msg, clientSocket); 
    }
  
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(std::remove(clients.begin(), clients.end(), clientSocket), clients.end());
    }
    closesocket(clientSocket);
    std::cout << "Klient rozlaczony.\n";
}

void server_chat() {
    std::string input;
    while (true) {
        std::getline(std::cin, input);
        if (input == "/exit") break; 

        std::string msg = "[Serwer] " + input; 
        broadcast(msg); 
    }
}

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

   
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY; 
    server.sin_port = htons(8888); 

    bind(serverSocket, (sockaddr*)&server, sizeof(server)); 
    listen(serverSocket, 5); 

    std::cout << "Serwer dziala. Oczekiwanie na klientow...\n";

    std::thread chatThread(server_chat); 

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr); 
        {
            std::lock_guard<std::mutex> lock(clients_mutex); 
            clients.push_back(clientSocket); 
        }
        std::thread(handle_client, clientSocket).detach(); 
        std::cout << "Nowy klient polaczony.\n";
    }

    closesocket(serverSocket);
    WSACleanup(); 
    return 0;
}
