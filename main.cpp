#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctime>
#include <sstream>
#include <iomanip>

std::string getCurrentTimeHTTPFormat() {
    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::gmtime(&now);
    std::stringstream ss;
    ss << std::put_time(timeinfo, "%a, %d %b %Y %H:%M:%S GMT");
    return ss.str();
}

int main() {
    // Create socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Bind and listen
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);
    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    listen(serverSocket, 5);

    std::cout<<"Listening..."<<std::endl;

    size_t visit_counter=0;

    while (true) {
        // Accept connection
        int clientSocket = accept(serverSocket, nullptr, nullptr);

        // Receive HTTP request
        char buffer[1024] = {0};
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        std::cout<<buffer<<std::endl;

        // Parse request, handle routing, and generate response
        std::string lastModified=getCurrentTimeHTTPFormat();

        // Construct HTTP response
        std::string response = "HTTP/1.1 200 OK\r\n"
                               "Content-Type: text/html\r\n"
                               "Cache-Control: max-age=3600\r\n" // Cache for 1 hour
                               "Last-Modified: " + lastModified + "\r\n" // Set the last modified time
                               "\r\n"
                               "<html><body>"
                               "<h1>Goodbye, cruel World!</h1>"
                               "<h2>Visit counter: "+std::to_string(++visit_counter)+"</h2>"
                               "</body></html>";

        // Send response
        send(clientSocket, response.c_str(), response.length(), 0);
        std::cout<<"Sent response..."<<std::endl;

        // Close connection
        close(clientSocket);
    }
    std::cout<<"Closing..."<<std::endl;

    // Close server socket
    close(serverSocket);
}
