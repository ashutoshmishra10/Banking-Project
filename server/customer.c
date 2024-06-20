#include <stdio.h>
#include <string.h>
#include "bank.h"

Customer* get_customer(Bank *bank, int account_number) {
    for (int i = 0; i < bank->customer_count; i++) {
        if (bank->customers[i].account_number == account_number) {
            return &bank->customers[i];
        }
    }
    return NULL;
}

void deposit_money(Bank *bank, int account_number, double amount) {
    Customer *customer = get_customer(bank, account_number);
    if (customer != NULL) {
        customer->balance += amount;
        printf("Deposited %.2f to account number %d. New balance: %.2f\n", amount, account_number, customer->balance);

        // Update passbook
        PassbookEntry entry;
        entry.entry_number = customer->passbook_entry_count + 1;
        strcpy(entry.description, "Deposit");
        entry.amount = amount;
        entry.balance_after = customer->balance;
        customer->passbook[customer->passbook_entry_count++] = entry;
    } else {
        printf("Customer with account number %d not found.\n", account_number);
    }
}

void withdraw_money(Bank *bank, int account_number, double amount) {
    Customer *customer = get_customer(bank, account_number);
    if (customer != NULL) {
        if (customer->balance >= amount) {
            customer->balance -= amount;
            printf("Withdrew %.2f from account number %d. New balance: %.2f\n", amount, account_number, customer->balance);

            // Update passbook
            PassbookEntry entry;
            entry.entry_number = customer->passbook_entry_count + 1;
            strcpy(entry.description, "Withdrawal");
            entry.amount = -amount;
            entry.balance_after = customer->balance;
            customer->passbook[customer->passbook_entry_count++] = entry;
        } else {
            printf("Insufficient balance.\n");
        }
    } else {
        printf("Customer with account number %d not found.\n", account_number);
    }
}

void check_passbook(Customer *customer) {
    printf("Passbook for account number %d:\n", customer->account_number);
    for (int i = 0; i < customer->passbook_entry_count; i++) {
        PassbookEntry entry = customer->passbook[i];
        printf("Entry %d: %s - Amount: %.2f - Balance after: %.2f\n",
               entry.entry_number, entry.description, entry.amount, entry.balance_after);
    }
}
