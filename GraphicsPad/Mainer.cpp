#include <Qt\qapplication.h>
//#include <MeGlWindow.h>
#include<Renderer\MeRenderer.h>
#include <Primitives\ShapeGenerator.h>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	ShapeData data = ShapeGenerator::makeCube();

	Geometry* cubeGeometry = 
		renderer.addGeometry(
		data.vertices, data.vertexBufferSize(), 
		data.indices, data.numIndices, GL_TRIANGLES);

	Renderable* cuybeRenderable1 = renderer.addRenderable(cubeGeometry);
	Renderable* cuybeRenderable2 = renderer.addRenderable(cubeGeometry);
	renderer.show();
	int ret = app.exec();
	return app.exec();
//	system("PAUSE");
}