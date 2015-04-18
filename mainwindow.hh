#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QTimer>
#include <QComboBox>
#include "glwidget.hh"
#include "scriptreader.hh"
#include "scripteditor.hh"
#include "paircorfunwidget.hh"

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
  void slot_inter_changed();

private:
  GLWidget* viewWidget;
  PairCorFunWidget* pcf;


  ScriptReader reader;
  ScriptEditor* editor;
  QTimer timer;
  QComboBox *comboBox_algo;
  QComboBox *comboBox_inter;
};

#endif // MAINWINDOW_HH
