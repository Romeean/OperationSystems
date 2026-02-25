#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <winsock2.h>


using namespace std;

int main() {
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    cout << "WSAStartup failed" << endl;
    return 1;
  }

  addrinfo hints, * result = nullptr;
  ZeroMemory(&hints, sizeof(hints));

  hints.ai_family = AF_INET;       // IPv4
  hints.ai_socktype = SOCK_STREAM; // TCP

  if (getaddrinfo("127.0.0.1", "12345", &hints, &result) != 0) {
    cout << "getaddrinfo failed" << endl;
    WSACleanup();
    return 1;
  }

  SOCKET connectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  if (connectSocket == INVALID_SOCKET) {
    cout << "Socket creation failed" << endl;
    freeaddrinfo(result);
    WSACleanup();
    return 1;
  }

  if (connect(connectSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
    cout << "Unable to connect to server!" << endl;
    closesocket(connectSocket);
    freeaddrinfo(result);
    WSACleanup();
    return 1;
  }

  string message = "Hello server. I am your client.";
  send(connectSocket, message.c_str(), (int)message.length(), 0);

  char buffer[1024];
  int bytesReceived = recv(connectSocket, buffer, sizeof(buffer) - 1, 0);

  if (bytesReceived > 0) {
    buffer[bytesReceived] = '\0';
    cout << "[CLIENT] Server echoed: " << buffer << endl;
  }

  freeaddrinfo(result);

  closesocket(connectSocket);
  WSACleanup();

  system("pause");
  return 0;
}