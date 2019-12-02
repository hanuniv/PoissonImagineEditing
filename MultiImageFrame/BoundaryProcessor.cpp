#include <QImage>
#include <QPainter>
#include <QtWidgets>
#include <utility>
#include <iostream>
#include "BoundaryProcessor.h"

using std::make_pair;
using std::cout;
using std::endl;

BoundaryProcessor::BoundaryProcessor()
{
	canvas = make_pair(0,0);
	current_boundary_ = NULL;
	clear();

}

BoundaryProcessor::~BoundaryProcessor()
{
	clear();
}

void BoundaryProcessor::set_rectangle()
{
	delete current_boundary_;//没有保存的current_boundary 会删除;
	current_boundary_ = NULL;
	boundary_type_ = brectangle;
}

void BoundaryProcessor::set_ellipse()
{
	delete current_boundary_;//没有保存的current_boundary 会删除;
	current_boundary_ = NULL;
	boundary_type_ = bellipse;
}

void BoundaryProcessor::set_polygon()
{
	delete current_boundary_;//没有保存的current_boundary 会删除;
	current_boundary_ = NULL;
	boundary_type_ = bpolygon;
}

void BoundaryProcessor::set_freehand()
{
	boundary_type_ = bfreehand;
	delete current_boundary_;//没有保存的current_boundary 会删除;
	current_boundary_ = NULL;
}

void BoundaryProcessor::cancel()
{
	if (!array_.empty())
	{
		delete array_.back();
		array_.pop_back();
		_has_been_masked = false;
	}
}

void BoundaryProcessor::set_pencolor(QColor c)
{
	pencolor  = c;
}


void BoundaryProcessor::flush_buffer()
{
	start_point_ = end_point_ = QPoint(0, 0);
	boundary_type_ = brectangle;

	delete current_boundary_;
	current_boundary_ = NULL;
	pencolor = Qt::red;
}


void BoundaryProcessor::clear()  
{
	flush_buffer();

	vector<Boundary*>::iterator it = array_.begin();
	for(;it!=array_.end();++it)
		delete (*it);
	array_.clear();

	_has_been_masked = false;
}

void BoundaryProcessor::paintEvent(QPainter& painter)
{
	painter.setBrush(Qt::NoBrush);
	vector<Boundary*>::iterator it = array_.begin();
	for(;it!= array_.end();++it)
	{
		painter.setPen((*it)->figcolor);
		(*it)->Draw(painter);
	}
	// In-time Draw Boundary

		painter.setPen(pencolor);
		switch (boundary_type_)
		{
		case bdefault:
		case bline:   break;
		case brectangle:
			rectangle(start_point_,end_point_).Draw(painter);
			break;
		case bellipse:
			ellipse(start_point_,end_point_).Draw(painter);
			/**/painter.setPen(Qt::DotLine);
			rectangle(start_point_,end_point_).Draw(painter);
			painter.setPen(Qt::SolidLine);  //这三行设置椭圆外的方框
			break;
		case bpolygon:
			painter.drawLine(start_point_,end_point_);
			break;
		case binpolygon:
			current_boundary_->Draw(painter);
			painter.drawLine(start_point_,end_point_);
			/**/painter.setPen(Qt::DotLine);
			painter.drawLine(current_boundary_->firstpoint(),end_point_);
			painter.setPen(Qt::SolidLine);  //这三行设置多边形显示封闭结果
			break;
		case bfreehand:
			if (current_boundary_!=NULL) current_boundary_->Draw(painter);
			break;
		}
}

void BoundaryProcessor::mouseLeftPressEvent( QMouseEvent *mouseevent )
{
	if (in_canvas(mouseevent->pos().rx(),mouseevent->pos().ry()))
		if (boundary_type_ == binpolygon)
		{
				current_boundary_->add(mouseevent->pos());//绘制多边形中，点击时加入一个点
		}
		else if (boundary_type_ == bfreehand)
		{
			current_boundary_ = new Freehand(mouseevent->pos());//手绘中，点击时新建对象
			current_boundary_->figcolor = pencolor;
		}
	start_point_ = end_point_ = mouseevent->pos();
}

void BoundaryProcessor::mouseMoveEvent( QMouseEvent *mouseevent )
{
	end_point_ = mouseevent->pos();
	if (in_canvas(mouseevent->pos().rx(),mouseevent->pos().ry()))
		if (boundary_type_ == bfreehand)
/**********/if(current_boundary_ == NULL)
				cout<<"Empty boundary encountered  in mouseMoveEvent."<<endl;
			else
				current_boundary_->add(end_point_);//手绘中，移动时加入一个点
	//cout<<end_point_.rx()<<' '<<end_point_.ry()<<endl;
	//如果绘制的线超出窗口仍然有坐标传入，如果在大窗口之外会有负值传入
}

bool BoundaryProcessor::mouseRightReleaseEvent( QMouseEvent *mouseevent)
{
	if(boundary_type_ == binpolygon)
	{
		current_boundary_->finish();
		array_.push_back(current_boundary_);
		current_boundary_ = NULL;
		_has_been_masked = false;
		start_point_=end_point_ = mouseevent->pos();
		boundary_type_=bpolygon;
		return false;
	}
	return true;
}

bool BoundaryProcessor::mouseLeftReleaseEvent( QMouseEvent *mouseevent)
{
	//跳出画布外，特殊处理
	if (!in_canvas(mouseevent->pos().rx(),mouseevent->pos().ry())){
		start_point_ = end_point_ = mouseevent->pos();
		if(boundary_type_ == binpolygon)
			return true;
		else
			return false;
	}
	bool setmousetrack = false;
	switch(boundary_type_)
	{
	case bdefault:
	case bline: break;
	case brectangle:
		current_boundary_ = new rectangle(start_point_,end_point_);
		current_boundary_->figcolor = pencolor;
		array_.push_back(current_boundary_);
		current_boundary_ = NULL;
		_has_been_masked = false;
		break;
	case bellipse:
		current_boundary_ = new ellipse(start_point_,end_point_);
		current_boundary_->figcolor = pencolor;
		array_.push_back(current_boundary_);
		current_boundary_ = NULL;
		_has_been_masked = false;
		break;
	case binpolygon:
		setmousetrack = true;
		break;
	case bpolygon:
		current_boundary_ = new polygon(start_point_);
		current_boundary_->figcolor = pencolor;
		boundary_type_ = binpolygon;
		setmousetrack = true;
		break;
	case bfreehand:
		current_boundary_->finish();
		array_.push_back(current_boundary_);
		current_boundary_ = NULL;
		_has_been_masked = false;
		break;
	}
	start_point_ = end_point_ = mouseevent->pos();
	return setmousetrack;
}

bool BoundaryProcessor::in_canvas( int i ,int j)
{
	return  0<=i && i<canvas.first && 0<=j && j<canvas.second; 
}
