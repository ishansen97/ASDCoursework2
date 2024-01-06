#pragma once
#include <string>
#include <iostream>
using namespace std;

class Category
{
private:
	string categoryName;
	double budgetAmount;

public:
	Category(string catName, double budgetAmt)
	{
		this->categoryName = catName;
		this->budgetAmount = budgetAmt;
	}

	string getName() { return categoryName; }
	double getBudget() { return budgetAmount; }

	void setName(string catName) { categoryName = catName; }
	void setBudget(double amount) { budgetAmount = amount; }

	void printDetails() {
		cout << "Name: " << categoryName << endl;
		cout << "Budget: " << budgetAmount << endl;
	}

	void printDetails(int indentation) {
		cout << string(indentation, '\t') << "Name: " << categoryName << endl;
		cout << string(indentation, '\t') << "Budget: " << budgetAmount << endl;
	}


};


