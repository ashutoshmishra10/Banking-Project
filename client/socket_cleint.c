#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "bank.h"

// Function prototypes
void communicate_with_server(int client_socket);

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8080);

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server...\n");

    // Communicate with server
    communicate_with_server(client_socket);

    // Cleanup
    close(client_socket);
    return 0;
}

void communicate_with_server(int client_socket) {
    char buffer[256];
    int account_number;
    double amount;
    int choice;

    while (1) {
        printf("1. Deposit\n2. Withdraw\n3. Check Passbook\n4. Exit\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter account number: ");
                scanf("%d", &account_number);
                printf("Enter amount to deposit: ");
                scanf("%lf", &amount);
                sprintf(buffer, "deposit %d %lf", account_number, amount);
                send(client_socket, buffer, strlen(buffer), 0);
                recv(client_socket, buffer, sizeof(buffer), 0);
                printf("Server response: %s\n", buffer);
                break;
            case 2:
                printf("Enter account number: ");
                scanf("%d", &account_number);
                printf("Enter amount to withdraw: ");
                scanf("%lf", &amount);
                sprintf(buffer, "withdraw %d %lf", account_number, amount);
                send(client_socket, buffer, strlen(buffer), 0);
                recv(client_socket, buffer, sizeof(buffer), 0);
                printf("Server response: %s\n", buffer);
                break;
            case 3:
                printf("Enter account number: ");
                scanf("%d", &account_number);
                sprintf(buffer, "passbook %d", account_number);
                send(client_socket, buffer, strlen(buffer), 0);
                recv(client_socket, buffer, sizeof(buffer), 0);
                printf("Server response: %s\n", buffer);
                break;
            case 4:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
}
