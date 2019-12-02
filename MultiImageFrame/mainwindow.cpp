#include <QtWidgets>
#include "mainwindow.h"
#include "ChildWindow.h"
#include "ImageWidget.h"
#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	//ui.setupUi(this);

	mdi_area_ = new QMdiArea;
	mdi_area_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	mdi_area_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setCentralWidget(mdi_area_);

	window_mapper_ = new QSignalMapper(this);
	connect(window_mapper_, SIGNAL(mapped(QWidget*)), this, SLOT(SetActiveSubWindow(QWidget*)));

	CreateActions();
	CreateMenus();
	CreateToolBars();
	CreateStatusBar();

}

MainWindow::~MainWindow()
{
	delete menu_file_;
	delete menu_edit_;
	delete menu_boundary_;
	//delete menu_help_;

	delete toolbar_file_;
	delete toolbar_edit_;
	delete toolbar_poisson_;

	//	delete action_new_;
	delete action_open_;
	delete action_save_;
	delete action_saveas_;

	//basic processing
	delete action_invert_;
	delete action_mirror_;
	delete action_gray_;
	delete action_restore_;

	//Draw Boundary
	delete action_draw_rect_;
	delete action_draw_ellipse_;
	delete action_draw_polygon_;
	delete action_draw_freehand_;
	delete action_undo_boundary;
	delete action_clear_boundary;
	delete action_set_pencolor;

	//Mask
	delete action_paintmaskwindow_;

	//Paste & Poisson
	delete action_copy_;
	delete action_paste_;
	delete action_poisson_paste_;

	delete mdi_area_;
	delete window_mapper_;
}

