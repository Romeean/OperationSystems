#include <iostream>
#include <WS2tcpip.h>
#include <winsock2.h> 

using namespace std;

int main() {
  WSADATA wsaData;
  WSAStartup(MAKEWORD(2, 2), &wsaData);

  addrinfo hints, * result = nullptr;
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family   = AF_INET;     // IPv4
  hints.ai_socktype = SOCK_STREAM; // TCP 
  hints.ai_protocol = IPPROTO_TCP; 
  hints.ai_flags    = AI_PASSIVE; 

  getaddrinfo("127.0.0.1", "12345", &hints, &result);

  SOCKET customSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  bind(customSocket, result->ai_addr, (int)result->ai_addrlen);
  freeaddrinfo(result);

  listen(customSocket, SOMAXCONN);
  cout << "[SERVER] is on port 12345. Waiting..." << endl;


  while (true) {
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    SOCKET clientSocket = accept(customSocket, (sockaddr*)&clientAddr, &clientAddrSize);

    if (clientSocket != INVALID_SOCKET) {
      cout << "Client connected" << endl;

      char buffer[1024];
      int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

      if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        cout << "Received: " << buffer << endl;

        string msg = "Hello from Server!";
        send(clientSocket, msg.c_str(), (int)msg.length(), 0);
      }

      closesocket(clientSocket);
      cout << "> Client served and closed." << endl;
    }
  }

  closesocket(customSocket);
  WSACleanup();
  return 0;
}