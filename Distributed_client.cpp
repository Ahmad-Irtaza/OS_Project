#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <thread>

#define SERVER_IP "192.168.1.100" // Replace with the server's IP address
#define PORT 8080

// Function to receive messages from the server
void receive_messages(int socket_fd) {
    char buffer[1024];
    int read_size;

    while ((read_size = read(socket_fd, buffer, sizeof(buffer))) > 0) {
        buffer[read_size] = '\0';
        std::cout << "Message from server: " << buffer << std::endl;
    }
}

// Main client function
int main() {
    int socket_fd;
    struct sockaddr_in server_addr;

    // Create the client socket
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Set the server's IP address (replace with actual server IP)
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Connected to the server!" << std::endl;

    // Start a thread to listen for messages from the server
    std::thread(receive_messages, socket_fd).detach();

    // Send messages to the server
    char message[1024];
    while (true) {
        std::cout << "Enter message: ";
        std::cin.getline(message, sizeof(message));

        if (send(socket_fd, message, strlen(message), 0) < 0) {
            perror("Send failed");
            break;
        }
    }

    close(socket_fd);
    return 0;
}
