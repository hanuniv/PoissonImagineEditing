#pragma once
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <vector>
#include "BoundaryProcessor.h"
//#include "ScanMaskSolver.h"
#include "AbstractProcess.h"
#include "CopyBoard.h"

class ChildWindow;
QT_BEGIN_NAMESPACE
class QImage;
class QPainter;
QT_END_NAMESPACE

using std::vector;
using std::unique_ptr;

enum DrawStatus
{
	kChoose, 
	kPaste, 
	kPoisson,
	kNone
};

class ImageWidget :
	public QWidget
{
	Q_OBJECT

public:
	ImageWidget(ChildWindow *relatewindow);
	~ImageWidget(void);

	//Accessor
	int ImageWidth();											// Width of image
	int ImageHeight();											// Height of image
	const QImage* image();								//这里应该用imagebackup,否则是自己对自己会出错
	//const ScanMask * scanmask();
	
	//void set_source_window(ChildWindow* childwindow);
	void set_copyboard(CopyBoard*);
	void set_draw_status_to_choose();
	void set_draw_status_to_paste();
	void set_draw_status_to_poisson();
	void set_draw_status_to_none();

	//BoundaryDraw
	void set_boundary_pen_color();
	BoundaryProcessor boundary_;						//绘制边界的一系列操作
	
	//Mask Accessor
	//const ScanMask* get_scanmask();					//生成scanmask的数据（绘图时是不运算scanmask的功能的)
	void producemask(ChildWindow* sourcewind);		//用sourcewind的数据绘制自己的mask image_

protected:
	void paintEvent(QPaintEvent *paintevent);
	void mousePressEvent(QMouseEvent *mouseevent);
	void mouseMoveEvent(QMouseEvent *mouseevent);
	void mouseReleaseEvent(QMouseEvent *mouseevent);

// 	void PasteProcess(QMouseEvent *mouseevent);					//直接粘贴的操作
// 	void PoissonProcess(QMouseEvent *mouseevent);				//Poisson处理后粘贴的操作

public slots:
	// File IO
	void Open(QString filename);								// Open an image file, support ".bmp, .png, .jpg" format
	void Save();												// Save image to current file
	void SaveAs();												// Save image to another file

	// Image processing Interface
	void Invert();												// Invert pixel value in image
	void Mirror();												// Mirror image vertically or horizontally
	void TurnGray();											// Turn image to gray-scale map
	void Restore();												// Restore image to origin
	
private:
	//Data
	QImage						*image_;						// image 用于实时显示
	QImage						*image_backup_;					// 与const QImage* image();函数对应
	
	//Processing Queue
	unique_ptr<Abstract_Process>			pt;								//当前操作对象
	vector<unique_ptr<Abstract_Process> >	pimg;							//存档数据
	void init_pimg();
	
	// Pointer
	//ChildWindow				*source_window_;				// Source child window
	//const ScanMask			*p_source_mask;					//用于指向数据源窗口的mask
	CopyBoard					*copyboard_;					//指向剪贴板
	//Boundary
	//ScanMask					scanmask_;						//用于存本窗口的mask	
	
	// Signs
	DrawStatus					draw_status_;					// Enum type of draw status
};

