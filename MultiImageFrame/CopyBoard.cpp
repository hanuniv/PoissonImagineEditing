#include "CopyBoard.h"
#include <iostream>

using std::cout;
using std::endl;

void CopyBoard::calculate(vector<Boundary*>::const_iterator it_begin,vector<Boundary*>::const_iterator it_end)
{
		
		if (img == NULL)
			return ;
		if (img->width()<=0 || img->height()<=0)
			return ;

// 		/**/if (boundary._has_been_masked) //�Ѿ��������boundaryû�и��£���Ҫ���¼���
// 			return &scanmask_;
//		boundary_._has_been_masked = true;

		cout<<"Calculating Mask...";
		scanmask = new ScanMask;
		scanmask->SetSize(img->width(),img->height());//��ʼ��
		scanmask->Calculate_from_boudary(it_begin,it_end);//ɨ�����㷨
		cout<<"Successful. Ready for Paste."<<endl;
}

CopyBoard::CopyBoard()
{
	img = NULL;
	scanmask = NULL;
}

CopyBoard::~CopyBoard()
{
	delete img;
	delete scanmask;
}

void CopyBoard::set_img( const QImage* im )
{
	delete img;
	img = new QImage(*im);
	delete scanmask;
	scanmask = NULL;
}
