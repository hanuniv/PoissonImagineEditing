#include "MaskIterator.h"
#include "IteratorOverflow.h"

ScanMask_iterator::ScanMask_iterator( int index,const ScanMask* p)
	:_index(index),_p(p)
{
}

bool ScanMask_iterator::operator==( const ScanMask_iterator& rhs) const
{
	return _p == rhs._p && _index == rhs._index;
}

bool ScanMask_iterator::operator!=( const ScanMask_iterator& rhs) const
{
	return !(*this == rhs); 
}

const TPoint ScanMask_iterator::operator*() const
{
	check_integrity();
	return _p->_loca.at(_index);
}


const TPoint* ScanMask_iterator::operator->() const
{
	return &_p->_loca.at(_index);
}


ScanMask_iterator& ScanMask_iterator::operator++()
{
	++_index;
	check_integrity();
	return *this;
}

ScanMask_iterator ScanMask_iterator::operator++( int )
{
	ScanMask_iterator tmp = *this;
	++_index;
	check_integrity();
	return tmp;
}

ScanMask_iterator& ScanMask_iterator::operator--()
{
	--_index;
	check_integrity();
	return *this;
}

ScanMask_iterator ScanMask_iterator::operator--( int )
{
	ScanMask_iterator tmp = *this;
	--_index;
	check_integrity();
	return tmp;
}


ScanMask_iterator ScanMask_iterator::operator+( int i)
{
	ScanMask_iterator tmp = *this;
	tmp._index += i;  //不检查是否越界
	return tmp;
}

ScanMask_iterator ScanMask_iterator::operator-( int i)
{
	return *this + (-i);
}

void ScanMask_iterator::check_integrity() const
{//确保_index不越界，且所指数据有意义
	if(_index > _p->_loca.size()) //最后一个是end的位置，保留
		throw iterator_overflow(_index, _p->_loca.size());
	if(_index < 0 )
		throw iterator_underflow(_index,0);
}

int ScanMask_iterator::index()
{
	return _index;
}

void ScanMask_Outline_iterator::check_integrity() const
{
	if(_index > _p->_loca.size()) //最后一个是end的位置，保留
		throw iterator_overflow(_index, _p->_loca.size());
	if(_index < _p->_loca_end_Inside )
		throw iterator_underflow(_index,_p->_loca_end_Inside);
}

ScanMask_Outline_iterator::ScanMask_Outline_iterator( int index,const ScanMask* p)
:ScanMask_iterator(index,p)
{

}

int ScanMask_Outline_iterator::index()
{
	return _index ;//这里可以设置返回的标号是从0开始：return _index - _p->_loca_end_Inside,但会和get_label表达不一致,就不修改了。
}

void ScanMask_Inside_iterator::check_integrity() const
{
	if(_index > _p->_loca_end_Inside) //最后一个是end的位置，保留
		throw iterator_overflow(_index, _p->_loca_end_Inside);
	if(_index < 0)
		throw iterator_underflow(_index,0);
}

ScanMask_Inside_iterator::ScanMask_Inside_iterator( int index,const ScanMask* p)
	:ScanMask_iterator(index,p)
{

}

int ScanMask_Inside_iterator::index()
{
	return _index;
}



ScanMask_Outside_iterator::ScanMask_Outside_iterator(int i ,int j,const ScanMask* p)
	:_p(p)
{
	_i = i;
	_j = j-1,
	_index = -1;
	next();
}

bool ScanMask_Outside_iterator::operator==( const ScanMask_Outside_iterator& rhs) const
{
	return _p == rhs._p && _i == rhs._i && _j == rhs._j;
}

bool ScanMask_Outside_iterator::operator!=( const ScanMask_Outside_iterator& rhs) const
{
	return !(*this == rhs); 
}

const TPoint ScanMask_Outside_iterator::operator*() const
{
	return TPoint(_i,_j);
}


const TPoint* ScanMask_Outside_iterator::operator->() const
{
	return &TPoint(_i,_j);
}


ScanMask_Outside_iterator& ScanMask_Outside_iterator::operator++()
{
	next();
	return *this;
}

ScanMask_Outside_iterator ScanMask_Outside_iterator::operator++( int )
{
	ScanMask_Outside_iterator tmp = *this;
	next();
	return tmp;
}

int ScanMask_Outside_iterator::index()
{
	return _index;
}

void ScanMask_Outside_iterator::next()
{
	if(_i == _p->_m) //终点
	{
		_j = 0;
		return;
	}
	for(int j = _j+1; j<_p->_n; ++j)
		if(_p->_a[_i][j] == UnMarked)
		{
			_j = j;
			++_index;
			return;
		}
	for (int i = _i+1; i< _p->_m; ++i)
		for (int j = 0;j<_p->_n;++j)
			if (_p->_a[i][j] == UnMarked)
			{
				_i = i;
				_j = j;
				++_index;
				return;
			}
	//否则已经遍历完所有节点，返回一个约定的结尾值
	_i = _p->_m;
	_j = 0;
}

