#include "ImageWidget.h"
#include <QtWidgets> 
#include <iostream>
#include <vector>
#include <algorithm>
#include "Eigen/Dense"
#include "ChildWindow.h"
#include "ImageProcessing.h"
#include "ScanMaskSolver.h" //只有在producemask时用到

using std::cout;
using std::endl;
using std::vector;
using std::max;
using std::min;

ImageWidget::ImageWidget(ChildWindow *relatewindow)
{
	image_ = new QImage();
	image_backup_ = new QImage();

	boundary_.canvas = std::make_pair(0,0);

	draw_status_ = kNone;

	//source_window_ = NULL;
	//p_source_mask = NULL;
	copyboard_ = NULL;
	//pt = NULL;
}

ImageWidget::~ImageWidget(void)
{
	delete image_;image_ = NULL;
	init_pimg();
}

int ImageWidget::ImageWidth()
{
	return image_->width();
}

int ImageWidget::ImageHeight()
{
	return image_->height();
}

void ImageWidget::set_draw_status_to_choose()
{
	boundary_.flush_buffer(); //绘图前清空上一步操作未结束可能有的缓存
	setMouseTracking(false); //不然上一步Polygon的残留可能会出错
	draw_status_ = kChoose;	
}

void ImageWidget::set_draw_status_to_paste()
{
	// 	cout<<"Calculating Mask...";
	// 	//p_source_mask = source_window_->imagewidget_->get_scanmask();//获得源窗口的mask,这里实际上对原窗口的mask数据作了计算
	// 	cout<<"Successful. Ready for Paste."<<endl;
	
	if (copyboard_->scanmask == NULL)//没有计算scanmask
	{
		cout<<"No Picture is copied.Quit Poisson"<<endl;
		//delete pt; pt = NULL;
		draw_status_ = kNone;
		return;
	}

	draw_status_ = kPoisson;

	//delete pt;
	//pt = new Paste_Process(p_source_mask,source_window_->imagewidget_->image());
	pt.reset(new Paste_Process(copyboard_->scanmask,copyboard_->img));
	pt->set_backup(image_backup_);
	//pimg.push_back(std::move(pt)); 
}
void ImageWidget::set_draw_status_to_poisson()
{
// 	cout<<"Calculating Mask...";
// 	//p_source_mask = source_window_->imagewidget_->get_scanmask();//获得源窗口的mask,这里实际上对原窗口的mask数据作了计算
// 	cout<<"Successful. "<<endl;

	if (copyboard_->scanmask == NULL)//没有计算scanmask
	{
		cout<<"No Picture is copied.Quit Poisson"<<endl;
		//delete pt; pt = NULL;
		draw_status_ = kNone;
		return;
	}

	draw_status_ = kPoisson;
	cout<<"Initializing Solver...";
	//delete pt;
	//pt = new Poisson_Process(p_source_mask,source_window_->imagewidget_->image());
	pt.reset(new Poisson_Process(copyboard_->scanmask,copyboard_->img));
	if(*pt){ //分解成功
		cout<<" Ready."<<endl;
		pt->set_backup(image_backup_);
		//pimg.push_back(std::move(pt));
	}
	else{
		cout<<"Quit Poisson"<<endl;
		//delete pt; pt = NULL;
		pt.reset();
		draw_status_ = kNone;
	}
}
void ImageWidget::set_draw_status_to_none()
{
	draw_status_ = kNone;
}


const QImage* ImageWidget::image()
{
	return image_backup_;
}

// const ScanMask * ImageWidget::scanmask()
// {
// 	return &scanmask_;
// }


// void ImageWidget::set_source_window(ChildWindow* childwindow)
// {
// 	source_window_ = childwindow;
// }


void ImageWidget::set_copyboard( CopyBoard* copyboard)
{
	copyboard_ = copyboard;
}


void ImageWidget::paintEvent(QPaintEvent *paintevent)
{
	QPainter painter;
	painter.begin(this);

	// Draw background
	painter.setBrush(Qt::lightGray);
	QRect back_rect(0, 0, width(), height());
	painter.drawRect(back_rect);

	// Draw image
	QRect rect = QRect( 0, 0, image_->width(), image_->height());
	painter.drawImage(rect, *image_); 

	// Draw Boundary
	if (draw_status_ == kChoose)
		boundary_.paintEvent(painter);
		
	painter.end();
	update();
}

void ImageWidget::mousePressEvent(QMouseEvent *mouseevent)
{
	if (Qt::LeftButton == mouseevent->button())
	{
		switch (draw_status_)
		{
		case kChoose:
			boundary_.mouseLeftPressEvent(mouseevent);
			break;
		case kPaste:
		case kPoisson:
			*(image_) = *(image_backup_);
			//设置参数
			pt->setdata(mouseevent->pos().rx(),
						mouseevent->pos().ry());
			pt->process(image_);
			break;
		}
		update();
	}
}

void ImageWidget::mouseMoveEvent(QMouseEvent *mouseevent)
{
	switch (draw_status_)
	{
	case kChoose:
		boundary_.mouseMoveEvent(mouseevent);
		break;

	case kPaste://有完全类似的处理方式
	case kPoisson:
		*(image_) = *(image_backup_);
		pt->setdata(mouseevent->pos().rx(),
					mouseevent->pos().ry());
		pt->process(image_);
		break;
	}

	update();
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *mouseevent)
{
	bool mousetrack;
	if (Qt::RightButton == mouseevent->button())
	{
		mousetrack = boundary_.mouseRightReleaseEvent(mouseevent);
		setMouseTracking(mousetrack);
	}
	else
	{
		switch (draw_status_)
		{
		case kChoose:
			mousetrack = boundary_.mouseLeftReleaseEvent(mouseevent);
			setMouseTracking(mousetrack);
			break;
		case kPaste:
		case kPoisson:
			//pt = NULL; //运算之前已经存档，结束运算。
			pimg.push_back(std::move(pt));
			*image_backup_ = *image_;
			draw_status_ = kNone;
		}
	}
	
	update();
}

