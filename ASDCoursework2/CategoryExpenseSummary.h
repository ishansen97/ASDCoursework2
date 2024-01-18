#pragma once
#include "Category.h"
using namespace std;

class CategoryExpenseSummary
{
private:
	Category* category;
	double expectedBudget, actualBudget;

public:
	CategoryExpenseSummary(Category* cat, double expectedBudgt, double actualBudgt)
	{
		this->category = cat;
		this->expectedBudget = expectedBudgt;
		this->actualBudget = actualBudgt;
	}

	double getExpectedBudget() { return expectedBudget; }
	double getActualBudget() { return actualBudget; }
};