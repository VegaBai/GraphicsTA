//#include <GL\glew.h>

#include <Renderer\MeRenderer.h>
//#include <Renderer\BufferInfo.h>
//#include <Renderer\Renderable.h>
#include <cassert>
#include <fstream>
#include <iostream>
using std::string;



MeRenderer* MeRenderer::instance = 0;


MeRenderer::MeRenderer()
{
	nextGeometryIndex = 0;
	nextRenderableIndex = 0;
}

void MeRenderer::initialize()
{
	glewInit();
	initializeBuffer();
	
}

void MeRenderer::initializeBuffer()
{
	bufferInfo.nextAvailableByteIndex = 0;
	glGenBuffers(1, &bufferInfo.bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferInfo.bufferID);
	glBufferData(GL_ARRAY_BUFFER, BufferInfo::MAX_BUFFER_SIZE, 0, GL_DYNAMIC_DRAW);
}

Geometry* MeRenderer::addGeometry(void* verts, uint vertexDataSizeBytes,
	void* indices, uint numIndices, //DataType indiceDataType,
	GLuint indexingMode)//, VertexLayoutInfo* vertexLayoutInfo)
{
	GLuint indexDataBytesRequired = numIndices * sizeof(GLushort);
	GLuint totalBytesRequired = vertexDataSizeBytes + indexDataBytesRequired;
	assert(bufferInfo.nextAvailableByteIndex + totalBytesRequired < BufferInfo::MAX_BUFFER_SIZE);

	Geometry* ret = geometries + nextGeometryIndex;
	nextGeometryIndex++;

	glBindBuffer(GL_ARRAY_BUFFER, bufferInfo.bufferID);
	ret->vertexDataBufferBufferByteOffset = bufferInfo.nextAvailableByteIndex;
	glBufferSubData(GL_ARRAY_BUFFER, ret->vertexDataBufferBufferByteOffset,
		vertexDataSizeBytes, verts);	
	bufferInfo.nextAvailableByteIndex += vertexDataSizeBytes;

	ret->indexDataBufferBufferByteOffset = bufferInfo.nextAvailableByteIndex;
	glBufferSubData(GL_ARRAY_BUFFER, ret->indexDataBufferBufferByteOffset,
		indexDataBytesRequired, indices);
	bufferInfo.nextAvailableByteIndex += indexDataBytesRequired;

	ret->indexDataBufferBufferByteOffset = bufferInfo.nextAvailableByteIndex;

	
	return ret;
}
Renderable * MeRenderer::addRenderable(
	const Geometry* what, 
	const glm::mat4& where, 
	const ShaderProgramInfo* how)
{
	assert(nextRenderableIndex != MAX_RENDERABLES);
	Renderable* ret = renderables + nextRenderableIndex;
	nextRenderableIndex++;

	ret->geometry = what;
	ret->modelToWorld = where;
	ret->shaderProgramInfo = how;

	return ret;
}

ShaderProgramInfo* MeRenderer::addShaderProgram(
	const char* vertexShaderFileName, 
	const char* fragmentShaderFileName)
{
	assert(nextShaderProgramIndex != MAX_SHADER_PROGRAM_INFOS);

	ShaderProgramInfo* ret = shaderProgramInfos + nextShaderProgramIndex;
	nextShaderProgramIndex++;

	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	string temp = readShaderCode(vertexShaderFileName);
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode(fragmentShaderFileName);
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		goto BADNESSHAPPENED;

	ret->programID = glCreateProgram();
	glAttachShader(ret->programID, vertexShaderID);
	glAttachShader(ret->programID, fragmentShaderID);

	glLinkProgram(ret->programID);

	if (!checkProgramStatus(ret->programID))
		goto BADNESSHAPPENED;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	//return ShaderInfo();

	return ret;

BADNESSHAPPENED:
	if (vertexShaderID)
		glDeleteShader(vertexShaderID);
	if (fragmentShaderID)
		glDeleteShader(fragmentShaderID);
	if (ret->programID)
	{
		glDeleteProgram(ret->programID);
		ret->programID = 0;
	}
	nextShaderProgramIndex--;
	return 0;
}

bool MeRenderer::checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		std::cout << buffer << std::endl;
		delete[] buffer;
		return false;
	}
	return true;
}

bool MeRenderer::checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool MeRenderer::checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

std::string MeRenderer::readShaderCode(const char* fileName)
{
	std::ifstream meInput(fileName);
	if (!meInput.good())
	{
		std::cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>());
}



MeRenderer::~MeRenderer()
{
}