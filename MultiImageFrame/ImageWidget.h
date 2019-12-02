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
	const QImage* image();								//����Ӧ����imagebackup,�������Լ����Լ������
	//const ScanMask * scanmask();
	
	//void set_source_window(ChildWindow* childwindow);
	void set_copyboard(CopyBoard*);
	void set_draw_status_to_choose();
	void set_draw_status_to_paste();
	void set_draw_status_to_poisson();
	void set_draw_status_to_none();

	//BoundaryDraw
	void set_boundary_pen_color();
	BoundaryProcessor boundary_;						//���Ʊ߽��һϵ�в���
	
	//Mask Accessor
	//const ScanMask* get_scanmask();					//����scanmask�����ݣ���ͼʱ�ǲ�����scanmask�Ĺ��ܵ�)
	void producemask(ChildWindow* sourcewind);		//��sourcewind�����ݻ����Լ���mask image_

protected:
	void paintEvent(QPaintEvent *paintevent);
	void mousePressEvent(QMouseEvent *mouseevent);
	void mouseMoveEvent(QMouseEvent *mouseevent);
	void mouseReleaseEvent(QMouseEvent *mouseevent);

// 	void PasteProcess(QMouseEvent *mouseevent);					//ֱ��ճ���Ĳ���
// 	void PoissonProcess(QMouseEvent *mouseevent);				//Poisson�����ճ���Ĳ���

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
	QImage						*image_;						// image ����ʵʱ��ʾ
	QImage						*image_backup_;					// ��const QImage* image();������Ӧ
	
	//Processing Queue
	unique_ptr<Abstract_Process>			pt;								//��ǰ��������
	vector<unique_ptr<Abstract_Process> >	pimg;							//�浵����
	void init_pimg();
	
	// Pointer
	//ChildWindow				*source_window_;				// Source child window
	//const ScanMask			*p_source_mask;					//����ָ������Դ���ڵ�mask
	CopyBoard					*copyboard_;					//ָ�������
	//Boundary
	//ScanMask					scanmask_;						//���ڴ汾���ڵ�mask	
	
	// Signs
	DrawStatus					draw_status_;					// Enum type of draw status
};

