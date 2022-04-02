#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define HOST_PORT 2022
#define QUEUE_SIZE 5

int main() {
    int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDescriptor == -1) {
        std::cout << "Some error occur while creating socket." << std::endl;
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddress{}, clientAddress{};
    socklen_t clientAddressLen = sizeof(clientAddress);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(HOST_PORT);

    if (bind(socketDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cout << "Some error occur while binding socket." << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "The socket was bound and ready for communication..." << std::endl;

    if(listen(socketDescriptor, QUEUE_SIZE) != 0) {
        std::cout << "Failed to listen..." << std::endl;
        exit(EXIT_FAILURE);
    }

    while(true) {
        int connectionDescriptor = accept(
                socketDescriptor,
                (struct sockaddr*)&clientAddress,
                &clientAddressLen
        );

        if(connectionDescriptor == -1) {
            std::cout << "Some error occur while accepting connection.";
            exit(EXIT_FAILURE);
        }

        int messageMaxLength = 10;
        char message[messageMaxLength];

        long messageSize = read(connectionDescriptor, message, messageMaxLength);
        std::cout << "Message size " << messageSize << std::endl;
        if (messageSize == -1) {
            std::cout << "Some error occur while receiving the message." << std::endl;
            exit(EXIT_FAILURE);
        }
        message[messageSize] = '\0';
        std::cout << "Received message " << message << std::endl;

        float responseFloats [] = {1.334, 1.335, 1.336, 1.337};
        std::cout << "Sending response to the client: ";
        for (int i = 0; i < 4; i++) {
            std::cout << responseFloats[i] << " ";
        }

        if (write(connectionDescriptor, (char *)&responseFloats, sizeof(responseFloats)) == -1) {
            std::cout << "Some error occur while replying to the message." << std::endl;
            exit(EXIT_FAILURE);
        }

        std::cout << "\nWaiting for new communication!\n";
    }
}
