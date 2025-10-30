#include <iostream>
#include <cstddef>
#include "TString.h"
#include "TStack.h"
#include "TQueue.h"
#include "TMultyStack.h"



int main()
{
	TMultyStack<char> d(3, 3);
	d.Push(0, 'a');
	d.Push(0, 'b');
	d.Push(0, 'c');
	d.Push(1, 'f');
	d.Push(1, 'g');
	d.Push(1, 'h');
	d.Push(2, 'j');
	cout << d;
	d.Push(1, 'k');
	cout << d;
	return 0;
}