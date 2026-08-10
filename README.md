# PC Remote Control

A lightweight **C++ remote-control server** that allows you to control a Windows PC's mouse from a phone or another device through a web browser.

The application runs a small HTTP server on the Windows computer. Once a phone connects to the computer's local IP address through a browser, it displays a simple remote-control interface with buttons for moving the mouse cursor and performing clicks.

The project uses **Windows Winsock** for network communication and the Windows API for mouse control.

---

## Features

* Control a Windows PC from a phone browser
* Move the mouse cursor:

  * Up
  * Down
  * Left
  * Right
* Perform left mouse clicks remotely
* Browser-based remote-control interface
* Continuous mouse movement while a direction button is held
* Lightweight HTTP server
* No additional backend framework required
* Built specifically for Windows

---

## How It Works

The application works as a small HTTP server running on the Windows PC.

The basic flow is:

```text
Phone Browser
      │
      │ HTTP Request
      ▼
Windows PC
      │
      ▼
C++ Winsock Server
      │
      ├── /up
      ├── /down
      ├── /left
      ├── /right
      └── /click
      │
      ▼
Windows Mouse API
      │
      ▼
PC Mouse
```

For example, when the phone requests:

```text
GET /left
```

the C++ server detects the request and moves the Windows mouse cursor to the left.

Similarly:

```text
GET /right
GET /up
GET /down
GET /click
```

trigger the corresponding mouse actions.

---

# Requirements

This project currently targets **Windows** because it uses:

* `winsock2.h`
* `windows.h`
* Windows mouse APIs
* Winsock networking

You will need:

* Windows 10 or Windows 11
* A C++ compiler
* Git
* A phone or another device on the same local network

### Recommended compiler

The easiest option is **MinGW-w64 / g++**.

You can also compile the project using Microsoft Visual C++.

---

# Download the Project

Clone the repository from GitHub:

```bash
git clone https://github.com/Doziehn920/pc_remote_control.git
```

Enter the project directory:

```bash
cd pc_remote_control/
```

Or download the repository as a ZIP file from GitHub and extract it.

---

# Project Structure

A simple project structure can look like this:

```text
pc_remote_control/
│
├── README.md
├── server.cpp
└── ...
```

The main C++ source file contains:

* Winsock initialization
* HTTP server
* Client connection handling
* Mouse-control commands
* Embedded HTML/CSS/JavaScript remote interface

---

# Compile the Project

## Option 1 — Using MinGW g++

If you have `g++` installed, open Command Prompt or PowerShell inside the project directory.

Compile with:

```bash
g++ server.cpp -o pc-remote.exe -lws2_32
```

The important part is:

```text
-lws2_32
```

because the application uses the Windows Winsock library.

If compilation succeeds, you should see a new executable:

```text
pc-remote.exe
```

---

## Option 2 — Using Microsoft Visual C++

If you are using Visual Studio Developer Command Prompt, you can compile with:

```bash
cl server.cpp ws2_32.lib
```

This should generate an executable that can be run on Windows.

---

# Run the Server

After compiling, run:

```bash
./pc-remote.exe
```

You should see:

```text
Server started on port 8080...
```

The application listens on:

```text
Port: 8080
```

---

# Find Your PC's Local IP Address

Your phone needs to connect to the Windows computer using its local network IP address.

On Windows, open Command Prompt and run:

```bash
ipconfig
```

Look for your active network adapter and find the IPv4 address.

For example:

```text
IPv4 Address. . . . . . . . . . . : 192.168.1.25
```

Your IP address will probably be different.

---

# Connect From Your Phone

Make sure:

* Your PC is connected to Wi-Fi or Ethernet.
* Your phone is connected to the same local network.
* The C++ server is running.

On your phone, open a web browser and enter:

```text
http://YOUR-PC-IP:8080
```

For example:

```text
http://192.168.1.25:8080
```

The PC Remote Control interface should appear in your phone's browser.

You can then use the controls to operate the PC mouse.

---

# Remote Controls

The application exposes several HTTP endpoints.

