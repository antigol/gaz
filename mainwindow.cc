#include "mainwindow.hh"
#include <QHBoxLayout>
#include <QFormLayout>
#include <QSplitter>
#include <QSettings>
#include <QMenuBar>
#include <QWidgetAction>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{

	QSplitter *splitter = new QSplitter(this);

	viewWidget = new GLWidget(splitter);

	splitter->addWidget(viewWidget);

	editor = new ScriptEditor(splitter);

	connect(editor, SIGNAL(edited()), this, SLOT(slot_code_edited()));
	connect(&timer, SIGNAL(timeout()), this, SLOT(slot_compile()));
	timer.setSingleShot(true);
	splitter->addWidget(editor);

	setCentralWidget(splitter);

	QSettings settings;
	splitter->restoreState(settings.value("splitterSizes").toByteArray());
	setGeometry(settings.value("geometry", geometry()).toRect());


	menuBar()->addAction("restart", this, SLOT(slot_compile()));

	QAction* action_pause = menuBar()->addAction("pause", viewWidget, SLOT(pause()));
	action_pause->setCheckable(true);
	action_pause->setChecked(false);

	QMenu* menu = menuBar()->addMenu("algorithm");

	comboBox = new QComboBox(menu);
	QWidgetAction *action = new QWidgetAction(menu);
	action->setDefaultWidget(comboBox);
	menu->addAction(action);

	comboBox->addItems(QStringList() << "multimap" << "x sort" << "naive");

	connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_algo_changed()));
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

void MainWindow::slot_code_edited()
{
	timer.start(1000);
}

void MainWindow::slot_compile()
{
	viewWidget->sys.pause(true);
	viewWidget->sys.ps.clear();
	int line = reader.run(&viewWidget->sys, editor->currentCode());
	editor->setLineError(line, reader.error);
	if (line != 0) {
		viewWidget->sys.ps.clear();
		viewWidget->sys.initialize();
	}
	viewWidget->sys.pause(false);
}

void MainWindow::slot_algo_changed()
{
	viewWidget->sys.algorithm = comboBox->currentIndex();
}
