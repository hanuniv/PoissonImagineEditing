#include "DrawBoundary.h"
#include "IteratorOverflow.h"
#include "qpainter.h"
#include <iostream>
#include <cmath>
#include "MyPoint.h"

using std::pow;


Boundary::Boundary()
{
	poly_rep = NULL;//指向一个空对象
}

Boundary::~Boundary()
{
}

void Boundary::finish(){}

void Boundary::add(TPoint){}

OutPoint Boundary::firstpoint() { return OutPoint(0,0);}

Boundary_iterator Boundary::begin() const
{
	return Boundary_iterator(0,this);
}

Boundary_iterator Boundary::end() const
{
	return Boundary_iterator(poly_rep->size(),this);
}

Boundary_iterator::Boundary_iterator( int index,const Boundary* boundary)
	:_index(index),_boudary(boundary)
{
}

bool Boundary_iterator::operator==( const Boundary_iterator& rhs) const
{
	return _boudary == rhs._boudary && _index == rhs._index;
}

bool Boundary_iterator::operator!=( const Boundary_iterator& rhs) const
{
	return !(*this == rhs); //利用已有的功能
}

const TPoint Boundary_iterator::operator*() const
{
	check_integrity();
	return _boudary->poly_rep->at(_index);
}


const TPoint* Boundary_iterator::operator->() const
{
	return &_boudary->poly_rep->at(_index);
}


Boundary_iterator& Boundary_iterator::operator++()
{
	++_index;
	check_integrity();
	return *this;
}

Boundary_iterator Boundary_iterator::operator++( int )
{
	Boundary_iterator tmp = *this;
	++_index;
	check_integrity();
	return tmp;
}

Boundary_iterator& Boundary_iterator::operator--()
{
	--_index;
	check_integrity();
	return *this;
}

Boundary_iterator Boundary_iterator::operator--( int )
{
	Boundary_iterator tmp = *this;
	--_index;
	check_integrity();
	return tmp;
}


Boundary_iterator Boundary_iterator::operator+( int i)
{
	Boundary_iterator tmp = *this;
	tmp._index += i;  //不检查是否越界
	return tmp;
}

Boundary_iterator Boundary_iterator::operator-( int i)
{
	return *this + (-i);
}

void Boundary_iterator::check_integrity() const
{//确保_index不越界，且所指数据有意义
	if(_index > _boudary->poly_rep->size()) //最后一个是end的位置，保留
		throw iterator_overflow(_index, _boudary->poly_rep->size());
	if(_index < 0 ) //最后一个是end的位置，保留
		throw iterator_underflow(_index,0);
	/*Or:
	iterator_overflow ex(_index,_max_bound);
	throw ex;*/

//必要时扩充容量
// 	if(_index >= Boundary::_data.size())
// 		Boundary::gen_elements(_index+1);
}






Line::Line(TPoint start_point, TPoint end_point)
{
	start_point_ = start_point;
	end_point_ = end_point;
	//设置多边形表示
	poly_rep = new vector<TPoint>(2);
	poly_rep->at(0)= start_point_;
	poly_rep->at(1) = end_point_;
}

void Line::Draw(QPainter &paint)
{
	paint.drawLine(start_point_.x,start_point_.y,
					end_point_.x,end_point_.y);
}

Line::~Line()
{
	delete poly_rep;
}


