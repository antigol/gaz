#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include "glwidget.hh"
#include "scriptreader.hh"
#include "scripteditor.hh"

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

	ScriptReader reader;
	ScriptEditor* editor;
};

#endif // MAINWINDOW_HH
