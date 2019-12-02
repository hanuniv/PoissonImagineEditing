#ifndef COPYBOARD_H_
#define COPYBOARD_H_
#include <QImage>
#include "ScanMaskSolver.h"

//剪贴板用于存储在剪贴板中的图片。

class CopyBoard
{
public:
	CopyBoard();
	~CopyBoard();
	void set_img(const QImage* im);
	void calculate(vector<Boundary*>::const_iterator it_begin,vector<Boundary*>::const_iterator it_end); //由Boundary的数据计算scanmask
	ScanMask *scanmask; //若为NULL，表明没有计算
	QImage *img;
};


#endif