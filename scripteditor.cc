#include "scripteditor.hh"
#include "ui_scripteditor.h"
#include <QSettings>
#include <QMap>
#include <QInputDialog>
#include <QFileDialog>
#include <QFileInfo>

ScriptEditor::ScriptEditor(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ScriptEditor)
{
	ui->setupUi(this);
	ui->tabWidget->setTabsClosable(true);
	connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(slot_close_tab(int)));
	connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(slot_edited()));
	connect(ui->button_new, SIGNAL(clicked()), this, SLOT(slot_new()));
	connect(ui->button_open, SIGNAL(clicked()), this, SLOT(slot_open()));

	QSettings s;
	QString defaultCode = "App.dimension(40, 40, 40);\n"
						  "App.radius(1);\n"
						  "App.mass(1);\n"
						  "App.color(1,0,0);\n"
						  "App.position(App.rand(-20,20), App.rand(-20,20), App.rand(-20,20));\n"
						  "App.momentum(0,0,0);\n"
						  "App.addParticle();";
	QMap<QString, QVariant> defaultMap;
	defaultMap.insert("default", defaultCode);

	QMap<QString, QVariant> map = s.value("code", defaultMap).toMap();

	for (auto it = map.constBegin(); it != map.constEnd(); ++it) {
		add_tab(it.key(), it.value().toString());
	}

	QString tabName = s.value("tabName", 0).toString();
	for (int i = 0; i < ui->tabWidget->count(); ++i)
		if (ui->tabWidget->tabText(i) == tabName)
			ui->tabWidget->setCurrentIndex(i);
}

ScriptEditor::~ScriptEditor()
{
	QSettings s;
	QMap<QString, QVariant> map;
	for (int i = 0; i < ts.size(); ++i) {
		map.insert(ui->tabWidget->tabText(i), ts[i]->toPlainText());
	}
	s.setValue("code", map);
	s.setValue("tabName", ui->tabWidget->tabText(ui->tabWidget->currentIndex()));
	delete ui;
}

void ScriptEditor::setLineError(int line, const QString &errorText)
{
	if (line == 0)
		ui->label_error->clear();
	else
		ui->label_error->setText(QString("line %1 : %2").arg(line).arg(errorText));
}

QString ScriptEditor::currentCode() const
{
	int index = ui->tabWidget->currentIndex();
	if (index == -1)
		return QString();
	else
		return ts[index]->toPlainText();
}

void ScriptEditor::slot_edited()
{
	emit edited();
}

void ScriptEditor::slot_close_tab(int index)
{
	ui->tabWidget->removeTab(index);
	delete ts[index];
	ts.removeAt(index);
}

void ScriptEditor::slot_new()
{
	QString name = QInputDialog::getText(this, "Name", "New tab name");

	if (!name.isEmpty()) {
		add_tab(name, "");
	}
}

void ScriptEditor::slot_open()
{
	QSettings set;
	QStringList names = QFileDialog::getOpenFileNames(this, "", set.value("dir").toString());

	for (const auto &name : names) {
		QFile file(name);
		file.open(QIODevice::ReadOnly);
		QFileInfo fi(name);
		add_tab(fi.baseName(), file.readAll());

		set.setValue("dir", fi.absolutePath());
	}
}

void ScriptEditor::add_tab(const QString &name, const QString &code)
{
	QTextEdit* te = new QTextEdit(this);
	te->setTabStopWidth(20);
	QFont font;
	font.setFamily(QString::fromUtf8("DejaVu Sans Mono"));
	font.setPointSize(9);
	te->setFont(font);
	te->setPlainText(code);
	ui->tabWidget->addTab(te, name);

	new Highlighter(te);

	connect(te, SIGNAL(textChanged()), this, SLOT(slot_edited()));
	ts.append(te);
}

Highlighter::Highlighter(QTextEdit *parent)
	: QSyntaxHighlighter(parent)
{
}

void Highlighter::highlightBlock(const QString &text)
{
	{
		QTextCharFormat green;
		green.setForeground(Qt::darkGreen);

		QRegExp expression("\\bApp.(print|position|momentum|mass|radius|color|addParticle|dimension|rand|randn)\\b");
		int index = text.indexOf(expression);
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, green);
			index = text.indexOf(expression, index + length);
		}
	}
	{
		QTextCharFormat blue;
		blue.setForeground(Qt::blue);

		QRegExp expression("\\b(for|var)\\b");
		int index = text.indexOf(expression);
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, blue);
			index = text.indexOf(expression, index + length);
		}
	}
	{
		QTextCharFormat red;
		red.setForeground(Qt::darkRed);

		QRegExp expression("\\b(Math|Date).[A-Za-z]+\\b");
		int index = text.indexOf(expression);
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, red);
			index = text.indexOf(expression, index + length);
		}
	}
	{
		QTextCharFormat format;
		format.setForeground(Qt::red);

		QRegExp expression("(//[^\n]*|\".*\")");
		int index = text.indexOf(expression);
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, format);
			index = text.indexOf(expression, index + length);
		}
	}
}
