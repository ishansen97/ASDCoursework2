#pragma once
#include <string>
#include <iostream>
using namespace std;

class Category
{
private:
	string categoryName;
	double budgetAmount;
	int accountType; // expense = 0, income = 1

public:
	Category(string catName, double budgetAmt, int acctType)
	{
		this->categoryName = catName;
		this->budgetAmount = budgetAmt;
		this->accountType = acctType;
	}

	string getName() { return categoryName; }
	double getBudget() { return budgetAmount; }
	int getAccountType() { return accountType; }

	void setName(string catName) { categoryName = catName; }
	void setBudget(double amount) { budgetAmount = amount; }
	void setAccountType(int acctType) { accountType = acctType; }

	void printDetails(int indentation = 0) {
		cout << string(indentation, '\t') << "Name: " << categoryName << endl;
		cout << string(indentation, '\t') << "Budget: " << budgetAmount << endl;
		cout << string(indentation, '\t') << "Expense/Income: " << (accountType == 0 ? "Expense" : "Income") << endl;
	}
};