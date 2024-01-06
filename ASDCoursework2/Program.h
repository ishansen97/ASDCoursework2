#pragma once
#include "DataStore.h"
#include "Transaction.h"
#include <iostream>
using namespace std;

class Program
{
private:
public:
	static DataStore* store;

	static void mainMenu() {
		Program::store = new DataStore();
		int userOption;
		do
		{
			cout << "Welcome to Expense tracking and Budgeting Application." << endl;
			cout << "Please select your option." << endl;
			cout << "\t 1. Manage Transactions." << endl;
			cout << "\t 2. Manage Categories." << endl;
			cout << "\t 3. Track your progress." << endl;
			cout << "\t 0. Exit." << endl;
			cout << "Enter your option: ";
			cin >> userOption;

			if (userOption != 0)
				handleUserOption(userOption);

		} while (userOption != 0);
	}

	static void displayTransactionMenu() {
		int transactionOption;
		do
		{
			cout << "Manage your transactions here." << endl;
			cout << "1. Add new transaction" << endl;
			cout << "2. View recent transactions." << endl;
			cout << "3. Edit transaction" << endl;
			cout << "4. Delete transaction" << endl;
			cout << "0. Exit" << endl;
			cout << "Enter your option: ";
			cin >> transactionOption;
			handleTransactionOption(transactionOption);
		} while (transactionOption != 0);
	}

	static void displayCategoryMenu() {
		int categoryOption;
		do
		{
			cout << "Manage the categories here." << endl;
			cout << "1. Add new category" << endl;
			cout << "2. View category list." << endl;
			cout << "3. Specify budgets for categories." << endl;
			cout << "0. Exit." << endl;
			cout << "Enter your option: ";
			cin >> categoryOption;
			handleCategoryOption(categoryOption);
		} while (categoryOption != 0);
	}

	#pragma region Handling user options

	static void handleUserOption(int option) {
		switch (option)
		{
		case 1:
			displayTransactionMenu();
			break;
		case 2:
			displayCategoryMenu();
			break;
		default:
			break;
		}
	}

	static void handleTransactionOption(int option) {
		switch (option)
		{
		case 1:
			addTransaction();
			break;
		case 2:
			viewRecentTransactions();
			break;
		case 3:
			editTransaction();
			break;
		case 4:
			deleteTransaction();
			break;
		default:
			break;
		}
	}

	static void handleCategoryOption(int option) {
		switch (option)
		{
			case 1:
				addCategory();
				break;
			case 2:
				viewCategories();
				break;
			case 3:
				setCategoryBudget();
				break;
			default:
				break;
		}
	}
	#pragma endregion


#pragma region Transaction Operations
	static void addTransaction() {
		int transactionType;
		int recurring;
		bool type;
		double amount;
		bool isRecurring;
		string categoryName, note;
		Category* category;

		cout << "Enter transaction details: " << endl;
		cout << "Transaction type (Expense/Income) (Enter 0 for Expense, 1 for Income): " << endl;
		cin >> transactionType;
		type = transactionType == 0;

		cout << "Amount: " << endl;
		cin >> amount;

		cout << "Recurring?: (Enter 1 for Recurring, else 0)" << endl;
		cin >> recurring;
		isRecurring = recurring == 1;

		cout << "Note: " << endl;
		cin >> note;

		cout << "Category: " << endl;
		cin >> categoryName;

		// create the transaction object here.
		category = Program::store->getCategory(categoryName);
		int transactionId = Program::store->getLastTransactionId() + 1;
		Transaction* transaction = new Transaction(transactionId, amount, type, note, category, isRecurring);

		Program::store->addTransaction(transactionId, transaction);
		cout << "Transaction added successfully." << endl;
	};

	static void viewRecentTransactions() {
		cout << "===== Transaction list =====" << endl;
		for (auto pair : Program::store->getTransactions())
		{
			pair.second->printDetails();
			cout << "=========" << endl;
		}
	}

	static void editTransaction() {
		int id, newAmount;
		string strAmount, newNote, expenseOrIncome;
		cout << "Please enter transaction ID to edit: " << endl;
		cin >> id;

		Transaction* transaction = store->getTransaction(id);

		cout << "Enter new amount (Press ENTER to skip):";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, strAmount);

		cout << "Enter new note (Press ENTER to skip):";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, newNote);

		cout << "Enter expense/Income state (0 for expense, 1 for income) (Press ENTER to skip):";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, expenseOrIncome);

		cout << "Recurring? (Enter 1 for Recurring, else 0)  (Press ENTER to skip):";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, newNote);

		// setting new values
		if (!strAmount.empty()) {
			transaction->setAmount(stoi(strAmount));
		}
		if (!newNote.empty()) {
			transaction->setNote(transaction->getNote());
		}
		if (!expenseOrIncome.empty()) {
			transaction->setTransactionType(stoi(expenseOrIncome));
		}

		transaction->printDetails();
	}

	static void deleteTransaction() {
		int id;
		cout << "Please enter transaction ID to edit: " << endl;
		cin >> id;

		store->deleteTransaction(id);
		cout << "Transaction deleted successfully.";
	}
#pragma endregion

#pragma region Category Operations
	static void addCategory() {
		string categoryName;
		double budgetAmt;

		cout << "Enter category name: " << endl;
		cin >> categoryName;

		cout << "Initial budget amount: " << endl;
		cin >> budgetAmt;

		Category* newCategory = new Category(categoryName, budgetAmt);
		store->addCategory(categoryName, newCategory);

		cout << "Category added successfully." << endl;
	}

	static void viewCategories() {
		cout << "===== List of categories =====" << endl;

		for (auto pair : store->getCategories())
		{
			pair.second->printDetails();
		}
	}

	static void setCategoryBudget() {
		for (auto pair : store->getExpenseCategories())
		{
			int budget;
			cout << "===============";
			cout << "Category: " << pair.first << endl;
			cout << "Current budget: " << to_string(round(pair.second->getBudget())) << endl;
			cout << "New budget: ";
			cin >> budget;
			cout << "===============";

			pair.second->setBudget(budget);
		}

	}
#pragma endregion


};

