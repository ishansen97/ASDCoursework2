#pragma once
#include <string>
#include "Category.h"
#include <iostream>

using namespace std;

class Transaction
{
private:
	int id;
	double amount;
	bool isExpense;
	string note;
	bool isRecurring;
	Category* category;

public:
	Transaction(int transId, double amt, bool expense, string note, Category* category, bool recurring)
	{
		this->id = transId;
		this->amount = amt;
		this->isExpense = expense;
		this->note = note;
		this->category = category;
		this->isRecurring = recurring;
	};

	int getAmount() { return amount; }
	string getTransactionType() {
		if (isExpense) return "Expense";
		return "Income";
	}

	string getNote() { return note; }
	string getRecurring() {
		if (isRecurring) return "Recurring";
		return "Non-recurring";
	}

	string getCategoryName() { return category->getName(); }

	#pragma region setters
	void setAmount(int amt) { amount = amt; }
	void setNote(string nt) { note = nt; }
	void setRecurring(bool rec) { isRecurring = rec; }
	void setTransactionType(int expenseOrIncome) { isExpense = expenseOrIncome == 0; }
	#pragma endregion


	void printDetails() {
		cout << "Id: " << id << endl;
		cout << "Amount: " << amount << endl;
		cout << "Expense/Income: " << (isExpense ? "Expense" : "Income") << endl;
		cout << "Note: " << note << endl;
		cout << "===== Category =====" << endl;
		category->printDetails(1);
		cout << "Is Recurring: " << (isRecurring ? "Yes" : "No") << endl;
	}
};


