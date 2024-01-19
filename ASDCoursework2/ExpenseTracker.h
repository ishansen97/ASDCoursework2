#pragma once
#include "Database.h"
#include "Transaction.h"
#include <iostream>
#include "BudgetService.h"
#include "ExpenseBudgetDetail.h"
using namespace std;

class ExpenseTracker
{
private:
	static Database* db;
	static BudgetService budgetService;
#pragma region Displaying menu items
	static void displayTransactionMenu() {
		int transactionOption;
		do
		{
			cout << "Select your transaction option." << endl;
			cout << "1. Add new transaction" << endl;
			cout << "2. View recent transactions." << endl;
			cout << "3. Edit transaction" << endl;
			cout << "4. Remove transaction" << endl;
			cout << "5. Main Menu" << endl;
			cout << "Enter your option: ";
			cin >> transactionOption;

			if (isInvalidUserOption(transactionOption, 1, 5)) 
				clearConsoleAndPrintMessage("Invalid input. Please try again!", &transactionOption);
			else
				manageTransactionOption(transactionOption);
		} while (transactionOption != 5);
	}

	static void displayCategoryMenu() {
		int categoryOption;
		do
		{
			cout << "Choose your category option." << endl;
			cout << "\t 1. Add new category" << endl;
			cout << "\t 2. View category list." << endl;
			cout << "\t 3. Perform bulk expense category budget update." << endl;
			cout << "\t 4. Set category budget." << endl;
			cout << "\t 5. Main Menu" << endl;
			cout << "Enter your option: ";
			cin >> categoryOption;

			if (isInvalidUserOption(categoryOption, 1, 5)) 
				clearConsoleAndPrintMessage("Invalid input. Please try again!", &categoryOption);
			else
				manageCategoryOption(categoryOption);
		} while (categoryOption != 5);
	}
#pragma endregion

#pragma region Handling user options

	static void manageUserOption(int option) {
		switch (option)
		{
		case 1:
			displayTransactionMenu();
			break;
		case 2:
			displayCategoryMenu();
			break;
		case 3:
			trackBudgetProgress();
			break;
		default:
			break;
		}
	}

	static void manageTransactionOption(int option) {
		switch (option)
		{
		case 1:
			addNewTransaction();
			break;
		case 2:
			viewRecentTransactionsList();
			break;
		case 3:
			editTransaction();
			break;
		case 4:
			removeTransaction();
			break;
		default:
			break;
		}
	}

	static void manageCategoryOption(int option) {
		switch (option)
		{
		case 1:
			addNewCategory();
			break;
		case 2:
			viewCategoryList();
			break;
		case 3:
			setCategoryBudgetInBulk();
			break;
		case 4:
			setCategoryBudget();
		default:
			break;
		}
	}
#pragma endregion

#pragma region Transaction Operations
	static void addNewTransaction() {
		int transactionType = -1;
		int recurring = -1;
		bool type;
		double amount = -1;
		bool isRecurring;
		string categoryName, note;
		Category* category;

		cout << "===== Enter transaction details: =====" << endl;

		while (!isValidFieldOption(transactionType, {0,1}))
		{
			cout << "Transaction type (Expense/Income) (Enter 0 for Expense, 1 for Income): " << endl;
			cin >> transactionType;

			if (!isValidFieldOption(transactionType, { 0,1 })) {
				clearConsoleAndPrintMessage("Invalid transaction type. Please try again.", &transactionType, -1);
			}
			type = transactionType == 0;
		}

		while (isInvalidNumber(amount))
		{
			cout << "Amount: " << endl;
			cin >> amount;

			if (isInvalidNumber(amount)) {
				clearConsoleAndPrintMessage("Invalid amount. Please try again later.", &amount, -1);
			}
		}

		while (!isValidFieldOption(recurring, {0,1}))
		{
			cout << "Recurring?: (Enter 1 for Recurring, else 0)" << endl;
			cin >> recurring;
			if (!isValidFieldOption(recurring, { 0,1 }))
				clearConsoleAndPrintMessage("Invalid input for Recurring field.", &recurring, -1);
			isRecurring = recurring == 1;
		}

		cout << "Note: " << endl;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, note);

		while (!isExistingCategory(categoryName))
		{
			cout << "Category: " << endl;
			getline(cin, categoryName);
			if (!isExistingCategory(categoryName))
				cout << "Invalid category name. Please try again." << endl;
		}

		// create the transaction object here.
		category = db->getCategory(categoryName);
		int transactionId = db->getLastTransactionId() + 1;
		Transaction* transaction = new Transaction(transactionId, amount, type, note, category, isRecurring);

