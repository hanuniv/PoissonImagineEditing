#ifndef MY_POINT_H_
#define MY_POINT_H_

#include <utility>
#include <QPoint>

using std::pair;

typedef pair<int,int> InPoint; //内置点存储方式
typedef QPoint	OutPoint; //外面点存储方式

class TPoint //提供基本的点存储和运算以及与外面的接口
{	
public:
	TPoint(int=0,int=0);
	TPoint(const TPoint &);
	TPoint(const InPoint&);
	TPoint(const OutPoint&);
	const TPoint& operator=(const InPoint&);
	const TPoint& operator=(const OutPoint&);
	friend bool operator==(const TPoint&,const TPoint&);
	friend bool operator!=(const TPoint&,const TPoint&);
	friend bool operator<(const TPoint&,const TPoint&);
	friend TPoint operator+(const TPoint&,const TPoint&);
	friend TPoint operator-(const TPoint&,const TPoint&);
	TPoint operator*(double);
	TPoint operator/(double);
	int x,y;
};

OutPoint T2Out(TPoint);
InPoint T2In(TPoint);

#endif 