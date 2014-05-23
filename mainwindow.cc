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

	connect(editor, SIGNAL(edited()), this, SLOT(code_edited()));
	connect(&timer, SIGNAL(timeout()), this, SLOT(compile()));
	timer.setSingleShot(true);
	splitter->addWidget(editor);

	setCentralWidget(splitter);

	QSettings settings;
	splitter->restoreState(settings.value("splitterSizes").toByteArray());
	setGeometry(settings.value("geometry", geometry()).toRect());
	compile();
}

MainWindow::~MainWindow()
{
	QSplitter* splitter = qobject_cast<QSplitter*>(centralWidget());
	QSettings settings;
	if (splitter) {
		settings.setValue("splitterSizes", splitter->saveState());
	}
	settings.setValue("geometry", geometry());
}

void MainWindow::code_edited()
{
	timer.start(1000);
}

void MainWindow::compile()
{
	_gl->_sys._ps.clear();
	int line = reader.run(&_gl->_sys, editor->currentCode());
	editor->setLineError(line, reader.error);
}
