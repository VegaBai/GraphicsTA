#include <Qt\qapplication.h>
//#include <MeGlWindow.h>
#include<Renderer\MeRenderer.h>
//#include <Primitives\ShapeGenerator.h>
#include <Test\ShapeGeneratorTest.h>
#include <glm\gtx\transform.hpp>

int main(int argc, char* argv[])
{

	QApplication app(argc, argv);
//	ShapeData data = ShapeGenerator::makeCube();
	ShapeDataTest data = ShapeGeneratorTest::makeCube();
	renderer.show();

	Geometry* cubeGeometry = 
		renderer.addGeometry(
		data.vertices, data.vertexBufferSize(), 
		data.indices, data.numIndices, GL_TRIANGLES);

	ShaderProgramInfo* shader = renderer.addShaderProgram(
		"VertexShaderTest.glsl",
		"FragmentShaderTest.glsl");

//	ShaderProgramInfo* shaderLight = renderer.addShaderProgram(
//		"VertexShaderCodeNoLight.glsl",
//		"FragmentShaderCodeNoLight.glsl"
//	);

	Renderable* cubeRenderable1 = renderer.addRenderable(cubeGeometry,
		glm::mat4(), shader);
//	Renderable* cubeRenderable2 = renderer.addRenderable(cubeGeometry,
//		glm::translate(-2.0f, 0.0f, 0.0f), shader);
//	Renderable* cubeLight = renderer.addRenderable(cubeGeometry,
//		glm::scale(0.2f, 0.2f, 0.2f), shaderLight);
	return app.exec();
	system("PAUSE");
}