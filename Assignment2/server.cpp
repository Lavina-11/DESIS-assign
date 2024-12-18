#include <iostream>
#include <winsock2.h>
#include <fstream>
#include <thread>
#include <algorithm>
#include <vector>
#include <string>
#include <map>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

struct ClientInfo {
    SOCKET socket;
    string username;
};

vector<ClientInfo> clients;

void handle_client(ClientInfo client_info) {
    char buffer[1024];
    int bytes_received;
    string message;

    while (true) {
        bytes_received = recv(client_info.socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            cerr << "Client disconnected: " << client_info.username << endl;
            closesocket(client_info.socket);
            break;
        }

        buffer[bytes_received] = '\0';
        message = string(buffer);

        // If message starts with "/msg", send private message to a specific user
        if (message.substr(0, 5) == "/msg ") {
            size_t pos = message.find(' ', 5);
            if (pos != string::npos) {
                string recipient = message.substr(5, pos - 5);
                string msg_content = message.substr(pos + 1);

                // Find the recipient client
                bool found = false;
                for (auto& client : clients) {
                    if (client.username == recipient) {
                        send(client.socket, ("Private message from " + client_info.username + ": " + msg_content).c_str(), msg_content.length(), 0);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    send(client_info.socket, "Recipient not found.\n", 21, 0);
                }
            }
        } else {
            // Broadcast message to all clients except the sender
            for (auto& client : clients) {
                if (client.socket != client_info.socket) {
                    string full_msg = client_info.username + ": " + message;
                    send(client.socket, full_msg.c_str(), full_msg.length(), 0);
                }
            }
        }

        // Log the message
        ofstream log_file("chat_log.txt", ios::app);
        log_file << client_info.username << ": " << message << endl;
        log_file.close();
    }

    // Remove the client from the list after disconnecting
    clients.erase(remove_if(clients.begin(), clients.end(), [&client_info](ClientInfo& client) {
        return client.socket == client_info.socket;
    }), clients.end());
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, 5);

    cout << "Server listening on port 8080..." << endl;

    while (true) {
        SOCKET client_socket = accept(server_socket, NULL, NULL);
        if (client_socket != INVALID_SOCKET) {
            cout << "Client connected." << endl;

            // Ask the new client for their username
            char buffer[1024];
            send(client_socket, "Enter your username: ", 21, 0);
            int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
            buffer[bytes_received] = '\0';

            string username = string(buffer);

            // Add client to the clients list
            ClientInfo new_client = { client_socket, username };
            clients.push_back(new_client);

            thread(handle_client, new_client).detach();
        }
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}
