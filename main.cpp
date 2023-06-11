#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctime>
#include <sstream>
#include <iomanip>


int main() {
    const int PORT = 8080;

    // Initialize socket
    std::cout << "Initializing IPV4/TCP socket..." << std::endl;
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket != 0) {
        std::cerr << "Failed to initialize socket!" << std::endl;
        return 1;
    }

    // Bind and listen
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);
    if (bind(serverSocket, reinterpret_cast<sockaddr *>(&serverAddress), sizeof(serverAddress)) != 0) {
        std::cerr << "Failed to bind!" << std::endl;
        return 1;
    }

    int queueLength = 5;
    if (listen(serverSocket, queueLength) != 0) {
        std::cerr << "Failed to set socket to listen!" << std::endl;
        return 1;
    }

    std::cout << "Listening for requests on port " << PORT << " ..." << std::endl;

    while (true) {
        // Accept connection
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        std::cout << "Accepting connection...";
        if (clientSocket != 0) {
            std::cerr << "Failed to accept connection!" << std::endl;
            return 1;
        }

        // Receive HTTP request
        char buffer[1024] = {0};
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        std::cout << buffer << std::endl;

        // Parse request, handle routing, and generate response

        // Construct HTTP response
        std::string response = "HTTP/1.1 200 OK\r\n"
                               "Content-Type: text/html\r\n"
                               "\r\n"
                               "<html><body>"
                               "<h1>Goodbye, cruel World!</h1>"
                               "</body></html>";

        // Send response
        send(clientSocket, response.c_str(), response.length(), 0);
        std::cout << "Sent response..." << std::endl;

        // Close connection
        close(clientSocket);
    }
    std::cout << "Closing..." << std::endl;

    // Close server socket
    close(serverSocket);
}