void MainWindow::CreateActions()
{
// 	action_new_ = new QAction(QIcon(":/MainWindow/Resources/images/new.png"), tr("&New"), this);
// 	action_new_->setShortcut(QKeySequence::New);
// 	action_new_->setStatusTip(tr("Create a new file"));

	// File IO
	action_open_ = new QAction(QIcon(":/MainWindow/Resources/images/open.png"), tr("&Open..."), this);
	action_open_->setShortcuts(QKeySequence::Open);
	action_open_->setStatusTip(tr("Open an existing file"));
	connect(action_open_, SIGNAL(triggered()), this, SLOT(Open()));

	action_save_ = new QAction(QIcon(":/MainWindow/Resources/images/save.png"), tr("&Save"), this);
	action_save_->setShortcuts(QKeySequence::Save);
	action_save_->setStatusTip(tr("Save the document to disk"));
	connect(action_save_, SIGNAL(triggered()), this, SLOT(Save()));

	action_saveas_ = new QAction(tr("Save &As..."), this);
	action_saveas_->setShortcuts(QKeySequence::SaveAs);
	action_saveas_->setStatusTip(tr("Save the document under a new name"));
	connect(action_saveas_, SIGNAL(triggered()), this, SLOT(SaveAs()));

	//Draw Boundary
	action_draw_rect_= new QAction(tr("Rect"), this);
	action_draw_rect_->setStatusTip(tr("Draw Rectangular Boundary"));
	connect(action_draw_rect_, SIGNAL(triggered()), this, SLOT(StartDrawBoundary_rect()));

	action_draw_ellipse_ = new QAction(tr("Ellipse"),this);
	action_draw_ellipse_->setStatusTip(tr("Draw Elliptical Boundary"));
	connect(action_draw_ellipse_,SIGNAL(triggered()),this,SLOT(StartDrawBoundary_ellipse()));

	action_draw_polygon_ = new QAction(tr("Polygon"),this);
	action_draw_polygon_->setStatusTip(tr("Draw Polygonal Boundary"));
	connect(action_draw_polygon_,SIGNAL(triggered()),this,SLOT(StartDrawBoundary_poly()));

	action_draw_freehand_ = new QAction(tr("Freehand"),this);
	action_draw_freehand_->setStatusTip(tr("Draw Freehand Boundary"));
	connect(action_draw_freehand_,SIGNAL(triggered()),this,SLOT(StartDrawBoundary_freehand()));
	
	action_undo_boundary = new QAction(tr("Del"),this);
	action_undo_boundary->setStatusTip(tr("Delete last boundary plot"));
	connect(action_undo_boundary,SIGNAL(triggered()),this,SLOT(UndoDrawBoundary()));

	action_clear_boundary = new QAction(tr("Clear"),this);
	action_clear_boundary->setStatusTip(tr("Clear boundary plot"));
	connect(action_clear_boundary,SIGNAL(triggered()),this,SLOT(ClearBoundary()));
	
	action_set_pencolor = new QAction(tr("PenColor"),this);
	action_set_pencolor->setStatusTip(tr("Set pen color for boundary line"));
	connect(action_set_pencolor,SIGNAL(triggered()),this, SLOT(SetBoundaryPenColor()));
	
	
	// Poisson image editing
	action_paintmaskwindow_ = new QAction(tr("Mask"),this);
	action_paintmaskwindow_->setStatusTip(tr("Paint the Mask Window"));
	connect(action_paintmaskwindow_,SIGNAL(triggered()),this,SLOT(PaintMaskWindow()));
	
	action_copy_ = new QAction(tr("Copy"),this);
	action_copy_->setStatusTip(tr("Copy the Image and Boundary from the current active window."));
	connect(action_copy_,SIGNAL(triggered()),this,SLOT(Copy()));

	action_paste_ = new QAction(tr("Paste"), this);
	action_paste_->setStatusTip(tr("Paste Selected Part"));
	connect(action_paste_, SIGNAL(triggered()), this, SLOT(Paste()));

	action_poisson_paste_ = new QAction(tr("PoissonPaste"), this);
	action_poisson_paste_->setStatusTip(tr("Seamless paste with poisson method"));
	connect(action_poisson_paste_, SIGNAL(triggered()), this, SLOT(PoissonPaste()));

	// Image processing
	action_invert_ = new QAction(tr("Inverse"), this);
	action_invert_->setStatusTip(tr("Invert all pixel value in the image"));
	connect(action_invert_, SIGNAL(triggered()), this, SLOT(Invert()));

	action_mirror_ = new QAction(tr("Mirror"), this);
	action_mirror_->setStatusTip(tr("Mirror image vertically or horizontally"));
	connect(action_mirror_, SIGNAL(triggered()), this, SLOT(Mirror()));

	action_gray_ = new QAction(tr("Grayscale"), this);
	action_gray_->setStatusTip(tr("Gray-scale map"));
	connect(action_gray_, SIGNAL(triggered()), this, SLOT(GrayScale()));

	action_restore_ = new QAction(tr("Restore"), this);
	action_restore_->setStatusTip(tr("Show origin image"));
	connect(action_restore_, SIGNAL(triggered()), this, SLOT(Restore()));
}

void MainWindow::CreateMenus()
{
	menu_file_ = menuBar()->addMenu(tr("&File"));
	menu_file_->setStatusTip(tr("File menu"));
//	menu_file_->addAction(action_new_);
	menu_file_->addAction(action_open_);
	menu_file_->addAction(action_save_);
	menu_file_->addAction(action_saveas_);

//	menu_file_->addAction(action_choose_polygon_);

	menu_edit_ = menuBar()->addMenu(tr("&Edit"));
	menu_edit_->setStatusTip(tr("Edit menu"));
	menu_edit_->addAction(action_copy_);
	menu_edit_->addAction(action_paste_);
	menu_edit_->addAction(action_poisson_paste_);
	menu_edit_->addAction(action_invert_);
	menu_edit_->addAction(action_mirror_);
	menu_edit_->addAction(action_gray_);
	menu_edit_->addAction(action_restore_);

	menu_boundary_ = menuBar()->addMenu(tr("&Boundary"));
	menu_boundary_->addAction(action_draw_rect_);
	menu_boundary_->addAction(action_draw_polygon_);
	menu_boundary_->addAction(action_draw_freehand_);
	menu_boundary_->addAction(action_undo_boundary);
	menu_boundary_->addAction(action_clear_boundary);
	menu_boundary_->addAction(action_set_pencolor);

	

}

