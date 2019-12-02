#ifndef MY_POINT_H_
#define MY_POINT_H_

#include <utility>
#include <QPoint>

using std::pair;

typedef pair<int,int> InPoint; //���õ�洢��ʽ
typedef QPoint	OutPoint; //�����洢��ʽ

class TPoint //�ṩ�����ĵ�洢�������Լ�������Ľӿ�
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