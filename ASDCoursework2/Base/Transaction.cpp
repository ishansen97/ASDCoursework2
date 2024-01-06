#include <>
using namespace std;

class Transaction
{
private:
	double amount;
	bool isExpense;
	Category category;

public:
	Transaction();
	~Transaction();

};

Transaction::Transaction()
{
}

Transaction::~Transaction()
{
}