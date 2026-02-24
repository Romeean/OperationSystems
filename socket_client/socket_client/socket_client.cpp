#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
  // 1. Инициализация Winsock
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    cout << "WSAStartup failed" << endl;
    return 1;
  }

  // 2. Настройка адреса (куда подключаемся)
  addrinfo hints, * result = nullptr;
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_INET;       // IPv4
  hints.ai_socktype = SOCK_STREAM; // TCP

  // Подключаемся к самому себе (127.0.0.1) на порт сервера 12345
  if (getaddrinfo("127.0.0.1", "12345", &hints, &result) != 0) {
    cout << "getaddrinfo failed" << endl;
    WSACleanup();
    return 1;
  }

  // 3. Создание сокета
  SOCKET connectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  if (connectSocket == INVALID_SOCKET) {
    cout << "Socket creation failed" << endl;
    freeaddrinfo(result);
    WSACleanup();
    return 1;
  }

  // 4. Соединение с сервером (Connect)
  if (connect(connectSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
    cout << "Unable to connect to server!" << endl;
    closesocket(connectSocket);
    freeaddrinfo(result);
    WSACleanup();
    return 1;
  }

  // Адрес больше не нужен, мы уже подключены
  freeaddrinfo(result);

  // 5. Отправка сообщения серверу
  string message = "Hello, server! I am your client.";
  send(connectSocket, message.c_str(), (int)message.length(), 0);
  cout << "[CLIENT] Message sent: " << message << endl;

  // 6. Получение ответа от сервера
  char buffer[1024];
  int bytesReceived = recv(connectSocket, buffer, sizeof(buffer) - 1, 0);

  if (bytesReceived > 0) {
    buffer[bytesReceived] = '\0'; // Закрываем строку
    cout << "[CLIENT] Server echoed: " << buffer << endl;
  }

  // 7. Завершение работы
  closesocket(connectSocket);
  WSACleanup();

  system("pause"); // Чтобы консоль не закрылась сразу
  return 0;
}