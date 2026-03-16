#!/bin/bash

echo "Starting deployment..."

DB_NAME="chat_app"
TABLES_FILE="sql/tables.sql"
SEED_FILE="sql/insert.sql"

echo "Starting MariaDB..."
sudo systemctl start mariadb

echo "Creating database..."

sudo mariadb <<EOF
CREATE DATABASE IF NOT EXISTS $DB_NAME;
EOF

echo "Importing tables..."
sudo mariadb $DB_NAME < $TABLES_FILE

echo "Importing seed data..."
sudo mariadb $DB_NAME < $SEED_FILE

echo "Building server..."

make clean
make

echo "Starting server..."

./mychat-server
