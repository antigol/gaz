#include "glwidget.hh"

GLWidget::GLWidget(QWidget *parent)
	: QGLWidget(parent)
{
	setMinimumSize(300, 300);
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}

GLWidget::~GLWidget()
{
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
							   "	gl_Position = proj * view * model * vec4(vertex, 1.0);"
							   "	_normal = nview * nmodel * vertex;"
							   "	_eye = vec3(view * model * vec4(vertex, 1.0));"
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
							   "	color += max(dot(n, light), 0.0) * color_diffuse;"
							   "	color += pow(max(dot(relf, light), 0.0), 4.0) * vec3(1.0, 1.0, 1.0);"
							   "	gl_FragColor = vec4(color, 1.0);"
							   "}"
							   );
	_p.bindAttributeLocation("vertex", vertex);
	_p.bindAttributeLocation("color", color);
	_p.link();
	_p.bind();
	_p.setUniformValue("light", QVector3D(1.0, 1.0, 1.0).normalized());

	_sphere.initializeGL(20, 20);

	_trans.translate(0, 0, -200.0);
	glEnable(GL_DEPTH_TEST);

	startTimer(0);
	_t.start();
}

void GLWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);

	QMatrix4x4 m;
	m.perspective(70.0, qreal(w)/qreal(h?h:1), 1.0, 10000.0);
	_p.setUniformValue("proj", m);
}

void GLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_p.setUniformValue("view", _trans * _v);
	_p.setUniformValue("nview", (_trans * _v).normalMatrix());

	QMatrix4x4 m;
	_sphere.bind();
	_p.enableAttributeArray(vertex);
	_p.setAttributeBuffer(vertex, GL_FLOAT, 0, 3);
	for (const Particle& z : _sys._ps) {
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
}

void GLWidget::timerEvent(QTimerEvent *)
{
	double dt = double(_t.restart()) / 1000.0;

	int col = _sys.evolve(dt);
	qDebug() << col << "collisions";
	qDebug() << "check collision = " << _t.elapsed() << "ms";

	updateGL();
}

#include <QMouseEvent>
void GLWidget::mousePressEvent(QMouseEvent *e)
{
	mouseLastPos = e->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
	QPointF d = e->pos() - mouseLastPos;
	QMatrix4x4 m;
	m.rotate(d.manhattanLength(), d.y(), d.x(), 0.0);
	_v = m * _v;
	mouseLastPos = e->pos();
}

#include <QWheelEvent>
void GLWidget::wheelEvent(QWheelEvent *e)
{
	_trans.translate(0, 0, e->delta() * 0.1);
}
