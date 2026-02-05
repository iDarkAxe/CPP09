#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <list>
#include <set>
#include <string>
#include <stdint.h>
#include <iostream>

enum debug_level
{
	NOTHING = 0,
	INFO = 1,
	DEBUG = 2
};

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL NOTHING
#endif

#ifndef THROW_ERROR_IF_DUPLICATE
#define THROW_ERROR_IF_DUPLICATE 0
#endif

class PmergeMe
{
//= Variables =//
private:
	typedef std::vector<unsigned int> typeVect;
	typedef std::list<unsigned int> typeList;
	typeVect vect;
	typeList lst;

public:
	typedef std::set<unsigned int> typeSet;
	std::vector<size_t> comparison_count_vect;
	size_t numberOfElements;
	bool show_short_args;
	static size_t max_short_args;

//= Methods =//
private:
	void sort_FJMI_vect_recursive(typeVect &temp_vec, size_t &comparison_count);
	void sort_FJMI_lst_recursive(typeList &temp_lst, size_t &comparison_count);

public:
	// Orthodox Canonical Form : Constructors / Destructors / Operators
	PmergeMe();
	PmergeMe(size_t numberOfElement);
	~PmergeMe();
	PmergeMe(const PmergeMe &f);
	PmergeMe &operator=(const PmergeMe &other);

	// const typeVect &getVector(void) const;
	// const typeList &getList(void) const;
	void clear(void);
	void printAllVect(void) const;
	void printShortVect(void) const;
	void printAllList(void) const;
	template <typename T>
	void storeInLoop(T &container, const char *array[]);
	void storeInVect(const char *array[]);
	void storeInList(const char *array[]);
	void storeValues(const char *array[]);
	void storeInListFromVect(void);
	void storeInVectFromList(void);
	void sort_FJMI_lst(void);
	void sort_FJMI_vect(void);
	bool areSameSize(void) const;
	template <typename Container>
	bool verifyOrder(const Container &cont) const;

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

public:
//= Templates =//
	template <typename T>
	static void printContainer(const T &container, bool useIndex = 0, const char *separator = " ");
	template <typename T>
	static void printShort(const T &container, const char *separator = " ");

private:
	// template <typename Container>
	// Container generateJacobsthalSequence(size_t n);
	template <typename Container>
	void binaryInsertContainer(Container &temp_container, unsigned int value, size_t limit, size_t &comparison_count);
};

#include "PmergeMe.tpp"

#endif
