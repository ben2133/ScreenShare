#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>

const int PORT = 8080;
const int BUFFER_SIZE = 1024;


void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    read(client_socket, buffer, BUFFER_SIZE);
    std::cout << "Received: " << buffer << std::endl;
    
    const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, C++!";
    write(client_socket, response, strlen(response));
    close(client_socket);
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (sockaddr*)&address, sizeof(address));
    listen(server_fd, 5);

    std::cout << "Server started on port " << PORT << std::endl;

    while (true) {
        int client_socket = accept(server_fd, nullptr, nullptr);
        std::thread(handle_client, client_socket).detach();
    }
}