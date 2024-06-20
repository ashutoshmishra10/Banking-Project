#include <stdio.h>
#include <string.h>
#include "bank.h"

void add_customer(Bank *bank, Customer customer) {
    if (bank->customer_count < MAX_CUSTOMERS) {
        bank->customers[bank->customer_count++] = customer;
    } else {
        printf("Bank is full. Cannot add more customers.\n");
    }
}

void remove_customer(Bank *bank, int account_number) {
    for (int i = 0; i < bank->customer_count; i++) {
        if (bank->customers[i].account_number == account_number) {
            for (int j = i; j < bank->customer_count - 1; j++) {
                bank->customers[j] = bank->customers[j + 1];
            }
            bank->customer_count--;
            printf("Customer with account number %d removed.\n", account_number);
            return;
        }
    }
    printf("Customer with account number %d not found.\n", account_number);
}
