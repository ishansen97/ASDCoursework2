#pragma once
#include "Category.h"
#include "Transaction.h"
#include <map>
#include <algorithm>
using namespace std;

class Database
{
private:
	map<string, Category*> categories;
	map<int, Transaction*> transactions;
	map<string, int> accountTypes;
	static Database* dbInstance;
	Database()
	{
		categories = 
		{
			{"entertainment", new Category("entertainment", 20000, 0)},
			{"fuel", new Category("fuel", 20000, 0)},
			{"gifts", new Category("gifts", 10000, 0)},
			{"travel", new Category("travel", 60000, 0)},
			{"salary", new Category("salary", 0, 1)},
		};

		transactions =
		{
			{1, new Transaction(2, 2500, true, "filling gas", categories["fuel"], false)},
			{2, new Transaction(3, 2500, true, "enjoying concert", categories["entertainment"], false)},
			{3, new Transaction(4, 5500, true, "gifts for myself", categories["gifts"], false)},
			{4, new Transaction(5, 13500, true, "solo travelling", categories["travel"], false)},
			{5, new Transaction(7, 150500, false, "Monthly salary", categories["salary"], true)}
		};

		accountTypes =
		{
			{"Expense", 0},
			{"Income", 1}
		};
	}
public:
	static Database* createDatabase() {
		if (dbInstance == 0)
			dbInstance = new Database();
		return dbInstance;
	}
#pragma region Account Types stuff
	map<string, int> getAccountTypes() { return accountTypes; }
#pragma endregion


#pragma region category stuff

	bool categoryExist(string catName) { return categories.find(catName) != categories.end(); }

	map<string, Category*> getCategories() { return categories; }

	map<string, Category*> getCategories(int accountType) { 
		map<string, Category*> results;
		auto func = [accountType](const pair<string, Category*>& elem) {
			return elem.second->getAccountType() == accountType;
		};

		copy_if(categories.begin(), categories.end(), inserter(results, results.begin()), func);
		return results;
	}

	void addNewCategory(string catName, Category* category) { categories[catName] = category; }
	Category* getCategory(string catName) { return categories.at(catName); }

#pragma endregion

#pragma region Transaction stuff

	bool transactionExist(int transactionId) { return transactions.find(transactionId) != transactions.end(); }

	map<int, Transaction*> getTransactions() { return transactions; }
	void addNewTransaction(int id, Transaction* tran) { transactions[id] = tran; }

	Transaction* getTransaction(int id) { return transactions.at(id); }

	map<int, Transaction*> getTransactions(int acctType) {
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

	void removeTransaction(int id) { transactions.erase(id); }
#pragma endregion
};