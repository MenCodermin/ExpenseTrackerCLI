#include <iostream>
#include "include/expense.h"
#include "include/json.hpp"
#include <string>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <map>

using json = nlohmann::json;

void ExpenseTracker::addExpense() {
    json tasks, task;
    int amount;
    std::string description;

    if (std::filesystem::exists(fileName)) {
        std::ifstream inFile(fileName);
        if (inFile.is_open()) {
            try {
                inFile >> tasks; // Parse JSON from file
            } catch (const std::exception &e) {
                std::cerr << "Error parsing JSON: " << e.what() << std::endl;
                tasks = json::array(); // Initialize tasks as an empty array
            }
            inFile.close();

            for (const auto &task : tasks) {
                if (task.contains("id") && task["id"].is_number()) {
                    id = std::max(id, static_cast<int>(task["id"]));
                }
            }
        } else {
            std::cerr << "Error opening file for reading: " << fileName << std::endl;
            return;
        }
    } else {
        tasks = json::array();
    }

    ++id;

    std::cout << "Enter the task description: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, description);

    std::cout << "Enter the amount: ";
    std::cin >> amount;

    std::string timestamp = getCurrentTimestamp();

    task = {{"id", id}, {"date", timestamp}, {"description", description}, {"amount", amount}};
    tasks.push_back(task);

    std::ofstream outFile(fileName);
    if (outFile.is_open()) {
        outFile << tasks.dump(4);
        outFile.close();
        std::cout << "Expense added successfully!" << std::endl;
    } else {
        std::cerr << "Error opening file for writing: " << fileName << std::endl;
    }
}


void ExpenseTracker::deleteExpense(int idToDelete)
{
    json expenses;
    bool expenseFound = false;
    char confirm;

    if (std::filesystem::exists(fileName))  // Checking if file exists or not,
                                            // if not, then create the one
    {
        std::ifstream inFile(fileName);
        if (inFile.is_open())
        {
            inFile >> expenses;  // Parse JSON from file
            inFile.close();
        }
        else
        {
            std::cerr << "Error opening file for reading: " << fileName << std::endl;
            return;
        }
    }
    else
    {
        std::cout << "File does not exist:" << fileName << std::endl;
        return;
    }

    for (auto it = expenses.begin(); it != expenses.end(); it++)
    {
        if ((*it)["id"] == idToDelete)
        {
            std::cout << "Are you sure you want to delete the task? (Y/N):" << std::endl;
            std::cin >> confirm;
            std::cin.ignore();
            if (confirm == 'y' || confirm == 'Y')
            {
                expenses.erase(it);
                expenseFound = true;
                break;
            }
            else if (confirm == 'n' || confirm == 'N')
            {
                std::cout << "Task removal canceled." << std::endl;
                expenseFound = false;
                return;
            }
            else
            {
                std::cout << "Wrong command. Returning to the Main Menu." << std::endl;
                return;
            }
        }
    }

    if (expenseFound)
    {
        std::cout << " Task with ID: " << idToDelete << " removed successfully." << std::endl;
    }
    else
    {
        std::cout << " Task with ID: " << idToDelete << " not found." << std::endl;
    }

}

std::string ExpenseTracker::getCurrentTimestamp()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::ostringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}




