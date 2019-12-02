#ifndef MASK_ITERATOR_H_
#define MASK_ITERATOR_H_
#include "MyPoint.h"
#include "ScanMaskSolver.h"

class ScanMask;

class ScanMask_iterator
{
public:
	ScanMask_iterator(int,const ScanMask*);
	bool operator==(const ScanMask_iterator&)const;
	bool operator!=(const ScanMask_iterator&)const;
	ScanMask_iterator& operator++(); //Prefix
	ScanMask_iterator operator++(int);//Suffix
	ScanMask_iterator& operator--(); //Prefix
	ScanMask_iterator operator--(int);//Suffix
	ScanMask_iterator operator+(int); 
	ScanMask_iterator operator-(int); 
	const TPoint operator*()const;       
	const TPoint* operator->()const;
	virtual int index();  //保留一个对下标的接口
protected:
	int _index;
	virtual void check_integrity() const;
	const ScanMask* _p;
};

class ScanMask_Outline_iterator:public ScanMask_iterator
{
public:
	int index();
	ScanMask_Outline_iterator(int,const ScanMask*);
protected:
	void check_integrity() const;
};

class ScanMask_Inside_iterator:public ScanMask_iterator
{
public:
	int index();
	ScanMask_Inside_iterator(int,const ScanMask*);
protected:
	void check_integrity() const;
};

class ScanMask_Outside_iterator
{
public:
	ScanMask_Outside_iterator(int,int,const ScanMask*);//起始位置坐标
	bool operator==(const ScanMask_Outside_iterator&)const;
	bool operator!=(const ScanMask_Outside_iterator&)const;
	ScanMask_Outside_iterator& operator++(); //Prefix
	ScanMask_Outside_iterator operator++(int);//Suffix
	const TPoint operator*()const;       
	const TPoint* operator->()const;
	int index();  //保留一个对下标的接口,index是在iterator内部标号的，开始迭代的点标0
protected:
	int _index,_i,_j;
	//virtual void check_integrity() const;
	void next();
	const ScanMask* _p;
};

#endif