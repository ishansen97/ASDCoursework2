#pragma once
#include "Category.h"
#include "Transaction.h"
#include <vector>
#include <unordered_map>
#include <map>
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
			{"salary", new Category("salary", 0)},
		};

		transactions =
		{
			{1, new Transaction(1, 1500, true, "buy shoes", categories["clothes"], false)},
			{2, new Transaction(2, 2500, true, "fueling the car", categories["fuel"], false)},
			{3, new Transaction(3, 150500, false, "Getting salary", categories["salary"], true)}
		};
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

	void addCategory(string catName, Category* category) { categories[catName] = category; }
	Category* getCategory(string catName) { return categories.at(catName); }
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
};

