#include "PmergeMe.hpp"
#include <iostream>
#include <sstream>

namespace Color {
	const bool use_color = 1;
	// Text Reset;
	const char *Color_Off="\033[0m";
	// Regular Colors
	const char *Red="\033[0;31m";
}

size_t PmergeMe::max_short_args = 4;

PmergeMe::PmergeMe() : levelOfRecursion(0), numberOfElements(0)
{
	// std::cout << "Default Constructor of PmergeMe" << std::endl;
	this->show_short_args = true;
}

PmergeMe::PmergeMe(size_t startingSize) : levelOfRecursion(0), numberOfElements(0)
{
	// std::cout << "Constructor with size of PmergeMe" << std::endl;
	this->vect.reserve(startingSize);
	this->show_short_args = true;
}

PmergeMe::~PmergeMe()
{
}

PmergeMe::PmergeMe(const PmergeMe &f)
{
	*this = f;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	if (this == &other)
		return *this;
	this->vect = other.vect;
	this->lst = other.lst;
	this->levelOfRecursion = other.levelOfRecursion;
	this->numberOfElements = other.numberOfElements;
	this->comparison_count_vect = other.comparison_count_vect;
	return *this;
}

// const PmergeMe::typeVect &PmergeMe::getVector(void) const
// {
// 	return this->vect;
// }

// const PmergeMe::typeList &PmergeMe::getList(void) const
// {
// 	return this->lst;
// }

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
	printContainer(this->vect);
}

/**
 * @brief Print some elements in the vector
 */
void PmergeMe::printShortVect(void) const
{
	printShort(this->vect);
}

/**
 * @brief Print all elements in the list
 */
void PmergeMe::printAllList(void) const
{
	printContainer(this->lst);
}

/**
 * @brief Store in the vector container
 *
 * @param[in] array array of strings
 */
void PmergeMe::storeInVect(const char *array[])
{
	storeInLoop(this->vect, array);
}

/**
 * @brief Store in the list container
 *
 * @param[in] array array of strings
 */
void PmergeMe::storeInList(const char *array[])
{
	storeInLoop(this->lst, array);
}

void PmergeMe::storeValues(const char *array[])
{
	storeInVect(array);
	storeInListFromVect();
}

void PmergeMe::storeInListFromVect(void)
{
	lst.assign(vect.begin(), vect.end());
}

void PmergeMe::storeInVectFromList(void)
{
	vect.assign(lst.begin(), lst.end());
}

// bool PmergeMe::areSameSize(void) const
// {
// 	return this->vect.size() == this->lst.size();
// }

void PmergeMe::sort_FJMI_vect(void)
{
	size_t comparison_count = 0;

	const size_t start_size = this->vect.size();
	if (start_size <= 1)
		return;

	// Phase 1: recursive pairing
	size_t maxGroupSize = splitIntoPairsRecursive<typeVect>(this->vect, 2, comparison_count);
	(void)maxGroupSize;
	if (start_size != this->vect.size())
	{
		// if (DEBUG_LEVEL >= INFO)
		std::cerr << "Error: Vector size changed during sorting! Before: " << start_size << ", After: " << this->vect.size() << std::endl;
	}
	if (!verifyOrder(this->vect))
	{
		// if (DEBUG_LEVEL >= INFO)
		std::cerr << "Vector is not sorted correctly!" << std::endl;
	}
	this->comparison_count_vect.push_back(comparison_count);
}

void PmergeMe::sort_FJMI_vect_recursive(typeVect &temp_vec, size_t &comparison_count)
{
	if (DEBUG_LEVEL >= DEBUG)
		std::cout << "Recursive call with vector of size " << temp_vec.size() << std::endl;
	if (temp_vec.size() <= 1)
		return;

	/* //= Phase 1 =//
	Group the elements into pairs
	and leave the last if odd number of elements. */
	std::vector<std::pair<typeElement, typeElement> > pairs;
	typeElement oddElement = 0;
	bool hasOddElement = false;
	// createPairs(temp_vec, pairs, comparison_count, oddElement, hasOddElement);

	/* //= Phase 2 =//
	Recursively sort the larger elements of each pair in ascending order */
	typeVect larger;
	typeVect smaller;

	for (std::vector<std::pair<typeElement, typeElement> >::const_iterator it = pairs.begin(); it != pairs.end(); ++it)
	{
		larger.push_back(it->second);
		smaller.push_back(it->first);
	}

	// Recursive sort of the larger elements
	if (larger.size() > 1)
		sort_FJMI_vect_recursive(larger, comparison_count);

	//= Phase 3 : Insertion =//
	// Start with the (recursively) sorted larger chain
	typeVect result(larger.begin(), larger.end());

	// Insert ALL smaller elements (including index 0) into the full sorted range
	for (size_t i = 0; i < smaller.size(); ++i)
		binaryInsertContainer(result, smaller[i], result.size(), comparison_count);

	// Insert possible odd element at the end
	if (hasOddElement)
		binaryInsertContainer(result, oddElement, result.size(), comparison_count);

	temp_vec = result;
}

void PmergeMe::sort_FJMI_lst(void)
{
	size_t comparison_count = 0;

	size_t start_size = this->lst.size();
	sort_FJMI_lst_recursive(this->lst, comparison_count);
	if (start_size != this->lst.size())
	{
		// if (DEBUG_LEVEL >= INFO)
		std::cerr << "Error: List size changed during sorting! Before: " << start_size << ", After: " << this->lst.size() << std::endl;
	}
	if (!verifyOrder(this->lst))
	{
		// if (DEBUG_LEVEL >= INFO)
		std::cerr << "List is not sorted correctly!" << std::endl;
	}
	this->comparison_count_vect.push_back(comparison_count);
}

void PmergeMe::sort_FJMI_lst_recursive(typeList &temp_lst, size_t &comparison_count)
{

	if (DEBUG_LEVEL >= DEBUG)
		std::cout << "Recursive call with list of size " << temp_lst.size() << std::endl;
	if (temp_lst.size() <= 1)
		return;

	/* //= Phase 1 =//
	Group the elements into pairs
	and leave the last if odd number of elements. */
	std::vector<std::pair<typeElement, typeElement> > pairs;
	typeElement oddElement = 0;
	bool hasOddElement = false;
	// createPairs(temp_lst, pairs, comparison_count, oddElement, hasOddElement);
	
	/* //= Phase 2 =//
	Recursively sort the larger elements of each pair in ascending order */
	typeList larger;
	typeList smaller;

	for (std::vector<std::pair<typeElement, typeElement> >::const_iterator it = pairs.begin(); it != pairs.end(); ++it)
	{
		larger.push_back(it->second);
		smaller.push_back(it->first);
	}

	// Recursive sort of the larger elements
	if (larger.size() > 1)
		sort_FJMI_lst_recursive(larger, comparison_count);

	//= Phase 3 : Insertion =//
	typeList result(larger.begin(), larger.end());
	// Insert ALL smaller elements into the full sorted range
	for (typeList::const_iterator it_sm = smaller.begin(); it_sm != smaller.end(); ++it_sm)
		binaryInsertContainer(result, *it_sm, result.size(), comparison_count);

	if (hasOddElement)
		binaryInsertContainer(result, oddElement, result.size(), comparison_count);

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
