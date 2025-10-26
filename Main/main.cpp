#include <iostream>
#include <cstddef>
#include "TString.h"
#include "TStack.h"
#include "TQueue.h"
#include "TMultyStack.h"



int main()
{
	TMultyStack<int> a(2, 2);
	a.Push(1, 2);
	a.Push(0, 4);
	a.Push(0, 7);
	a.Push(0, 3);
	std::cout << a;
}