void MainWindow::CreateToolBars()
{
	toolbar_file_ = addToolBar(tr("File"));
//	toolbar_file_->addAction(action_new_);
	toolbar_file_->addAction(action_open_);
	toolbar_file_->addAction(action_save_);

	// Poisson Image Editing
	toolbar_poisson_ = addToolBar(tr("Poisson"));
	toolbar_poisson_->addAction(action_draw_rect_);
	toolbar_poisson_->addAction(action_draw_ellipse_);
	toolbar_poisson_->addAction(action_draw_polygon_);
	toolbar_poisson_->addAction(action_draw_freehand_);

	toolbar_poisson_->addSeparator();
	toolbar_poisson_->addAction(action_undo_boundary);
	toolbar_poisson_->addAction(action_clear_boundary);
	toolbar_poisson_->addAction(action_set_pencolor);

	toolbar_poisson_->addSeparator();
	toolbar_poisson_->addAction(action_paintmaskwindow_);
	toolbar_poisson_->addAction(action_copy_);
	toolbar_poisson_->addAction(action_paste_);
	toolbar_poisson_->addAction(action_poisson_paste_);
	toolbar_poisson_->addAction(action_restore_);

	toolbar_edit_ = addToolBar(tr("Edit"));
	toolbar_edit_->addSeparator();
	toolbar_edit_->addAction(action_invert_);
	toolbar_edit_->addAction(action_mirror_);
	toolbar_edit_->addAction(action_gray_);

}

void MainWindow::CreateStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

void MainWindow::Open()
{
	QString filename = QFileDialog::getOpenFileName(this);
	if(!filename.isEmpty())
	{
		QMdiSubWindow *existing = FindChild(filename);

		if (existing)
		{
			mdi_area_->setActiveSubWindow(existing);
			return;
		}

		ChildWindow *child = CreateChildWindow();
		if (child->LoadFile(filename))
		{
			statusBar()->showMessage(tr("File loaded"), 2000);
			child->show();

			// Change size of the child window so it can fit image size
			int x = child->geometry().x();
			int y = child->geometry().y();
			int width = child->imagewidget_->ImageWidth();
			int height = child->imagewidget_->ImageHeight();
			mdi_area_->activeSubWindow()->setFixedSize(width+2*x, height+x+y);
		}
		else
		{
			child->close();
		}
	}
}

void MainWindow::Save()
{
	SaveAs();
}

void MainWindow::SaveAs()
{
	if(!mdi_area_->activeSubWindow()) return; //如果没有活动窗口，退出
	ChildWindow *window = qobject_cast<ChildWindow *>(mdi_area_->activeSubWindow()->widget());
	window->imagewidget_->SaveAs();
}

ChildWindow *MainWindow::CreateChildWindow()
{
	ChildWindow *child = new ChildWindow;
	mdi_area_->addSubWindow(child);
	return child;
}

