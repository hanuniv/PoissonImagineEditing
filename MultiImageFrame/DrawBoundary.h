#ifndef	DRAWBOUNDARY_H_
#define DRAWBOUNDARY_H_
#include <QWidget>
#include <vector>
#include <utility>
#include "MyPoint.h"

using std::vector;

enum BoundaryType{ bdefault,bline,brectangle,bellipse,bpolygon,bfreehand,binpolygon}; //inpolygon���ڱ�ʶ����polygon�Ĺ���

class Boundary_iterator;

class Boundary
{
public:
	typedef Boundary_iterator const_iterator;

	Boundary_iterator begin()const;
	Boundary_iterator end()const;
// 	friend  void Boundary_iterator::check_integrity()const;
// 	friend  int Boundary_iterator::operator*()const;
	friend class Boundary_iterator;

	Boundary();
	virtual ~Boundary();
	virtual void finish(); //����Polygon ����;
	virtual OutPoint firstpoint(); //����Polygon ���ص�һ����
	virtual void add(TPoint);//����Polygon�ӵ�;
	virtual void Draw(QPainter &paint)=0;
	QColor figcolor; //����ͼ����ɫ

protected:
	vector<TPoint>* poly_rep; //ָ��ͼ��Ķ���α�ʾ
};

class Boundary_iterator
{
public:
	Boundary_iterator(int index,const Boundary* boundary);
	bool operator==(const Boundary_iterator&)const;
	bool operator!=(const Boundary_iterator&)const;
	Boundary_iterator& operator++(); //Prefix
	Boundary_iterator operator++(int);//Suffix
	Boundary_iterator& operator--(); //Prefix
	Boundary_iterator operator--(int);//Suffix
	Boundary_iterator operator+(int); 
	Boundary_iterator operator-(int); 
	const TPoint operator*()const;                //��pair����ֵ
	const TPoint* operator->()const;
private:
	void check_integrity() const;
	int _index;
	const Boundary* _boudary;
};


class Line:public Boundary
{
public:
	Line(TPoint start_point, TPoint end_point);
	~Line();
	void Draw(QPainter &paint);
private:
	TPoint start_point_, end_point_;
};

class ellipse:public Boundary
{
public:
	ellipse(TPoint start_point, TPoint end_point);
	~ellipse();
	void Draw(QPainter &paint);
private:
	TPoint start_point_, end_point_;
};

class rectangle:public Boundary
{
public:
	rectangle(TPoint start_point, TPoint end_point);
	~rectangle();
	void Draw(QPainter &paint);
private:
	TPoint start_point_, end_point_;
};

class polygon:public Boundary
{
public:
	polygon(TPoint start_point);
	polygon(TPoint start_point, TPoint end_point);
	~polygon();
	void Draw(QPainter &paint);
	void finish();
	OutPoint firstpoint();
	void add(TPoint);
protected:
	std::vector<TPoint> vertex;
};

class Freehand:public polygon //Ϊpolygon�ļ̳�
{
public:
	Freehand(TPoint);
	~Freehand();
};

void printRepresentation(Boundary_iterator it, Boundary_iterator itend);

#endif