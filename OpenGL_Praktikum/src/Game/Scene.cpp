#include "Scene.h"
#include <AssetManager.h>

float rightArmRotationAngle = 0.0f;

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
		// Shader laden
		m_assets.addShaderProgram("shader", AssetManager::createShaderProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl"));
		m_shader = m_assets.getShaderProgram("shader");
		m_shader->use();

		// Würfel-Geometrie
		static const float cubeVert[] = {
			0.5, -0.5, -0.5, 1, 0, 0,
			0.5, -0.5, 0.5, 0, 1, 0,
			-0.5, -0.5, 0.5, 0, 0, 1,
			-0.5, -0.5, -0.5, 1, 1, 0,
			0.5, 0.5, -0.5, 1, 0, 1,
			0.5, 0.5, 0.5, 0, 1, 1,
			-0.5, 0.5, 0.5, 1, 1, 1,
			-0.5, 0.5, -0.5, 0.5, 1, 0.5
		};

		static const int cubeInd[] = {
			1, 2, 3,  7, 6, 5,
			4, 5, 1,  5, 6, 2,
			2, 6, 7,  0, 3, 7,
			0, 1, 3,  4, 7, 5,
			0, 4, 1,  1, 5, 2,
			3, 2, 7,  4, 0, 7
		};

		// VAO + VBO
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVert), cubeVert, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glGenBuffers(1, &m_vio);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vio);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeInd), cubeInd, GL_STATIC_DRAW);

		glBindVertexArray(0);

		// Face Culling & Depth
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);

		// Szene aufbauen
		m_root = std::make_shared<Transform>();
		//m_root->scale(glm::vec3(0.15f)); // <- Szene kleiner skalieren!
		//m_root->rotate(glm::vec3(0.0f, glm::radians(45.0f), 0.0f));

		m_torso = std::make_shared<Transform>();
		m_torso->translate(glm::vec3(0.0f, -2.75f, 0.0f));
		m_torso->scale(glm::vec3(2.0f, 4.0f, 1.0f));

		glm::vec3 arm(1.3f, 0.0f, 0.0f);

		m_leftUpperArm = std::make_shared<Transform>();
		m_leftUpperArm->translate(glm::vec3(-0.65f, 0.5f, 0.5f));
		m_leftUpperArm->scale(glm::vec3(0.25f, 0.2f, 1.0f));
		m_leftUpperArm->rotateAroundPoint(glm::vec3(0.0f, 0.4f, 0.0f), arm);


		m_leftLowerArm = std::make_shared<Transform>();
		m_leftLowerArm->translate(glm::vec3(glm::vec3(0.0f, 0.0f, 0.8f)));
		m_leftLowerArm->scale(glm::vec3(1.0f, 1.0f, 0.5f));


		m_rightUpperArm = std::make_shared<Transform>();
		m_rightUpperArm->translate(glm::vec3(0.65f, 0.3f, 0.5f));
		m_rightUpperArm->scale(glm::vec3(0.25f, 0.2f, 1.0f));
		m_rightUpperArm->rotateAroundPoint(glm::vec3(0.0f, 0.5f, 0.0f), arm);

		m_rightLowerArm = std::make_shared<Transform>();
		m_rightLowerArm->translate(glm::vec3(0.0f, 0.0f, 0.8f));
		m_rightLowerArm->scale(glm::vec3(1.0f, 1.0f, 0.5f));

		m_leftLeg = std::make_shared<Transform>();
		m_leftLeg->translate(glm::vec3(-0.25f, -0.7f, 0.0f));
		m_leftLeg->scale(glm::vec3(0.25f, 0.4f, 0.25f));

		m_rightLeg = std::make_shared<Transform>();
		m_rightLeg->translate(glm::vec3(0.25f, -0.7f, 0.0f));
		m_rightLeg->scale(glm::vec3(0.25f, 0.4f, 0.25f));

		// Struktur aufbauen
		m_root->addChild(m_torso);
		m_torso->addChild(m_leftUpperArm);
		m_leftUpperArm->addChild(m_leftLowerArm);
		m_torso->addChild(m_rightUpperArm);
		m_rightUpperArm->addChild(m_rightLowerArm);
		m_torso->addChild(m_leftLeg);
		m_torso->addChild(m_rightLeg);

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
	// Framebuffer löschen vor dem Zeichnen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_shader->use();

	/*fovy zB. glm::radians(90.0f)	Weitwinkel: größere Perspektivverzerrung, mehr vom Raum sichtbar
	near z.B. 1.0f	Alles näher als 1.0 wird abgeschnitten (Clipping)
	far z.B. 10.0f	Alles weiter als 10.0 wird nicht gezeichnet
	aspectRatio	Falsches Seitenverhältnis verzerrt Bild (z.B. gestaucht/gestreckt)*/

	glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, 4.0f, 20.0f), // Kameraposition
		glm::vec3(0.0f, 0.0f, 0.0f), //Blickpunkt
		glm::vec3(0.0f, 1.0f, 0.0f) //Up-Vektor
		);

	//Seitenverhältnis
	float aspectRatio = static_cast<float>(m_window->getFrameBufferWidth()) / static_cast<float>(m_window->getFrameBufferHeight());
	glm::mat4 projection = glm::perspective(
		glm::radians(45.0f),
		aspectRatio,
		0.1f,
		100.0f);

	m_shader->setUniform("viewMatrix", view, false);
	m_shader->setUniform("projectionMatrix", projection, false);

	// Hier keine Einzelzeichnung mehr nötig – alles passiert im Renderbaum
	renderNode(m_root, glm::mat4(1.0f));
}


void Scene::renderNode(std::shared_ptr<Transform> node, const glm::mat4& parentMatrix)
{
	glm::mat4 model = parentMatrix * node->getMatrix();
	m_shader->setUniform("modelMatrix", model, false);

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	for (auto& child : node->getChildren())
	{
		renderNode(child, model);
	}
}

void Scene::update(float dt)
{
	static float time = 0.0f;
	time += dt;
	m_root->rotate(glm::vec3(0.0f, glm::radians(0.5f), 0.0f));
	float armSwing = glm::radians(15.0f * sinf(time * 2.0f));
	glm::vec3 arm(0.01 * (sinf(time)), 0, 0);
	float legSwing = glm::radians(15.0f) * sinf(time * 2.0f);

	//m_leftUpperArm->setRotation(glm::vec3(armSwing, 0.0f, 0.0f)); glm::vec3(0.05f * armSwing, 0.0f, 0.0f)
	m_leftUpperArm->rotateAroundPoint(glm::vec3(0.0f, 0.5f, 0.0f), arm);
	m_rightUpperArm->rotateAroundPoint(glm::vec3(0.0f, 0.5f, 0.0f), -arm);
	//m_rightUpperArm->rotateAroundPoint(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.05f * (-armSwing), 0.0f, 0.0f));

	m_leftLeg->rotateAroundPoint(glm::vec3(0.25f, 0.0f, 0.0f), glm::vec3(0.05f * legSwing, 0.0f, 0.0f));
	//m_leftLeg->setRotation(glm::vec3(-legSwing, 0.0f, 0.0f));
	m_rightLeg->rotateAroundPoint(glm::vec3(-0.25f, 0.0f, 0.0f), glm::vec3(0.05f * (-legSwing), 0.0f, 0.0f));
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