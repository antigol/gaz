#ifndef PAIRCORFUNWIDGET_HH
#define PAIRCORFUNWIDGET_HH

#include <QWidget>
#include "system.hh"

class PairCorFunWidget : public QWidget
{
  Q_OBJECT
public:
  explicit PairCorFunWidget(QWidget *parent = 0);

  System* sys;

private:
  void paintEvent(QPaintEvent* e) override;
  void mousePressEvent(QMouseEvent*) override;

};

#endif // PAIRCORFUNWIDGET_HH
