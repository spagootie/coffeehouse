# 4-Tier Chat Application (C++ / MariaDB / Crow)

## Overview

This project implements a 4-tier chat application architecture:

1. **Database Layer** – MariaDB database
2. **Data Layer** – C++ repository classes using MariaDB C API
3. **Business Layer** – C++ service classes containing validation and business logic
4. **Service Layer** – REST API built using Crow (C++ web framework)
5. **Client Layer** – Console application that consumes the REST API

The purpose of this project is to demonstrate layered architecture where each tier has a clearly defined responsibility and communicates only with the adjacent layer.

---

## Architecture

```
Console Client
      ↓ HTTP
Service Layer (Crow REST API)
      ↓
Business Layer (ChatService)
      ↓
Data Layer (ChatRepository)
      ↓
MariaDB Database
```

Each layer exposes all functionality of the layer beneath it, as required.

---

## Technologies Used

- C++17
- MariaDB
- MariaDB C Client Library (`libmysqlclient`)
- Crow (header-only C++ web framework)
- Boost (required by Crow)
- GNU Make
- Linux (tested on Ubuntu/Debian-based systems)

---

## Project Structure

```
chat-app/
│
├── data/
│   ├── ChatRepository.h
│   └── ChatRepository.cpp
│
├── business/
│   ├── ChatService.h
│   └── ChatService.cpp
│
├── service/
│   └── server.cpp
│
├── client/
│   └── client.cpp
│
├── external/
│   └── Crow/
│       └── include/
│
├── Makefile
├── tables.sql
├── insert.sql
└── README.md
```

---

## Layer Responsibilities

### Database Layer
Contains:
- Tables
- Constraints
- Relationships
Defined in:
- `tables.sql`
- `insert.sql`

### Data Layer (ChatRepository)
- Connects to MariaDB
- Performs CRUD operations
- Contains raw SQL queries
- No business logic

### Business Layer (ChatService)
- Wraps all data layer methods
- Adds validation
- Acts as an abstraction over the repository

### Service Layer (Crow REST API)
- Exposes HTTP endpoints
- Calls business layer methods
- Returns JSON responses

### Client Layer
- Console-based
- Makes HTTP requests to service layer
- Demonstrates full CRUD flow

---

## Installation Instructions (Linux)

### 1. Install Dependencies

```
sudo apt update
sudo apt install build-essential
sudo apt install libmysqlclient-dev
sudo apt install libboost-all-dev
```

### 2. Clone Crow

Inside the project root:

```
git clone https://github.com/CrowCpp/Crow.git external/Crow
```

Crow is used as a header-only dependency.

---

## Database Setup

### 1. Start MariaDB

```
sudo systemctl start mariadb
```

### 2. Create Database

```
mysql -u root -p
```

Inside MariaDB:

```
CREATE DATABASE chat_app;
USE chat_app;
SOURCE tables.sql;
SOURCE insert.sql;
```

Ensure the credentials in `ChatRepository.cpp` match your MariaDB configuration:

```
const char* HOST = "localhost";
const char* USER = "chat_user";
const char* PASS = "chatpass";
const char* DB   = "chat_app";
```

---

## Build Instructions

From the project root:

```
make
```

This builds:
- `server`
- `client`

---

## Running the Application

### Start the Server

```
make run-server
```

Server runs at:

```
http://localhost:18080
```

### Run the Client

In another terminal:

```
make run-client
```

---

## Available API Endpoints

### Get All Users

```
GET /users
```

Example:

```
curl http://localhost:18080/users
```

---

### Create User

```
POST /users
```

Example:

```
curl -X POST http://localhost:18080/users \
-H "Content-Type: application/json" \
-d '{"username":"madison","displayName":"Madison"}'
```

---

### Get Messages for Conversation

```
GET /messages/<conversation_id>
```

Example:

```
curl http://localhost:18080/messages/1
```

---

## Demonstrating Full CRUD Flow

To demonstrate service functionality:

1. Insert a user via POST
2. Retrieve users via GET
3. Insert a message
4. Retrieve messages
5. Delete (if implemented)
6. Confirm deletion via GET

Take screenshots of:
- Server running
- Curl responses
- Database query results
- Client execution

---

## Makefile Targets

Build everything:

```
make
```

Run server:

```
make run-server
```

Run client:

```
make run-client
```

Clean build artifacts:

```
make clean
```

---

## Hosting Options

This service can be hosted on:

- Render
- Railway
- AWS EC2
- Docker container

Basic deployment steps:
1. Push repository to GitHub
2. Install dependencies on host
3. Run `make`
4. Execute `./server`

---

## Notes

- C++17 is required.
- MariaDB client development library must be installed.
- Boost headers are required for Crow.
- Linux is required for this Makefile configuration.

---

## Summary

This project demonstrates a properly layered architecture:

- Clear separation of concerns
- HTTP-based service layer
- Business logic abstraction
- Database interaction encapsulation
- Build automation with Make
- Testable via console and curl

The design models real-world professional application structure and deployment workflow.

