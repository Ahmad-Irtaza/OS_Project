#include <iostream>
#include <vector>
#include <thread>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <algorithm>  // for std::remove

#define PORT 8085

// Function to handle communication with clients
void handle_client(int client_socket, std::vector<int>& clients) {
    char buffer[1024];
    int read_size;

    while ((read_size = read(client_socket, buffer, sizeof(buffer))) > 0) {
        buffer[read_size] = '\0';
        std::cout << "Received from client: " << buffer << std::endl;

        // Broadcast the message to all clients
        for (int client : clients) {
            if (client != client_socket) {
                send(client, buffer, strlen(buffer), 0);
            }
        }
    }

    // Remove client from the list when it disconnects
    close(client_socket);
    clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
}

// Main server function
int main() {
    int server_fd, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    std::vector<int> clients;

    // Create server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to a specific port
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    while (true) {
        // Accept incoming client connections
        if ((client_socket = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len)) < 0) {
            perror("Accept failed");
            continue;
        }

        std::cout << "New client connected" << std::endl;

        // Add client to the client list
        clients.push_back(client_socket);

        // Create a thread to handle client communication
        std::thread(handle_client, client_socket, std::ref(clients)).detach();
    }

    return 0;
}
