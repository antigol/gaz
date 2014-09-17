#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QTimer>
#include <QComboBox>
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
	void slot_code_edited();
	void slot_compile();
	void slot_algo_changed();

private:
    GLWidget* viewWidget;

	ScriptReader reader;
	ScriptEditor* editor;
	QTimer timer;
	QComboBox *comboBox;
};

#endif // MAINWINDOW_HH
