#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <list>
#include <set>
#include <string>
#include <iostream>

enum debug_level
{
	NOTHING = 0,
	INFO = 1,
	DEBUG = 2
};

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL INFO
#endif

#ifndef THROW_ERROR_IF_DUPLICATE
#define THROW_ERROR_IF_DUPLICATE 0
#endif

namespace Color {
	extern const bool use_color;
    extern const char *Color_Off;
    extern const char *Red;
}

class PmergeMe
{
//= Variables =//
public:
	typedef unsigned int typeElement;
	static bool show_short_args;
	static size_t max_short_args;

private:
	typedef std::vector<typeElement> typeVect;

//= Methods =//
public:
	//= Exceptions =//
	class ArgumentEmptyException : public std::exception
	{
	public:
		const char *what() const throw();
	};
	class ArgumentInvalidException : public std::exception
	{
	public:
		const char *what() const throw();
	};
	class DuplicateException : public std::exception
	{
	public:
		const char *what() const throw();
	};
	class ContainerChangedSizeException : public std::exception
	{
	public:
		const char *what() const throw();
	};
	class ContainerNotSortedException : public std::exception
	{
	public:
		const char *what() const throw();
	};

public:
//= Templates =//
	template <typename T>
	static void printContainer(const T &container, bool useIndex = false, const char *separator = " ");
	template <typename T>
	static void printShort(const T &container, const char *separator = " ");
	template <typename Container>
	static bool verifyOrder(const Container &cont);
	template <typename Container>
	static void printPairs(const Container &container, size_t pairSize, size_t levelOfRecursion);
	template <typename Container>
	static size_t sort_FJMI(Container &cont);
	template <typename T>
	static void storeInLoop(T &container, const char *array[]);

private:
	static typeVect generateJacobsthalSequence(size_t n);
	static typeVect buildJacobsthalOrder(size_t size);
	template <typename Container>
	static size_t splitIntoPairsRecursive(Container &container, size_t pairSize, size_t levelOfRecursion, size_t &comparison_count);
	template <typename Container>
	static size_t mergeInsertUnwindLevel(Container &container, size_t maxGroupSize, size_t &comparison_count);
	template <typename Container>
	static void extractGroups(const Container &container, size_t halfSize,
		std::vector<Container> &mainGroups, std::vector<Container> &pendGroups,
		Container &stragglerGroup, bool &hasStragglerGroup, Container &remainder);
	template <typename Container>
	static void insertPendGroups(std::vector<Container> &mainGroups,
		std::vector<Container> &pendGroups, size_t numPairsNow, size_t &comparison_count);
	template <typename Container>
	static size_t binaryInsertGroup(std::vector<Container> &mainGroups,
		const Container &group, size_t hi, size_t &comparison_count);
};

#include "PmergeMe.tpp"

#endif
