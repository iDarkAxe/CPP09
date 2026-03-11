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
private:
	typedef unsigned int typeElement;
	typedef std::vector<typeElement> typeVect;
	typedef std::list<typeElement> typeList;
	typedef std::set<typeElement> typeSet;
	typeVect vect;
	typeList lst;

public:
	std::vector<size_t> comparison_count_vect;
	size_t levelOfRecursion;
	size_t numberOfElements;
	bool show_short_args;
	static size_t max_short_args;

//= Methods =//
public:
	// Orthodox Canonical Form : Constructors / Destructors / Operators
	PmergeMe();
	PmergeMe(size_t startingSize);
	~PmergeMe();
	PmergeMe(const PmergeMe &f);
	PmergeMe &operator=(const PmergeMe &other);

	void clear(void);
	void printAllVect(void) const;
	void printShortVect(void) const;
	void printAllList(void) const;

	void storeInVect(const char *array[]);
	void storeInList(const char *array[]);
	// void storeValues(const char *array[]);
	void storeInListFromVect(void);
	// void storeInVectFromList(void);
	void sort_FJMI_lst(void);
	void sort_FJMI_vect(void);
	// bool areSameSize(void) const;
	size_t size_vect(void) const;
	size_t size_lst(void) const;

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
	static void printContainer(const T &container, bool useIndex = 0, const char *separator = " ");
	template <typename T>
	static void printShort(const T &container, const char *separator = " ");
	template <typename Container>
	bool verifyOrder(const Container &cont) const;
	template <typename Container>
	void printPairs(const Container &container, size_t pairSize) const;
	template <typename Container>
	void sort_FJMI(Container &cont);
private:
	template <typename T>
	void storeInLoop(T &container, const char *array[]);
	typeVect generateJacobsthalSequence(size_t n);
	typeVect buildJacobsthalOrder(size_t size);
	template <typename Container>
	size_t splitIntoPairsRecursive(Container &container, size_t pairSize, size_t &comparison_count);
	template <typename Container>
	size_t mergeInsertUnwindLevel(Container &container, size_t maxGroupSize, size_t &comparison_count);
	template <typename Container>
	void extractGroups(const Container &container, size_t halfSize,
		std::vector<Container> &mainGroups, std::vector<Container> &pendGroups,
		Container &stragglerGroup, bool &hasStragglerGroup, Container &remainder);
	template <typename Container>
	void insertPendGroups(std::vector<Container> &mainGroups,
		std::vector<Container> &pendGroups, size_t numPairsNow, size_t &comparison_count);
	template <typename Container>
	size_t binaryInsertGroup(std::vector<Container> &mainGroups,
		const Container &group, size_t hi, size_t &comparison_count);
};

#include "PmergeMe.tpp"

#endif
