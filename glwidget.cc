#include "glwidget.hh"
#include <QSettings>
#include <QPainter>
#include <iostream>

GLWidget::GLWidget(QWidget *parent)
	: QGLWidget(parent)
{
	setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

	setAutoFillBackground(false);

	QSettings s;
	_v = s.value("MATRIXVIEW", QMatrix4x4()).value<QMatrix4x4>();
}

GLWidget::~GLWidget()
{
	QSettings s;
	s.setValue("MATRIXVIEW", _v);
}

void GLWidget::pause()
{
    if (sys.isRunning())
		sys.stop();
	else
		sys.start();
}

constexpr int vertex = 0;
constexpr int color = 1;

void GLWidget::initializeGL()
{
	_p.addShaderFromSourceCode(QGLShader::Vertex,
							   "attribute vec3 vertex;"
							   "uniform mat4 proj, view, model;"
							   "uniform mat3 nview, nmodel;"
							   "varying vec3 _normal, _eye;"
							   "void main(void)"
							   "{"
							   "	vec4 v = view * model * vec4(vertex, 1.0);"
							   "	gl_Position = proj * v;"
							   "	_normal = nview * nmodel * vertex;"
							   "	_eye = v.xyz;"
							   "}"
							   );
	_p.addShaderFromSourceCode(QGLShader::Fragment,
							   "uniform vec3 light;\n"
							   "uniform vec3 color_ambiant, color_diffuse;\n"
							   "varying vec3 _normal, _eye;\n"
							   "void main()\n"
							   "{\n"
							   "	vec3 n = normalize(_normal);\n"
							   "	vec3 e = normalize(_eye);\n"
							   "	vec3 relf = e - 2.0 * dot(e, n) * n;\n"
							   "	vec3 color = color_ambiant;"
							   "	color += max(dot(n, light), 0.0) * color_diffuse;\n"
							   "	color += pow(max(dot(relf, light), 0.0), 6.0) * vec3(1.0, 1.0, 1.0);\n"
							   "	gl_FragColor = vec4(color, 1.0);"
							   "}"
							   );
	_p.bindAttributeLocation("vertex", vertex);
	_p.bindAttributeLocation("color", color);
	_p.link();
	_p.bind();
	_p.setUniformValue("light", QVector3D(1.0, 1.0, 1.0).normalized());

	_sphere.initializeGL(20, 20);

	sys.start();
	startTimer(1000/25);
}

void GLWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);

	QMatrix4x4 m;
	m.perspective(50.0, qreal(w)/qreal(h?h:1), 1.0, 100000.0);
	_p.bind();
	_p.setUniformValue("proj", m);
}

void GLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT, GL_FILL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	_p.bind();
	_p.setUniformValue("view", _v);
	_p.setUniformValue("nview", _v.normalMatrix());

	QMatrix4x4 m;
	m.scale(sys._dim[0], sys._dim[1], sys._dim[2]);
	_p.setUniformValue("model", m);
	_p.setUniformValue("nmodel", m.normalMatrix());

	_p.setUniformValue("color_ambiant", 1.0, 1.0, 1.0);
	_p.setUniformValue("color_diffuse", 1.0, 1.0, 1.0);

	static const GLfloat cube[24*3] = {
		// x-
		-1.0, -1.0, -1.0,
		-1.0, +1.0, -1.0,
		-1.0, +1.0, +1.0,
		-1.0, -1.0, +1.0,

		// x+
		+1.0, -1.0, -1.0,
		+1.0, +1.0, -1.0,
		+1.0, +1.0, +1.0,
		+1.0, -1.0, +1.0,

		// z-
		-1.0, -1.0, -1.0,
		-1.0, +1.0, -1.0,
		+1.0, +1.0, -1.0,
		+1.0, -1.0, -1.0,

		// z+
		-1.0, -1.0, +1.0,
		-1.0, +1.0, +1.0,
		+1.0, +1.0, +1.0,
		+1.0, -1.0, +1.0,
	};
	_p.enableAttributeArray(vertex);
	_p.setAttributeArray(vertex, cube, 3);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	glDrawArrays(GL_LINE_LOOP, 4, 4);
	glDrawArrays(GL_LINE_LOOP, 8, 4);
	glDrawArrays(GL_LINE_LOOP, 12, 4);
	_p.disableAttributeArray(vertex);

	_sphere.bind();
	_p.enableAttributeArray(vertex);
	_p.setAttributeBuffer(vertex, GL_FLOAT, 0, 3);

	for (const Particle& z : sys.ps) {
		m.setToIdentity();
		m.translate(z.q[0], z.q[1], z.q[2]);
		m.scale(z.r);
		_p.setUniformValue("model", m);
		_p.setUniformValue("nmodel", m.normalMatrix());
		_p.setUniformValue("color_ambiant", z.color[0] / 2.0, z.color[1] / 2.0, z.color[2] / 2.0);
		_p.setUniformValue("color_diffuse", z.color[0], z.color[1], z.color[2]);

		_sphere.drawElements();
	}

	_p.disableAttributeArray(vertex);
	_sphere.release();

	if (sys.limited) {
		QPainter p;
		p.begin(this);
		p.setPen(Qt::red);
		p.setFont(QFont(QString::fromUtf8("DejaVu Sans Mono")));
		p.drawText(QPointF(10.0, 20.0), "Simulation slowed (not in real time)");
		p.end();
	}
}

#include <QMouseEvent>
void GLWidget::mousePressEvent(QMouseEvent *e)
{
	_mouseLastPos = e->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
	QPointF d = e->pos() - _mouseLastPos;
	QMatrix4x4 m;
	if (e->buttons() & Qt::LeftButton)
		m.rotate(0.3*d.manhattanLength(), d.y(), d.x(), 0.0);
	if (e->buttons() & Qt::RightButton)
		m.translate(-0.5*d.x(), 0.5*d.y(), 0.0);
	_v = m * _v;
	_mouseLastPos = e->pos();
}

#include <QWheelEvent>
void GLWidget::wheelEvent(QWheelEvent *e)
{
	QMatrix4x4 m;
	m.translate(0, 0, e->delta() * 0.05);
  _v = m * _v;
}

#include <QKeyEvent>
void GLWidget::keyPressEvent(QKeyEvent* e)
{

}

void GLWidget::keyReleaseEvent(QKeyEvent* e)
{

}

void GLWidget::timerEvent(QTimerEvent *)
{
    updateGL();
}
