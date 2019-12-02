#ifndef IMAGEPROCESSING_H_
#define IMAGEPROCESSING_H_
//��һϵ�в��������װ

#include "AbstractProcess.h"
#include "ScanMaskSolver.h"
#include "Report_Once.h"

class Invert_Process: public Abstract_Process
{
public:
	void process(Tpic* image_);
};

class Mirror_Process:public Abstract_Process
{
public:
	Mirror_Process();
	void process(Tpic* image_);
	bool horizontal,vertical;
};

class TurnGrey_Process:public Abstract_Process
{
public:
	void process(Tpic* image_);
};

class Paste_Process : public Abstract_Process
{
public:
	Paste_Process(const ScanMask*,const Tpic*);
	void process(Tpic* image_);
	void setdata(int,int);
private:
	int xpos,ypos;
	const ScanMask*  p_source_mask_;
	const Tpic*	 p_source_image_;
};

class Poisson_Process:public Abstract_Process
{
public:
	Poisson_Process(const ScanMask*,const Tpic*);
	void process(Tpic* image_);
	void setdata(int,int);
	operator bool();  //����ת�������ڷ��طֽ��Ƿ�ɹ�
private:
	int xpos,ypos;
	const ScanMask*				p_source_mask_;
	const Tpic*					p_source_image_;
	Eigen::VectorXd				gradr,gradg,gradb;   //���ڲ����ݶ�ֵ
	MaskPoissonSolver			mask_solver;		//Poisson Solver
	Report_Once					once;				//Report Error Message
	bool						successful_init;	//��ǳ�ʼ���Ƿ�ɹ�
};

class Gray_Mask_Process:public Abstract_Process
{
public:
	Gray_Mask_Process(const ScanMask*);
	void process(Tpic* image_);

private:
	const ScanMask*  p_source_mask_;
};
#endif