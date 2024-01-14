#pragma once
class Helper
{
public:
	static bool isInvalidOption(int option, int low, int high) {
		return (cin.fail() || option < low || option > high);
	}

	static void clearConsoleAndPrintMessage(string message, int* optionPtr, int value = 0) {
		cin.clear();  // Clear the error flag
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << message << endl;
		*optionPtr = value;
	}

	static void clearConsoleAndPrintMessage(string message, double* optionPtr, int value = 0) {
		cin.clear();  // Clear the error flag
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << message << endl;
		*optionPtr = value;
	}

	static void clearConsoleAndPrintMessage(string message) {
		cin.clear();  // Clear the error flag
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << message << endl;
	}

	static bool isExistingCategoryName(string catName) {
		return store->isCategoryAvailable(catName);
	}

	static bool isValidTransaction(int transactionId) {
		return store->isTransactionAvailable(transactionId);
	}

	static bool isValidFieldOption(int input, const int(&options)[]) {
		if (isInvalidNumber(input))
			return false;

		int size = sizeof(*options) / sizeof(*options);
		return (find(options, options + size, input) != options + size);
	}

	static bool isInvalidNumber(double input) {
		return (cin.fail() || input < 0);
	}
};

