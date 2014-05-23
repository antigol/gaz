#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QTimer>
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
	void code_edited();
	void compile();

private:
	GLWidget* _gl;

	ScriptReader reader;
	ScriptEditor* editor;
	QTimer timer;
};

#endif // MAINWINDOW_HH
