#include "PmergeMe.hpp"
#include <iostream>
#include <sstream>
#include <set>
#include <cstdlib>

PmergeMe::PmergeMe()
{
	// std::cout << "Default Constructor of PmergeMe" << std::endl;
	this->show_short_args = true;
	this->max_short_args = 4;
}

PmergeMe::PmergeMe(size_t numberOfElement)
{
	// std::cout << "Constructor with size of PmergeMe" << std::endl;
	this->vect.reserve(numberOfElement);
	this->show_short_args = true;
	this->max_short_args = 4;
}

PmergeMe::~PmergeMe()
{
	;
}

PmergeMe::PmergeMe(const PmergeMe &f)
{
	*this = f;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	if (this != &other)
	{
		this->vect = other.vect;
		this->lst = other.lst;
		return *this;
	}
	return *this;
}

std::vector<unsigned int>& PmergeMe::getVector(void)
{
	return this->vect;
}

std::list<unsigned int>& PmergeMe::getList(void)
{
	return this->lst;
}

void PmergeMe::clear(void)
{
	this->vect.clear();
	this->lst.clear();
	this->comparison_count.clear();
	this->numberOfElements = 0;
}

/**
 * @brief Print all elements in the vector
 */
void PmergeMe::printAllVect(void) const
{
	size_t index = 0;
	for (std::vector<unsigned int>::const_iterator it = this->vect.begin(); it != vect.end(); it++)
	{
		std::cout << "it[" << index << "] = " << *it << std::endl;
		index++;
	}
}

/**
 * @brief Print all elements in the list
 */
void PmergeMe::printAllList(void) const
{
	size_t index = 0;
	for (std::list<unsigned int>::const_iterator it = this->lst.begin(); it != lst.end(); it++)
	{
		std::cout << "it[" << index << "] = " << *it << std::endl;
		index++;
	}
}

/**
 * @brief Store in the two container at once
 * 
 * @param[in] array[] array of strings
 */
void PmergeMe::storeInVect(const char *array[])
{
	if (!array)
		throw ArgumentEmptyException();
	std::set<unsigned int> duplicate_test;
	std::string item;
	char *pointer = NULL;
	unsigned int value;
	
	size_t i;
	for (i = 0; array[i]; i++)
	{
		item = array[i];
		if (item.empty())
			throw ArgumentEmptyException();
		if (item.find_first_not_of("0123456789+") != std::string::npos)
		{
			if (DEBUG_LEVEL >= DEBUG)
				std::cout << "Input numbers should only be positive integers" << std::endl;
			if (DEBUG_LEVEL >= INFO)
				std::cout << item << std::endl;
			throw ArgumentInvalidException();
		}
		value = strtoul(array[i], &pointer, 10);
		if (&pointer == &array[i] || pointer == NULL || (*pointer != 0 && *pointer != ' ' ))
		{
			if (DEBUG_LEVEL >= DEBUG)
			{
				std::cout << "Input numbers should only be positive integers" << std::endl;
				std::cout << "Faulty argument is '" << array[i] << "'" << std::endl;
			}
			throw ArgumentInvalidException();
		}
		if (!duplicate_test.insert(static_cast<unsigned int>(value)).second)
		{
			if (DEBUG_LEVEL >= INFO)
			{
				std::cout << "Faulty number is '" << value << "'" << std::endl;
			}
			if (THROW_ERROR_IF_DUPLICATE)
				throw DuplicateException();
		}
		vect.push_back(static_cast<unsigned int>(value));
	}
	numberOfElements = i;
	std::cout << "Before : ";
	if (show_short_args)
		PmergeMe::printShort(vect);
	else
		PmergeMe::printAll(vect);
	std::cout << "After :  ";
	if (show_short_args)
		PmergeMe::printShort(vect);
	else
		PmergeMe::printAll(duplicate_test);
}

void PmergeMe::storeInListFromVect(void)
{
	lst.assign(vect.begin(), vect.end());
}

void PmergeMe::sort_FJMI_vect(void)
{
	size_t comparison_count;

	comparison_count = sort_FJMI_vect_recursive(this->vect);
	this->comparison_count.push_back(comparison_count);
	return;
}

size_t PmergeMe::sort_FJMI_vect_recursive(std::vector<unsigned int>& temp_vec)
{
	static size_t comparison_count = 0;

	if (DEBUG_LEVEL >= DEBUG)
		std::cout << "Recursive call with vector of size " << temp_vec.size() << std::endl;
	if (temp_vec.size() <= 1)
		return (0);

	/* //= Phase 1 =//
	Group the elements into pairs 
	and leave the last if odd number of elements. */
	std::vector<std::pair<unsigned int, unsigned int> > pairs;
	unsigned int oddElement = 0;
	bool hasOddElement = false;

	if (temp_vec.size() % 2 == 1) {
		hasOddElement = true;
		oddElement = temp_vec.back();
	}

	size_t pairCount = temp_vec.size() / 2;
	for (size_t i = 0; i < pairCount; ++i)
	{
		unsigned int a = temp_vec[2 * i];
		unsigned int b = temp_vec[2 * i + 1];
		if (a > b)
			std::swap(a, b);
		comparison_count++; // Counting comparison for performance analysis
		pairs.push_back(std::make_pair(a, b));
	}

	/* //= Phase 2 =//
	Recursively sort the larger elements of each pair in ascending order */
	std::vector<unsigned int> larger;
	std::vector<unsigned int> smaller;

	for (std::vector<std::pair<unsigned int, unsigned int> >::const_iterator it = pairs.begin(); it != pairs.end(); ++it)
	{
		larger.push_back(it->second);
		smaller.push_back(it->first);
	}

	// Tri récursif des plus grands éléments
	if (larger.size() > 1) {
		sort_FJMI_vect_recursive(larger);
	}

	/* //= Phase 3 : Insertion =//
	Insert the smaller elements into the sorted sequence
	using the Jacobsthal sequence */
	std::vector<unsigned int> result;

	if (!smaller.empty()) {
		result.push_back(smaller[0]);
	}
	result.insert(result.end(), larger.begin(), larger.end());

	std::vector<size_t> jacobsthal = generateJacobsthalSequence<std::vector<size_t> >(smaller.size());
	std::vector<bool> inserted(smaller.size(), false);
	if (!inserted.empty())
		inserted[0] = true;

	/* //= Phase 4 : Final Insertion =//
	Insert the smaller elements into the sorted sequence using binary search */
	for (std::vector<size_t>::const_iterator it = jacobsthal.begin(); it != jacobsthal.end(); ++it)
	{
		size_t idx = *it;
		if (idx < smaller.size() && idx > 0 && !inserted[idx]) {
			binaryInsertContainer(result, smaller[idx], result.size());
			inserted[idx] = true;
		}
	}

	for (size_t i = 1; i < smaller.size(); ++i)
	{
		if (!inserted[i])
			binaryInsertContainer(result, smaller[i], result.size());
	}

	if (hasOddElement)
		binaryInsertContainer(result, oddElement, result.size());

	temp_vec = result;
	return (comparison_count);
}

void PmergeMe::sort_FJMI_lst(void)
{
	size_t comparison_count = 0;

	// comparison_count = sort_FJMI_lst_recursive(this->lst);
	this->comparison_count.push_back(comparison_count);
	return;
}



const char* PmergeMe::ArgumentEmptyException::what() const throw()
{
	return "At least one argument is empty";
}

const char* PmergeMe::ArgumentInvalidException::what() const throw()
{
	return "This argument is invalid";
}

const char* PmergeMe::DuplicateException::what() const throw()
{
	return "There is a duplicate";
}
