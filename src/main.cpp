#include <iostream>

#include "include/expense.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: expense-tracker <command> [options]\n";
        std::cout << "Available commands: add, list, summary, delete\n";
        return 1;
    }
    ExpenseTracker exp;
    exp.processCommand(argc, argv);
    return 0;
}