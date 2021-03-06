#include "mainwindow.hh"
#include "glwidget.hh"
#include "scriptreader.hh"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setOrganizationName("geiger");
	a.setApplicationName("gaz");

	if (a.arguments().size() == 2) {
		GLWidget w;
		w.show();

		ScriptReader reader;
		if (!reader.runFile(&w.sys, a.arguments().at(1)))
			return 0;

		return a.exec();
	} else {
		MainWindow w;
		w.show();
		return a.exec();
	}

	return 0;
}
