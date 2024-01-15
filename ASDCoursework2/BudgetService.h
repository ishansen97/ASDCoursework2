#pragma once
#include "DataStore.h"
using namespace std;
class BudgetService
{
private:
	DataStore* store;
public:
	BudgetService() { store = DataStore::createStore(); }

	map<string, CategoryExpenseSummary*> getExpenseSummaries() {
		auto acctTypes = store->getAccountTypes()["Expense"];
		map<string, CategoryExpenseSummary*> categorySummaries = {};
		auto expenseTransactions = store->getTransactionsByAccountType(acctTypes);
		for (auto pair : store->getCategories(acctTypes))
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
		auto acctTypes = store->getAccountTypes()["Income"];
		map<string, double> categorySummaries = {};

		auto incomeTransactions = store->getTransactionsByAccountType(acctTypes);
		for (auto pair : store->getCategories(acctTypes))
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
};