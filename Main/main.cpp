#include <iostream>
#include <cstddef>
#include "TString.h"
#include "TStack.h"
#include "TQueue.h"



int main()
{
	TQueue<int> a(2);
	a.Put(3);
	a.Put(6);
	a.Get();
	std::cout << a;
}