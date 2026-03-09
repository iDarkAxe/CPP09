#ifndef PMERGEME_TPP
#define PMERGEME_TPP

#include "PmergeMe.hpp"
#include <cstdlib>
#include <cmath>

template <typename T>
void PmergeMe::printContainer(const T& container, bool useIndex, const char *separator)
{
	if (useIndex)
	{
		size_t index = 0;
		for (typename T::const_iterator it = container.begin(); it != container.end(); it++)
		{
			std::cout << "it[" << index << "] = " << *it << std::endl;
			index++;
		}
	}
	else
	{
		for (typename T::const_iterator it = container.begin(); it != container.end(); ++it)
		{
			std::cout << *it << separator;
		}
		std::cout << std::endl;
	}
}

template <typename T>
void PmergeMe::printShort(const T &container, const char *separator)
{
	size_t numberOfElementsToPrint = max_short_args;
	for (typename T::const_iterator it = container.begin(); it != container.end() && numberOfElementsToPrint > 0; ++it)
	{
		std::cout << *it << separator;
		--numberOfElementsToPrint;
	}
	if (container.size() > max_short_args)
		std::cout << "[...]";
	std::cout << std::endl;
}

/**
 * @brief Store in the container from array of strings
 *
 * @param[in,out] container The container to store values into
 * @param[in] array array of strings
 */
template <typename T>
void PmergeMe::storeInLoop(T &container, const char *array[])
{
	if (!array)
		throw ArgumentEmptyException();
	// Check for duplicates, a insert on a set will fail if duplicate
	#if THROW_ERROR_IF_DUPLICATE == 1
		typeSet duplicate_test;
	#endif
	std::string item;
	char *pointer = NULL;
	typeElement value;

	size_t i;
	for (i = 0; array[i]; i++)
	{
		item = array[i];
		if (item.empty())
			throw ArgumentEmptyException();
		if (item.find_first_not_of("0123456789+") != std::string::npos)
		{
			if (DEBUG_LEVEL >= DEBUG)
				std::cerr << "Input numbers should only be positive integers" << std::endl;
			if (DEBUG_LEVEL >= INFO)
				std::cerr << item << std::endl;
			throw ArgumentInvalidException();
		}
		value = static_cast<typeElement>(strtoul(array[i], &pointer, 10));
		if (&pointer == &array[i] || pointer == NULL || (*pointer != 0 && *pointer != ' '))
		{
			if (DEBUG_LEVEL >= DEBUG)
			{
				std::cerr << "Input numbers should only be positive integers" << std::endl;
				std::cerr << "Faulty argument is '" << array[i] << "'" << std::endl;
			}
			throw ArgumentInvalidException();
		}
		#if THROW_ERROR_IF_DUPLICATE == 1
		if (!duplicate_test.insert(static_cast<typeElement>(value)).second)
		{
			if (DEBUG_LEVEL >= INFO)
			{
				std::cerr << "Faulty number is '" << value << "'" << std::endl;
			}
			if (THROW_ERROR_IF_DUPLICATE)
				throw DuplicateException();
		}
		#endif
		container.push_back(static_cast<typeElement>(value));
	}
	numberOfElements = i;
}

/**
 * @brief Insert a value into a sorted container using binary search
 *
 * @param[in,out] temp_container The container to insert into
 * @param[in] value The value to insert
 * @param[in] comparison_count number of comparison
 */
template <typename Container>
void PmergeMe::binaryInsertContainer(Container &temp_container, typename Container::value_type value, size_t limit, size_t &comparison_count)
{
	typename Container::iterator left = temp_container.begin();
	typename Container::iterator right = temp_container.begin();
	std::advance(right, std::min(limit, temp_container.size()));

	while (left != right)
	{
		typename Container::iterator mid = left;
		typename Container::difference_type dist = std::distance(left, right);
		std::advance(mid, dist / 2);

		comparison_count++;
		if (*mid < value)
			left = ++mid;
		else
			right = mid;
	}

	temp_container.insert(left, value);
}

/**
 * @brief Generate the Jacobsthal sequence up to a given number
 * see : https://en.wikipedia.org/wiki/Jacobsthal_number
 *
 * @param[in,out] n The upper limit for the sequence
 * @return T The generated Jacobsthal sequence
 */
