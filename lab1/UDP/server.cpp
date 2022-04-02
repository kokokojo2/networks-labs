#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

#define HOST_PORT 2022


int main() {
    int socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketDescriptor == -1) {
        std::cout << "Some error occur while creating socket." << std::endl;
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddress{}, clientAddress{};
    int clientAddressLen = sizeof(clientAddress);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(HOST_PORT);

    if (bind(socketDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cout << "Some error occur while binding socket." << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "The socket was bound and ready for communication..." << std::endl;

    while(true) {
        int messageMaxLength = 10;
        char message[messageMaxLength];

        long messageSize = recvfrom(
            socketDescriptor,
            message,
            messageMaxLength,
            0,
            (struct sockaddr *)&clientAddress,
            (socklen_t *)(&clientAddressLen)
        );
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

        if (sendto(
            socketDescriptor,
            (char *)&responseFloats,
            sizeof(responseFloats),
            0,
            (struct sockaddr *)&clientAddress,
            clientAddressLen
                ) == -1) {
            std::cout << "Some error occur while replying to the message." << std::endl;
            exit(EXIT_FAILURE);
        }

        std::cout << "\nWaiting for new communication!\n";
    }
}
