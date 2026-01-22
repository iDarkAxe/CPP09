#include "PmergeMe.hpp"
#include <iostream>
#include <sys/time.h>
#include <string>
/* Tester prototype */
int tester(void);

long delta_timeval(const struct timeval &start, const struct timeval &end);

// int more(int argc, const char **argv);

#define COUNT_TIME_WITH_STORE 1
#define USE_STORE_FROM 1

/* Mandatory main function */
int main(int argc, const char **argv)
{
	// return (tester());
	// return (more(argc, argv));
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
	PmergeMe pmergeme(static_cast<size_t>(argc - 1));
	try 
	{
		// pmergeme.show_short_args = false;
		// pmergeme.max_short_args = 4;

		// Storing values in vector
		struct timeval start_tv, end_tv;
		long microseconds;
		gettimeofday(&start_tv, NULL);
		pmergeme.storeInVect(&argv[1]);
		#if USE_STORE_FROM == 1
		pmergeme.storeInListFromVect();
		#endif

		if (DEBUG_LEVEL >= DEBUG)
		{
			gettimeofday(&end_tv, NULL);
			microseconds = delta_timeval(start_tv, end_tv);
			std::cout << "Storing in containers took " << microseconds << " us" << std::endl;
		}
			
		std::cout << "Before : ";
		if (pmergeme.show_short_args)
			PmergeMe::printShort(pmergeme.getVector());
		else
			PmergeMe::printAll(pmergeme.getVector());
		std::cout << "After :  ";
		if (pmergeme.show_short_args)
			PmergeMe::printShort(pmergeme.sorted_set);
		else
			PmergeMe::printAll(pmergeme.sorted_set);

		// Vector sorting
		#if COUNT_TIME_WITH_STORE == 0
		gettimeofday(&start_tv, NULL);
		#endif
		pmergeme.sort_FJMI_vect();
		gettimeofday(&end_tv, NULL);
		microseconds = delta_timeval(start_tv, end_tv);
		std::cout << "Time to process a range of " << pmergeme.numberOfElements << " elements with std::vector : " << microseconds << " us" << std::endl;

		// Storing values in list
		#if USE_STORE_FROM == 0
		gettimeofday(&start_tv, NULL);
		pmergeme.storeInList(&argv[1]);
		gettimeofday(&end_tv, NULL);
		#endif

		// List sorting
		#if COUNT_TIME_WITH_STORE == 0
		gettimeofday(&start_tv, NULL);
		#endif
		pmergeme.sort_FJMI_lst();
		gettimeofday(&end_tv, NULL);
		microseconds = delta_timeval(start_tv, end_tv);
		std::cout << "Time to process a range of " << pmergeme.numberOfElements << " elements with std::list : " << microseconds << " us" << std::endl;

		if (DEBUG_LEVEL >= INFO)
		{
			std::cout << "Nbr of Comparisons :" << std::endl;
			PmergeMe::printAll(pmergeme.comparison_count_vect);
		}
		if (DEBUG_LEVEL >= DEBUG)
		{
			std::cout << "Vect :" << std::endl;
			pmergeme.printAllVect();
			std::cout << "\n";
			std::cout << "List :" << std::endl;
			pmergeme.printAllList();
		}
	}
	catch(const std::exception& e)
	{
		if (DEBUG_LEVEL >= INFO)
			std::cerr << e.what() << std::endl;
		else
			std::cerr << "Error" << std::endl;
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
			PmergeMe pmergeme(size[index]);
			pmergeme.show_short_args = false;
			std::cout << "testing with " << tests[index] << ", size = "<< size[index] << ":\n";
			pmergeme.storeInVect(tests[index]);
			pmergeme.sort_FJMI_vect();
			pmergeme.printAllVect();
		}
	}
	catch(const std::exception& e)
	{
		if (DEBUG_LEVEL >= DEBUG)
			std::cerr << e.what() << std::endl;
		else
			std::cerr << "Error" << std::endl;
		return (1);
	}
	return (0);
}
