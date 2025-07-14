# Fjalor – Albanian Dictionary App

Fjalor is a fast and lightweight dictionary application that enables users to store, retrieve, and manage Albanian word definitions through a socket-based API server. It features a modern web interface with a clean admin panel for browsing, editing, and expanding the dictionary.

---

## Features

### C++ Socket Server
- Handles dictionary commands: `CREATE`, `READ`, `UPDATE`, `DELETE`, `DUMP`
- Persists word definitions in a JSON file (`data.json`)
- Returns structured JSON responses over HTTP-like protocol
- Compatible with both command-line and web frontend requests

### Web Admin Panel
- Built with **Tailwind CSS** and **Alpine.js**
- Sortable and paginated word-definition table
- Inline editing of definitions and deletion support
- Dedicated creation form for adding new entries
- Responsive and minimal neutral styling

---

## Getting Started

### Prerequisites
- C++17 compiler (e.g. `g++`)
- `nlohmann/json` library (`sudo apt install nlohmann-json3-dev`)
- Static file server (e.g. Python `http.server`) to host `admin.html`

### Build and Run

```bash
g++ crud_server.cpp -o fjalor_server
g++ crud_client.cpp -o fjalor_client
./fjalor_server       # Start dictionary server
./fjalor_client       # Optional: interact via CLI
```
---
### Usage
```python
python3 -m http.server
```
```text
http://localhost:8000/index.html
```
### Admin Interface
```text
http://localhost:8000/admin.html
```