ellipse::ellipse(TPoint start_point, TPoint end_point)
{
	start_point_ = start_point;
	end_point_ = end_point;

	//设置多边形表示

	if (abs(start_point_.x-end_point_.x) > abs(start_point_.y-end_point_.y))
	{//x方向跨度大，以它生成.
		poly_rep = new vector<TPoint>(2*abs(start_point_.x-end_point_.x)+1);
		
		vector<TPoint>::iterator it1 = poly_rep->begin(),it2 = poly_rep->end()-1;

		TPoint center_point_((start_point_ + end_point_)/2);
		double a = abs(start_point_.x - center_point_.x),
				b = abs(start_point_.y - center_point_.y);

		if (start_point_.x < end_point_.x){
			for (int i = start_point_.x; i < end_point_.x; ++i, ++it1,--it2)
			{
				it1->x = i;
				it1->y = center_point_.y + b * sqrt(1 - pow(double(i - center_point_.x)/a,2));
				it2->x = i;
				it2->y = center_point_.y - b * sqrt(1 - pow(double(i - center_point_.x)/a,2));
			}
			it1->x = end_point_.x;
			it1->y = center_point_.y;
		}
		else{
			for (int i = start_point_.x; i > end_point_.x; --i, ++it1,--it2)
			{
				it1->x = i;
				it1->y = center_point_.y + b * sqrt(1 - pow(double(i - center_point_.x)/a,2));
				it2->x = i;
				it2->y = center_point_.y - b * sqrt(1 - pow(double(i - center_point_.x)/a,2));
			}
			it1->x = end_point_.x;
			it1->y = center_point_.y;
		}
	}
	else//y方向跨度大，以它生成.
	{		
		poly_rep = new vector<TPoint>(2*abs(start_point_.y-end_point_.y)+1);
		vector<TPoint>::iterator it1 = poly_rep->begin(),it2 = poly_rep->end()-1;

		TPoint center_point_((start_point_ + end_point_)/2);

		double  a = abs(start_point_.x - center_point_.x),
				b = abs(start_point_.y - center_point_.y);
		if (start_point_.y < end_point_.y){
			for (int i = start_point_.y; i < end_point_.y; ++i, ++it1,--it2)
			{
				it1->x = i;
				it1->y = center_point_.x + a * sqrt(1 - pow(double(i - center_point_.y)/b,2));
				it2->y = i;
				it2->x = center_point_.x - a * sqrt(1 - pow(double(i - center_point_.y)/b,2));
			}
			it1->y = end_point_.y;
			it1->x = center_point_.x;
		}
		else{
			for (int i = start_point_.y; i > end_point_.y; --i, ++it1,--it2)
			{
				it1->y = i;
				it1->x = center_point_.x + a * sqrt(1 - pow(double(i - center_point_.y)/b,2));
				it2->y = i;
				it2->x = center_point_.x - a * sqrt(1 - pow(double(i - center_point_.y)/b,2));
			}
			it1->y = end_point_.y;
			it1->x = center_point_.x;
		}
	}
}

void ellipse::Draw( QPainter &paint )
{
	paint.drawEllipse(start_point_.x,start_point_.y,
		-start_point_.x+end_point_.x,
		-start_point_.y+end_point_.y);
}

ellipse::~ellipse()
{
	delete poly_rep;
}

rectangle::rectangle( TPoint start_point, TPoint end_point)
{
	start_point_ = start_point;
	end_point_ = end_point;

	//设置多边形表示
	poly_rep = new vector<TPoint>(5);
	poly_rep->at(0)= start_point_;

	poly_rep->at(1).x = start_point_.x;
	poly_rep->at(1).y =end_point_.y;

	poly_rep->at(2) = end_point_;

	poly_rep->at(3).x = end_point_.x;
	poly_rep->at(3).y = start_point_.y;

	poly_rep->at(4) = start_point_;
}

void rectangle::Draw( QPainter &paint )
{
	paint.drawRect(start_point_.x,start_point_.y,
		-start_point_.x+end_point_.x,
		-start_point_.y+end_point_.y);
}

rectangle::~rectangle()
{
	delete poly_rep;
}

polygon::polygon(TPoint start_point, TPoint end_point)
{
	vertex.push_back(start_point);
	vertex.push_back(end_point);
	//设置多边形表示;
	poly_rep = &vertex;
}

polygon::polygon(TPoint start_point)
{
	vertex.push_back(start_point);
	//设置多边形表示
	poly_rep = &vertex;
}

void polygon::Draw( QPainter &paint )
{
	std::vector<TPoint>::iterator it1,it2;
	for(it1=it2= vertex.begin(),it2++; it2 != vertex.end();it1=it2,++it2)
		paint.drawLine(it1->x,it1->y,it2->x,it2->y);
}

void polygon::finish()
{
	add(vertex[0]);
}

polygon::~polygon()
{
	vertex.clear();
	
	//这里不需要也不能 delete poly_rep，因为它指向的就是vertex
}

void polygon::add(TPoint p)
{
	if (p != vertex.back()) //重复节点不加
	{
		vertex.push_back(p);
	}
	
}

OutPoint polygon::firstpoint()
{
	if(!vertex.empty())
		return T2Out(vertex[0]);
	return OutPoint();
}

Freehand::Freehand(TPoint p):polygon(p)
{

}

Freehand::~Freehand(){}

void printRepresentation(Boundary_iterator it, Boundary_iterator itend)
{
	for(;it	!= itend; ++it){
		std::cout<<'('<<it->x<<','<<it->y<<") - ";
	}
	std::cout<<'('<<it->x<<','<<it->y<<')'<<std::endl;
}




