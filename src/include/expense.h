#pragma once

#include <set>
#include "json.hpp"

class ExpenseTracker
{
private:
    int id;
    std::string directory = "/mnt/Storage/CppStorage/ExpenseTrackerCLI/obj";
    std::string fileName = directory + "/expense.json";

public:
    ExpenseTracker() : id(0){}
    ~ExpenseTracker(){}
    //void list();
    //void updateExpense();
    void addExpense();
    void deleteExpense(int idToDelete);
    //void ensureFileExists();
    //void listSummary();
    //void saveToFile(const nlohmann::json& expense);
    std::string getCurrentTimestamp();
};