#include "TError.h"


TError::TError(char* error,
	char* function,
	char* file,
	int line)
{
	err = move(error);
	func = move(function);
	fil = move(file);
	lin = line;
	//std::cout << "\nError: " << err << "\nFunction: " << func << "\nFile: " << fil <<"\nLine:"<<line<< endl;
}

TError::TError(const char* error, const char* function, const char* file, int line)
{
	err = move(error);
	func = move(function);
	fil = move(file);
	lin = line;
	//std::cout << "\nError: " << err << "\nFunction: " << func << "\nFile: " << fil << "\nLine:" << line << endl;
}

void TError::what()
{
	std::cout << "\nError: " << err << "\nFunction: " << func << "\nFile: " << fil << "\nLine:" << lin << std::endl;
}