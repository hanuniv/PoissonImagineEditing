#include "IteratorOverflow.h"

iterator_overflow::iterator_overflow( int index,int max )
	:_index(index),_max(max){}


int iterator_overflow::index()
{
	return _index;
}

int iterator_overflow::max()
{
	return _max;
}

void iterator_overflow::what_happened( std::ostream &os /*= std::cerr*/ )
{
	os << "Internal error: current index "<< _index
		<< "exceeds maximum bound:" <<_max;
}

iterator_underflow::iterator_underflow( int index,int min )
	:_index(index),_min(min){}


int iterator_underflow::index()
{
	return _index;
}

int iterator_underflow::min()
{
	return _min;
}

void iterator_underflow::what_happened( std::ostream &os /*= std::cerr*/ )
{
	os << "Internal error: current index "<< _index
		<< "exceeds minimum bound:" <<_min;
}
