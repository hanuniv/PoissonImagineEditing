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
#include "MyPoint.h" //�ṩTPoint���������õļ���
#include "DrawBoundary.h" //�ṩ���Ƶ�ĵ�����
#include "MaskIterator.h"//�ṩ��Mask�ĵ�����

using std::vector;
using std::pair;
using std::map;

//��ScanMask������ʵ��ɨ���������ڲ�����
//��MaskPoissonSolver����ScanMaskһ��ʹ��

enum MaskType{UnMarked,LineMark,Inside,Outline}; //�ڲ������õı�ǣ��ֱ��ʾ���ⲿ������ǣ��ڲ����߽�

//enum MaskProcess{Mdefault,MMarked,Mfilled,Mlined}; //������й��̵ı�ǣ�������ʾ����˳��

class ScanMask_iterator;
class ScanMask_Inside_iterator;
class ScanMask_Outline_iterator;
class ScanMask_Outside_iterator;

class ScanMask
{
public:
	typedef map<TPoint,int> Tlabel; //λ��ӳ�䵽��ţ����ڲ�ѯ
	typedef vector<TPoint> Tloca;   //���ӳ�䵽λ�ã����ڱ���
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

	Getcenter get_center;    //���ڲ������ݵõ�һ���ȽϺ��ʵ�����

	//��Boundary���������ڲ�����
	void Calculate_from_boudary(vector<Boundary*>::const_iterator,vector<Boundary*>::const_iterator);
	
	int get_label(int i,int j) const;
	//int get_label(Tlabel::const_iterator it) const;

	bool is_inpic(const TPoint&)const; //�ж��Ƿ񳬳�ͼ��Χ��Ĭ�ϳ���ͼ��ĵ���ȥ
	bool is_inpic(int,int)const;
	bool is_onboundary(Tlabel::const_iterator it) const;
	bool is_onboundary(int,int)const;
	bool is_inside(int,int)const;
	int  num_inside()const;
	int  num_boundary()const;

	//������ݽ��������������һ��λ��
	void printpic(int=-1,int=-1) const;

private:
	void Markline(int i,int j);//LineMark->UnMarked,������LineMark
	void UnMarkline(int i,int j);//lineMark���䣬������UnMarked

	//	ɨ�����㷨��һϵ�����ò���
	//���ô���Ӧ���ǣ��ȶ�����ͼ��Mark��Ȼ�����һ��fill_inside,������һ��drawlines��Outline��Ǹ���LineMark���
	//void Mark(Boundary::const_iterator,Boundary::const_iterator); //���ڲ������ϸ���LineMark���(�Ḳ�ǵ���LineMark֮��ı��)
	void Mark(const Boundary* bd);//���ڷ����㷨
	void fill_inside(); ////���ڲ������ϸ���Inside���(�Ḳ�ǵ���LineMark֮��ı��)
	//void drawlines(Boundary::const_iterator,Boundary::const_iterator);//���ڲ������ϸ���Outline���(�Ḳ�ǵ��������)
	void ScanMask::drawlines(const Boundary * bd);//���ڷ����㷨
	void generate_label();//���ڲ��ͱ߽�λ������һ������б����ڱ����Ͳ�ѯ����±꣬��Ŵ�0��ʼ

	//���˵�����֮�����������˽��
	int _m,_n; //Size of Canvas
	vector<vector<MaskType>> _a;

	Tlabel _label;//λ��ӳ�䵽��ţ����ڲ�ѯ
	Tloca _loca;  //���ӳ�䵽λ�ã����ڱ���,�ȴ�Inside,�ٴ�Outline,
	int _loca_end_Inside;
};



class MaskPoissonSolver //�����ɵ��еõ�Poisson���̵Ĺ��ߣ�
{
public:
	typedef Eigen::SparseMatrix<double> SpMat; // declares a column-major sparse matrix type of double
	
	MaskPoissonSolver();
	bool init(const ScanMask *); //��ScanMask��ʼ��,���طֽ�ɹ����
	Eigen::VectorXd Solve(const Eigen::VectorXd& b); //bӦ����mά��
	Eigen::VectorXd get_b(const Eigen::VectorXd& grad,const Eigen::VectorXd& bd);//���ڲ��ݶȺͱ߽�����ݼ����Ҳ��ֵ

private:
	const ScanMask				* _scanmask;
	int							m;				 //�����ά��
	SpMat						A,B;				//ϡ��������ڼ����ڲ��ݶȺͱ߽������
	Eigen::SimplicialLDLT<SpMat>solver;			//�����
	//�߽���Ϊ��������SimplicialLDLT�Ϻ��ʡ�SimplicialCholesky,SparseLU,BiCGSTABҲ���á�
};

#endif