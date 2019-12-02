#ifndef SCANMASK_H_
#define SCANMASK_H_
#include <vector>
#include <utility>
#include <map>
#include "Eigen/Sparse"
#include "Eigen/SparseLU"        //OK
#include "Eigen/SparseQR"  
#include "Eigen/SparseCholesky" 
#include "Eigen/IterativeLinearSolvers"
#include "Eigen/Dense"
#include "Getcenter.h"
#include "MyPoint.h" //提供TPoint类用于内置的计算
#include "DrawBoundary.h" //提供绘制点的迭代器
#include "MaskIterator.h"//提供对Mask的迭代器

using std::vector;
using std::pair;
using std::map;

//类ScanMask：用于实现扫描线填充和内部遍历
//类MaskPoissonSolver：与ScanMask一起使用

enum MaskType{UnMarked,LineMark,Inside,Outline}; //内部参数用的标记，分别表示：外部，填充标记，内部，边界

//enum MaskProcess{Mdefault,MMarked,Mfilled,Mlined}; //标记运行过程的标记，用于提示操作顺序

class ScanMask_iterator;
class ScanMask_Inside_iterator;
class ScanMask_Outline_iterator;
class ScanMask_Outside_iterator;

class ScanMask
{
public:
	typedef map<TPoint,int> Tlabel; //位置映射到标号，用于查询
	typedef vector<TPoint> Tloca;   //标号映射到位置，用于遍历
	typedef ScanMask_Inside_iterator const_inside_iterator;
	typedef ScanMask_Outline_iterator const_boundary_iterator;
	typedef ScanMask_Outside_iterator const_outside_iterator;

	//Iterator
	friend ScanMask_iterator;
	friend ScanMask_Inside_iterator;
	friend ScanMask_Outline_iterator;
	friend ScanMask_Outside_iterator;
	ScanMask_Inside_iterator inside_begin() const;
	ScanMask_Inside_iterator inside_end() const;
	ScanMask_Outline_iterator boundary_begin()const;
	ScanMask_Outline_iterator boundary_end() const;
	ScanMask_Outside_iterator outside_begin()const;
	ScanMask_Outside_iterator outside_end()const;

	ScanMask();
	~ScanMask();
	void SetSize(int m,int n);

	Getcenter get_center;    //由内部的数据得到一个比较合适的中心

	//用Boundary序列生成内部数据
	void Calculate_from_boudary(vector<Boundary*>::const_iterator,vector<Boundary*>::const_iterator);
	
	int get_label(int i,int j) const;
	//int get_label(Tlabel::const_iterator it) const;

	bool is_inpic(const TPoint&)const; //判断是否超出图像范围，默认超出图像的点舍去
	bool is_inpic(int,int)const;
	bool is_onboundary(Tlabel::const_iterator it) const;
	bool is_onboundary(int,int)const;
	bool is_inside(int,int)const;
	int  num_inside()const;
	int  num_boundary()const;

	//输出数据结果，可以特殊标记一个位置
	void printpic(int=-1,int=-1) const;

private:
	void Markline(int i,int j);//LineMark->UnMarked,否则置LineMark
	void UnMarkline(int i,int j);//lineMark不变，其他标UnMarked

	//	扫描线算法的一系列内置操作
	//调用次序应当是：先对所有图像Mark，然后调用一次fill_inside,最后调用一次drawlines的Outline标记覆盖LineMark标记
	//void Mark(Boundary::const_iterator,Boundary::const_iterator); //在内部数据上覆盖LineMark标记(会覆盖掉除LineMark之外的标记)
	void Mark(const Boundary* bd);//用于泛型算法
	void fill_inside(); ////在内部数据上覆盖Inside标记(会覆盖掉除LineMark之外的标记)
	//void drawlines(Boundary::const_iterator,Boundary::const_iterator);//在内部数据上覆盖Outline标记(会覆盖掉其它标记)
	void ScanMask::drawlines(const Boundary * bd);//用于泛型算法
	void generate_label();//对内部和边界位置生成一个标号列表用于遍历和查询点的下标，标号从0开始

	//有了迭代器之后可以数据置私有
	int _m,_n; //Size of Canvas
	vector<vector<MaskType>> _a;

	Tlabel _label;//位置映射到标号，用于查询
	Tloca _loca;  //标号映射到位置，用于遍历,先存Inside,再存Outline,
	int _loca_end_Inside;
};



class MaskPoissonSolver //用于由点列得到Poisson方程的工具，
{
public:
	typedef Eigen::SparseMatrix<double> SpMat; // declares a column-major sparse matrix type of double
	
	MaskPoissonSolver();
	bool init(const ScanMask *); //用ScanMask初始化,返回分解成功与否
	Eigen::VectorXd Solve(const Eigen::VectorXd& b); //b应当是m维的
	Eigen::VectorXd get_b(const Eigen::VectorXd& grad,const Eigen::VectorXd& bd);//由内部梯度和边界的数据计算右侧的值

private:
	const ScanMask				* _scanmask;
	int							m;				 //存矩阵维数
	SpMat						A,B;				//稀疏矩阵用于计算内部梯度和边界的数据
	Eigen::SimplicialLDLT<SpMat>solver;			//求解器
	//边界作为常数，用SimplicialLDLT较合适。SimplicialCholesky,SparseLU,BiCGSTAB也能用。
};

#endif