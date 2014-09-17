#include "paircorfunwidget.hh"
#include <QPainter>
#include <QDebug>

PairCorFunWidget::PairCorFunWidget(QWidget *parent) :
  QWidget(parent)
{
  resize(100, 100);
}

void PairCorFunWidget::paintEvent(QPaintEvent* )
{
  int nBins = width();
  QVector<double> pcf = sys->pairCorelation(nBins);


  double max = *std::max_element(pcf.begin(), pcf.end());
//  static double maxg = max;
//  maxg = std::max(maxg, max);
  for (double& x : pcf) x /= max;


  QPainter p;
  p.begin(this);

  p.scale(1, -1);
  p.translate(0, -height());

  p.setPen(QPen(Qt::yellow,  1.0));
  p.setBrush(Qt::yellow);

  for (int i = 0; i < pcf.size(); ++i) {
    p.drawLine(i, 0, i, height() * pcf[i]);
  }
  p.end();
}

void PairCorFunWidget::mousePressEvent(QMouseEvent*)
{
  update();
}
