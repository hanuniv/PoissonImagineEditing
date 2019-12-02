#include "ScanMaskSolver.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cassert>
#include <algorithm>
#include <functional>

using std::map;
using std::cout;
using std::endl;
using std::for_each;
using std::mem_fun;
using std::bind1st;

int round(double t)
{
	return int(t+0.5);
}

ScanMask::ScanMask()
{
	_loca_end_Inside = 0;
}

ScanMask::~ScanMask()
{

}

bool ScanMask::is_inpic(const TPoint& p)const
{
	return (0<=p.x && p.x<_m && 0<=p.y && p.y<_n);
}

bool ScanMask::is_inpic( int i,int j)const
{
	return (0<=i && i<_m && 0<=j && j<_n);
}


void ScanMask::SetSize( int m,int n )
{
	_m = m;
	_n = n;
	_a.resize(_m);
	_label.clear();
	for (vector<vector<MaskType>>::iterator it = _a.begin();it != _a.end(); ++it)
		it->resize(_n,UnMarked);
	for(int i=0; i<_m; ++i) //需要在这里初始化数据，否则上次的结果会影响下面的操作。
		for(int j = 0; j<_n; j++)
			_a[i][j] = UnMarked; 
	get_center.reset(); //初始化get_center
}

//void ScanMask::Mark(Boundary::const_iterator it_begin,Boundary::const_iterator it_end)
void ScanMask::Mark(const Boundary* bd)
{/*
    Determine the internal of the matrix.
    by generate a regular markpoint for filling, which is unique for a line and a given y coordinate
    This function go through the drawing process, and in the way
    set or cancel markpoint,
    When a line is horizontal, markpoint is set at its endpoint,
    no mark is set if the horizontal line pass the angle without going inside
    */
    Boundary::const_iterator it_begin = bd->begin(),it_end = bd->end();
	
	int i,j,pred_y;
    double r;

	Boundary::const_iterator it = it_end - 1;
    for(;it!=it_begin;--it)
        if(is_inpic(*it) && it->y != it_begin->y){
            pred_y = it->y; //find first point before starTPoint from which the y coordinate is dif(ferent
            break;
        }
     if (it == it_begin)
		 return; // All point has the same y coordinate, do not mark anything
         
	 Boundary::const_iterator it1 = it_begin,it2 = it_begin + 1;
	 for(;it2 != it_end; ++it1,++it2)
		if(is_inpic(*it1))
	{
			//添加点计算get_center
			get_center.addpoint(*it1);

            //p[0]x coordinates,p[1]y coordinates'
            //mark should be different from existing other symbols in a[]'
            if(it1->x == it2->x && it1->y == it2->y) //'Same Point, ignored, the marking would be left to following regular line'
                continue;

            if( it1->y == it2->y) //'Horizontal line (same y coordinate) '
                continue;

            double l = double(it2->x-it1->x)/double(it2->y-it1->y);
            if( it1->y > it2->y){
                r = it1->x;
                for( j=it1->y-1;j>it2->y;--j){//start&end point not considered
                    r = (r - l);
                    Markline(round(r),j);
                }
            }
            else{
                r = it1->x;
                for( j=it1->y+1; j<it2->y; ++j){//start&end point not considered
                    r = (r + l);
                    Markline(round(r),j);
                }
            }
            if( (pred_y - it1->y) * (it1->y - it2->y) > 0){
                Markline(it1->x,it1->y);
                pred_y = it1->y;
            }
            else if( (pred_y - it1->y) * (it1->y - it2->y) < 0){
                UnMarkline(it1->x,it1->y);
                pred_y = it1->y;
            }
	}

}

void ScanMask::fill_inside()
{ //fill Inside
	for( int j=0; j < _n; ++j){
		int f = -1;
		for( int i= 0; i<_m; ++i)
			if( _a[i][j] != LineMark){
				if( f == 1)
					_a[i][j] = Inside;
			}
			else
				f = -f;
	}

}

//void ScanMask::drawlines(Boundary::const_iterator it_begin,Boundary::const_iterator it_end)
void ScanMask::drawlines(const Boundary * bd)
{
	Boundary::const_iterator it_begin = bd->begin(),it_end = bd->end();
	Boundary::const_iterator it1 = it_begin,it2 = it_begin + 1;
	for(;it2 != it_end;++it1,++it2)
		if(is_inpic(*it1))
	{
		int i,j;
		if (it1->x == it2->x && it1->y == it2->y)
			continue;

		if(it2->x == it1->x){
			if(it1->y< it2->y)
				for(i = it1->y;i<it2->y;++i)
					_a[it2->x][i] = Outline;
			else
				for(i=it1->y; i>it2->y; --i)
					_a[it2->x][i] = Outline;
			continue;
		}

		double slope = double(it2->y-it1->y)/double(it2->x-it1->x);

		if(abs(slope) >= 1){
			slope = 1 / slope;
			double r = it1->x;
			if(it1->y < it2->y)
				for(i = it1->y; i<it2->y; i++){
					_a[round(r)][i] = Outline;
					r = (r + slope);
				}
			else
				for(i=it1->y; i>it2->y; --i){
					_a[round(r)][i] = Outline;
					r = (r - slope);
				}
		}
		else{
			double r = it1->y;
			if(it1->x < it2->x)
				for(i= it1->x; i < it2->x; i++){
					_a[i][round(r)] = Outline;
					r = (r + slope) ;
				}
			else
				for(i = it1->x; i> it2->x; --i){
					_a[i][round(r)] = Outline;
					r = (r - slope) ;
				}
		}
	}
}

