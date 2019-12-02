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

class BoundaryProcessor //这个类封装ImageWidget中关于绘制边界的操作
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
	bool mouseRightReleaseEvent(QMouseEvent *mouseevent); //返回对mousetrack的控制
	bool mouseLeftReleaseEvent(QMouseEvent *mouseevent);//返回对mousetrack的控制

	vector<Boundary*>		array_;			 //已经保存的边界

	bool in_canvas(int,int); 
	pair<int,int>			canvas;        //画布大小,用于控制点跑到子窗口外面的问题

	bool					_has_been_masked;   //用于判断数据是否已经被计算过mask,因为get_mask操作要用很多次，这样可以减少运算。每次array_变动都要设为false

private:
	//存了缓冲的数据
	BoundaryType				boundary_type_;
	Boundary*					current_boundary_;
	QPoint						start_point_;					// Left top point of rectangle region
	QPoint						end_point_;						// Right bottom point of rectangle region
	QColor						pencolor;

};
#endif

