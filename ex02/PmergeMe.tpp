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
		for (typename T::const_iterator it = container.begin(); it != container.end(); ++it)
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
		if (pointer == array[i] || pointer == NULL || (*pointer != 0 && *pointer != ' '))
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

    if (pairCount < 1)
        return pairSize / 2;
    size_t halfSize = pairSize / 2;

    // Phase 1: Compare and swap pairs so the larger element's group is second
    typename Container::iterator groupStart = container.begin();
    for (size_t p = 0; p < pairCount; ++p)
    {
        typename Container::iterator it2 = groupStart;
        std::advance(it2, halfSize);
        typename Container::iterator LastOfFirstHalf = it2;
        --LastOfFirstHalf;

        typename Container::iterator LastOfSecondHalf = it2;
        std::advance(LastOfSecondHalf, halfSize - 1);

        comparison_count++;
        if (*LastOfFirstHalf > *LastOfSecondHalf)
        {
            typename Container::iterator s1 = groupStart;
            typename Container::iterator s2 = groupStart;
            std::advance(s2, halfSize);
            for (size_t j = 0; j < halfSize; ++j, ++s1, ++s2)
                std::iter_swap(s1, s2);
        }

        std::advance(groupStart, pairSize);
    }

    if (DEBUG_LEVEL >= DEBUG)
        printPairs(container, pairSize);
    levelOfRecursion++;

    // Recurse with double pairSize (only if more than 1 pair to sort among winners)
    if (pairCount >= 2)
        splitIntoPairsRecursive(container, pairSize * 2, comparison_count);
   
    return std::pow(2, levelOfRecursion);
}

/**
 * @brief Binary search and insert a group into the main chain by its key (last element).
 * Searches in mainGroups[0..hi) and inserts at the found position.
 */
template <typename Container>
size_t PmergeMe::binaryInsertGroup(std::vector<Container> &mainGroups,
	const Container &group, size_t hi, size_t &comparison_count)
{
    typename Container::value_type key = group.back();
    size_t lo = 0;

    while (lo < hi)
    {
        size_t mid = lo + (hi - lo) / 2;
        comparison_count++;
        if (mainGroups[mid].back() < key)
            lo = mid + 1;
        else
            hi = mid;
    }
    mainGroups.insert(mainGroups.begin()
        + static_cast<typename std::vector<Container>::difference_type>(lo),
        group);
    return lo;
}

/**
 * @brief Split a flat container into groups of halfSize, then separate them:
 * - mainGroups: winners (second half of each pair, sorted by recursion)
 * - pendGroups: losers (first half of each pair, each <= its paired winner)
 * - stragglerGroup: last group if odd number of groups (unpaired)
 * - remainder: leftover elements that don't fill a complete group
 */
template <typename Container>
void PmergeMe::extractGroups(const Container &container, size_t halfSize,
	std::vector<Container> &mainGroups, std::vector<Container> &pendGroups,
	Container &stragglerGroup, bool &hasStragglerGroup, Container &remainder)
{
    size_t totalGroupsOfHalf = container.size() / halfSize;
    bool hasOddGroup = (totalGroupsOfHalf % 2 != 0);
    size_t numPairsNow = totalGroupsOfHalf / 2;

    std::vector<Container> groups;
    typename Container::const_iterator it = container.begin();
    for (size_t g = 0; g < totalGroupsOfHalf; ++g)
    {
        Container grp;
        for (size_t j = 0; j < halfSize; ++j, ++it)
            grp.push_back(*it);
        groups.push_back(grp);
    }
    while (it != container.end())
    {
        remainder.push_back(*it);
        ++it;
    }

    for (size_t p = 0; p < numPairsNow; ++p)
    {
        pendGroups.push_back(groups[2 * p]);
        mainGroups.push_back(groups[2 * p + 1]);
    }

    hasStragglerGroup = false;
    if (hasOddGroup)
    {
        stragglerGroup = groups[totalGroupsOfHalf - 1];
        hasStragglerGroup = true;
    }
}

