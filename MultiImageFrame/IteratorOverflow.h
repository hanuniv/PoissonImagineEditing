#ifndef ITERATOR_OVERFLOW_H_
#define ITERATOR_OVERFLOW_H_

#include <iostream>

class iterator_overflow
{
public:
	iterator_overflow(int index,int max);
	int index();
	int max();
	void what_happened(std::ostream &os = std::cerr);
private:
	int _index;
	int _max;
};

class iterator_underflow
{
public:
	iterator_underflow(int index,int min);
	int index();
	int min();
	void what_happened(std::ostream &os = std::cerr);
private:
	int _index;
	int _min;
};

#endif

