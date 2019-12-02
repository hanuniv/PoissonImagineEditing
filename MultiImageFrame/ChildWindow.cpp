#include "ChildWindow.h"
#include "ImageWidget.h"


ChildWindow::ChildWindow(void)
{
	imagewidget_ = new ImageWidget(this);
	setCentralWidget(imagewidget_);
	maskwindow = NULL;
}


ChildWindow::~ChildWindow(void)
{
	delete imagewidget_;
}

bool ChildWindow::LoadFile(QString filename)
{
	imagewidget_->Open(filename);
	return true;
}

/*********************

½áÊøÊ±Òªdelete imagewidget

*************************/