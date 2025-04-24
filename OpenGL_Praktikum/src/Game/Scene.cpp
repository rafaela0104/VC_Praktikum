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

		float vertices[] = {
			//R - Linker Balken
			-0.4f, 0.2f, 1.0f, 0.0f, 0.0f, //0
			-0.32f, 0.2f, 1.0f, 0.0f, 0.0f, //1
 			-0.4f, 0.6f, 0.0f, 1.0f, 0.4f, //2
			-0.32f, 0.6f, 0.0f, 1.0f, 0.4f, //3

			//R - Oberer Balken
			-0.4f, 0.6f, 1.0f, 0.0f, 0.0f, //4
			-0.1f, 0.6f, 0.0f, 1.0f, 0.0f, //5
			-0.1f, 0.55f, 0.0f, 1.0f, 0.0f, //6
			-0.4f, 0.55f, 1.0f, 0.0f, 0.0f, //7

			-0.1f, 0.55f, 0.0f, 1.0f, 0.0f, //8
			-0.2f, 0.55f, 0.0f, 1.0f, 0.0f, //9
			-0.32f, 0.4f, 0.0f, 0.0f, 1.0f, // 10

			-0.1f, 0.2f, 0.3f, 0.6f, 0.7f, //11
			-0.2f, 0.2f, 0.3f, 0.6f, 0.7f, //12

			//N - erster Balken
			0.1f, 0.2f, 1.0f, 0.0f, 1.0f, //13
			0.175f, 0.2f, 1.0f, 0.0f, 1.0f, //14
			0.1f, 0.6f, 0.0f, 1.0f, 1.0f, //15
			0.175f, 0.6f, 0.0f, 1.0f, 1.0f, //16

			//N - zweiter Balken (Schräge)
			0.4f, 0.2f, 1.0f, 1.0f, 0.0f, // 17
			0.3f, 0.2f, 1.0f, 1.0f, 0.0f, //18
			0.225f, 0.6f, 0.0f, 1.0f, 1.0f, // 19

			//N - dritter Balken (zweite Senkrechte)
			0.475f, 0.6f, 1.0f, 0.4f, 0.0f, // 20
			0.4f, 0.6f, 1.0f, 0.4f, 0.0f, //21
			0.475f, 0.2f, 1.0f, 1.0f, 0.0f //22
			};

		int indices[] = {0, 1, 2,
						 3, 2, 1,
						 6, 5, 4,
						 7, 6, 4,
						 8, 9, 10,
						 12, 11, 10,
						 13, 14, 15,
						 16, 15, 14,
						 16, 18, 17,
						 16, 17, 19,
						 17, 20, 21,
						 20, 17, 22
						};

		//my code
		//1a VBO erzeugen und binden
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//1b VAO erzeugen und binden
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		//1c VAO und VBO aktivieren
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		//1d IBO erstellen und binden
		glGenBuffers(1, &m_vio);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vio);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		//1e
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		//1.4
		// Alle Dreiecke die im Uhrzeigersinn sind werden nicht angezeigt -> entweder Dreiecke gegen den Uhrzeiger erstellen oder erlauben das Dreiecke gerendert werden dürfen, wenn die im Uhrzeigersinn sind
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
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
