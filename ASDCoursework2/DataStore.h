#pragma once
#include "Category.h"
#include "Transaction.h"
#include "CategoryExpenseSummary.h"
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
using namespace std;

class DataStore
{
private:
	map<string, Category*> categories;
	map<int, Transaction*> transactions;
public:
	DataStore()
	{
		categories = 
		{
			{"entertainment", new Category("entertainment", 20000)},
			{"clothes", new Category("clothes", 10000)},
			{"fuel", new Category("fuel", 20000)},
			{"gifts", new Category("gifts", 10000)},
			{"travel", new Category("travel", 60000)},
			{"salary", new Category("salary", 0)},
		};

		transactions =
		{
			{1, new Transaction(1, 1500, true, "buy shoes", categories["clothes"], false)},
			{2, new Transaction(2, 2500, true, "fueling the car", categories["fuel"], false)},
			{3, new Transaction(3, 2500, true, "partying", categories["entertainment"], false)},
			{4, new Transaction(4, 5500, true, "gifts for my family", categories["gifts"], false)},
			{5, new Transaction(5, 13500, true, "travelling to galle", categories["travel"], false)},
			{6, new Transaction(6, 10000, true, "buy 2 shirts", categories["clothes"], false)},
			{7, new Transaction(7, 150500, false, "Getting salary", categories["salary"], true)}
		};
	}

#pragma region category stuff

	bool isCategoryAvailable(string catName) {
		return categories.find(catName) != categories.end();
	}

	map<string, Category*> getCategories() { return categories; }

	map<string, Category*> getExpenseCategories() { 
		map<string, Category*> expenseCategories = {};
		for (auto pair : categories)
		{
			if (pair.first != "salary") {
				expenseCategories.insert(pair);
			}
		}

		return expenseCategories;
	}

	map<string, Category*> getIncomeCategories() { 
		map<string, Category*> incomeCategories = {};
		for (auto pair : categories)
		{
			if (pair.first == "salary") {
				incomeCategories.insert(pair);
			}
		}

		return incomeCategories;
	}

	void addCategory(string catName, Category* category) { categories[catName] = category; }
	Category* getCategory(string catName) { return categories.at(catName); }

#pragma endregion

#pragma region Transaction stuff

	bool isTransactionAvailable(int transactionId) {
		return transactions.find(transactionId) != transactions.end();
	}

	map<int, Transaction*> getTransactions() { return transactions; }
	void addTransaction(int id, Transaction* transaction) { transactions[id] = transaction; }

	Transaction* getTransaction(int id) 
	{ 
		try {
			return transactions.at(id);
		}
		catch (const out_of_range &e) {
			cout << "Exception: " << e.what();
		}
	}

	int getLastTransactionId() {
		if (!transactions.empty()) {
			return (transactions.rbegin())->first;
		}
		return 0;
	}

	void deleteTransaction(int id) { transactions.erase(id); }

#pragma endregion

#pragma region Budget Calculation stuff
	map<string, CategoryExpenseSummary*> getExpenseSummaries() {
		map<string, CategoryExpenseSummary*> categorySummaries = {};
		for (auto pair : getExpenseCategories())
		{
			string categoryName = pair.first;
			double expectedBudget = pair.second->getBudget();
			double actualBudget = 0;
			// get the calculations for each category
			for (auto transactionPair : getTransactions())
			{
				Transaction* transaction = transactionPair.second;
				if (transaction->getCategoryName() == categoryName) {
					actualBudget += transaction->getAmount();
				}
			}

			CategoryExpenseSummary* summary = new CategoryExpenseSummary(pair.second, expectedBudget, actualBudget);
			categorySummaries.insert({ categoryName, summary });
		}

		return categorySummaries;
	}

	map<string, double> getIncomeSummaries() {
		map<string, double> categorySummaries = {};
		for (auto pair : getIncomeCategories())
		{
			string categoryName = pair.first;
			double actualBudget = 0;
			// get the calculations for each category
			for (auto transactionPair : getTransactions())
			{
				Transaction* transaction = transactionPair.second;
				if (transaction->getCategoryName() == categoryName) {
					actualBudget += transaction->getAmount();
				}
			}

			categorySummaries.insert({ categoryName, actualBudget });
		}

		return categorySummaries;
	}

	tuple<double, double> getBudgetTotals() {
		double expectedSum = 0, actualSum = 0;
		for (auto expensePair : getExpenseSummaries()) {
			expectedSum += expensePair.second->getExpectedBudget();
			actualSum += expensePair.second->getActualBudget();
		}

		return make_tuple(expectedSum, actualSum);
	}

	double calculateBalance() {
		double totalIncome = 0, totalExpense = 0;
		for (auto incomePair : getIncomeSummaries()) {
			totalIncome += incomePair.second;
		}

		for (auto expensePair : getExpenseSummaries()) {
			totalExpense += expensePair.second->getActualBudget();
		}

		return (totalIncome - totalExpense);
	}
#pragma endregion
};

