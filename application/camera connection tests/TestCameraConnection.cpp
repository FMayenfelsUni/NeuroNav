#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "your_camera_ip_address"
#define SERVER_PORT 8765 // Change to your camera's port
#define BUFFER_SIZE 1024 // Adjust buffer size as needed

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Create a TCP socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    // Set up the server address struct
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        return -1;
    }

    // Now you can send and receive data with the camera
    char message[] = "Hello, camera!";
    send(sock, message, strlen(message), 0);

    char buffer[BUFFER_SIZE] = { 0 };
    int valread;

    // Receive response from the camera
    valread = recv(sock, buffer, BUFFER_SIZE, 0);
    if (valread < 0) {
        std::cerr << "Error receiving data from camera" << std::endl;
    }
    else if (valread == 0) {
        std::cerr << "Connection closed by camera" << std::endl;
    }
    else {
        std::cout << "Camera response: " << buffer << std::endl;
    }

    // Close the socket when done
    close(sock);

    return 0;
}
