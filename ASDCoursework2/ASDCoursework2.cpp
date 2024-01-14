// ASDCoursework2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Category.h"
#include "Program.h"
using namespace std;

DataStore* Program::store;
int main()
{
    Program::mainMenu();  

    //cout << "-1.0000 < 0" << ((-1.0 < 1) ? "true": "false") << endl;
    /*int number = 2;
    int number2 = 5;
    int* ptr = &number;
    *ptr = 4;
    cout << "Number: " << number << "reference: " << ptr << endl;*/
    return 0;
}
