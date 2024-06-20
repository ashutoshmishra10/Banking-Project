#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "bank.h"

// Function prototypes
void handle_client(int client_socket, Bank *bank, int sem_id);
void setup_ipc(int *shm_id, Bank **bank, int *sem_id);
void acquire_semaphore(int sem_id);
void release_semaphore(int sem_id);

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int shm_id, sem_id;
    Bank *bank;

    // Setup IPC (shared memory and semaphore)
    setup_ipc(&shm_id, &bank, &sem_id);

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    // Bind socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Socket bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_socket, 5) < 0) {
        perror("Socket listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Bank server started...\n");

    // Accept clients in a loop
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("Client connection failed");
            continue;
        }
        printf("Client connected...\n");
        
        // Handle client in a new process
        if (fork() == 0) {
            close(server_socket);
            handle_client(client_socket, bank, sem_id);
            close(client_socket);
            exit(0);
        }
        close(client_socket);
    }

    // Cleanup
    close(server_socket);
    shmctl(shm_id, IPC_RMID, NULL); // Remove shared memory
    semctl(sem_id, 0, IPC_RMID); // Remove semaphore

    return 0;
}

void handle_client(int client_socket, Bank *bank, int sem_id) {
    char buffer[256];
    int account_number;
    double amount;

    // Receive command from client
    recv(client_socket, buffer, sizeof(buffer), 0);
    
    if (strncmp(buffer, "deposit", 7) == 0) {
        sscanf(buffer, "deposit %d %lf", &account_number, &amount);
        acquire_semaphore(sem_id);
        deposit_money(bank, account_number, amount);
        release_semaphore(sem_id);
        sprintf(buffer, "Deposited %.2f to account number %d.", amount, account_number);
        send(client_socket, buffer, strlen(buffer), 0);
    } else if (strncmp(buffer, "withdraw", 8) == 0) {
        sscanf(buffer, "withdraw %d %lf", &account_number, &amount);
        acquire_semaphore(sem_id);
        withdraw_money(bank, account_number, amount);
        release_semaphore(sem_id);
        sprintf(buffer, "Withdrew %.2f from account number %d.", amount, account_number);
        send(client_socket, buffer, strlen(buffer), 0);
    } else if (strncmp(buffer, "passbook", 8) == 0) {
        sscanf(buffer, "passbook %d", &account_number);
        acquire_semaphore(sem_id);
        Customer *customer = get_customer(bank, account_number);
        if (customer != NULL) {
            // Prepare passbook entries to send to client
            int len = sprintf(buffer, "Passbook for account number %d:\n", account_number);
            for (int i = 0; i < customer->passbook_entry_count; i++) {
                len += sprintf(buffer + len, "Entry %d: %s - Amount: %.2f - Balance after: %.2f\n",
                               customer->passbook[i].entry_number,
                               customer->passbook[i].description,
                               customer->passbook[i].amount,
                               customer->passbook[i].balance_after);
            }
            send(client_socket, buffer, len, 0);
        } else {
            sprintf(buffer, "Customer with account number %d not found.", account_number);
            send(client_socket, buffer, strlen(buffer), 0);
        }
        release_semaphore(sem_id);
    }
}

void setup_ipc(int *shm_id, Bank **bank, int *sem_id) {
    key_t key = ftok("shmfile", 65); // Generate a unique key

    // Shared memory
    *shm_id = shmget(key, sizeof(Bank), 0666|IPC_CREAT);
    if (*shm_id < 0) {
        perror("Shared memory creation failed");
        exit(EXIT_FAILURE);
    }
    *bank = (Bank *) shmat(*shm_id, NULL, 0);
    if (*bank == (void *) -1) {
        perror("Shared memory attach failed");
        exit(EXIT_FAILURE);
    }
    (*bank)->customer_count = 0;

    // Semaphore
    *sem_id = semget(key, 1, 0666|IPC_CREAT);
    if (*sem_id < 0) {
        perror("Semaphore creation failed");
        exit(EXIT_FAILURE);
    }
    semctl(*sem_id, 0, SETVAL, 1); // Initialize semaphore to 1
}

void acquire_semaphore(int sem_id) {
    struct sembuf sb = {0, -1, 0}; // Decrement semaphore by 1
    semop(sem_id, &sb, 1);
}

void release_semaphore(int sem_id) {
    struct sembuf sb = {0, 1, 0}; // Increment semaphore by 1
    semop(sem_id, &sb, 1);
}
