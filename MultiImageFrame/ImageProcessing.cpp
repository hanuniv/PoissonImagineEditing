#include "ImageProcessing.h"
#include <algorithm>

using std::for_each;

void Invert_Process::process( Tpic* image_ )
{
	for (int i=0; i<image_->width(); i++)
	{
		for (int j=0; j<image_->height(); j++)
		{
			QRgb color = image_->pixel(i, j);
			image_->setPixel(i, j, qRgb(255-qRed(color), 255-qGreen(color), 255-qBlue(color)) );
		}
	}
	set_changed(); // ��ʾ�任�ɹ�������ѹ�����ջ��
	// equivalent member function of class QImage
	// image_->invertPixels(QImage::InvertRgb);
}

Mirror_Process::Mirror_Process()
{
	horizontal = false;
	vertical = true;
}

void Mirror_Process::process( Tpic* image_ )
{
	QImage image_tmp(*image_);
	int width = image_->width();
	int height = image_->height();

	if (horizontal)
	{
		if (vertical)
		{
			for (int i=0; i<width; i++)
			{
				for (int j=0; j<height; j++)
				{
					image_->setPixel(i, j, image_tmp.pixel(width-1-i, height-1-j));
				}
			}
		} 
		else
		{
			for (int i=0; i<width; i++)
			{
				for (int j=0; j<height; j++)
				{
					image_->setPixel(i, j, image_tmp.pixel(i, height-1-j));
				}
			}
		}

	}
	else
	{
		if (vertical)
		{
			for (int i=0; i<width; i++)
			{
				for (int j=0; j<height; j++)
				{
					image_->setPixel(i, j, image_tmp.pixel(width-1-i, j));
				}
			}
		}
	}
	set_changed(); 
	// equivalent member function of class QImage
	//*(image_) = image_->mirrored(true, true);
}

void TurnGrey_Process::process( Tpic* image_ )
{
	for (int i=0; i<image_->width(); i++)
	{
		for (int j=0; j<image_->height(); j++)
		{
			QRgb color = image_->pixel(i, j);
			int gray_value = (qRed(color)+qGreen(color)+qBlue(color))/3;
			image_->setPixel(i, j, qRgb(gray_value, gray_value, gray_value) );
		}
	}
	set_changed();
}

Paste_Process::Paste_Process( const ScanMask* p1,const Tpic* p2)
{
	p_source_mask_ = p1;
	p_source_image_ = p2;
}

void Paste_Process::setdata( int i,int j)
{
	xpos = i;
	ypos = j;
}


void Paste_Process::process( Tpic* image_ )
{
	//�������ڲ���_label������Ҳ������_loca�����������ڲ���iterator����
// 	ScanMask::Tlabel::const_iterator it = p_source_mask_->_label.cbegin(),
// 		itend = p_source_mask_->_label.cend();
// 	for (;it != itend;++it)
// 	{
// 		int targetx = xpos-p_source_mask_->get_center().x+it->first.x,//���������ͼ�ο��Ƶ�������غ�
// 			targety = ypos-p_source_mask_->get_center().y+it->first.y;
// 		if(0<=targetx && targetx<image_->width()&& 0<=targety && targety<image_->height())
// 			image_->setPixel(targetx,targety,p_source_image_->pixel(it->first.x,it->first.y));
// 	}
	//���ڲ���iterator����,����ֻ�����ڲ�����ǰ�渴���ڲ��ͱ߽�
	for(ScanMask::const_inside_iterator it = p_source_mask_->inside_begin(),itend = p_source_mask_->inside_end();
		it!=itend;
		++it)
	{
		int targetx = xpos-p_source_mask_->get_center().x+it->x,//���������ͼ�ο��Ƶ�������غ�
			targety = ypos-p_source_mask_->get_center().y+it->y;
		if(0<=targetx && targetx<image_->width()&& 0<=targety && targety<image_->height())
			image_->setPixel(targetx,targety,p_source_image_->pixel(it->x,it->y));
	}

	set_changed(); 
}

Poisson_Process::Poisson_Process( const ScanMask* p1,const Tpic* p2)
{
	p_source_mask_ = p1;
	p_source_image_ = p2;
	successful_init = mask_solver.init(p_source_mask_);
	//��ʼ��ʱ�����ڵ��ݶ�
	int m = p_source_mask_->num_inside();
	gradr.resize(m);
	gradg.resize(m);
	gradb.resize(m);
	ScanMask::const_inside_iterator it = p_source_mask_->inside_begin(),it_end = p_source_mask_->inside_end();
	for(; it!= it_end;++it)
	{
		int i = it->x,j = it->y;
		gradr(it.index()) =  4 * qRed(p_source_image_->pixel(i,j)) 
			- qRed(p_source_image_->pixel(i-1,j))
			- qRed(p_source_image_->pixel(i+1,j))
			- qRed(p_source_image_->pixel(i,j-1))
			- qRed(p_source_image_->pixel(i,j+1));
		gradg(it.index()) =  4 * qGreen(p_source_image_->pixel(i,j)) 
			- qGreen(p_source_image_->pixel(i-1,j))
			- qGreen(p_source_image_->pixel(i+1,j))
			- qGreen(p_source_image_->pixel(i,j-1))
			- qGreen(p_source_image_->pixel(i,j+1));
		gradb(it.index()) =  4 * qBlue(p_source_image_->pixel(i,j)) 
			- qBlue(p_source_image_->pixel(i-1,j))
			- qBlue(p_source_image_->pixel(i+1,j))
			- qBlue(p_source_image_->pixel(i,j-1))
			- qBlue(p_source_image_->pixel(i,j+1));
	}
	
}
Poisson_Process::operator bool()
{
	return successful_init;
}
void Poisson_Process::setdata( int i,int j)
{
	xpos = i;
	ypos = j;
}

