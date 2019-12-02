#include "MyPoint.h"
#include <utility>
#include <QPoint>

using std::make_pair;

TPoint::TPoint( int a,int b):x(a),y(b){}

TPoint::TPoint( const TPoint& p)
{
	x = p.x;
	y = p.y;
}

TPoint::TPoint( const InPoint& p)
{
	x = p.first;
	y = p.second;
}

TPoint::TPoint( const OutPoint& p)
{
	x = p.x();
	y = p.y();
}

const TPoint& TPoint::operator=( const InPoint& p)
{
	x = p.first;
	y = p.second;
	return *this;
}

const TPoint& TPoint::operator=( const OutPoint& p)
{
	x = p.x();
	y = p.y();
	return *this;
}

TPoint TPoint::operator*( double l)
{
	return TPoint(int(x*l),int(y*l));
}

TPoint TPoint::operator/( double l)
{
	return TPoint(int(x/l),int(y/l));
}



OutPoint T2Out( TPoint p)
{
	return OutPoint(p.x,p.y);
}

InPoint T2In( TPoint p)
{
	return make_pair(p.x, p.y);
}


TPoint operator+( const TPoint& p1,const TPoint& p2)
{
	return TPoint(p1.x + p2.x, p1.y + p2.y);
}


TPoint operator-( const TPoint&p1,const TPoint& p2)
{
	return TPoint(p1.x - p2.x, p1.y - p2.y);
}

bool operator==( const TPoint&p1,const TPoint& p2)
{
	return p1.x == p2.x && p1.y == p2.y;
}


bool operator!=( const TPoint& p1,const TPoint& p2)
{
	return !(p1 == p2);
}

bool operator<( const TPoint&p1,const TPoint& p2)
{	
	return  p1.x < p2.x || p1.x == p2.x && p1.y < p2.y;
}

