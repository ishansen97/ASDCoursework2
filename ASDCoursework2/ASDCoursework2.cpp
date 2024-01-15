// ASDCoursework2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Program.h"
using namespace std;

DataStore* Program::store;
DataStore* DataStore::storeInstance = 0;
BudgetService Program::budgetService;
int main()
{
    Program::mainMenu();  
    return 0;
}
