#ifndef SCRIPTEDITOR_HH
#define SCRIPTEDITOR_HH

#include <QWidget>

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
	void setText(const QString& text);
	QString text() const;

signals:
	void edited();

private slots:
	void slot_edited();

private:
	Ui::ScriptEditor *ui;
};

#include <QSyntaxHighlighter>

class Highlighter : public QSyntaxHighlighter
{
public:
	Highlighter(QObject * parent);

private:
	void highlightBlock(const QString& text);
};

#endif // SCRIPTEDITOR_HH
