#pragma once

#include <set>

#include "json.hpp"

class ExpenseTracker
{
   private:
    int id;
    std::string directory = "./obj";
    std::string fileName = directory + "/expense.json";
    int getMonth(const std::string& date);

   public:
    ExpenseTracker() : id(0) {}
    ~ExpenseTracker() {}
    void updateExpense(std::string& newExpenseDescription, int newExpenseAmount, int idToUpdateExp);
    void processCommand(int argc, char* argv[]);
    void addExpense(std::string& expenseDescription, int expenseAmount);
    void deleteExpense(int idToDelete);
    void ensureFileExists();
    void listAllExpenses();
    void listSummary();
    void listSpecificMonthSummary(int month);
    void saveToFile(const nlohmann::json& expense);
    void waitForUser();
    std::string getCurrentTimestamp();
};