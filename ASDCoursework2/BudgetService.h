#pragma once
#include "Database.h"
#include "ExpenseBudgetDetail.h"
#include <numeric>
using namespace std;
class BudgetService
{
private:
	Database* db;
public:
	BudgetService() { db = Database::createDatabase(); }

	map<string, ExpenseBudgetDetail*> getExpenseBudget() {
		auto acctTypes = db->getAccountTypes()["Expense"];
		map<string, ExpenseBudgetDetail*> categoryBudgets;
		auto expenseTransactions = db->getTransactions(acctTypes);
		for (auto pair : db->getCategories(acctTypes))
		{
			string categoryName = pair.first;
			double expectedBudget = pair.second->getBudget();
			auto lambdaFunc = [categoryName](int sum, const std::pair<int, Transaction*>& elem) {
				if (elem.second->getCategoryName() == categoryName)
					return sum + elem.second->getAmount();
				return sum;
			};

			double actualBudget = accumulate(expenseTransactions.begin(), expenseTransactions.end(), 0, lambdaFunc);
			categoryBudgets.insert({ categoryName, new ExpenseBudgetDetail(pair.second, expectedBudget, actualBudget) });
		}

		return categoryBudgets;
	}

	map<string, double> getIncomeSummaries() {
		auto acctTypes = db->getAccountTypes()["Income"];
		map<string, double> incomeSummaries = {};

		auto incomeTransactions = db->getTransactions(acctTypes);
		for (auto pair : db->getCategories(acctTypes))
		{
			string categoryName = pair.first;
			auto lambdaFunc = [categoryName](int initSum, const std::pair<int, Transaction*>& elem) {
				if (elem.second->getCategoryName() == categoryName)
					return initSum + elem.second->getAmount();
				return initSum;
			};

			double actualBudget = accumulate(incomeTransactions.begin(), incomeTransactions.end(), 0, lambdaFunc);
			incomeSummaries.insert({ categoryName, actualBudget });
		}

		return incomeSummaries;
	}

	tuple<double, double, double> getBudgetTotals() {
		double expectedSum = 0, actualSum = 0, incomeSum = 0;
		auto expenseSummaries = getExpenseBudget();
		auto incomeSummaries = getIncomeSummaries();

		incomeSum = accumulate(incomeSummaries.begin(), incomeSummaries.end(), 0, [](int initialSum, const pair<string, double>& elem)
			{
				return initialSum + elem.second;
			});

		expectedSum = accumulate(expenseSummaries.begin(), expenseSummaries.end(), 0, [](int initialSum, const pair<string, ExpenseBudgetDetail*>& elem)
			{
				return initialSum + elem.second->getExpectedBudget();
			});

		actualSum = accumulate(expenseSummaries.begin(), expenseSummaries.end(), 0, [](int initialSum, const pair<string, ExpenseBudgetDetail*>& elem)
			{
				return initialSum + elem.second->getActualBudget();
			});

		return make_tuple(incomeSum, expectedSum, actualSum);
	}

	double calculateBalance() {
		double totalIncome = 0, totalExpense = 0;
		auto incomeSummaries = getIncomeSummaries();
		auto expenseBudget = getExpenseBudget();
		totalIncome = accumulate(incomeSummaries.begin(), incomeSummaries.end(), 0, [](int initSum, const pair<string, double>& elem)
			{
				return initSum + elem.second;
			});

		totalExpense = accumulate(expenseBudget.begin(), expenseBudget.end(), 0, [](int initSum, const pair<string, ExpenseBudgetDetail*>& elem)
			{
				return initSum + elem.second->getActualBudget();
			});

		return (totalIncome - totalExpense);
	}
};