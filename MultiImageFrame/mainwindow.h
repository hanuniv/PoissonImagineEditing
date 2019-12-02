#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "DrawBoundary.h"
#include "CopyBoard.h"

class ChildWindow;
QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	// File IO
//	void New();
	void Open();								// Open an existing file
	void Save();								// Save image to file
	void SaveAs();
	ChildWindow *CreateChildWindow();
	void SetActiveSubWindow(QWidget* window);

	// Image Processing
	void Invert();								// Invert each pixel's rgb value
	void Mirror();								// Mirror image vertically or horizontally
	void GrayScale();							// Turn image to gray-scale map
	void Restore();								// Restore image to origin

	// Poisson Image Editing
	void StartDrawBoundary_rect();
	void StartDrawBoundary_poly();
	void StartDrawBoundary_freehand();					// Start Draw Boundary
	void StartDrawBoundary_ellipse();
	void SetBoundaryPenColor();
	void UndoDrawBoundary();
	void ClearBoundary();
	void PaintMaskWindow();
	void PoissonPaste();								// Paste region to object image
	void Paste();
	void Copy();

private:
	void CreateActions();
	void CreateMenus();
	void CreateToolBars();
	void CreateStatusBar();

	QMdiSubWindow *FindChild(const QString &filename);

	void SetOtherDrawStatusToNone(ChildWindow *);//设置只有active_window绘制边界

private:
	Ui::MainWindowClass ui;
	//file operation
	QMenu						*menu_file_,*menu_edit_,*menu_boundary_,*menu_help_;
	
	QToolBar					*toolbar_file_,*toolbar_edit_,*toolbar_poisson_;

//	QAction						*action_new_;
	QAction						*action_open_;
	QAction						*action_save_;
	QAction						*action_saveas_;

	//basic processing
	QAction						*action_invert_;
	QAction						*action_mirror_;
	QAction						*action_gray_;
	QAction						*action_restore_;

	//Draw Boundary
	QAction						*action_draw_rect_;
	QAction						*action_draw_ellipse_;
	QAction						*action_draw_polygon_;
	QAction						*action_draw_freehand_;
	QAction						*action_undo_boundary;
	QAction						*action_clear_boundary;
	QAction						*action_set_pencolor;

	//Mask
	QAction						*action_paintmaskwindow_;

	//Copy，Paste & Poisson
	QAction						*action_copy_;
	QAction						*action_paste_;
	QAction						*action_poisson_paste_;

	QMdiArea					*mdi_area_;
	QSignalMapper				*window_mapper_;

	//ChildWindow					*child_source_; //在绘制边界时指向窗口，在点击Paste时传给ImageWidget
	
	CopyBoard					copyboard;//所有窗口指向剪贴板
};

#endif // MAINWINDOW_H