void Poisson_Process::process( Tpic* image_ )
{
	once.reset();//���ڱ������
	int m = p_source_mask_->num_inside();
	int n = p_source_mask_->num_boundary();

	//����b�������߽�
	Eigen::VectorXd br(m+n),bg(m+n),bb(m+n);
	ScanMask::const_boundary_iterator it = p_source_mask_->boundary_begin(),it_end = p_source_mask_->boundary_end();
	for (;it != it_end;++it)
	{
		int targetx = xpos-p_source_mask_->get_center().x+it->x,//���������ͼ�ο��Ƶ�������غ�
			targety = ypos-p_source_mask_->get_center().y+it->y;
		QRgb c;
		if(0<=targetx && targetx<image_->width()&& 0<=targety && targety<image_->height())
		{  //�߽���Ŀ��ͼ���ڣ���Ŀ��ͼ��ı߽�
			c = image_->pixel(targetx,targety);
		}
		else
		{ //�߽糬��Ŀ��ͼ����Դͼ��߽�
			once.report("Target Boundary incomplete, fill with source image.\n");
			c = p_source_image_->pixel(it->x,it->y);
		}
		br(it.index()) = qRed(c);
		bg(it.index()) = qGreen(c);
		bb(it.index()) = qBlue(c);
	}
	//���
	Eigen::VectorXd		ansr = mask_solver.Solve(mask_solver.get_b(gradr,br)),
		ansg = mask_solver.Solve(mask_solver.get_b(gradg,bg)),
		ansb = mask_solver.Solve(mask_solver.get_b(gradb,bb));
	//��ͼ
	for (int i = 0; i < m; ++i){
		double t = max(max(ansr(i),ansg(i)),ansb(i));
		//��Ҫ�ض�<0�ķ���
		if(ansr(i)<0) ansr(i) =0;
		if(ansg(i)<0) ansg(i) =0;
		if(ansb(i)<0) ansb(i) =0;
// 		if(ansr(i)>255) ansr(i) =255;
// 		if(ansg(i)>255) ansg(i) =255;
// 		if(ansb(i)>255) ansb(i) =255;
		//��Ҫ�����������ֵķ����Ա�����ɫ.Ҳ���Խض�
		if (t>255)
		{
			ansr(i) *= 255/t;
			ansg(i) *= 255/t;
			ansb(i) *= 255/t;
		}
	}
	ScanMask::const_inside_iterator itr = p_source_mask_->inside_begin(),itrend = p_source_mask_->inside_end();
	for (;itr != itrend;++itr)
	{
		int targetx = xpos-p_source_mask_->get_center().x+itr->x,//���������ͼ�ο��Ƶ�������غ�
			targety = ypos-p_source_mask_->get_center().y+itr->y;
		if(0<=targetx && targetx<image_->width()&& 0<=targety && targety<image_->height())
			image_->setPixel(targetx,targety,qRgb(ansr(itr.index()),ansg(itr.index()),ansb(itr.index())));
	}
	set_changed(); 
}



Gray_Mask_Process::Gray_Mask_Process( const ScanMask* p1)
{
	p_source_mask_ = p1;
}

void Gray_Mask_Process::process( Tpic* image_ )
{
	for(ScanMask::const_outside_iterator it = p_source_mask_->outside_begin(),it_end = p_source_mask_->outside_end();
		it != it_end;
		++it)
	{
				QRgb color = image_->pixel(it->x, it->y);
				int gray_value = (qRed(color)+qGreen(color)+qBlue(color))/3;
				image_->setPixel(it->x, it->y, qRgb(gray_value, gray_value, gray_value) ); 			
	}
	for(ScanMask::const_boundary_iterator it = p_source_mask_->boundary_begin(),it_end = p_source_mask_->boundary_end();
		it != it_end;
		++it)
	{
			image_->setPixel(it->x, it->y, qRgb(255,255,255) );		
	}
//	for_each(p_source_mask_->boundary_begin(),p_source_mask_->boundary_end(),[&](TPoint p){image_->setPixel(p.x,p.y,qRgb(255,255,255));});

// 	for(int i = 0; i<image_->width(); ++i)
// 		for(int j = 0; j < image_->height(); ++j)
// 			if (p_source_mask_->_a[i][j] == UnMarked)//�ⲿ�û�ɫ
// 			{
// 				QRgb color = image_->pixel(i, j);
// 				int gray_value = (qRed(color)+qGreen(color)+qBlue(color))/3;
// 				image_->setPixel(i, j, qRgb(gray_value, gray_value, gray_value) );
// 			}else if (p_source_mask_->_a[i][j] == Outline) //�߽��ð�ɫ
// 			{
// 				image_->setPixel(i, j, qRgb(255,255,255) );
// 			}
	//���Iteratorû�д���
//	ScanMask::const_inside_iterator it = p_source_mask_->inside_begin(),it_end = p_source_mask_->inside_end();
// 	ScanMask::const_boundary_iterator it = p_source_mask_->boundary_begin(),it_end = p_source_mask_->boundary_end();
// 	for (;it!=it_end;++it)
// 	{
// 		image_->setPixel(it->x,it->y,qRgb(255,255,255));
// 	}
// 	set_changed(); 
}
