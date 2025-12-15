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

std::vector<unsigned int> &PmergeMe::getVector(void)
{
	return this->vect;
}

std::list<unsigned int> &PmergeMe::getList(void)
{
	return this->lst;
}

void PmergeMe::clear(void)
{
	this->vect.clear();
	this->lst.clear();
	this->comparison_count_vect.clear();
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
 * @param[in] array array of strings
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
		value = static_cast<unsigned int>(strtoul(array[i], &pointer, 10));
		if (&pointer == &array[i] || pointer == NULL || (*pointer != 0 && *pointer != ' '))
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
		PmergeMe::printShort(duplicate_test);
	else
		PmergeMe::printAll(duplicate_test);
}

void PmergeMe::storeInListFromVect(void)
{
	lst.assign(vect.begin(), vect.end());
}

void PmergeMe::sort_FJMI_vect(void)
{
	size_t comparison_count = 0;

	sort_FJMI_vect_recursive(this->vect, comparison_count);
	this->comparison_count_vect.push_back(comparison_count);
}

void PmergeMe::sort_FJMI_vect_recursive(std::vector<unsigned int> &temp_vec, size_t &comparison_count)
{
	if (DEBUG_LEVEL >= DEBUG)
		std::cout << "Recursive call with vector of size " << temp_vec.size() << std::endl;
	if (temp_vec.size() <= 1)
		return;

	/* //= Phase 1 =//
	Group the elements into pairs
	and leave the last if odd number of elements. */
	std::vector<std::pair<unsigned int, unsigned int> > pairs;
	unsigned int oddElement = 0;
	bool hasOddElement = false;

	if (temp_vec.size() % 2 == 1)
	{
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

	// Recursive sort of the larger elements
	if (larger.size() > 1)
		sort_FJMI_vect_recursive(larger, comparison_count);

	/* //= Phase 3 : Insertion =//
	Insert the smaller elements into the sorted sequence
	using the Jacobsthal sequence */
	std::vector<unsigned int> result;

	// Instert smaller[0] and all the larger
	if (!smaller.empty())
		result.push_back(smaller[0]);
	result.insert(result.end(), larger.begin(), larger.end());

	/* //= Phase 4 : Final Insertion =//
	Insert the smaller elements into the sorted sequence using binary search */
	typedef std::vector<size_t> jacobsthalType;
	jacobsthalType jacobsthal = generateJacobsthalSequence<jacobsthalType>(smaller.size());
	std::vector<bool> inserted(smaller.size(), false);
	if (!inserted.empty())
		inserted[0] = true;

	// Insertion as Jacobsthal
	size_t prev_jacob = 1;
	size_t j = 0;
	for (jacobsthalType::iterator it = jacobsthal.begin(); it != jacobsthal.end(); ++it, j++)
	{
		size_t jacob_idx = *it;
		if (jacob_idx >= smaller.size())
			continue;

		// Insert in descending  order between prev_jacob and jacob_idx
		for (size_t i = jacob_idx; i > prev_jacob && i > 0; --i)
		{
			if (!inserted[i])
			{
				// The search zone is limited by the position of the associated larger
				size_t search_limit = i + j + 1;
				if (search_limit > result.size())
					search_limit = result.size();

				binaryInsertContainer(result, smaller[i], search_limit, comparison_count);
				inserted[i] = true;
			}
		}
		prev_jacob = jacob_idx;
	}

	// Insérer les éléments restants
	for (size_t i = 1; i < smaller.size(); ++i)
	{
		if (!inserted[i])
			binaryInsertContainer(result, smaller[i], result.size(), comparison_count);
	}

	if (hasOddElement)
		binaryInsertContainer(result, oddElement, result.size(), comparison_count);

	temp_vec = result;
}

void PmergeMe::sort_FJMI_lst(void)
{
	size_t comparison_count = 0;

	sort_FJMI_lst_recursive(this->lst, comparison_count);
	this->comparison_count_vect.push_back(comparison_count);
}

void PmergeMe::sort_FJMI_lst_recursive(std::list<unsigned int> &temp_lst, size_t &comparison_count)
{

	if (DEBUG_LEVEL >= DEBUG)
		std::cout << "Recursive call with list of size " << temp_lst.size() << std::endl;
	if (temp_lst.size() <= 1)
		return;

	/* //= Phase 1 =//
	Group the elements into pairs
	and leave the last if odd number of elements. */
	std::vector<std::pair<unsigned int, unsigned int> > pairs;
	unsigned int oddElement = 0;
	bool hasOddElement = false;

	std::list<unsigned int>::const_iterator it = temp_lst.begin();
	while (it != temp_lst.end())
	{
		unsigned int a = *it++;
		if (it == temp_lst.end())
		{
			hasOddElement = true;
			oddElement = a;
			break;
		}
		unsigned int b = *it++;
		if (a > b)
			std::swap(a, b);
		comparison_count++; // Counting comparison for performance analysis
		pairs.push_back(std::make_pair(a, b));
	}

	/* //= Phase 2 =//
	Recursively sort the larger elements of each pair in ascending order */
	std::list<unsigned int> larger;
	std::list<unsigned int> smaller;

	for (std::vector<std::pair<unsigned int, unsigned int> >::const_iterator it = pairs.begin(); it != pairs.end(); ++it)
	{
		larger.push_back(it->second);
		smaller.push_back(it->first);
	}

	// Recursive sort of the larger elements
	if (larger.size() > 1)
		sort_FJMI_lst_recursive(larger, comparison_count);

	/* //= Phase 3 : Insertion =//
	Insert the smaller elements into the sorted sequence
	using the Jacobsthal sequence */
	std::list<unsigned int> result;

	// Insert *smaller and all the larger
	if (!smaller.empty())
		result.push_back(*smaller.begin());
	result.insert(result.end(), larger.begin(), larger.end());

	typedef std::list<size_t> jacobsthalType;
	jacobsthalType jacobsthal = generateJacobsthalSequence<jacobsthalType>(smaller.size());
	std::vector<bool> inserted(smaller.size(), false);
	if (!inserted.empty())
		inserted[0] = true;

	// Insertion as Jacobsthal
	size_t prev_jacob = 1;
	size_t j = 0;
	for (jacobsthalType::const_iterator it_jac = jacobsthal.begin(); it_jac != jacobsthal.end(); ++it_jac, ++j)
	{
		size_t jacob_idx = *it_jac;
		if (jacob_idx >= smaller.size())
			continue;

		// Insert in descending  order between prev_jacob and jacob_idx
		for (size_t i = jacob_idx; i > prev_jacob && i > 0; --i)
		{
			if (!inserted[i])
			{
				// The search zone is limited by the position of the associated larger
				size_t search_limit = i + j + 1;
				if (search_limit > result.size())
					search_limit = result.size();

				std::list<unsigned int>::const_iterator it_sm = smaller.begin();
				std::advance(it_sm, i);
				binaryInsertContainer(result, *it_sm, search_limit, comparison_count);
				inserted[i] = true;
			}
		}
		prev_jacob = jacob_idx;
	}

	for (size_t i = 1; i < smaller.size(); ++i)
	{
		if (!inserted[i])
		{
			std::list<unsigned int>::const_iterator it_sm = smaller.begin();
			std::advance(it_sm, i);
			binaryInsertContainer(result, *it_sm, result.size());
		}
	}

	if (hasOddElement)
		binaryInsertContainer(result, oddElement, result.size());

	temp_lst = result;
}

const char *PmergeMe::ArgumentEmptyException::what() const throw()
{
	return "At least one argument is empty";
}

const char *PmergeMe::ArgumentInvalidException::what() const throw()
{
	return "This argument is invalid";
}

const char *PmergeMe::DuplicateException::what() const throw()
{
	return "There is a duplicate";
}