template <typename Container>
Container PmergeMe::generateJacobsthalSequence(size_t n)
{
	Container jacobsthal;

	if (n == 0)
		return jacobsthal;
	jacobsthal.push_back(1);
	jacobsthal.push_back(1);
	if (n == 1)
		return jacobsthal;
	jacobsthal.push_back(3);
	size_t prev1 = 1;
	size_t prev2 = 3;
	size_t next;
	while (true)
	{
		next = prev2 + 2 * prev1;
		if (next > n)
			break;
		jacobsthal.push_back(next);
		prev1 = prev2;
		prev2 = next;
	}
	return jacobsthal;
}

template <typename Container>
Container PmergeMe::buildJacobsthalOrder(size_t size)
{
    Container order;
    if (size == 0)
		return order;

    Container jacob = generateJacobsthalSequence<Container>(size);
    size_t last_pos = 1;
    
    for (size_t i = 2; i < jacob.size(); ++i)
    {
        size_t current_jacob = jacob[i];
        size_t val = current_jacob;
        while (val > last_pos)
        {
            if (val <= size)
                order.push_back(val);
            val--;
        }
        last_pos = current_jacob;
    }
    size_t val = size;
    while (val > last_pos)
    {
        order.push_back(val);
        val--;
    }
    return order;
}

template <typename Container>
bool PmergeMe::verifyOrder(const Container &cont) const
{
	typeElement previous = 0;
	previous = *cont.begin();
	if (DEBUG_LEVEL >= DEBUG)
		std::cout << "Testing correct order on container: " << &cont << std::endl;
	for (typename Container::const_iterator it = cont.begin(); it != cont.end(); ++it)
	{
		if (DEBUG_LEVEL >= DEBUG)
			std::cout << "Comparing " << previous << " and " << *it << std::endl;
		if (previous > *it)
		{
			if (DEBUG_LEVEL >= INFO)
				std::cerr << "Elements out of order at index " << std::distance(cont.begin(), it) << ": " << previous << " and " << *(it) << std::endl;
			return false;
		}
		previous = *it;
	}
	return true;
}

template <typename Container>
void PmergeMe::printPairs(const Container &container, size_t pairSize) const
{
    size_t pairCount = container.size() / pairSize;
    std::cout << "______Level " << levelOfRecursion << " with pair size " << pairSize << "______" << std::endl;

    typename Container::const_iterator groupStart = container.begin();
    for (size_t i = 0; i < pairCount; ++i)
    {
        std::cout << i << "/" << pairCount - 1 << ": ";
        typename Container::const_iterator it = groupStart;
        for (size_t j = 0; j < pairSize; ++j, ++it)
        {
            if (Color::use_color && (j == pairSize / 2 - 1 || j == pairSize - 1))
                std::cout << Color::Red << *it << " " << Color::Color_Off;
            else
                std::cout << *it << " ";
        }
        std::cout << std::endl;
        std::advance(groupStart, pairSize);
    }

    // Straggler
    if (pairCount * pairSize < container.size())
    {
        std::cout << "Straggler: ";
        typename Container::const_iterator it = groupStart;
        while (it != container.end())
            std::cout << *it++ << " ";
        std::cout << std::endl;
    }
}
template <typename Container>
size_t PmergeMe::splitIntoPairsRecursive(Container &container, size_t pairSize, size_t &comparison_count)
{
    size_t pairCount = container.size() / pairSize;
    if (pairCount < 2)
        return pairSize;

    size_t halfSize = pairSize / 2;

    typename Container::iterator groupStart = container.begin();
    for (size_t p = 0; p < pairCount; ++p)
    {
        typename Container::iterator it1 = groupStart;
        typename Container::iterator it2 = groupStart;
        std::advance(it2, halfSize); // début de la 2e moitié
        typename Container::iterator LastOfFirstHalf = it2; // sauvegarde avant avancement
        --LastOfFirstHalf;

        typename Container::iterator LastOfSecondHalf = it2;
        std::advance(LastOfSecondHalf, halfSize - 1);

        comparison_count++;
        if (*LastOfFirstHalf > *LastOfSecondHalf)
        {
            for (size_t j = 0; j < halfSize; ++j, ++it1, ++it2)
                std::iter_swap(it1, it2); //same as std::swap(*it1, *it2) but for iterators
        }

        std::advance(groupStart, pairSize);
    }

    printPairs(container, pairSize);
    levelOfRecursion++;
    splitIntoPairsRecursive(container, pairSize * 2, comparison_count);
	return std::pow(2, levelOfRecursion);
}

#endif // PMERGEME_TPP
