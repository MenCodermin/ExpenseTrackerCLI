#include "include/expense.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "include/json.hpp"

using json = nlohmann::json;

void ExpenseTracker::addExpense(std::string& expenseDescription, int expenseAmount)
{
    json tasks, task;
    int amount = expenseAmount;
    std::string description = expenseDescription;
    std::string timestamp = getCurrentTimestamp();

    if (amount <= 0 && description.empty())
    {
        std::cerr << "Empty info\n";
    }

    if (std::filesystem::exists(fileName))
    {
        std::ifstream inFile(fileName);
        if (inFile.is_open())
        {
            try
            {
                inFile >> tasks;  // Parse JSON from file
                inFile.close();
            }
            catch (const std::exception& e)
            {
                std::cerr << "Error parsing JSON: " << e.what() << std::endl;
            }

            for (const auto& task : tasks)
            {
                if (task.contains("id") && task["id"].is_number())
                {
                    id = std::max(id, static_cast<int>(task["id"]));
                }
            }
        }
        else
        {
            std::cerr << "Error opening file for reading: " << fileName << std::endl;
        }
    }
    else
    {
        tasks = json::array();  // Initialize tasks as an empty array
    }

    ++id;

    task = {{"id", id}, {"description", description}, {"amount", amount}, {"date", timestamp}};
    tasks.push_back(task);

    std::ofstream outFile(fileName);
    if (outFile.is_open())
    {
        outFile << tasks.dump(4);
        outFile.close();
        std::cout << "Expense added successfully!" << std::endl;
    }
    else
    {
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
    saveToFile(expenses);
}

void ExpenseTracker::updateExpense(std::string& newExpenseDescription, int newExpenseAmount,
                                   int idToUpdateExp)
{
    json expenses;
    int idToUpdate = idToUpdateExp, newAmount = newExpenseAmount;
    std::string newDescription = newExpenseDescription;
    bool taskFound;

    ensureFileExists();

    std::ifstream inFile(fileName);
    if (inFile.is_open())
    {
        inFile >> expenses;
        inFile.close();
    }
    else
    {
        std::cout << "Error reading file: " << fileName << std::endl;
        return;
    }

    if (expenses.empty())
    {
        std::cout << "No task available." << std::endl;
        return;
    }

    for (auto& expense : expenses)
    {
        if (expense["id"] == idToUpdate)
        {
            expense["description"] = newDescription;
            expense["amount"] = newAmount;
            expense["date"] = getCurrentTimestamp();
            taskFound = true;
            break;
        }
    }

    if (taskFound)
    {
        std::cout << "Task was successfully updated." << std::endl;
    }
    else
    {
        std::cout << "Task with ID:" << idToUpdate << "not found." << std::endl;
        return;
    }

    saveToFile(expenses);
}

void ExpenseTracker::listAllExpenses()
{
    json expenses;

    if (std::filesystem::exists(fileName))
    {
        std::ifstream inFile(fileName);
        if (inFile.is_open())
        {
            try
            {
                inFile >> expenses;
                inFile.close();
                if (expenses.empty())
                {
                    std::cout << "No Expensess." << std::endl;
                    return;
                }

                for (const auto& expense : expenses)
                {
                    std::cout << "ID:" << expense["id"] << "\n";
                    std::cout << "Description:" << expense["description"] << "\n";
                    std::cout << "Amount:" << expense["amount"] << "\n";
                    std::cout << "Date:" << expense["date"] << "\n";
                }
            }
            catch (const std::exception& e)
            {
                std::cerr << "Error parsing JSON: " << e.what() << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Error opening file for reading: " << fileName << std::endl;
    }
}

void ExpenseTracker::listSummary()
{
    json expenses;
    int totalAmount = 0;
    if (std::filesystem::exists(fileName))
    {
        std::ifstream inFile(fileName);
        try
        {
            inFile >> expenses;
            inFile.close();

            for (const auto& expense : expenses)
            {
                if (expense.contains("amount") && expense["amount"].is_number())
                {
                    totalAmount += expense["amount"].get<int>();
                }
            }

            std::cout << "The total amount of Expenses is: " << totalAmount << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error parsing JSON." << e.what() << std::endl;
        }
    }
    else
    {
        std::cerr << "Error opening file for reading: " << fileName << std::endl;
    }
}

void ExpenseTracker::listSpecificMonthSummary(int month)
{
    json expenses;
    int totalAmount = 0, amount = 0;
    if (std::filesystem::exists(fileName))
    {
        std::ifstream inFile(fileName);
        try
        {
            inFile >> expenses;
            inFile.close();

            for (const auto& expense : expenses)
            {
                if (expense.contains("date") && expense["date"].is_string() &&
                    expense.contains("amount") && expense["amount"].is_number())
                {
                    std::string date = expense["date"].get<std::string>();
                    amount += expense["amount"].get<int>();

                    int expenseMonth = getMonth(date);

                    if (expenseMonth == month)
                    {
                        totalAmount += amount;
                    }
                }
            }

            std::cout << "Total Expenses for month " << month << ": " << totalAmount << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error parsing JSON." << e.what() << std::endl;
        }
    }
    else
    {
        std::cerr << "Error opening file for reading: " << fileName << std::endl;
    }
}

void ExpenseTracker::processCommand(int argc, char* argv[])
{
    std::string command = argv[1];

    if (command == "list")
    {
        listAllExpenses();
    }
    else if (command == "add")
    {
        std::string description = "";
        int amount = 0;

        for (int i = 2; i < argc; ++i)
        {
            std::string arg = argv[i];

            if (arg == "--description" && i + 1 < argc)
            {
                description = argv[++i];
            }
            else if (arg == "--amount" && i + 1 < argc)
            {
                amount = std::stoi(argv[++i]);
            }
        }

        if (!description.empty() && amount > 0)
        {
            addExpense(description, amount);
        }
    }
    else if (command == "update")
    {
        std::string description = "";
        int amount = 0, id = 0;
        for (int i = 2; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg == "--id" && i + 1 < argc)
            {
                id = std::stoi(argv[++i]);
            }
            else if (arg == "--description" && i + 1 < argc)
            {
                description = argv[++i];
            }
            else if (arg == "--amount" && i + 1 < argc)
            {
                amount = std::stoi(argv[++i]);
            }
        }

        if (!description.empty() && amount > 0 && id > 0)
        {
            updateExpense(description, amount, id);
        }
    }
    else if (command == "summary")
    {
        bool forMonth = false;
        int month = 0;

        for (int i = 2; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg == "--month" && i + 1 < argc)
            {
                forMonth = true;
                month = std::stoi(argv[++i]);
            }
        }

        if (forMonth)
        {
            listSpecificMonthSummary(month);
        }
        else
        {
            listSummary();
        }
    }
    else if (command == "delete")
    {
        int id = 0;
        for (int i = 2; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg == "--id" && i + 1 < argc)
            {
                id = std::stoi(argv[++i]);
            }
        }
        if (id > 0)
        {
            deleteExpense(id);
        }
        else
        {
            std::cout << "Invalid argument for 'delete'. \n";
            std::cout << "Usage: expense-tracker delete --id <id>\n";
        }
    }
    else
    {
        std::cout << "Unknown command: " << command << "\n";
        std::cout << "Available commands: add, list, summary, delete\n";
    }
}

int ExpenseTracker::getMonth(const std::string& date)
{
    int year, month;
    char dash;

    std::istringstream dateStream(date);
    dateStream >> year >> dash >> month;
    return month;
}

std::string ExpenseTracker::getCurrentTimestamp()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::ostringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d");
    return ss.str();
}

void ExpenseTracker::ensureFileExists()
{
    if (!std::filesystem::exists(fileName))
    {
        std::ofstream outFile(fileName);
        if (outFile.is_open())
        {
            outFile << json::array().dump(4);  // Create an empty JSON array if file doesn't exist
            outFile.close();
            std::cout << "Created an empty task file: " << fileName << std::endl;
        }
        else
        {
            std::cerr << "Error creating file: " << fileName << std::endl;
            std::exit(1);
        }
    }
}

void ExpenseTracker::saveToFile(const json& tasks)
{
    std::ofstream outFile(fileName);
    if (outFile.is_open())
    {
        outFile << tasks.dump(4);  // Pretty print with 4 spaces
        outFile.close();
        std::cout << "Tasks updated successfully in: " << fileName << std::endl;
    }
    else
    {
        std::cerr << "Error opening file for writing: " << fileName << std::endl;
    }
}