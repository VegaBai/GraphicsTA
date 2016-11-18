#pragma once

#include <GL\glew.h>
#include <Renderer\BufferInfo.h>
#include <QtOpenGL\qglwidget>
#include <Renderer\Geometry.h>
#include <Renderer\Renderable.h>
#include <glm\glm.hpp>
#include <Renderer\ShaderProgramInfo.h>
#include <string>
#include <Camera.h>


class MeRenderer : public QGLWidget
{
	static MeRenderer* instance;

	Camera camera;

	BufferInfo bufferInfo;

	const static GLuint MAX_GEOMETRIES = 100;
	GLuint nextGeometryIndex;
	Geometry geometries[MAX_GEOMETRIES];
	const static GLuint MAX_RENDERABLES = 100;
	GLuint nextRenderableIndex;
	Renderable renderables[MAX_RENDERABLES];
	const static GLuint MAX_SHADER_PROGRAM_INFOS = 100;
	GLuint nextShaderProgramIndex;
	ShaderProgramInfo shaderProgramInfos[MAX_SHADER_PROGRAM_INFOS];

	void initializeBuffer();

	bool checkStatus(
		GLuint objectID,
		PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
		PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
		GLenum statusType);
	bool checkShaderStatus(GLuint shaderID);
	bool checkProgramStatus(GLuint programID);
	std::string readShaderCode(const char* fileName);

	MeRenderer();
	MeRenderer(MeRenderer&);
	MeRenderer& operator=(MeRenderer&);
	~MeRenderer();

	glm::vec3 lightPosition = glm::vec3(-2.0f, 2.0f, -3.0f);

protected:
	void initializeGL();
	void paintGL();
	void mouseMoveEvent(QMouseEvent* e);
	void keyPressEvent(QKeyEvent* e);

public:
	Geometry* addGeometry(void* verts, uint vertexDataSize,
		void* indices, uint numIndices, //DataType indiceDataType,
		GLuint indexingMode);//, VertexLayoutInfo* vertexLayoutInfo)
	Renderable* addRenderable(
		const Geometry* what = 0,
		const glm::mat4& where = glm::mat4(),
		const ShaderProgramInfo* how = 0);
	ShaderProgramInfo* addShaderProgram(
		const char* vertexShaderFileName,
		const char* fragmentShaderFileName);
	inline static MeRenderer& getInstance() 
	{ 
		if (instance == 0)
			instance = new MeRenderer();
		return *instance; 
	}
};

#define renderer MeRenderer::getInstance()