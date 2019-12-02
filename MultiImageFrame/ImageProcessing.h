#ifndef IMAGEPROCESSING_H_
#define IMAGEPROCESSING_H_
//将一系列操作用类封装

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
	operator bool();  //类型转换，用于返回分解是否成功
private:
	int xpos,ypos;
	const ScanMask*				p_source_mask_;
	const Tpic*					p_source_image_;
	Eigen::VectorXd				gradr,gradg,gradb;   //存内部的梯度值
	MaskPoissonSolver			mask_solver;		//Poisson Solver
	Report_Once					once;				//Report Error Message
	bool						successful_init;	//标记初始化是否成功
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