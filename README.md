# Banking-Project
Creating an Online Banking Management System in C with the functionalities

# Online Banking Management System

## Overview
This project implements an online banking management system with administrative and customer functionalities. It uses inter-process communication and socket programming for client-server interactions.

## Features
- Add and remove customers
- Delete accounts
- Retrieve account and customer information
- Deposit and withdraw money
- Check passbooks
- Conduct balance inquiries
- Manage joint accounts

## Technologies Used
- C programming language
- Linux IPC (Inter-process Communication)
- Socket programming
- Semaphores for synchronization

## How to Build
Navigate to the `server` and `client` directories and run `make` to build the respective binaries.

```bash
cd server
make
cd ../client
make
