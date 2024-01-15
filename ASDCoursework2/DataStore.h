#pragma once
#include "Category.h"
#include "Transaction.h"
#include "CategoryExpenseSummary.h"
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
using namespace std;

class DataStore
{
private:
	map<string, Category*> categories;
	map<int, Transaction*> transactions;
	map<string, int> accountTypes;
public:
	DataStore()
	{
		categories = 
		{
			{"entertainment", new Category("entertainment", 20000, 0)},
			{"clothes", new Category("clothes", 10000, 0)},
			{"fuel", new Category("fuel", 20000, 0)},
			{"gifts", new Category("gifts", 10000, 0)},
			{"travel", new Category("travel", 60000, 0)},
			{"salary", new Category("salary", 0, 1)},
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

		accountTypes =
		{
			{"Expense", 0},
			{"Income", 1}
		};
	}

#pragma region Account Types stuff
	map<string, int> getAccountTypes() { return accountTypes; }
#pragma endregion


#pragma region category stuff

	bool isCategoryAvailable(string catName) { return categories.find(catName) != categories.end(); }

	map<string, Category*> getCategories() { return categories; }

	map<string, Category*> getCategories(int accountType) { 
		map<string, Category*> results;
		auto lambdaFunc = [accountType](const pair<string, Category*>& elem) {
			return elem.second->getAccountType() == accountType;
		};

		copy_if(categories.begin(), categories.end(), inserter(results, results.begin()), lambdaFunc);
		return results;
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

	Transaction* getTransaction(int id) { return transactions.at(id); }

	map<int, Transaction*> getTransactionsByAccountType(int acctType) {
		map<int, Transaction*> results;
		auto lambdaFunc = [acctType](const pair<int, Transaction*>& elem) {
			return elem.second->getCategory()->getAccountType() == acctType;
		};

		copy_if(transactions.begin(), transactions.end(), inserter(results, results.begin()), lambdaFunc);

		return results;
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
		auto expenseTransactions = getTransactionsByAccountType(accountTypes["Expense"]);
		for (auto pair : getCategories(accountTypes["Expense"]))
		{
			string categoryName = pair.first;
			double expectedBudget = pair.second->getBudget();
			auto lambdaFunc = [categoryName](int initSum, const std::pair<int, Transaction*>& elem) {
				if (elem.second->getCategoryName() == categoryName)
					return initSum + elem.second->getAmount();
				return initSum;
			};

			double actualBudget = accumulate(expenseTransactions.begin(), expenseTransactions.end(), 0, lambdaFunc);
			CategoryExpenseSummary* summary = new CategoryExpenseSummary(pair.second, expectedBudget, actualBudget);
			categorySummaries.insert({ categoryName, summary });
		}

		return categorySummaries;
	}

	map<string, double> getIncomeSummaries() {
		map<string, double> categorySummaries = {};
		auto incomeTransactions = getTransactionsByAccountType(accountTypes["Income"]);
		for (auto pair : getCategories(accountTypes["Income"]))
		{
			string categoryName = pair.first;
			auto lambdaFunc = [categoryName](int initSum, const std::pair<int, Transaction*>& elem) {
				if (elem.second->getCategoryName() == categoryName)
					return initSum + elem.second->getAmount();
				return initSum;
			};

			double actualBudget = accumulate(incomeTransactions.begin(), incomeTransactions.end(), 0, lambdaFunc);
			categorySummaries.insert({ categoryName, actualBudget });
		}

		return categorySummaries;
	}

	tuple<double, double, double> getBudgetTotals() {
		double expectedSum = 0, actualSum = 0, incomeSum = 0;
		auto expenseSummaries = getExpenseSummaries();
		auto incomeSummaries = getIncomeSummaries();

		incomeSum = accumulate(incomeSummaries.begin(), incomeSummaries.end(), 0, [](int initialSum, const pair<string, double>& elem)
			{
				return initialSum + elem.second;
			});

		expectedSum = accumulate(expenseSummaries.begin(), expenseSummaries.end(), 0, [](int initialSum, const pair<string, CategoryExpenseSummary*>& elem)
			{
				return initialSum + elem.second->getExpectedBudget();
			});

		actualSum = accumulate(expenseSummaries.begin(), expenseSummaries.end(), 0, [](int initialSum, const pair<string, CategoryExpenseSummary*>& elem)
			{
				return initialSum + elem.second->getActualBudget();
			});

		return make_tuple(incomeSum, expectedSum, actualSum);
	}

	double calculateBalance() {
		double totalIncome = 0, totalExpense = 0;
		auto incomeSummaries = getIncomeSummaries();
		auto expenseSummaries = getExpenseSummaries();
		totalIncome = accumulate(incomeSummaries.begin(), incomeSummaries.end(), 0, [](int initSum, const pair<string, double>& elem) 
			{
				return initSum + elem.second;
			});

		totalExpense = accumulate(expenseSummaries.begin(), expenseSummaries.end(), 0, [](int initSum, const pair<string, CategoryExpenseSummary*>& elem)
			{
				return initSum + elem.second->getActualBudget();
			});

		return (totalIncome - totalExpense);
	}
#pragma endregion
};