| Endpoint | Action                     |
| -------- | -------------------------- |
| `/up`    | Move mouse upward          |
| `/down`  | Move mouse downward        |
| `/left`  | Move mouse left            |
| `/right` | Move mouse right           |
| `/click` | Perform a left mouse click |

For example:

```text
http://192.168.1.25:8080/up
```

moves the mouse upward.

And:

```text
http://192.168.1.25:8080/click
```

performs a left click.

---

# Continuous Mouse Movement

The browser interface uses JavaScript to repeatedly send requests while a direction button is being held.

For example:

```javascript
setInterval(() => {
    fetch('/' + direction);
}, 50);
```

This allows the mouse to continue moving instead of moving only once per button press.

When the button is released, the interval is cleared:

```javascript
clearInterval(moveInterval);
```

---

# Windows Firewall

If the phone cannot connect to the server, Windows Firewall may be blocking port `8080`.

When Windows asks whether the application should be allowed through the firewall, allow it on your **Private network** if appropriate.

You can also check Windows Firewall settings and make sure the application is allowed to communicate on your local network.

> Do not expose this application directly to the public internet unless you have added proper authentication and security controls.

---

# Troubleshooting

## `g++ is not recognized`

If you get:

```text
'g++' is not recognized as an internal or external command
```

you need to install a C++ compiler such as MinGW-w64 and make sure its `bin` directory is included in your Windows `PATH`.

---

## `undefined reference to WSAStartup`

Make sure you included the Winsock library when compiling:

```bash
g++ server.cpp -o pc-remote.exe -lws2_32
```

The `-lws2_32` flag is required.

---

## Phone cannot connect

Check the following:

1. The C++ server is running.
2. The PC and phone are on the same Wi-Fi/network.
3. You are using the PC's correct IPv4 address.
4. Port `8080` is not blocked by Windows Firewall.
5. You are using:

```text
http://
```

rather than:

```text
https://
```

For example:

```text
http://192.168.1.25:8080
```

---

## `Phone connected!` does not appear

Check the server's console.

The application waits for an incoming connection from a client. Make sure the address entered on the phone contains the correct IP address and port.

---

# Security Considerations

This project is designed primarily as a **local-network experiment and learning project**.

The current implementation does not provide:

* User authentication
* Password protection
* HTTPS/TLS
* Encryption
* Authorization
* Rate limiting
* Remote-user identification

Anyone who can reach the server on the network may potentially send commands to it.

Therefore, **do not expose port 8080 to the public internet in its current form.**

For safer usage, keep the application restricted to a trusted local network.

---

# Technologies Used

### C++

The core application and server are written in C++.

### Windows Winsock

Used to create the TCP server and handle network connections.

```cpp
#include <winsock2.h>
```

### Windows API

Used for controlling the mouse cursor.

```cpp
#include <windows.h>
```

The application uses APIs such as:

```cpp
GetCursorPos()
SetCursorPos()
mouse_event()
```

### HTML / CSS / JavaScript

The remote-control interface is embedded directly into the C++ server response and rendered by the phone's browser.

---

# Learning Goals

This project demonstrates several useful concepts:

* TCP socket programming
* HTTP request handling
* C++ networking with Winsock
* Windows API interaction
* Browser-to-PC communication
* HTTP endpoints
* JavaScript `fetch()`
* Touch-based browser controls
* Client/server architecture

It is a simple example of how a browser can act as a remote interface for a native Windows application.

---

# Future Improvements

Possible improvements include:

*  Add password authentication
*  Add HTTPS/TLS
*  Improve the mobile interface
*  Add right-click support
*  Add double-click support
*  Add mouse sensitivity controls
*  Add keyboard controls
*  Add clipboard sharing
*  Add screen streaming
*  Generate a temporary pairing code
*  Support multiple authenticated devices
*  Add automatic PC discovery on the local network
*  Add connection status
*  Add request validation and rate limiting

---

# Disclaimer

This project is intended for **personal use, experimentation, and educational purposes**.

Only use it on computers and networks that you own or have permission to control.

Because the application can control mouse input remotely, proper authentication and network security should be implemented before using it outside a trusted local environment.

---

## License

This project is currently provided for educational and personal use.

If you intend to distribute or modify it publicly, add an appropriate open-source license such as MIT.
