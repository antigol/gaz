#ifndef GLWIDGET_HH
#define GLWIDGET_HH

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMatrix4x4>
#include <QSound>
#include "system.hh"
#include "glsphere.hh"

class GLWidget : public QGLWidget
{
	Q_OBJECT

public:
	GLWidget(QWidget *parent = 0);
	~GLWidget();

	System sys;

public slots:
	void pause();

private slots:

private:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void mousePressEvent(QMouseEvent *e) override;
	void mouseMoveEvent(QMouseEvent *e) override;
	void wheelEvent(QWheelEvent *e) override;
  void keyPressEvent(QKeyEvent* e) override;
  void keyReleaseEvent(QKeyEvent* e) override;
	void timerEvent(QTimerEvent *e) override;

	QGLShaderProgram _p;
	QMatrix4x4 _v;


	GLSphere _sphere;

	QPointF _mouseLastPos;
};

#endif // GLWIDGET_HH
