#ifndef RPN_HPP
#define RPN_HPP

#include <queue>
#include <string>

class RPN : public std::queue<std::string>
{
private:

public:
	RPN();
	~RPN();
	RPN(const RPN &f);
	RPN &operator=(const RPN &other);
	void store(std::string input, std::string separator);
	double calculate(void);
};

#endif
