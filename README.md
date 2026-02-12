# DashBoardIO

![Language](https://img.shields.io/badge/language-C%2B%2B20-blue.svg)
![UI](https://img.shields.io/badge/UI-Dear%20ImGui-orange.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

**A High-Performance, Server-Driven UI (SDUI) Engine for Remote Machine Management.**

DashBoardIO is a native C++ application designed to decouple the user interface from the underlying logic. By parsing JSON configurations at runtime, it allows remote servers (e.g., Python, Go, or C++ backends) to define and update the client's UI dynamically. This enables "building UIs on the fly" without recompiling the client, making it perfect for monitoring remote machines, streaming data, and rapid prototyping of internal tools.

## 🚀 Key Features

* **Server-Driven UI (SDUI):** The client is a stateless renderer; the server is the brain. Change your UI layout, widgets, and data binding server-side via JSON, and the client updates instantly.
* **High-Performance Native App:** Built with **Modern C++ (C++17/20)** and **Dear ImGui**, ensuring low memory footprint and immediate-mode rendering speeds that web dashboards can't match.
* **Asynchronous Networking:** Non-blocking I/O ensures the UI remains responsive even while streaming heavy data payloads or handling complex network requests.
* **Hot-Reloading:** Modify the JSON response on your server, and DashBoardIO reflects the changes in real-time on the next fetch cycle.
* **Cross-Platform:** Designed to run on Linux, Windows, and macOS.

## 🛠️ Tech Stack

* **Core:** C++20
* **UI Library:** [Dear ImGui](https://github.com/ocornut/imgui) (Immediate Mode GUI)
* **Networking:** [Drogon](https://github.com/drogonframework/drogon)
* **Build System:** CMake

## 🏗️ Architecture

DashBoardIO follows a strict separation of concerns:

1.  **Network Layer:** Polls or streams configuration data from a specified endpoint.
2.  **State Layer:** Parses JSON into a local state object using double-buffering to prevent read/write tearing.
3.  **Render Layer:** The ImGui loop reads the "active" buffer and reconstructs the UI every frame based on the schema definitions (e.g., `{ "type": "plot", "data": [...] }`).

## 📦 Build Instructions

### Prerequisites

* C++20 compatible compiler (GCC/Clang/MSVC)
* CMake (3.15+)
* OpenGL / GLFW dependencies

### Building from Source

```bash
# Clone the repository
git clone https://github.com/Muthu-Arun/dashBoardIO.git
cd DashBoardIO

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build
cmake --build .
