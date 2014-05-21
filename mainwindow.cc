#include "mainwindow.hh"
#include "ui_widgettool.h"
#include <QHBoxLayout>
#include <QFormLayout>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::WidgetTool)
{
	QWidget* cw = new QWidget(this);
	QHBoxLayout *l = new QHBoxLayout(cw);

	_gl = new GLWidget(cw);
	l->addWidget(_gl);

	QWidget* tool = new QWidget(this);
	ui->setupUi(tool);
	l->addWidget(tool);

	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onSetClicked()));

	setCentralWidget(cw);

	QSettings s;
	ui->nSpinBox->setValue(s.value("n", ui->nSpinBox->value()).toInt());
	ui->speedDoubleSpinBox->setValue(s.value("speed", ui->speedDoubleSpinBox->value()).toDouble());
	ui->dimXDoubleSpinBox->setValue(s.value("dimX", ui->dimXDoubleSpinBox->value()).toDouble());
	ui->dimYDoubleSpinBox->setValue(s.value("dimY", ui->dimYDoubleSpinBox->value()).toDouble());
	ui->dimZDoubleSpinBox->setValue(s.value("dimZ", ui->dimZDoubleSpinBox->value()).toDouble());
}

MainWindow::~MainWindow()
{
	QSettings s;
	s.setValue("n", ui->nSpinBox->value());
	s.setValue("speed", ui->speedDoubleSpinBox->value());
	s.setValue("dimX", ui->dimXDoubleSpinBox->value());
	s.setValue("dimY", ui->dimYDoubleSpinBox->value());
	s.setValue("dimZ", ui->dimZDoubleSpinBox->value());
	delete ui;
}

void MainWindow::onSetClicked()
{
	_gl->_sys.setSizes(ui->dimXDoubleSpinBox->value(), ui->dimYDoubleSpinBox->value(), ui->dimZDoubleSpinBox->value());
	_gl->_sys.initRandomParticles(ui->nSpinBox->value(), ui->speedDoubleSpinBox->value());
}
