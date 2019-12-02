#include "AbstractProcess.h"


bool Abstract_Process::haschanged()
{
	return _haschanged;
}

void Abstract_Process::set_changed()
{
	_haschanged = true;
}

void Abstract_Process::set_backup( Tpic* image_mat_)
{
	image_mat_backup_ = new QImage();
	*image_mat_backup_ = *image_mat_;
	_haschanged = true;
}

void Abstract_Process::retrieve_backup(Tpic * image_mat_)
{
	*image_mat_ =  image_mat_backup_->copy();
}

Abstract_Process::Abstract_Process()
{
	_haschanged =false;
	image_mat_backup_ = NULL;
}

void Abstract_Process::process( Tpic * image_)
{

}

void Abstract_Process::setdata( int i,int j)
{

}

Abstract_Process::operator bool()
{
	return true;
}

