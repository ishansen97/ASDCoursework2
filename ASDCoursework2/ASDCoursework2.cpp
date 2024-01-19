// ASDCoursework2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "ExpenseTracker.h"
using namespace std;

Database* ExpenseTracker::db;
Database* Database::dbInstance = 0;
BudgetService ExpenseTracker::budgetService;
int main()
{
    ExpenseTracker::displayMainMenu();  
    return 0;
}