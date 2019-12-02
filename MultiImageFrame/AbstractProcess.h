#ifndef ABSTRACT_CHANGE_H_
#define ABSTRACT_CHANGE_H_
#include "Eigen/Dense"
#include <QImage>



//抽象基类，用于图像存备份和恢复
//其他图像操作都从它继承

class Abstract_Process
{
	public:
		typedef QImage Tpic;
		Abstract_Process();
		bool haschanged();		
		void set_changed();
		void set_backup(Tpic*);
		void retrieve_backup(Tpic*);

		//操作函数
		virtual void process(Tpic* image_); 
		virtual void setdata(int,int); //用于设定参数
		virtual operator bool();
	protected:
		bool		_haschanged;								//whether should restore to previous state;
		Tpic* 		image_mat_backup_;//用指针，轻量化操作
};

#endif