void ScanMask::Markline( int i,int j )
{
	if(_a[i][j] == LineMark)
		_a[i][j] = UnMarked;
	else
		_a[i][j] = LineMark;
}

void ScanMask::UnMarkline( int i,int j )
{
	if( _a[i][j] == LineMark )
		return;
	else
		_a[i][j] = UnMarked;
}


void ScanMask::generate_label()  //这里也同时生成_loca
{
	_label.clear();
	_loca.clear();
	int i,j,k = 0; 
	for(i=0; i<_m; ++i)
		for(j = 0; j<_n;++j)
			if (_a[i][j] == Inside)
			{
				//_label[make_pair(i,j)] = k;
				_label.insert(Tlabel::value_type(TPoint(i,j),k));
				_loca.push_back(TPoint(i,j));
				++k;
			}
	_loca_end_Inside = k;
	for(i=0; i<_m; ++i)
		for(j = 0; j<_n;++j)
			if (_a[i][j] == Outline)
			{
				//_label[make_pair(i,j)] = k;
				_label.insert(Tlabel::value_type(TPoint(i,j),k));
				_loca.push_back(TPoint(i,j));
				++k;
			}
}

int ScanMask::get_label( int i,int j ) const
{
	Tlabel::const_iterator it = _label.find(TPoint(i,j));
	if (it != _label.end())
		return it->second;
	else{
		cout<<"Outside Point label requiry encountered: ("<<i<<','<<j<<") \n";
		return -1; //如果不在边界或内部
	}
}

// int ScanMask::get_label( Tlabel::const_iterator it ) const
// {
// 	return it->second;
// }

bool ScanMask::is_onboundary( Tlabel::const_iterator it )const
{
	return _a[it->first.x][it->first.y] == Outline;
}

bool ScanMask::is_onboundary( int i ,int j) const
{
	return is_inpic(i,j) && _a[i][j] == Outline;
}

bool ScanMask::is_inside( int i,int j) const
{
	return is_inpic(i,j) && _a[i][j] == Inside;
}

void ScanMask::printpic(int i0,int j0)const //输出数据结果，可以特殊标记一个位置
{
	std::fstream fout;
	fout.open("ScanMaskPic.txt",std::ios::out);
	int i,j;
	for(i = 0;i< _m; ++i){
		for(j = 0; j<_n; j++){
			if(_a[i][j] == UnMarked)
				fout<< "  ";
			else if(_a[i][j] == LineMark)
				fout<< " /";
			else if(_a[i][j] == Outline)
				fout<< " -";
			else if(_a[i][j] == Inside)
				fout<< " *";
			if(i == i0 && j == j0) //特殊位置加一个着重号
				fout<<'#';
			else
				fout<<' ';
		}
		fout<<endl;
	}
	cout<<endl;
}

ScanMask_Inside_iterator ScanMask::inside_begin() const
{
	return ScanMask_Inside_iterator(0,this);
}

ScanMask_Inside_iterator ScanMask::inside_end() const
{
	return ScanMask_Inside_iterator(_loca_end_Inside,this);
}

ScanMask_Outline_iterator ScanMask::boundary_begin() const
{
	return ScanMask_Outline_iterator(_loca_end_Inside,this);
}

ScanMask_Outline_iterator ScanMask::boundary_end() const
{
	return ScanMask_Outline_iterator(_loca.size(),this);
}

ScanMask_Outside_iterator ScanMask::outside_begin() const
{
	return ScanMask_Outside_iterator(0,0,this);
}

ScanMask_Outside_iterator ScanMask::outside_end() const
{
	return ScanMask_Outside_iterator(_m,0,this);
}

int ScanMask::num_inside() const
{
	return _loca_end_Inside;
}

int ScanMask::num_boundary() const
{
	return _loca.size() - _loca_end_Inside;
}

