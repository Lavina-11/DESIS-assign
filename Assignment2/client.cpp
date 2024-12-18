#include <iostream>
#include <winsock2.h>
#include <fstream>
#include <thread>
#include <string>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

void receive_messages(SOCKET server_socket) {
    char buffer[1024];
    int bytes_received;

    while (true) {
        bytes_received = recv(server_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            cerr << "Server disconnected." << endl;
            break;
        }

        buffer[bytes_received] = '\0';
        string message = "Server: " + string(buffer);

        // Log the message to a file
        ofstream log_file("chat_log.txt", ios::app);
        log_file << message << endl;
        log_file.close();

        cout << message << endl;
    }
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8080);

    if (connect(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        cerr << "Connection failed." << endl;
        return 1;
    }

    cout << "Connected to server." << endl;

    // Start receiving messages in a separate thread
    thread(receive_messages, server_socket).detach();

    string message;
    while (true) {
        cout << "Enter message (or /msg <username> <message> for private chat): ";
        getline(cin, message);

        if (message == "exit") {
            break;
        }

        // Send the message to the server
        send(server_socket, message.c_str(), message.length(), 0);
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}
