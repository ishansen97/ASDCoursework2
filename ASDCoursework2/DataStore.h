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
	static DataStore* storeInstance;
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
public:
	static DataStore* createStore() {
		if (storeInstance == 0)
			storeInstance = new DataStore();
		return storeInstance;
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
};