void ScanMask::Calculate_from_boudary( vector<Boundary*>::const_iterator it_begin,vector<Boundary*>::const_iterator it_end)
{
	//标记
// 	vector<Boundary*>::const_iterator it = it_begin;
// 	for (;it!=it_end;it++)
// 		Mark((*it)->begin(),(*it)->end());
	//for_each(it_begin,it_end,&ScanMask::Mark); //没有传this指针
	//for_each传参数，可以用bind2nd,function object constructor,mem_fun,mem_fun_ref用引用
	//mem_fun转换成员函数为函数对象，用于vector<object*>中依次调用object*指向的object中的无参（只有this指针）函数
	//mem_fun_ref用在vector<object>中

	//OK
	//for_each(it_begin,it_end,bind1st(mem_fun(&ScanMask::Mark),this));
	for_each(it_begin,it_end,[&](Boundary*const t){this->Mark(t);});

	//填充内部
	fill_inside();			

	//填充边界
// 	it = it_begin;
// 	for (;it!=it_end;it++)
// 		drawlines((*it)->begin(),(*it)->end());

	//OK
	//for_each(it_begin,it_end,bind1st(mem_fun(&ScanMask::drawlines),this));
	for_each(it_begin,it_end,[&](Boundary*const t){this->drawlines(t);});

	//生成标号
	generate_label(); 
}


MaskPoissonSolver::MaskPoissonSolver()
{

}

bool MaskPoissonSolver::init( const ScanMask * scanmask)
{
	_scanmask = scanmask;
	m = _scanmask->num_inside();//内点的数目

	A.resize(m,m);
	B.resize(m,m+_scanmask->num_boundary());
	
	//set coefficients: 遍历内部
	ScanMask::const_inside_iterator 
		it = _scanmask->inside_begin(),it_end = _scanmask->inside_end();
		

	typedef Eigen::Triplet<double> T;
	std::vector<T> coefficients,bdlocation;
	for (;it != it_end; ++it)
	{
		int i = it->x,j = it->y, l = it.index();
// 		assert(l>=0&&l<m);//下标符合语法,这没有检测出错误
// 		try 
// 		{
// 			if(!(_scanmask->get_label(i-1,j)>=0&&_scanmask->get_label(i-1,j)<m)) throw 1; 
// 			if(!(_scanmask->get_label(i+1,j)>=0&&_scanmask->get_label(i+1,j)<m)) throw 2;
// 			if(!(_scanmask->get_label(i,j-1)>=0&&_scanmask->get_label(i,j-1)<m)) throw 3;
// 			if(!(_scanmask->get_label(i,j+1)>=0&&_scanmask->get_label(i,j+1)<m)) throw 4;
// 
// 		}
// 		catch(int e){ //若下标出错，输出结果 -- 已修正，没有再发现错误
// 			cout<<"Failure Caught in Solver init: Index out of range.\n";
// 			_scanmask->printpic(i,j);
// 			return false;
// 		}
		coefficients.push_back(T(l,l,4));
		if(_scanmask->is_inside(i-1,j))
			coefficients.push_back(T(l,_scanmask->get_label(i-1,j),-1));
		else
			bdlocation.push_back(T(l,_scanmask->get_label(i-1,j),-1));

		if(_scanmask->is_inside(i+1,j))
			coefficients.push_back(T(l,_scanmask->get_label(i+1,j),-1));
		else
			bdlocation.push_back(T(l,_scanmask->get_label(i+1,j),-1));

		if(_scanmask->is_inside(i,j-1))
			coefficients.push_back(T(l,_scanmask->get_label(i,j-1),-1));
		else
			bdlocation.push_back(T(l,_scanmask->get_label(i,j-1),-1));

		if(_scanmask->is_inside(i,j+1))
			coefficients.push_back(T(l,_scanmask->get_label(i,j+1),-1));
		else
			bdlocation.push_back(T(l,_scanmask->get_label(i,j+1),-1));
	}
	A.setFromTriplets(coefficients.begin(), coefficients.end());
	B.setFromTriplets(bdlocation.begin(),bdlocation.end());
	std::cout<<"Coefficients are set...";
	solver.compute(A);

	if(solver.info()!=Eigen::Success) {
		// decomposition failed
		cout<<"Fail to decompose.";
		return false;
	}
	cout<<"Decompose Succeeded.";
	return true;
}

Eigen::VectorXd MaskPoissonSolver::Solve( const Eigen::VectorXd& b)//b应当是m维的
{
	
	//ans = Eigen::VectorXd::Constant(m,0);

	Eigen::VectorXd ans(m) ;
	ans	= solver.solve(b);

	if(solver.info()!=Eigen::Success) {
		// solving failed
		cout<<"Fail to solve.\n";
		return Eigen::VectorXd::Constant(m,0);
	}
	return ans;
}

Eigen::VectorXd MaskPoissonSolver::get_b( const Eigen::VectorXd& grad,const Eigen::VectorXd& bd )
{
	return grad - B*bd;
}







