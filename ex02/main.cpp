#include "PmergeMe.hpp"
#include <iostream>

/* Tester prototype */
// int tester(void);

/* Mandatory main function */
int main(int argc, const char **argv)
{
	// return (tester());
	if (argc < 2)
	{
		if (DEBUG_LEVEL >= INFO)
		{
			std::cout << "Usage: " << argv[0] << " \"[PositiveNumbers]\"\n" << "Ford-Johnson MergeInsert Sorter\n\n" << "[PositiveNumbers]:\n0-MAX_UNSIGNED_INT\t\tAll positive numbers (from 0 to the MAX_UNSIGNED_INT).\n\n" << "Example:\n" << argv[0] << "  3 5 9 7 4\n->\nBefore: 3 5 9 7 4\nAfter: 3 4 5 7 9\nTime to process a range of 5 elements with std::vector : 0.00031 us\nTime to process a range of 5 elements with std::list : 0.00014 us"  << '\n';
		}
		else
			std::cout << "Error\n";
		return (0);
	}
	PmergeMe pmergeme(argc - 1);
	try 
	{
		pmergeme.storeInVect(&argv[1]);
		pmergeme.storeInListFromVect();
		pmergeme.sort_FJMI_vect();
		std::cout << "Time to process a range of " << pmergeme.numberOfElements << " elements with std::vector : " << 0.00031 << " us" << std::endl;
		pmergeme.sort_FJMI_vect();
		std::cout << "Time to process a range of " << pmergeme.numberOfElements << " elements with std::list : " << 0.00031 << " us" << std::endl;
		// std::cout << "Vect :" << std::endl;
		// pmergeme.printAllVect();
		// std::cout << "\n";
		// std::cout << "List :" << std::endl;
		// pmergeme.printAllList();
	}
	catch(const std::exception& e)
	{
		if (DEBUG_LEVEL >= INFO)
			std::cout << e.what() << std::endl;
		else
			std::cout << "Error" << std::endl;
		return (1);
	}
}
