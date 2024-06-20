// common/bank.h
#ifndef BANK_H
#define BANK_H

#define MAX_CUSTOMERS 100
#define MAX_NAME_LENGTH 502
#define MAX_PASSBOOK_ENTRIES 100

typedef struct {
    int entry_number;
    char description[100];
    double amount;
    double balance_after;
} PassbookEntry;

typedef struct {
    int account_number;
    char name[MAX_NAME_LENGTH];
    double balance;
    int is_joint;
    PassbookEntry passbook[MAX_PASSBOOK_ENTRIES];
    int passbook_entry_count;
} Customer;

typedef struct {
    Customer customers[MAX_CUSTOMERS];
    int customer_count;
} Bank;

void add_customer(Bank *bank, Customer customer);
void remove_customer(Bank *bank, int account_number);
Customer* get_customer(Bank *bank, int account_number);
void deposit_money(Bank *bank, int account_number, double amount);
void withdraw_money(Bank *bank, int account_number, double amount);
void check_passbook(Customer *customer);

#endif // BANK_H
