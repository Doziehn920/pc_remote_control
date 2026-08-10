#include <iostream>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsa;
    SOCKET serverSocket, clientSocket;
    sockaddr_in server, client;

    int clientSize = sizeof(client);

    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Server setup
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    // Bind
    bind(serverSocket, (sockaddr*)&server, sizeof(server));

    // Listen
    listen(serverSocket, 1);

    std::cout << "Server started on port 8080..." << std::endl;

    // Accept connection
    clientSocket = accept(serverSocket, (sockaddr*)&client, &clientSize);

    std::cout << "Phone connected!" << std::endl;

    char buffer[1024] = {0};

    
while (true)
{
    SOCKET clientSocket = accept(serverSocket, (sockaddr*)&client, &clientSize);

    char buffer[4096] = {0};

    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesReceived > 0)
    {
        std::string request(buffer);

        std::cout << "\n========== REQUEST ==========\n";
        std::cout << request << std::endl;

        // COMMANDS
        if (request.find("GET /left") != std::string::npos)
        {
            POINT p;
            GetCursorPos(&p);

            SetCursorPos(p.x - 20, p.y);

            std::cout << "MOVE LEFT\n";
        }

        if (request.find("GET /right") != std::string::npos)
        {
            POINT p;
            GetCursorPos(&p);

            SetCursorPos(p.x + 20, p.y);

            std::cout << "MOVE RIGHT\n";
        }

        if (request.find("GET /up") != std::string::npos)
        {
            POINT p;
            GetCursorPos(&p);

            SetCursorPos(p.x, p.y - 20);

            std::cout << "MOVE UP\n";
        }

        if (request.find("GET /down") != std::string::npos)
        {
            POINT p;
            GetCursorPos(&p);

            SetCursorPos(p.x, p.y + 20);

            std::cout << "MOVE DOWN\n";
        }

        if (request.find("GET /click") != std::string::npos)
        {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

            std::cout << "CLICK\n";
        }

        // WEBPAGE RESPONSE
const char* response =
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html\r\n"
"\r\n"

"<html>"

"<head>"
"<title>PC Controller</title>"

"<style>"

"body {"
"  margin: 0;"
"  height: 100vh;"
"  display: flex;"
"  justify-content: center;"
"  align-items: center;"
"  background: linear-gradient(135deg, #1e3c72, #2a5298);"
"  font-family: Arial, sans-serif;"
"}"

".remote {"
"  background: rgba(255,255,255,0.1);"
"  backdrop-filter: blur(10px);"
"  border-radius: 25px;"
"  padding: 30px;"
"  box-shadow: 0 8px 32px rgba(0,0,0,0.3);"
"  text-align: center;"
"  color: white;"
"  width: 320px;"
"}"

"h1 {"
"  margin-bottom: 20px;"
"  font-size: 22px;"
"  letter-spacing: 1px;"
"}"

".btn {"
"  width: 90px;"
"  height: 90px;"
"  margin: 5px;"
"  font-size: 18px;"
"  font-weight: bold;"
"  border: none;"
"  border-radius: 15px;"
"  cursor: pointer;"
"  background: rgba(255,255,255,0.2);"
"  color: white;"
"  transition: 0.2s;"
"  box-shadow: 0 4px 10px rgba(0,0,0,0.3);"
"}"

".btn:hover {"
"  background: rgba(255,255,255,0.35);"
"  transform: scale(1.05);"
"}"

".btn:active {"
"  transform: scale(0.95);"
"}"

".row {"
"  display: flex;"
"  justify-content: center;"
"  align-items: center;"
"}"

"</style>"

"</head>"

"<body>"

"<div class='remote'>"

"<h1>PC Remote Control</h1>"

"<script>"
"let moveInterval;"

"function startMove(direction)"
"{"
"   moveInterval = setInterval(() => {"
"       fetch('/' + direction);"
"   }, 50);"
"}"

"function stopMove()"
"{"
"   clearInterval(moveInterval);"
"}"
"</script>"

"<!-- UP -->"
"<div class='row'>"
"<button class='btn' "
"ontouchstart=\"startMove('up')\" "
"ontouchend=\"stopMove()\">"
"▲"
"</button>"
"</div>"

"<!-- LEFT RIGHT -->"
"<div class='row'>"
"<button class='btn' "
"ontouchstart=\"startMove('left')\" "
"ontouchend=\"stopMove()\">"
"◀"
"</button>"

"<button class='btn' "
"onclick=\"fetch('/click')\">"
"●"
"</button>"

"<button class='btn' "
"ontouchstart=\"startMove('right')\" "
"ontouchend=\"stopMove()\">"
"▶"
"</button>"
"</div>"

"<!-- DOWN -->"
"<div class='row'>"
"<button class='btn' "
"ontouchstart=\"startMove('down')\" "
"ontouchend=\"stopMove()\">"
"▼"
"</button>"
"</div>"

"</div>"

"</body>"
"</html>";

        send(clientSocket, response, strlen(response), 0);
    }

    closesocket(clientSocket);
}

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}