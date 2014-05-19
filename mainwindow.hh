#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include "glwidget.hh"

namespace Ui {
class WidgetTool;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void onSetClicked();

private:
	GLWidget* _gl;
	Ui::WidgetTool *ui;
};

#endif // MAINWINDOW_HH
