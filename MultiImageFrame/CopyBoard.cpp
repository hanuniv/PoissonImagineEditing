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

// 		/**/if (boundary._has_been_masked) //已经计算过，boundary没有更新，不要重新计算
// 			return &scanmask_;
//		boundary_._has_been_masked = true;

		cout<<"Calculating Mask...";
		scanmask = new ScanMask;
		scanmask->SetSize(img->width(),img->height());//初始化
		scanmask->Calculate_from_boudary(it_begin,it_end);//扫描线算法
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
