#include "PmergeMe.hpp"
#include <iostream>
#include <sys/time.h>
#include <string>
/* Tester prototype */
// int tester(void);

long delta_timeval(const struct timeval &start, const struct timeval &end);

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
		struct timeval start_tv, end_tv;
		long microseconds;
		gettimeofday(&start_tv, NULL);
		pmergeme.storeInVect(&argv[1]);
		gettimeofday(&end_tv, NULL);
		microseconds = delta_timeval(start_tv, end_tv);
		std::cout << "Storing in vector took " << microseconds << " us" << std::endl;
		pmergeme.storeInListFromVect();
		gettimeofday(&start_tv, NULL);
		pmergeme.sort_FJMI_vect();
		gettimeofday(&end_tv, NULL);
		microseconds = delta_timeval(start_tv, end_tv);
		std::cout << "Time to process a range of " << pmergeme.numberOfElements << " elements with std::vector : " << microseconds << " us" << std::endl;
		gettimeofday(&start_tv, NULL);
		pmergeme.sort_FJMI_lst();
		gettimeofday(&end_tv, NULL);
		microseconds = delta_timeval(start_tv, end_tv);
		std::cout << "Time to process a range of " << pmergeme.numberOfElements << " elements with std::list : " << microseconds << " us" << std::endl;
		if (DEBUG_LEVEL >= INFO)
		{
			std::cout << "Nbr of Comparisons :" << std::endl;
			PmergeMe::printAll(pmergeme.comparison_count);
		}
		if (DEBUG_LEVEL >= DEBUG)
		{
			std::cout << "Vect :" << std::endl;
			PmergeMe::printAll(pmergeme.getVector());
			// std::cout << "\n";
			// std::cout << "List :" << std::endl;
			// PmergeMe::printAll(pmergeme.getList());
		}
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

long delta_timeval(const struct timeval &start, const struct timeval &end)
{
	return (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
}

int tester(void)
{
	const char* test0[] = {"5", "3", "7", NULL};
	const char* test1[] = {"5", "3", "1", NULL};
	const char* test2[] = {"5", "3", "1", "123", "87", "41", NULL};
	const char *test3[] = {"36083", "29268", "28059", "75396", "53211", "71200", "58414", "18446", "87365", "75070", NULL};
	const char *test4[] = {"42346", "88031", "46152", "26817", "97818", "38034", "25125", "24311", "14630", "2412", NULL};
	const char *test5[] = {"89075", "72891", "52", "26817", "26647", "38034", "54666", "26829", "229", "8888", NULL};

	// Tableau de pointeurs vers ces tableaux
	const char *
		*tests[] = {test0, test1, test2, test3, test4, test5, NULL};
	const size_t size[] = {3, 3, 6, 10, 10, 10};

	try 
	{
		for (size_t index = 0; tests[index]; index++)
		{
			PmergeMe pmergme(size[index]);
			std::cout << "testing with " << tests[index] << ", size = "<< size[index] << ":\n";
			pmergme.storeInVect(tests[index]);
			pmergme.printAllVect();
		}
	}
	catch(const std::exception& e)
	{
		if (DEBUG_LEVEL >= DEBUG)
			std::cout << e.what() << std::endl;
		else
			std::cout << "Error" << std::endl;
		return (1);
	}
	return (0);
}
