#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include<arpa/inet.h>

#define SERVER_PORT 2022
#define SERVER_ADDRESS "127.0.0.1"


int main() {
    int socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketDescriptor == -1) {
        std::cout << "Some error occur while creating socket." << std::endl;
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    if (inet_aton(SERVER_ADDRESS , &serverAddress.sin_addr) == 0)
    {
        exit(EXIT_FAILURE);
    }

    char message[] = "abcdefghyz";
    std::cout << "Sending data..." << std::endl;

    if (sendto(
            socketDescriptor,
            message,
            strlen(message),
            0,
            (struct sockaddr *)&serverAddress,
            sizeof(serverAddress)
    ) == -1) {

        std::cout << "Some error occur while sending the message." << std::endl;
        exit(EXIT_FAILURE);
    }

    int responseMaxLength = sizeof (float) * 4;
    char receivedMessage [responseMaxLength];
    int len = sizeof(serverAddress);
    std::cout << "Sent" << std::endl;

    long messageSize = recvfrom(
        socketDescriptor,
        receivedMessage,
        responseMaxLength,
        0,
        (struct sockaddr *)&serverAddress,
        (socklen_t *)(&len)
    );

    if (messageSize == -1) {
        std::cout << "Some error occur while receiving the message." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Received response from server:" << std::endl;
    float* decodedFloats = (float *)receivedMessage;
    for (int i = 0; i < messageSize / 4; i++) {
        std::cout << decodedFloats[i] << " ";
    }
    return 0;
}
