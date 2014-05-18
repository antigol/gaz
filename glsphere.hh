#ifndef GLSPHERE_HH
#define GLSPHERE_HH

#include <QGLBuffer>

class GLSphere
{
public:
	GLSphere();

	// construit les VBO et IBO (à lancer dans initializeGL)
	void initializeGL();
	void initializeGL(GLint slices , GLint stacks);

	// bind VBO et IBO (à lancer avant drawElements)
	void bind();

	// release VBO et IBO (à lancer après drawElements)
	void release();

	// dessine
	void drawElements();

private:
	QGLBuffer _vbo, _ibo;
	GLuint vbo_sz;
	GLuint ibo_sz[3];
};

#endif // GLSPHERE_HH