/**
 * @brief Insert pend groups (b2, b3, ...) into the main chain using Jacobsthal order.
 * b1 must already be inserted at front before calling this.
 * Each b_k is binary-searched up to the position of its paired winner a_k,
 * which keeps the search space optimal (power of 2).
 * posOfWinner tracks how winner positions shift as insertions happen.
 */
template <typename Container>
void PmergeMe::insertPendGroups(std::vector<Container> &mainGroups,
	std::vector<Container> &pendGroups, size_t numPairsNow, size_t &comparison_count)
{
    size_t pendCount = pendGroups.size();
    if (pendCount <= 1)
        return;

    std::vector<size_t> posOfWinner(numPairsNow);
    for (size_t k = 0; k < numPairsNow; ++k)
        posOfWinner[k] = k + 1;

    std::vector<size_t> order;
    {
        typeVect jacobOrder = buildJacobsthalOrder(pendCount);
        for (size_t i = 0; i < jacobOrder.size(); ++i)
        {
            size_t val = jacobOrder[i];
            if (val >= 2 && val <= pendCount)
                order.push_back(val);
        }
    }

    for (size_t i = 0; i < order.size(); ++i)
    {
        size_t k = order[i] - 1;
        size_t lo = binaryInsertGroup(mainGroups, pendGroups[k], posOfWinner[k], comparison_count);

        for (size_t w = 0; w < numPairsNow; ++w)
        {
            if (posOfWinner[w] >= lo)
                posOfWinner[w]++;
        }
    }
}

/**
 * @brief Merge-insertion unwinding at one recursion level.
 * Separates the container into main chain (winners) and pend (losers),
 * inserts b1 at front for free, then inserts remaining pend and straggler
 * using binary search. Rebuilds the flat container at the end.
 *
 * @param[in] maxGroupSize Size of a full pair at this level (= 2 * halfSize)
 */
template <typename Container>
size_t PmergeMe::mergeInsertUnwindLevel(Container &container, size_t maxGroupSize, size_t &comparison_count)
{
    size_t halfSize = maxGroupSize / 2;
    size_t numPairsNow = (container.size() / halfSize) / 2;

    std::vector<Container> mainGroups;
    std::vector<Container> pendGroups;
    Container stragglerGroup;
    bool hasStragglerGroup;
    Container remainder;
    extractGroups(container, halfSize, mainGroups, pendGroups,
        stragglerGroup, hasStragglerGroup, remainder);

    if (!pendGroups.empty())
        mainGroups.insert(mainGroups.begin(), pendGroups[0]);

    insertPendGroups(mainGroups, pendGroups, numPairsNow, comparison_count);

    if (hasStragglerGroup)
        binaryInsertGroup(mainGroups, stragglerGroup, mainGroups.size(), comparison_count);

    container.clear();
    for (size_t g = 0; g < mainGroups.size(); ++g)
    {
        for (typename Container::iterator grpIt = mainGroups[g].begin(); grpIt != mainGroups[g].end(); ++grpIt)
            container.push_back(*grpIt);
    }
    for (typename Container::iterator remIt = remainder.begin(); remIt != remainder.end(); ++remIt)
        container.push_back(*remIt);

    return halfSize;
}

template <typename Container>
void PmergeMe::sort_FJMI(Container &cont)
{
	size_t comparison_count = 0;
	levelOfRecursion = 0;

	const size_t start_size = cont.size();
	if (start_size <= 1)
		return;

	// Phase 1: recursive pairing
	size_t maxGroupSize = splitIntoPairsRecursive<Container>(cont, 2, comparison_count);

	// Phase 2: merge-insertion unwinding, from largest group size down to 2
	for (size_t groupSize = maxGroupSize; groupSize >= 2; groupSize /= 2)
		mergeInsertUnwindLevel<Container>(cont, groupSize, comparison_count);
	if (start_size != cont.size())
        throw ContainerChangedSizeException();
	if (!verifyOrder(cont))
        throw ContainerNotSortedException();
	this->comparison_count_vect.push_back(comparison_count);
	levelOfRecursion = 0;
}

#endif // PMERGEME_TPP