void ImageWidget::Open(QString filename)
{
	// Load file
	if (!filename.isEmpty())
	{
		image_->load(filename);
		*(image_backup_) = *(image_);
		boundary_.canvas = std::make_pair(image_->width(),image_->height());
	}

//	setFixedSize(image_->width(), image_->height());
//	relate_window_->setWindowFlags(Qt::Dialog);
//	relate_window_->setFixedSize(QSize(image_->width(), image_->height()));
//	relate_window_->setWindowFlags(Qt::SubWindow);
	
	//image_->invertPixels(QImage::InvertRgb);
	//*(image_) = image_->mirrored(true, true);
	//*(image_) = image_->rgbSwapped();
	cout<<"image size: "<<image_->width()<<' '<<image_->height()<<endl;
	update();
}

void ImageWidget::Save()
{
	SaveAs();
}

void ImageWidget::SaveAs()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), ".", tr("Images(*.bmp *.png *.jpg)"));
	if (filename.isNull())
	{
		return;
	}	

	image_->save(filename);
}

void ImageWidget::Invert()
{
	//delete pt;					//结束之前的操作
	pt.reset(new Invert_Process());
	pt->set_backup(image_backup_);
	pt->process(image_);
	pimg.push_back(std::move(pt)); //原则是process后存档，pt自然无效，不需要delete，不需要置NULL，因为始终只有一份指针
// 	if(pt->haschanged())
// 		pimg.push_back(pt);
// 	else
// 		delete pt;
	//pt = NULL; //不要让后面影响已经存入的数据
	*image_backup_ = *image_;
	update();
}

void ImageWidget::Mirror()
{	
	//delete pt;
	pt.reset(new Mirror_Process());
	pt->set_backup(image_backup_);
	pt->process(image_);
	pimg.push_back(std::move(pt));
	//pt = NULL;
	*image_backup_ = *image_;
	update();
}

void ImageWidget::TurnGray()
{
	//delete pt;
	pt.reset(new TurnGrey_Process());
	pt->set_backup(image_backup_);
	pt->process(image_);
	pimg.push_back(std::move(pt));
	//pt = NULL;
	*image_backup_ = *image_;
	update(); 
}

void ImageWidget::Restore()
{
	if (pimg.empty())
	{
		return;
	}
	if(pimg.size()>1){ //删除最后一次存储的图像
		//delete pt; //可能之前操作一半未释放
		pt = std::move(pimg.back());
		pimg.pop_back();
		pt->retrieve_backup(image_backup_);
		//delete pt; pt = NULL;
		cout<<"Restored to previous picture."<<endl<<endl;
	}
	else{//保留初始的图像
		pimg.back()->retrieve_backup(image_backup_);
		cout<<"Has restored to original picture."<<endl<<endl;
	}

	*image_ = *image_backup_;
	update();
}

// const ScanMask* ImageWidget::get_scanmask()
// {
// 	if (image_ == NULL)
// 		return NULL;
// 	if (image_->width()<=0 || image_->height()<=0)
// 		return NULL;
// 
// /**/if (boundary_._has_been_masked) //已经计算过，boundary没有更新，不要重新计算
// 		return &scanmask_;
// 	
// 	boundary_._has_been_masked = true;
// 	scanmask_.SetSize(image_->width(),image_->height());//初始化
// 	scanmask_.Set_from_boudary(boundary_.array_.begin(),boundary_.array_.end());//扫描线算法
// 
// 	return &scanmask_;
// }

void ImageWidget::producemask(ChildWindow* sourcewind )
{
	//初始化原图
	delete image_;
	image_ = new QImage(*sourcewind->imagewidget_->image());

	//存档一份数据
	pt.reset(new Abstract_Process());
	pt->set_backup(image_);
	init_pimg();
	pimg.push_back(std::move(pt));

	//设置绘制边界的画板大小
	boundary_.canvas = std::make_pair(image_->width(),image_->height());

	//绘制Mask 后的图像
	//pt = new Gray_Mask_Process(sourcewind->imagewidget_->get_scanmask());
	ScanMask scanmask;//临时建一个scanmask
	scanmask.SetSize(sourcewind->imagewidget_->ImageWidth(),sourcewind->imagewidget_->ImageHeight());
	scanmask.Calculate_from_boudary(sourcewind->imagewidget_->boundary_.array_.cbegin(),
							sourcewind->imagewidget_->boundary_.array_.cend());
	pt.reset(new Gray_Mask_Process(&scanmask));
	pt->process(image_);
	*image_backup_ = *image_;
	pt.reset();
	//delete pt;pt = NULL;
}

void ImageWidget::set_boundary_pen_color()
{
	boundary_.set_pencolor(QColorDialog::getColor(Qt::red,this,tr("PenColor")));
}

void ImageWidget::init_pimg()
{
// 	if (!pimg.empty())
// 	{
// 		std::vector<unique_ptr<Abstract_Process>>::iterator it = pimg.begin();
// 		for(;it!= pimg.end();it++)
// 		{
// 			//delete *it;
// 			
// 		}
// 		pimg.clear();
// 	}
	pimg.clear();

}

