#include "glsphere.hh"
#include <cmath>

GLSphere::GLSphere() :
	_vbo(QGLBuffer::VertexBuffer),
	_ibo(QGLBuffer::IndexBuffer)
{
}

void GLSphere::initializeGL()
{
	initializeGL(10, 10);
}

void GLSphere::initializeGL(GLint slices, GLint stacks)
{
	QVector<GLfloat> positions;

	QVector<GLuint> indices0;
	QVector<GLuint> indices1;
	QVector<GLuint> indices2;



	unsigned int size = 2 + slices * (stacks - 1);

	positions.reserve(3 * size);

	double alpha = M_PI / double(stacks);
	double beta = 2.0*M_PI / double(slices);

	positions << 0.0 << 0.0 << 1.0;

	for (int i = 1; i < stacks; ++i) {
		for (int j = 0; j < slices; ++j) {
			float r = sin(i*alpha);
			float z = cos(i*alpha);
			float y = sin(j*beta) * r;
			float x = cos(j*beta) * r;

			positions << x << y << z;
		}
	}

	positions << 0.0 << 0.0 << -1.0;

	indices0.reserve(slices+2);
	for (int i = 0; i <= slices; ++i)
		indices0 << i;
	indices0 << 1;

	indices1.reserve((stacks-2) * 2 * (slices+1));
	for (int i = 0; i < stacks-2; ++i) {
		for (int j = 0; j < slices; ++j) {
			indices1 << 1+i*slices+j;
			indices1 << 1+(i+1)*slices+j;
		}
		indices1 << 1+i*slices;
		indices1 << 1+(i+1)*slices;
	}

	indices2.reserve(slices+2);
	for (int i = 1; i <= slices+1; ++i)
		indices2 << size-i;
	indices2 << size-2;

	vbo_sz = 3 * size * sizeof (GLfloat);
	_vbo.create();
	_vbo.bind();
	_vbo.allocate(positions.constData(), vbo_sz);
	_vbo.release();

	ibo_sz[0] = indices0.size() * sizeof (GLuint);
	ibo_sz[1] = indices1.size() * sizeof (GLuint);
	ibo_sz[2] = indices2.size() * sizeof (GLuint);
	_ibo.create();
	_ibo.bind();
	_ibo.allocate(ibo_sz[0] + ibo_sz[1] + ibo_sz[2]);
	_ibo.write(0, indices0.constData(), ibo_sz[0]);
	_ibo.write(ibo_sz[0], indices1.constData(), ibo_sz[1]);
	_ibo.write(ibo_sz[0] + ibo_sz[1], indices2.constData(), ibo_sz[2]);
	_ibo.release();
}

void GLSphere::bind()
{
	_vbo.bind();
	_ibo.bind();
}

void GLSphere::release()
{
	_ibo.release();
	_vbo.release();
}

#define BUFFER_OFFSET(a) ((char*)NULL + (a))

void GLSphere::drawElements()
{
	glDrawElements(GL_TRIANGLE_FAN, ibo_sz[0] / sizeof (GLuint), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	glDrawElements(GL_QUAD_STRIP, ibo_sz[1] / sizeof (GLuint), GL_UNSIGNED_INT, BUFFER_OFFSET(ibo_sz[0]));
	glDrawElements(GL_TRIANGLE_FAN, ibo_sz[2] / sizeof (GLuint), GL_UNSIGNED_INT, BUFFER_OFFSET(ibo_sz[0] + ibo_sz[1]));
}
