#include "Scene.h"
#include <AssetManager.h>

Scene::Scene(OpenGLWindow * window) :
	m_window(window)
{
	assert(window != nullptr);
}

Scene::~Scene()
{}

bool Scene::init()
{
	try
	{
		//Load shader
		m_assets.addShaderProgram("shader", AssetManager::createShaderProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl"));
		m_shader = m_assets.getShaderProgram("shader");
		m_shader->use();

		static const float cubeVert[] =  {0.5, -0.5, -0.5, 1, 0, 0,
								  0.5, -0.5, 0.5, 0, 1, 0,
								  -0.5, -0.5, 0.5, 0, 0, 1,
								  -0.5, -0.5, -0.5, 1, 1, 0,
								  0.5, 0.5, -0.5, 1, 0, 1,
								  0.5, 0.5, 0.5, 0, 1, 1,
								  -0.5, 0.5, 0.5, 1, 1, 1,
								  -0.5, 0.5, -0.5, 0.5, 1, 0.5};

		static const int cubeInd[] = {1, 2, 3,
									  7, 6, 5,
									  4, 5, 1,
									  5, 6, 2,
									  2, 6, 7,
									  0, 3, 7,
									  0, 1, 3,
									  4, 7, 5,
									  0, 4, 1,
									  1, 5, 2,
									  3, 2, 7,
									  4, 0, 7};

		//my code
		//1a VBO erzeugen und binden
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVert), cubeVert, GL_STATIC_DRAW);

		//1b VAO erzeugen und binden
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		//1c VAO und VBO aktivieren
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		//1d IBO erstellen und binden
		glGenBuffers(1, &m_vio);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vio);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeInd), cubeInd, GL_STATIC_DRAW);

		//1e
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		//1.4
		// Alle Dreiecke die im Uhrzeigersinn sind werden nicht angezeigt -> entweder Dreiecke gegen den Uhrzeiger erstellen oder erlauben das Dreiecke gerendert werden dürfen, wenn die im Uhrzeigersinn sind
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);

		//2.2
		m_cubeTransform =  std::make_shared<Transform>();
		m_cubeTransform->rotate(glm::vec3(glm::radians(45.0f), glm::radians(45.0f),0.0f));
		std::cout << "Scene initialization done\n";
		return true;
	}
	catch (std::exception& ex)
	{
		throw std::logic_error("Scene initialization failed:\n" + std::string(ex.what()) + "\n");
	}
}

void Scene::render(float dt)
{

	//my code
	m_shader->use();
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	m_shader->setUniform("modelMatrix", m_cubeTransform->getMatrix(), false);





}

void Scene::update(float dt)
{

}

OpenGLWindow * Scene::getWindow()
{
	return m_window;
}

void Scene::onKey(Key key, Action action, Modifier modifier)
{

}

void Scene::onMouseMove(MousePosition mouseposition)
{

}

void Scene::onMouseButton(MouseButton button, Action action, Modifier modifier)
{

}

void Scene::onMouseScroll(double xscroll, double yscroll)
{

}

void Scene::onFrameBufferResize(int width, int height)
{

}
void Scene::shutdown()
{

}
