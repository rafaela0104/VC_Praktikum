#include "Scene.h"
#include <AssetManager.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

Scene::Scene(OpenGLWindow* window) : m_window(window)
{
	assert(window != nullptr);
}

Scene::~Scene() {}

bool Scene::init()
{
	try {
		// Shader laden
		m_assets.addShaderProgram("shader", AssetManager::createShaderProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl"));
		m_shader = m_assets.getShaderProgram("shader");
		m_shader->use();

		// Cube-Daten
		static const float cubeVert[] = {
			// Position        // Farbe
			 0.5, -0.5, -0.5, 1, 0, 0,
			 0.5, -0.5,  0.5, 0, 1, 0,
			-0.5, -0.5,  0.5, 0, 0, 1,
			-0.5, -0.5, -0.5, 1, 1, 0,
			 0.5,  0.5, -0.5, 1, 0, 1,
			 0.5,  0.5,  0.5, 0, 1, 1,
			-0.5,  0.5,  0.5, 1, 1, 1,
			-0.5,  0.5, -0.5, 0.5, 1, 0.5
		};

		static const int cubeInd[] = {
			1, 2, 3,  7, 6, 5,
			4, 5, 1,  5, 6, 2,
			2, 6, 7,  0, 3, 7,
			0, 1, 3,  4, 7, 5,
			0, 4, 1,  1, 5, 2,
			3, 2, 7,  4, 0, 7
		};

		// VBO
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVert), cubeVert, GL_STATIC_DRAW);

		// VAO
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// IBO
		glGenBuffers(1, &m_vio);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vio);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeInd), cubeInd, GL_STATIC_DRAW);

		glBindVertexArray(0);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_GREATER);
		glClearDepth(0.0f);

		// Transformations-Hierarchie
		m_root = std::make_shared<Transform>();
		m_root->rotate(glm::vec3(0.0f, glm::radians(45.0f), 0.0f)); // Figur seitlich

		m_torso = std::make_shared<Transform>();
		m_torso->rotate(glm::vec3(0.0f, glm::radians(45.0f), 0.0f));
		/*m_head = std::make_shared<Transform>();
		m_head->translate(glm::vec3(0.0f, 1.5f, 0.0f));
		m_head->scale(glm::vec3(0.6f));

		m_leftUpperArm = std::make_shared<Transform>();
		m_leftUpperArm->translate(glm::vec3(-0.75f, 0.75f, 0.0f));
		m_leftUpperArm->scale(glm::vec3(0.6f));

		m_rightUpperArm = std::make_shared<Transform>();
		m_rightUpperArm->translate(glm::vec3(0.75f, 0.75f, 0.0f));
		m_rightUpperArm->scale(glm::vec3(0.6f));

		m_leftLeg = std::make_shared<Transform>();
		m_leftLeg->translate(glm::vec3(-0.3f, -1.0f, 0.0f));

		m_rightLeg = std::make_shared<Transform>();
		m_rightLeg->translate(glm::vec3(0.3f, -1.0f, 0.0f));*/

		// Hierarchie aufbauen
		m_root->addChild(m_torso);
		m_torso->addChild(m_head);
		m_torso->addChild(m_leftUpperArm);
		m_torso->addChild(m_rightUpperArm);
		m_torso->addChild(m_leftLeg);
		m_torso->addChild(m_rightLeg);

		std::cout << "Scene initialization done\n";
		return true;
	}
	catch (std::exception& ex) {
		throw std::logic_error("Scene initialization failed:\n" + std::string(ex.what()) + "\n");
	}
}

void Scene::render(float dt)
{
	m_shader->use();

	float time = glfwGetTime();
	m_shader->setUniform("time", time);

	// Kamera-View und Projektions-Matrix
	glm::mat4 view = glm::lookAt(glm::vec3(0, 1, 5), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
	m_shader->setUniform("viewMatrix", view, false);
	m_shader->setUniform("projMatrix", proj, false);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderNode(m_root, glm::mat4(1.0f));
}

void Scene::renderNode(std::shared_ptr<Transform> node, const glm::mat4& parentMatrix)
{
	glm::mat4 model = parentMatrix * node->getMatrix();
	m_shader->setUniform("modelMatrix", model, false);

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	for (auto& child : node->getChildren()) {
		renderNode(child, model);
	}
}

void Scene::update(float dt)
{
	float walkSpeed = glm::radians(20.0f) * sin(glfwGetTime());

	
	m_leftLeg->rotateAroundPoint(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(walkSpeed, 0.0f, 0.0f));
	m_rightLeg->rotateAroundPoint(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-walkSpeed, 0.0f, 0.0f));

	m_leftUpperArm->rotateAroundPoint(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(-walkSpeed, 0.0f, 0.0f));
	m_rightUpperArm->rotateAroundPoint(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(walkSpeed, 0.0f, 0.0f));
}

OpenGLWindow* Scene::getWindow() { return m_window; }
void Scene::onKey(Key key, Action action, Modifier modifier) {}
void Scene::onMouseMove(MousePosition mouseposition) {}
void Scene::onMouseButton(MouseButton button, Action action, Modifier modifier) {}
void Scene::onMouseScroll(double xscroll, double yscroll) {}
void Scene::onFrameBufferResize(int width, int height) {}
void Scene::shutdown() {}
