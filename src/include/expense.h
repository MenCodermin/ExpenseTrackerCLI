#pragma once

#include <set>
#include "json.hpp"

class ExpenseTracker
{
private:
    int id;
    std::set<int> deletedIds;
    std::string directory = "E:/Projects/TaskManagerCLI/build";
    std::string fileName = directory + "/tasks.json";
    nlohmann::json task;
    std::string getCurrentTimestamp();

public:
    ExpenseTracker() : id(0){}
    ~ExpenseTracker(){}
    void printMenu();
    void updateList();
    void addList();
    void deleteList();
    void printList();
    void ensureFileExists();
    void updateTaskStatus();
    void printTasksByStatus();
    void saveToFile(const nlohmann::json& tasks);
    void waitForUser();
    int getValidStatus();
};