		db->addNewTransaction(transactionId, transaction);
		cout << "Transaction added successfully." << endl;
	};

	static void viewRecentTransactionsList() {
		cout << "===== Transaction list =====" << endl;
		for (auto pair : db->getTransactions())
		{
			pair.second->printDetails();
			cout << "=========" << endl;
		}
	}

	static void editTransaction() {
		int transactionId = -1, newAmount = -1;
		string strAmount, newNote, expenseOrIncome;

		while (isInvalidNumber(transactionId) || !isValidTransaction(transactionId))
		{
			cout << "Please enter transaction ID to edit: " << endl;
			cin >> transactionId;
			if (isInvalidNumber(transactionId)) {
				clearConsoleAndPrintMessage("Invalid input. Please try again.", &transactionId, -1);
			}
			if (!isValidTransaction(transactionId)) {
				clearConsoleAndPrintMessage("Invalid transaction Id. Please try again.", &transactionId, -1);
			}
		}

		Transaction* transaction = db->getTransaction(transactionId);

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

	static void removeTransaction() {
		int transactionId = -1;

		while (isInvalidNumber(transactionId) || !isValidTransaction(transactionId))
		{
			cout << "Please enter transaction ID to edit: " << endl;
			cin >> transactionId;
			if (isInvalidNumber(transactionId)) {
				clearConsoleAndPrintMessage("Invalid number. Please try again.", &transactionId, -1);
			}
			if (!isValidTransaction(transactionId)) {
				clearConsoleAndPrintMessage("Invalid transaction Id. Please try again.", &transactionId, -1);
			}
		}

		db->removeTransaction(transactionId);
		cout << "Transaction deleted successfully." << endl;
	}
#pragma endregion

#pragma region Category Operations
	static void addNewCategory() {
		string categoryName;
		double budgetAmt = -1;
		int acctType = -1;
		bool existingCategory = false;
		int inputPromptCount = 0;

		while (isExistingCategory(categoryName) || inputPromptCount == 0)
		{
			cout << "Enter category name: " << endl;
			cin >> categoryName;
			if (isExistingCategory(categoryName)) {
				clearConsoleAndPrintMessage("The category already exists. Please try again.");
			}
			inputPromptCount++;
		}

		while (isInvalidNumber(budgetAmt))
		{
			cout << "budget amount (initial): " << endl;
			cin >> budgetAmt;
			if (isInvalidNumber(budgetAmt)) {
				clearConsoleAndPrintMessage("Invalid input. Please try again.", &budgetAmt, -1);
			}
		}

		while (!isValidFieldOption(acctType, {0,1}))
		{
			cout << "Account Type (Expense = 0, Income = 1): " << endl;
			cin >> acctType;
			if (!isValidFieldOption(acctType, { 0,1 })) {
				clearConsoleAndPrintMessage("Invalid input. Please try again.", &acctType, -1);
			}
		}

		Category* newCategory = new Category(categoryName, budgetAmt, acctType);
		db->addNewCategory(categoryName, newCategory);

		cout << "Category added successfully." << endl;
	}

	static void viewCategoryList() {
		cout << "===== List of categories =====" << endl;

		for (auto pair : db->getCategories())
		{
			pair.second->printDetails();
			cout << "-------------------------" << endl;
		}

		cout << "==============================" << endl;
	}

	static void setCategoryBudgetInBulk() {
		for (auto pair : db->getCategories(db->getAccountTypes()["Expense"]))
		{
			double categoryBudget = -1;
			cout << "===============" << endl;
			cout << "Category: " << pair.first << endl;
			cout << "Current budget: " << pair.second->getBudget() << endl;
			cout << "New budget: ";
			while (isInvalidNumber(categoryBudget))
			{
				cin >> categoryBudget;
				if (isInvalidNumber(categoryBudget))
					clearConsoleAndPrintMessage("Invalid input. Please try again.", &categoryBudget, -1);
				cout << "===============" << endl;
			}

			pair.second->setBudget(categoryBudget);
		}

		cout << "The category budgets have been set." << endl;
	}

	static void setCategoryBudget() {
		string categoryName;
		double budgetAmt = -1;

		while (!isExistingCategory(categoryName))
		{
			cout << "Please enter the category to set the budget." << endl;
			cin >> categoryName;
			if (!isExistingCategory(categoryName))
				clearConsoleAndPrintMessage("Invalid category. Please try again.");
		}

		Category* category = db->getCategory(categoryName);

		while (isInvalidNumber(budgetAmt))
		{
			cout << "Set the budget amount: " << endl;
			cin >> budgetAmt;
			if (isInvalidNumber(budgetAmt))
				clearConsoleAndPrintMessage("Invalid amount. Please try again.", &budgetAmt, -1);

		}

		category->setBudget(budgetAmt);
		cout << "The budget for " << categoryName << " has been set." << endl;
	}
#pragma endregion

#pragma region Track Progress operations

	static void trackBudgetProgress() {
		string incomeHeader = "INCOME";
		string incomeAmountHeader = "AMOUNT";
		string header1 = "EXPENSE";
		string header2 = "EXPECTED BUDGET";
		string header3 = "ACTUAL BUDGET";
		string expenseTotalHeader = "EXPENSE TOTAL";
		string incomeTotalHeader = "INCOME TOTAL";
		string balanceHeader = "BALANCE";
		// display income
		cout << string(80, '=') << endl;
		cout << incomeHeader << string(20 - incomeHeader.length(), ' ') << incomeAmountHeader << endl;
		for (auto incomeSummaryPair : budgetService.getIncomeSummaries())
		{
			string categoryName = incomeSummaryPair.first;
			double amount = incomeSummaryPair.second;
			cout << categoryName << string(20 - categoryName.length(), ' ') << amount << endl;
		}
		// display expenses
		cout << string(80, '=') << endl;
		cout << header1 << string(20 - header1.length(), ' ') << header2 << string(30 - header2.length(), ' ') << header3 << endl;
		for (auto expenseSummary : budgetService.getExpenseBudget())
		{
			string categoryName = expenseSummary.first;
			ExpenseBudgetDetail* expenseDetail = expenseSummary.second;
			cout << categoryName << string(20 - categoryName.length(), ' ') << expenseDetail->getExpectedBudget() << string(40 - to_string(expenseDetail->getExpectedBudget()).length(), ' ') << expenseDetail->getActualBudget() << endl;
		}
		cout << string(80, '=') << endl;
		// get the budget totals
		auto budgetTotals = budgetService.getBudgetTotals();
		cout << incomeTotalHeader << string(20 - incomeTotalHeader.length(), ' ') << get<0>(budgetTotals) << endl;
		cout << expenseTotalHeader << string(20 - expenseTotalHeader.length(), ' ') << get<1>(budgetTotals) << string(40 - to_string(get<0>(budgetTotals)).length(), ' ') << get<2>(budgetTotals) << endl;

		// get the balance
		double balance = budgetService.calculateBalance();
		cout << balanceHeader << string(20 - balanceHeader.length(), ' ') << balance << endl;
		cout << string(80, '=') << endl;
	}
#pragma endregion

#pragma region helpers
	static bool isInvalidUserOption(int option, int low, int high) {
		return (cin.fail() || option < low || option > high);
	}

	static void clearConsoleAndPrintMessage(string message, int* optionPtr, int value = 0) {
		cin.clear();  // Clear the error flag
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << message << endl;
		*optionPtr = value;
	}

	static void clearConsoleAndPrintMessage(string message, double* optionPtr, int value = 0) {
		cin.clear();  // Clear the error flag
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << message << endl;
		*optionPtr = value;
	}

	static void clearConsoleAndPrintMessage(string message) {
		cin.clear();  // Clear the error flag
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << message << endl;
	}

	static bool isExistingCategory(string catName) {
		return db->categoryExist(catName);
	}

	static bool isValidTransaction(int transactionId) {
		return db->transactionExist(transactionId);
	}

	static bool isValidFieldOption(int input, const int(&options) []) {
		if (isInvalidNumber(input))
			return false;

		int size = sizeof(*options);
		return (find(options, options + size, input) != options + size);
	}

	static bool isInvalidNumber(double input) {
		return (cin.fail() || input < 0);
	}
#pragma endregion


public:
	static void displayMainMenu() {
		db = Database::createDatabase();
		int userOption;
		do
		{
			cout << "Welcome to Expense tracking and Budgeting Application." << endl;
			cout << "Please select your option." << endl;
			cout << "\t 1. Manage Transactions." << endl;
			cout << "\t 2. Manage Categories." << endl;
			cout << "\t 3. View Budget progress." << endl;
			cout << "\t 4. Exit." << endl;
			cout << "Enter your option: ";
			cin >> userOption;

			if (isInvalidUserOption(userOption, 1, 4)) {
				clearConsoleAndPrintMessage("Invalid input. Please try again!", &userOption);
			}
			else
				manageUserOption(userOption);

		} while (userOption != 4);
	}
};

