#include "PmergeMe.hpp"
#include <iostream>
#include <sys/time.h>
#include <iomanip>
#include <string>

/* Tester prototype */
int tester(void);

void print_example(const char* program_name);
long delta_timeval(const struct timeval &start, const struct timeval &end);

#define USE_STORE_FROM 0

/* Mandatory main function */
int main(int argc, const char **argv)
{
	// return (tester());
	if (argc < 2)
	{
		if (DEBUG_LEVEL >= INFO)
			print_example(argv[0]);
		else
			std::cout << "Error\n";
		return (1);
	}
	size_t numberOfArgs = static_cast<size_t>(argc - 1);
	const char **array = &argv[1];

	struct timeval start_tv, end_tv;
	long microseconds_vect = 0, microseconds_list = 0;
	std::vector<PmergeMe::typeElement> vect;
	std::vector<size_t> comparison_count_vect;
	std::list<PmergeMe::typeElement> list;

	PmergeMe::show_short_args = false;
	PmergeMe::max_short_args = 4;

	try 
	{
		// Storing values in vector
		gettimeofday(&start_tv, NULL);
		PmergeMe::storeInLoop(vect, array);
		gettimeofday(&end_tv, NULL);
		microseconds_vect += delta_timeval(start_tv, end_tv);

		// Storing values in list
		gettimeofday(&start_tv, NULL);
		#if USE_STORE_FROM == 1
		list.assign(vect.begin(), vect.end());
		#else
		PmergeMe::storeInLoop(list, array);
		#endif
		gettimeofday(&end_tv, NULL);
		microseconds_list += delta_timeval(start_tv, end_tv);

		std::cout << "Before : ";
		PmergeMe::printShort(vect);

		// Vector sorting
		gettimeofday(&start_tv, NULL);
		comparison_count_vect.push_back(PmergeMe::sort_FJMI(vect));
		gettimeofday(&end_tv, NULL);
		microseconds_vect += delta_timeval(start_tv, end_tv);

		// List sorting
		gettimeofday(&start_tv, NULL);
		comparison_count_vect.push_back(PmergeMe::sort_FJMI(list));
		gettimeofday(&end_tv, NULL);
		microseconds_list += delta_timeval(start_tv, end_tv);

		std::cout << "After :  ";
		PmergeMe::printShort(vect);
		std::cout << "Time to process a range of " << numberOfArgs << " elements with std::vector : " << std::setw(8) << microseconds_vect << " us" << std::endl;

		std::cout << "Time to process a range of " << numberOfArgs << " elements with std::list   : " << std::setw(8) << microseconds_list << " us" << std::endl;
		
		if (vect.size() != list.size() || vect.size() != numberOfArgs)
		{
			std::cerr << "Error: Vector and List sizes differ after sorting! Vector size: " << vect.size() << ", List size: " << list.size() << std::endl;
		}

		std::cout << "Nbr of Comparisons :" << std::endl;
		PmergeMe::printContainer(comparison_count_vect);
	}
	catch(const std::exception& e)
	{
		if (DEBUG_LEVEL >= INFO)
			std::cerr << e.what() << std::endl;
		else
			std::cerr << "Error" << std::endl;
		return (1);
	}
	return (0);
}

void print_example(const char* program_name)
{
	std::cout << "Usage: " << program_name << " \"[PositiveNumbers]\"\n"
		<< "Ford-Johnson MergeInsert Sorter\n\n"
		<< "[PositiveNumbers]:\n"
		<< "0-MAX_UNSIGNED_INT\t\tAll positive numbers (from 0 to the MAX_UNSIGNED_INT).\n\n"
		<< "Example:\n$> " 
		<< program_name << "  3 5 9 7 4\n"
		<< "Before: 3 5 9 7 4\n"
		<< "After: 3 4 5 7 9\n"
		<< "Time to process a range of 5 elements with std::vector : 0.00031 us\n"
		<< "Time to process a range of 5 elements with std::list :   0.00014 us"  << '\n';
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
			std::vector<PmergeMe::typeElement> vect;
			std::cout << "testing with " << tests[index] << ", size = "<< size[index] << ":\n";
			PmergeMe::storeInLoop(vect, tests[index]);
			PmergeMe::sort_FJMI(vect);
			PmergeMe::printContainer(vect);
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
