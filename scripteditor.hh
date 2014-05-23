#ifndef SCRIPTEDITOR_HH
#define SCRIPTEDITOR_HH

#include <QWidget>
#include <QTextEdit>

namespace Ui {
class ScriptEditor;
}

class ScriptEditor : public QWidget
{
	Q_OBJECT

public:
	explicit ScriptEditor(QWidget *parent = 0);
	~ScriptEditor();

	void setLineError(int line, const QString& errorText);
	QString currentCode() const;

signals:
	void edited();

private slots:
	void slot_edited();
	void slot_close_tab(int index);
	void slot_new();
	void slot_open();

private:
	void add_tab(const QString &name, const QString &code);

	Ui::ScriptEditor *ui;
	QList<QTextEdit*> ts;
};

#include <QSyntaxHighlighter>

class Highlighter : public QSyntaxHighlighter
{
public:
	Highlighter(QTextEdit * parent);

private:
	void highlightBlock(const QString& text);
};

#endif // SCRIPTEDITOR_HH
