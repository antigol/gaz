#include "mainwindow.hh"
#include <QHBoxLayout>
#include <QFormLayout>
#include <QSplitter>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{

	QSplitter *splitter = new QSplitter(this);

	_gl = new GLWidget(splitter);

	splitter->addWidget(_gl);

	editor = new ScriptEditor(splitter);

	connect(editor, SIGNAL(edited()), this, SLOT(onSetClicked()));
	splitter->addWidget(editor);

	setCentralWidget(splitter);

	QSettings s;
	QString defaultCode = "App.dimension(40, 40, 40);\n"
						  "App.radius(1);\n"
						  "App.mass(1);\n"
						  "App.color(1,0,0);\n"
						  "App.position(App.rand(-20,20), App.rand(-20,20), App.rand(-20,20));\n"
						  "App.momentum(0,0,0);\n"
						  "App.addParticle();";
	editor->setText(s.value("code", defaultCode).toString());
}

MainWindow::~MainWindow()
{
	QSettings s;
	s.setValue("code", editor->text());
}

void MainWindow::onSetClicked()
{
	_gl->_sys._ps.clear();
	int line = reader.run(&_gl->_sys, editor->text());

	editor->setLineError(line, reader.error);
}
