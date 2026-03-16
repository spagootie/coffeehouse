# Chat Application Deployment Guide

## Overview

This document explains how to deploy the Chat Application locally.
It lists all dependencies, database setup instructions, build steps,
and how to verify that the system is functioning correctly.

The repository also includes a working script called `deploy.sh`
which automates most of the setup process.

---

# Project Structure

Important directories in the repository:

```
client/
business/
data/
service/
sql/
Makefile
deploy.sh
README.md
```

Each directory corresponds to a layer in the application architecture.

- **Client layer** – browser interface and test client  
- **Service layer** – HTTP server  
- **Business layer** – application logic  
- **Data layer** – database access  
- **SQL directory** – schema and seed data  

---

# Required Software

## Arch Linux

```bash
sudo pacman -Syu
sudo pacman -S base-devel mariadb gcc make asio
```

## Debian / Ubuntu

```bash
sudo apt update
sudo apt install build-essential mariadb-server \
                 default-libmysqlclient-dev libasio-dev
```

Required components:

- MariaDB database server
- MariaDB client development libraries
- ASIO networking library
- g++ compiler with C++17 support
- make build tool

Crow headers are included in the repository.

---

# Once you have all of the dependencies, just run deploy.sh. Everything below is for running it manually.

---

# Database Setup

Start MariaDB:

```bash
sudo systemctl start mariadb
sudo systemctl enable mariadb
```

Create the database and user:

```bash
sudo mariadb <<SQL
CREATE DATABASE IF NOT EXISTS chat_app;
CREATE USER IF NOT EXISTS 'chat_user'@'localhost'
IDENTIFIED BY 'chatpass';
GRANT ALL PRIVILEGES ON chat_app.* TO 'chat_user'@'localhost';
FLUSH PRIVILEGES;
SQL
```

Import schema and seed data:

```bash
mariadb -u chat_user -pchatpass chat_app < sql/tables.sql
mariadb -u chat_user -pchatpass chat_app < sql/insert.sql
```

---

# Building the Project

From the project root directory:

```bash
make clean
make
```

This compiles the server binary.

---

# Running the Server

Start the server:

```bash
./mychat-server
```

Open the application in a browser:

```
http://localhost:18080
```

---

# Verification

To confirm the system works:

1. Open the web interface.
2. Create a user through the UI.

Verify with SQL:

```bash
mariadb -u chat_user -pchatpass chat_app -e \
"SELECT * FROM users;"
```

3. Create a conversation.
4. Send a message.

Verify messages in the database:

```bash
mariadb -u chat_user -pchatpass chat_app -e \
"SELECT * FROM messages;"
```

5. Edit a message.
6. Delete a message.

---

# Testing the API

Example request:

```bash
curl http://localhost:18080/users
```

Query recent messages:

```bash
mariadb -u chat_user -pchatpass chat_app -e \
"SELECT * FROM messages ORDER BY message_id DESC LIMIT 10;"
```

---

# Troubleshooting

**Database does not exist**

Create it manually:

```sql
CREATE DATABASE chat_app;
```

**Server returns 404**

Ensure the server is started from the repository root directory.

**MariaDB running with skip-grant-tables**

Restart MariaDB:

```bash
sudo systemctl restart mariadb
```

---

# Screenshot Checklist

Include screenshots showing:

1. Web interface loaded
2. Creating a user
3. Database showing inserted user
4. Creating a conversation
5. Sending a message
6. Editing a message
7. Deleting a message
8. Running the automated test client
