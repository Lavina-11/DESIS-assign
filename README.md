# DESIS-assignments

## Assignment - 1
All the explanation is given along with the code in comments

## Assignment - 2

This is a simple **multi-user chat system** where multiple users can chat with each other in real-time using a server-client model. Here’s how the system works:

**NOTE:**Before compiling make sure you have lates version i.e. mingw64 or later because only it can handle threads
---

### **1. Server Side:**

- **Start the Server**: The **server** listens for incoming client connections on a specific port (port 8080).
  
- **Handle Client Connections**: The server accepts connections from multiple clients. When a client connects, the server asks the client for their **username**.

- **Manage Clients**: The server keeps track of all connected clients by storing their **socket** and **username**.

- **Handle Messages**:
  - **Broadcasting**: When a client sends a message, the server broadcasts it to **all other clients** (except the sender).
  - **Private Messaging**: If a client sends a message starting with `/msg <username>`, the server checks for the specified username and sends the message only to that client.
  
- **Log Messages**: All messages (both private and broadcast) are saved in a **log file** (`chat_log.txt`) for record-keeping.

- **Client Disconnect**: When a client disconnects, the server stops listening to that client and removes them from the list of active clients.

---

### **2. Client Side:**

- **Connect to the Server**: The client connects to the **server** using the server's IP address and port number (localhost and port 8080).

- **Provide Username**: Once connected, the client is prompted to **enter a username**.

- **Send Messages**:
  - **Broadcasting**: The client can send a regular message to **all clients** (e.g., "Hello everyone!").
  - **Private Messaging**: To send a private message to another user, the client types `/msg <username> <message>` (e.g., `/msg Bob Hi Bob!`).
  
- **Receive Messages**: The client can receive both **broadcast messages** and **private messages** directed at them.

- **Exit the Chat**: The client can exit the chat by typing `exit` or by simply Ctrl+C , which will disconnect them from the server.

---

### **3. Adding Another User (Client):**

To add another user to the chat, simply open another terminal window or shell and run the **client** program again. This simulates a new user connecting to the server.

---
The feature of saving the chat may have some issue. I am working on them so far
