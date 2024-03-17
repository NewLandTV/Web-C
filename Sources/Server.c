#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <WinSock2.h>
#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_LENGTH 1024
#define PORT 3000

int main(void)
{
    WSADATA wsaData;
    SOCKET listenSocket;
    SOCKET client;

    struct sockaddr_in address;
    struct sockaddr_in clientAddress;

    int i, j;
    int receiveLength = 0;
    int length;
    int socketAddressInSize = sizeof(struct sockaddr_in);
    unsigned char buffer[BUFFER_LENGTH];
    unsigned char path[BUFFER_LENGTH];
    unsigned char data[BUFFER_LENGTH];

    // Initialize Win Sock 2.2
    if (WSAStartup(MAKEWORD(2, 2), &wsaData))
    {
        puts("Failed WSAStartup.");
        getch();

        return 1;
    }

    // Create socket
    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (listenSocket == INVALID_SOCKET)
    {
        printf("Failed to create the listen socket : %d\n", WSAGetLastError());
        getch();

        return 1;
    }

    // Bind socket
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.S_un.S_addr = INADDR_ANY;

    if (bind(listenSocket, (struct sockaddr*)&address, sizeof(address)) != 0)
    {
        printf("Failed to bind : %d\n", WSAGetLastError());
        getch();

        return 1;
    }

    // Listen
    if (listen(listenSocket, 5) != 0)
    {
        printf("Failed to listen : %d\n", WSAGetLastError());
        getch();

        return 1;
    }

    // Accept loop
    while (1)
    {
        length = sizeof(clientAddress);
        client = accept(listenSocket, (struct sockaddr*)&clientAddress, &length);

        if (client == INVALID_SOCKET)
        {
            printf("Failed to accept : %d\n", WSAGetLastError());

            break;
        }

        // Clear buffers
        memset(path, 0, BUFFER_LENGTH);
        memset(data, 0, BUFFER_LENGTH);

        // Connect
        receiveLength = recvfrom(client, buffer, BUFFER_LENGTH, 0, (struct sockaddr*)&clientAddress, &socketAddressInSize);
        buffer[receiveLength - 1] = 0;

        if (buffer[0] == 0)
        {
            strcpy(buffer, NULL);
        }

        // Show serial
        printf("%s\n", buffer);

        // Method
        for (i = 0; i < i < strlen(buffer); i++)
        {
            // GET
            if (buffer[i] == 'G' && buffer[i + 1] == 'E' && buffer[i + 2] == 'T' && buffer[i + 3] == ' ')
            {
                for (j = 0; buffer[i + j + 4] != ' '; j++)
                {
                    path[j] = buffer[i + j + 4];
                }

                break;
            }

            // POST
            if (buffer[i] == 'P' && buffer[i + 1] == 'O' && buffer[i + 2] == 'S' && buffer[i + 3] == 'T' && buffer[i + 4] == ' ')
            {
                for (j = 0; buffer[i + j + 4] != ' '; j++)
                {
                    path[j] = buffer[i + j + 4];
                }

                break;
            }
        }

        printf("Request : %s\n", path);

        // HTTP
        unsigned char* header = "HTTP/1.0 200 OK\n"
                                "Content-type: text/html\n"
                                "\n";

        send(client, header, strlen(header), 0);

        // Routing
        if (strcmp(path, "/home") == 0)
        {
            strcpy(data,
                "<!DOCTYPE html>\n"
                "<html lang=\"ko\">\n"
                "<head>\n"
                "    <meta charset=\"utf-8\"\n"
                "</head>\n"
                "<body>\n"
                "    <h1>Home Page<h1>\n"
                "    <p>JkhTV</p>\n"
                "</body>\n"
                "</html>"
            );
        }
        else    // 404 Not found page
        {
            strcpy(data,
                "<!DOCTYPE html>\n"
                "<html lang=\"ko\">\n"
                "<head>\n"
                "    <meta charset=\"utf-8\"\n"
                "</head>\n"
                "<body>\n"
                "    <h1>404 Not Found!<h1>\n"
                "</body>\n"
                "</html>"
            );
        }

        // Response(Send HTML Data)
        if (send(client, data, strlen(data), 0) < 1)
        {
            printf("Failed to send : %d\n", WSAGetLastError());

            break;
        }

        // Close client
        closesocket(client);
    }

    // Close Win Sock
    closesocket(listenSocket);
    WSACleanup();

    return 0;
}
