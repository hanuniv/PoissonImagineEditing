#ifndef BOUNDARY_PROCESSOR_H_
#define BOUNDARY_PROCESSOR_H_
#include <vector>
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <utility>
#include "DrawBoundary.h"

using std::vector;
using std::pair;

class BoundaryProcessor //������װImageWidget�й��ڻ��Ʊ߽�Ĳ���
{	
public:
	BoundaryProcessor();
	~BoundaryProcessor();

	void set_rectangle();
	void set_ellipse();
	void set_polygon();
	void set_freehand();
	void set_pencolor(QColor);
	void flush_buffer();
	void cancel();
	void clear();

	void paintEvent(QPainter& painter);
	void mouseLeftPressEvent(QMouseEvent *mouseevent);
	void mouseMoveEvent(QMouseEvent *mouseevent);
	bool mouseRightReleaseEvent(QMouseEvent *mouseevent); //���ض�mousetrack�Ŀ���
	bool mouseLeftReleaseEvent(QMouseEvent *mouseevent);//���ض�mousetrack�Ŀ���

	vector<Boundary*>		array_;			 //�Ѿ�����ı߽�

	bool in_canvas(int,int); 
	pair<int,int>			canvas;        //������С,���ڿ��Ƶ��ܵ��Ӵ������������

	bool					_has_been_masked;   //�����ж������Ƿ��Ѿ��������mask,��Ϊget_mask����Ҫ�úܶ�Σ��������Լ������㡣ÿ��array_�䶯��Ҫ��Ϊfalse

private:
	//���˻��������
	BoundaryType				boundary_type_;
	Boundary*					current_boundary_;
	QPoint						start_point_;					// Left top point of rectangle region
	QPoint						end_point_;						// Right bottom point of rectangle region
	QColor						pencolor;

};
#endif

