#include "scripteditor.hh"
#include "ui_scripteditor.h"

ScriptEditor::ScriptEditor(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ScriptEditor)
{
	ui->setupUi(this);
	Highlighter* hl = new Highlighter(this);
	hl->setDocument(ui->textEdit->document());

	connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(slot_edited()));
}

ScriptEditor::~ScriptEditor()
{
	delete ui;
}

void ScriptEditor::setLineError(int line, const QString &errorText)
{
	if (line == 0)
		ui->label->clear();
	else
		ui->label->setText(QString("line %1 : %2").arg(line).arg(errorText));
}

void ScriptEditor::setText(const QString &text)
{
	ui->textEdit->setText(text);
}

QString ScriptEditor::text() const
{
	return ui->textEdit->toPlainText();
}

void ScriptEditor::slot_edited()
{
	emit edited();
}

Highlighter::Highlighter(QObject *parent)
	: QSyntaxHighlighter(parent)
{
}

void Highlighter::highlightBlock(const QString &text)
{
	{
		QTextCharFormat green;
		green.setForeground(Qt::darkGreen);

		QRegExp expression("\\bApp.[A-Za-z]+\\b");
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
