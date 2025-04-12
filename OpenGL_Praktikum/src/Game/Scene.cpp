#include "Scene.h"
#include "ShaderProgram.h"
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
			// R - linker Balken
			-0.9f, -0.9f, 1.0f, 0.0f, 0.0f,
			-0.8f, -0.9f, 1.0f, 0.0f, 0.0f,
			-0.8f,  0.9f, 1.0f, 0.0f, 0.0f,
			-0.9f,  0.9f, 1.0f, 0.0f, 0.0f,

			// R - oberer Querbalken
			-0.8f,  0.9f, 0.0f, 1.0f, 0.0f,
			-0.5f,  0.9f, 0.0f, 1.0f, 0.0f,
			-0.5f,  0.8f, 0.0f, 1.0f, 0.0f,
			-0.8f,  0.8f, 0.0f, 1.0f, 0.0f,

			// R - schräge Andeutung (Bein)
			-0.8f,  0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.9f, 0.0f, 0.0f, 1.0f,
			-0.65f,-0.9f, 0.0f, 0.0f, 1.0f,

			// N - linker Balken
			 0.0f, -0.9f, 1.0f, 1.0f, 0.0f,
			 0.1f, -0.9f, 1.0f, 1.0f, 0.0f,
			 0.1f,  0.9f, 1.0f, 1.0f, 0.0f,
			 0.0f,  0.9f, 1.0f, 1.0f, 0.0f,

			//rechten Teil von "R"
			-0.5f,  0.8f, 1.0f, 0.0f, 0.0f,  // Oben
	        -0.4f,  0.8f, 1.0f, 0.0f, 0.0f,  // Oben
	        -0.4f,  0.5f, 1.0f, 0.0f, 0.0f,  // Unten
	        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,


			// N - rechter Balken
			 0.4f, -0.9f, 1.0f, 1.0f, 0.0f,
			 0.5f, -0.9f, 1.0f, 1.0f, 0.0f,
			 0.5f,  0.9f, 1.0f, 1.0f, 0.0f,
			 0.4f,  0.9f, 1.0f, 1.0f, 0.0f,

			// N - Diagonale
			 0.1f,  0.9f, 0.0f, 1.0f, 1.0f,
			 0.2f,  0.9f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.9f, 0.0f, 1.0f, 1.0f,
			 0.4f, -0.9f, 0.0f, 1.0f, 1.0f
		};


		int indices[] = {
			// R
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			8, 9,10,10,11, 8,
			24, 25, 26, 26, 27, 24,

			// N
		   12,13,14,14,15,12,
		   16,17,18,18,19,16,
		   20,21,22,22,23,20
		};


		//1a
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//1b
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		//1c
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0); //Position

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		//1d
		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		//1e
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_numIndices = sizeof(indices) / sizeof(indices[0]);

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
	//1.2a
	m_shader->use();
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

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
