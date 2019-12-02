#ifndef COPYBOARD_H_
#define COPYBOARD_H_
#include <QImage>
#include "ScanMaskSolver.h"

//���������ڴ洢�ڼ������е�ͼƬ��

class CopyBoard
{
public:
	CopyBoard();
	~CopyBoard();
	void set_img(const QImage* im);
	void calculate(vector<Boundary*>::const_iterator it_begin,vector<Boundary*>::const_iterator it_end); //��Boundary�����ݼ���scanmask
	ScanMask *scanmask; //��ΪNULL������û�м���
	QImage *img;
};


#endif