void MainWindow::SetActiveSubWindow(QWidget* window)
{
	if (!window)
	{
		return;
	}

	mdi_area_->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

QMdiSubWindow *MainWindow::FindChild(const QString &filename)
{
	QString canonical_filepath = QFileInfo(filename).canonicalFilePath();

	foreach (QMdiSubWindow *window, mdi_area_->subWindowList())
	{
		ChildWindow *child = qobject_cast<ChildWindow *>(window->widget());
		if (child->current_file() == canonical_filepath)
		{
			return window;
		}
	}
	return 0;
}

void MainWindow::Invert()
{
	if(!mdi_area_->activeSubWindow()) return; //如果没有活动窗口，退出
	ChildWindow *window = qobject_cast<ChildWindow *>(mdi_area_->activeSubWindow()->widget());
	if(window) window->imagewidget_->Invert();
}

void MainWindow::Mirror()
{
	if(!mdi_area_->activeSubWindow()) return; //如果没有活动窗口，退出
	ChildWindow *window = qobject_cast<ChildWindow *>(mdi_area_->activeSubWindow()->widget());
	if(window) window->imagewidget_->Mirror();
}

void MainWindow::GrayScale()
{
	if(!mdi_area_->activeSubWindow()) return; //如果没有活动窗口，退出
	ChildWindow *window = qobject_cast<ChildWindow *>(mdi_area_->activeSubWindow()->widget());
	if(window) window->imagewidget_->TurnGray();
}

void MainWindow::Restore()
{
	if(!mdi_area_->activeSubWindow()) return; //如果没有活动窗口，退出
	ChildWindow *window = qobject_cast<ChildWindow *>(mdi_area_->activeSubWindow()->widget());
	if(window) window->imagewidget_->Restore();
}

void MainWindow::PoissonPaste()
{
	if(!mdi_area_->activeSubWindow()) return; //如果没有活动窗口，退出
	// Paste image rect region to object image
	ChildWindow *child = qobject_cast<ChildWindow *>(mdi_area_->activeSubWindow()->widget());
	if(child){
		//child->imagewidget_->set_source_window(child_source_);
		child->imagewidget_->set_copyboard(&copyboard);
		child->imagewidget_->set_draw_status_to_poisson();
	}
}
void MainWindow::Paste()
{
	if(!mdi_area_->activeSubWindow()) return; //如果没有活动窗口，退出
	// Paste image rect region to object image
	ChildWindow *child = qobject_cast<ChildWindow *>(mdi_area_->activeSubWindow()->widget());
	if(child){
		//child->imagewidget_->set_source_window(child_source_);
		child->imagewidget_->set_copyboard(&copyboard);
		child->imagewidget_->set_draw_status_to_paste();
	}
}


void MainWindow::Copy()
{
	if(!mdi_area_->activeSubWindow()) return; //如果没有活动窗口，退出
	// Paste image rect region to object image
	ChildWindow *child = qobject_cast<ChildWindow *>(mdi_area_->activeSubWindow()->widget());
	if(child){
		copyboard.set_img(child->imagewidget_->image());
		/*copyboard.scanmask = *child->imagewidget_->scanmask();*/
		copyboard.calculate(child->imagewidget_->boundary_.array_.cbegin(),
							child->imagewidget_->boundary_.array_.cend());
	}
}

void MainWindow::StartDrawBoundary_rect()
{
	if(!mdi_area_->activeSubWindow()) return; //如果没有活动窗口，退出
	// Set source child window
	ChildWindow *window = qobject_cast<ChildWindow *>(mdi_area_->activeSubWindow()->widget());
	if(window){
		window->imagewidget_->set_draw_status_to_choose();
		//child_source_ = window;
		window->imagewidget_->boundary_.set_rectangle();
		SetOtherDrawStatusToNone(window);
	}
}
void MainWindow::StartDrawBoundary_poly()
{
	if(!mdi_area_->activeSubWindow()) return; //如果没有活动窗口，退出
	// Set source child window
	ChildWindow *window = qobject_cast<ChildWindow *>(mdi_area_->activeSubWindow()->widget());
	if(window){
		window->imagewidget_->set_draw_status_to_choose();
		//child_source_ = window;
		window->imagewidget_->boundary_.set_polygon();
		SetOtherDrawStatusToNone(window);
	}
}
void MainWindow::StartDrawBoundary_freehand()
{
	if(!mdi_area_->activeSubWindow()) return; //如果没有活动窗口，退出
	// Set source child window
	ChildWindow *window = qobject_cast<ChildWindow *>(mdi_area_->activeSubWindow()->widget());
	if(window){
		window->imagewidget_->set_draw_status_to_choose();
		//child_source_ = window;
		window->imagewidget_->boundary_.set_freehand();
		SetOtherDrawStatusToNone(window);
	}
}
void MainWindow::StartDrawBoundary_ellipse()
{
	if(!mdi_area_->activeSubWindow()) return; //如果没有活动窗口，退出
	// Set source child window
	ChildWindow *window = qobject_cast<ChildWindow *>(mdi_area_->activeSubWindow()->widget());
	if(window){
		window->imagewidget_->set_draw_status_to_choose();
		//child_source_ = window;
		window->imagewidget_->boundary_.set_ellipse();
		SetOtherDrawStatusToNone(window);
	}
}

void MainWindow::SetBoundaryPenColor()
{
	if(!mdi_area_->activeSubWindow()) return; //如果没有活动窗口，退出
	// Set source child window
	ChildWindow *window = qobject_cast<ChildWindow *>(mdi_area_->activeSubWindow()->widget());
	if(window) window->imagewidget_->set_boundary_pen_color();
}

void MainWindow::UndoDrawBoundary()
{
	if(!mdi_area_->activeSubWindow()) return; //如果没有活动窗口，退出
	ChildWindow *window = qobject_cast<ChildWindow *>(mdi_area_->activeSubWindow()->widget());
	if(window) window->imagewidget_->boundary_.cancel();
}

void MainWindow::ClearBoundary()
{
	if(!mdi_area_->activeSubWindow()) return; //如果没有活动窗口，退出
	ChildWindow *window = qobject_cast<ChildWindow *>(mdi_area_->activeSubWindow()->widget());
	if(window) window->imagewidget_->boundary_.clear();
}

void MainWindow::SetOtherDrawStatusToNone(ChildWindow *active_window)
{
	foreach (QMdiSubWindow *window, mdi_area_->subWindowList())
	{
		ChildWindow *child = qobject_cast<ChildWindow *>(window->widget());
		if (child != active_window)
		{
			child->imagewidget_->set_draw_status_to_none();
		}
	}
}

void MainWindow::PaintMaskWindow()
{
	if(!mdi_area_->activeSubWindow()) return;
	ChildWindow *window = qobject_cast<ChildWindow *>(mdi_area_->activeSubWindow()->widget());
	if(!window) return;
	ChildWindow *childmask;
	//寻找maskwindow,如果不存在，还要新建，这里查找是因为用户可能已经关闭了窗口，这时候window->maskwindow可能没有意义。
	childmask = NULL;
	foreach (QMdiSubWindow *subwindow, mdi_area_->subWindowList())
	{
		ChildWindow *child = qobject_cast<ChildWindow *>(subwindow->widget());
		if (child == window->maskwindow){
			childmask = window->maskwindow;
			break;
		}
	}
	if(!childmask){
		childmask = new ChildWindow;
		mdi_area_->addSubWindow(childmask);
		window->maskwindow = childmask;
	}

	childmask->imagewidget_->producemask(window);
	childmask->current_file_ = window->current_file_;
	statusBar()->showMessage(tr("Mask_Produced"), 2000);
	childmask->show();	
	int x = childmask->geometry().x();
	int y = childmask->geometry().y();
	int width = childmask->imagewidget_->ImageWidth();
	int height = childmask->imagewidget_->ImageHeight();
	mdi_area_->activeSubWindow()->setFixedSize(width+2*x, height+x+y);
	//child->close();
}


/*********************

对样例程序修改：

MainWindow Action对应的函数添加
	if(!mdi_area_->activeSubWindow()) return;
要检验是否为NULL，否则运行时直接点按钮会出错

修改了Paste函数内语句顺序，因为点击Paste时同时处理了源数据的ScanMask

SetOtherDrawStatusToNone
	为了显示与child_source的变化一致，设置只有child_source绘制边界

*********************/