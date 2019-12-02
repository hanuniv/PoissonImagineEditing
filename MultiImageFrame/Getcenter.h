#ifndef GET_CENTER_H_
#define GET_CENTER_H_

#include "MyPoint.h"

class Getcenter{
	//一个用于对输入数据点计算一个比较合适的中心的小类
	//重载了(),可以像函数一样使用
public:
	Getcenter();
	void reset();
	void addpoint(TPoint);
	TPoint operator()() const; //注意这里用const,程序里用的是const引用
private:
	double centerx,centery; //记录当前点列数目和重心
	int npoint;

};

#endif