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
  QVector<double> pcf = sys->pairCorelation(nBins, 2.5);


  double max = *std::max_element(pcf.begin(), pcf.end());
  for (double& x : pcf) x /= max;


  QPainter p;
  p.begin(this);

  p.scale(1, -1);
  p.translate(0, -height());

  for (int i = 0; i < pcf.size(); ++i) {
    p.drawLine(i, 0, i, height() * pcf[i]);
  }
  p.end();
}

void PairCorFunWidget::mousePressEvent(QMouseEvent*)
{
  update();
}
