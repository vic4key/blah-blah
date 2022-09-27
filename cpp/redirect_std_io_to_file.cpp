#include <iostream>
#include <fstream>

int main ()
{
	std::ofstream file;
	file.open ("std_io.txt");
	std::streambuf* sbuf = std::cout.rdbuf();
	std::cout.rdbuf(file.rdbuf());
	// cout is now pointing to a file
	return 0;
}